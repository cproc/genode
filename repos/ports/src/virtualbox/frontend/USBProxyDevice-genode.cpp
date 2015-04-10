/*
 * \brief  USBProxyDevice implementation for Genode
 * \author Christian Prochaska
 * \date   2015-04-13
 */

/*
 * Copyright (C) 2015 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

#include <base/allocator_avl.h>
#include <base/printf.h>
#include <usb/usb.h>
#include <usb_session/connection.h>
#include <util/construct_at.h>

#include <VBox/vmm/pdm.h>

#include "USBProxyDevice.h"

namespace Usb_proxy_device_genode {

	struct Data
	{
		private:

			Genode::Allocator_avl _alloc;

		public:

			Usb::Connection usb_connection;
			int             interface_id = 0;

			Data(unsigned int vendor_id, unsigned int product_id)
			: _alloc(Genode::env()->heap()),
			  usb_connection(&_alloc, vendor_id, product_id)
			  { }

	};


	static int open(PUSBPROXYDEV pProxyDev, const char *pszAddress, void *pvBackend);
	static int init(PUSBPROXYDEV pProxyDev);
	static void close(PUSBPROXYDEV pProxyDev);
	static int reset(PUSBPROXYDEV pProxyDev, bool fResetOnLinux);
	static int set_config(PUSBPROXYDEV pProxyDev, int iCfg);
	static int claim_interface(PUSBPROXYDEV pProxyDev, int iIf);
	static int release_interface(PUSBPROXYDEV pProxyDev, int iIf);
	static int set_interface(PUSBPROXYDEV pProxyDev, int iIf, int iSetting);
	static int clear_halted_endpoint(PUSBPROXYDEV  pDev, unsigned int iEp);
	static int urb_queue(PUSBPROXYDEV pProxyDev, PVUSBURB pUrb);
	static int urb_cancel(PUSBPROXYDEV pProxyDev, PVUSBURB pUrb);
	static PVUSBURB urb_reap(PUSBPROXYDEV pProxyDev, RTMSINTERVAL cMillies);
	static int wakeup(PUSBPROXYDEV pProxyDev);

#if 0
	static Genode::Signal_receiver signal_receiver;
	static Genode::Signal_context signal_context;
	static Genode::Signal_context_capability signal_context_cap = signal_receiver.manage(&signal_context);
#endif

};

int Usb_proxy_device_genode::open(PUSBPROXYDEV pProxyDev, const char *pszAddress, void *pvBackend)
{
	/* TODO: extract IDs from pszAddress */

	unsigned int vendor_id = 0x04b3;
	unsigned int product_id = 0x310c;

	PDBG("vendor_id: %x, product_id: %x", vendor_id, product_id);

	Usb_proxy_device_genode::Data *data =
		USBPROXYDEV_2_DATA(pProxyDev, Usb_proxy_device_genode::Data*);

	Genode::construct_at<Usb_proxy_device_genode::Data>(data, vendor_id, product_id);

#if 0
	data->usb_connection.tx_channel()->sigh_ack_avail(signal_context_cap);
	data->usb_connection.tx_channel()->sigh_ready_to_submit(signal_context_cap);
#endif

	return VINF_SUCCESS;
}

int Usb_proxy_device_genode::init(PUSBPROXYDEV pProxyDev)
{
	PDBG("not implemented");
	return -1;
}

void Usb_proxy_device_genode::close(PUSBPROXYDEV pProxyDev)
{
	Usb_proxy_device_genode::Data *data =
		USBPROXYDEV_2_DATA(pProxyDev, Usb_proxy_device_genode::Data*);

	data->~Data();

	PDBG("not implemented");
}

int Usb_proxy_device_genode::reset(PUSBPROXYDEV pProxyDev, bool fResetOnLinux)
{
	PDBG("not implemented");
	return -1;
}

int Usb_proxy_device_genode::set_config(PUSBPROXYDEV pProxyDev, int iCfg)
{
	PDBG("not implemented");
	return -1;
}

