/*
 * \brief  LwIP netif for the Nic session
 * \author Emery Hemingway
 * \date   2016-09-28
 */

/*
 * Copyright (C) 2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef __LWIP__NIC_NETIF_H__
#define __LWIP__NIC_NETIF_H__

#ifndef __cplusplus
#error nic_netif.h contains C++ code and is not usable from C
#endif

/* Genode includes */
#include <nic/packet_allocator.h>
#include <nic_session/connection.h>
#include <base/component.h>
#include <base/log.h>


namespace Lwip {

extern "C" {
/* LwIP includes */
#include <lwip/netif.h>
#include <netif/etharp.h>
#if LWIP_IPV6
#include <lwip/ethip6.h>
#endif
#include <lwip/init.h>
#include <lwip/dhcp.h>
}

	class Nic_netif;

	extern "C" {
		static err_t nic_netif_init(struct netif *netif);
		static err_t nic_netif_linkoutput(struct netif *netif, struct pbuf *p);
	}
}

class Lwip::Nic_netif
{
	private:

		enum { NIC_BUF_SIZE = Nic::Packet_allocator::DEFAULT_PACKET_SIZE * 128 };

		Nic::Packet_allocator _nic_tx_alloc;
		Nic::Connection _nic;

		struct netif _netif;

		ip_addr_t ip, nm, gw;


		/*************************
		 ** Nic signal handlers **
		 *************************/

		void handle_link_state()
		{
			if (_nic.link_state()) {
				netif_set_link_up(&_netif);
				// XXX: if (!(ip.addr && nm.addr && gw.addr))
					dhcp_start(&_netif);
			} else {
				netif_set_link_down(&_netif);
				/* XXX: inform above */
			}
		}

		void handle_rx_packets()
		{
			auto &rx = *_nic.rx();

			while (rx.packet_avail() && rx.ready_to_ack()) {
				Nic::Packet_descriptor packet = rx.get_packet();

				Genode::size_t len = packet.size();
				LINK_STATS_INC(len);

#if ETH_PAD_SIZE
				len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

				char const *src = rx.packet_content(packet);

				/*
				 * TODO: this is where the pbufs for received data
				 * are allocated, but with 'pbuf_alloced_custom' this
				 * can be zero-copy, need to test if deferring the
				 * freeing of packets until the application has
				 * processed data is better or worse for performance
				 */
				struct pbuf *p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

				if (p) {
#if ETH_PAD_SIZE
					pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

					/*
					 * We iterate over the pbuf chain until we have read the entire
					 * packet into the pbuf.
					 */
					for(struct pbuf *q = p; q != 0; q = q->next) {
						char *dst = (char*)q->payload;
						Genode::memcpy(dst, src, q->len);
						src += q->len;
					}
#if ETH_PAD_SIZE
					pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

					if (_netif.input(p, &_netif) != ERR_OK) {
						Genode::error("error forwarding physical Nic packet to LwIP");
						pbuf_free(p);
					}
				}

				rx.acknowledge_packet(packet);
			}
		}

		Genode::Signal_handler<Nic_netif> _link_state_handler;
		Genode::Signal_handler<Nic_netif> _rx_packet_handler;

	public:

		Nic_netif(Genode::Env &env,
		          Genode::Allocator &alloc,
		          Genode::Xml_node config)
		:
			_nic_tx_alloc(&alloc),
			_nic(env, &_nic_tx_alloc, NIC_BUF_SIZE, NIC_BUF_SIZE,
			    config.attribute_value("label", Genode::String<160>()).string()),
			_link_state_handler(env.ep(), *this, &Nic_netif::handle_link_state),
			_rx_packet_handler( env.ep(), *this, &Nic_netif::handle_rx_packets)

		{
			memset(&_netif, 0x00, sizeof(_netif));

			{
				ip4_addr_t v4dummy;
				IP4_ADDR(&v4dummy, 0, 0, 0, 0);

				netif* r = netif_add(&_netif, &v4dummy, &v4dummy, &v4dummy,
				                     this, nic_netif_init, ethernet_input);
				if (r == NULL) {
					Genode::error("failed to initialize Nic to LwIP interface");
					throw r;
				}
			}
			netif_set_up(&_netif);
			
			if (!config.has_attribute("ip_addr")) {
				Genode::log("configuring LwIP interface with DHCP");
				dhcp_start(&_netif);

			} else { /* get addressing from config */
				typedef Genode::String<IPADDR_STRLEN_MAX> Str;

				Str ip_str = config.attribute_value("ip_addr", Str());
				ip_addr_t ipaddr;
				if (!ipaddr_aton(ip_str.string(), &ipaddr)) {
					Genode::error("LwIP configured with invalid IP address '",ip_str,"'");
					throw ip_str;
				}

				if (IP_IS_V6_VAL(ipaddr)) {
					netif_create_ip6_linklocal_address(&_netif, 1);
					err_t err = netif_add_ip6_address(&_netif, ip_2_ip6(&ipaddr), NULL);
					if (err != ERR_OK) {
						Genode::error("failed to set LwIP IPv6 address to '",ip_str,"'");
						throw err;
					}
				} else {
					netif_set_ipaddr(&_netif, ip_2_ip4(&ipaddr));

					if (config.has_attribute("netmask")) {
						Str str = config.attribute_value("netmask", Str());
						ip_addr_t ip;
						ipaddr_aton(str.string(), &ip);
						netif_set_netmask(&_netif, ip_2_ip4(&ip));
					}

					if (config.has_attribute("gateway")) {
						Str str = config.attribute_value("gateway", Str());
						ip_addr_t ip;
						ipaddr_aton(str.string(), &ip);
						netif_set_gw(&_netif, ip_2_ip4(&ip));
					}
				}

			}

			netif_set_default(&_netif);
		}

		/**
		 * Set a custom status callback
		 */
		void set_status_callback(netif_status_callback_fn status_callback) {
			netif_set_status_callback(&_netif, status_callback); }

		/**
		 * Set a custom link callback
		 */
		void set_link_callback(netif_status_callback_fn link_callback) {
			netif_set_link_callback(&_netif, link_callback); }

		/**
		 * Callback issued by LwIP to initialize netif struct
		 *
		 * \noapi
		 */
		err_t init()
		{
#if LWIP_NETIF_HOSTNAME
			/* Initialize interface hostname */
			_netif.hostname = main.hostname.string();
#endif /* LWIP_NETIF_HOSTNAME */

			_netif.name[0] = 'e';
			_netif.name[1] = 'n';

			_netif.output = etharp_output;
#if LWIP_IPV6
			_netif.output_ip6 = ethip6_output;
#endif /* LWIP_IPV6 */

			_netif.linkoutput      = nic_netif_linkoutput;

			/* Set physical MAC address */
			Nic::Mac_address const mac = _nic.mac_address();
			for(int i=0; i<6; ++i)
				_netif.hwaddr[i] = mac.addr[i];

			_netif.mtu        = 1500; /* XXX: just a guess */
			_netif.hwaddr_len = ETHARP_HWADDR_LEN;
			_netif.flags      = NETIF_FLAG_BROADCAST |
			                    NETIF_FLAG_ETHARP    |
			                    NETIF_FLAG_LINK_UP;

			/* set Nic session signal handlers */
			_nic.link_state_sigh(_link_state_handler);
			_nic.rx_channel()->sigh_packet_avail(_rx_packet_handler);
			_nic.rx_channel()->sigh_ready_to_ack(_rx_packet_handler);

			return ERR_OK;
		}

		/**
		 * Callback issued by LwIP to write a Nic packet
		 *
		 * \noapi
		 */
		err_t linkoutput(struct pbuf *p)
		{		
			auto &tx = *_nic.tx();

			/* flush acknowledgements */
			while (tx.ack_avail())
				tx.release_packet(tx.get_acked_packet());
		
			if (!tx.ready_to_submit())
				return ERR_WOULDBLOCK;
		
			Nic::Packet_descriptor packet;
			try { packet = tx.alloc_packet(p->tot_len); }
			catch (...) { return ERR_WOULDBLOCK; }

#if ETH_PAD_SIZE
			pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

			/*
			 * We iterate over the pbuf chain until we have read the entire
			 * pbuf into the packet.
			 */
			char *dst = tx.packet_content(packet);
			for(struct pbuf *q = p; q != 0; q = q->next) {
				char const *src = (char*)q->payload;
				Genode::memcpy(dst, src, q->len);
				dst += q->len;
			}

#if ETH_PAD_SIZE
			pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

			tx.submit_packet(packet);
			LINK_STATS_INC(packet.length());
			return ERR_OK;
		}
};


/**************************
 ** LwIP netif callbacks **
 **************************/

namespace Lwip {
	extern "C" {

/**
 * Initialize the netif
 */
static err_t nic_netif_init(struct netif *netif)
{
	Lwip::Nic_netif *nic_netif = (Lwip::Nic_netif *)netif->state;
	return nic_netif->init();
}

/**
 * Send a raw packet to the Nic session
 */
static err_t nic_netif_linkoutput(struct netif *netif, struct pbuf *p)
{
	Lwip::Nic_netif *nic_netif = (Lwip::Nic_netif *)netif->state;
	return nic_netif->linkoutput(p);
}

	}
}

#endif /* __LWIP__NIC_NETIF_H__ */