int Usb_proxy_device_genode::claim_interface(PUSBPROXYDEV pProxyDev, int iIf)
{
	PDBG("not implemented");
	return -1;
}

int Usb_proxy_device_genode::release_interface(PUSBPROXYDEV pProxyDev, int iIf)
{
	PDBG("not implemented");
	return -1;
}

int Usb_proxy_device_genode::set_interface(PUSBPROXYDEV pProxyDev, int iIf, int iSetting)
{
	PDBG("not implemented");
	return -1;
}

int Usb_proxy_device_genode::clear_halted_endpoint(PUSBPROXYDEV  pDev, unsigned int iEp)
{
	PDBG("not implemented");
	return -1;
}

int Usb_proxy_device_genode::urb_queue(PUSBPROXYDEV pProxyDev, PVUSBURB pUrb)
{
	PDBG("EndPt = %u, enmType = %d", pUrb->EndPt, (int)pUrb->enmType);

	Usb_proxy_device_genode::Data *data =
		USBPROXYDEV_2_DATA(pProxyDev, Usb_proxy_device_genode::Data*);

	if (pUrb->enmType == VUSBXFERTYPE_MSG) { 

		if (pUrb->cbData < sizeof(VUSBSETUP))
			return VERR_BUFFER_UNDERFLOW;

		PVUSBSETUP pSetup = (PVUSBSETUP)pUrb->abData;

		Usb::Packet_descriptor p = data->usb_connection.source()->alloc_packet(pSetup->wLength);

		PDBG("request_type = 0x%x, request = 0x%x, value = 0x%x, index = 0x%x, length = %u",
			pSetup->bmRequestType, pSetup->bRequest, pSetup->wValue, pSetup->wIndex, pSetup->wLength);

		p.type                 = Usb::Packet_descriptor::CTRL;
		p.succeded             = false;
		p.control.request      = pSetup->bRequest;
		p.control.request_type = pSetup->bmRequestType;
		p.control.value        = pSetup->wValue;
		p.control.index        = pSetup->wIndex;
		p.control.timeout      = 100;
		p.completion           = 0;

		if (pSetup->wLength > 0) {
			char *packet_content = data->usb_connection.source()->packet_content(p);
			Genode::memcpy(packet_content, (pSetup + 1), pSetup->wLength);
		}

		data->usb_connection.source()->submit_packet(p);

		return VINF_SUCCESS;

	} else
		PDBG("unsupported transfer type %d", (int)pUrb->enmType);

	return -1;
}

int Usb_proxy_device_genode::urb_cancel(PUSBPROXYDEV pProxyDev, PVUSBURB pUrb)
{
	PDBG("not implemented");
	return -1;
}

PVUSBURB Usb_proxy_device_genode::urb_reap(PUSBPROXYDEV pProxyDev, RTMSINTERVAL cMillies)
{
	PDBG("cMillies = %u", cMillies);

	Usb_proxy_device_genode::Data *data =
		USBPROXYDEV_2_DATA(pProxyDev, Usb_proxy_device_genode::Data*);

	Usb::Packet_descriptor p = data->usb_connection.source()->get_acked_packet();

	PDBG("got packet");

	return 0;
}

int Usb_proxy_device_genode::wakeup(PUSBPROXYDEV pProxyDev)
{
	PDBG("not implemented");
	return -1;
}

extern const USBPROXYBACK g_USBProxyDeviceHost =
{
    "host",
    sizeof(Usb_proxy_device_genode::Data),
    Usb_proxy_device_genode::open,
    Usb_proxy_device_genode::init,
    Usb_proxy_device_genode::close,
    Usb_proxy_device_genode::reset,
    Usb_proxy_device_genode::set_config,
    Usb_proxy_device_genode::claim_interface,
    Usb_proxy_device_genode::release_interface,
    Usb_proxy_device_genode::set_interface,
    Usb_proxy_device_genode::clear_halted_endpoint,
    Usb_proxy_device_genode::urb_queue,
    Usb_proxy_device_genode::urb_cancel,
    Usb_proxy_device_genode::urb_reap,
	Usb_proxy_device_genode::wakeup,
    0
};
