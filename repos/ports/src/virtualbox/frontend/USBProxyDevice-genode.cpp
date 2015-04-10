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
#include <timer_session/connection.h>
#include <usb/usb.h>
#include <usb_session/connection.h>
#include <util/construct_at.h>

#include <VBox/vmm/pdm.h>

#include "USBProxyDevice.h"

namespace Usb_proxy_device_genode {

	struct Ack_avail_signal_context : Genode::Signal_context { };
	struct Timeout_signal_context : Genode::Signal_context { };
	struct Wakeup_signal_context : Genode::Signal_context  { };

	class Data
	{
		private:

			Genode::Allocator_avl _alloc;

		public:

			Usb::Connection                   usb_connection;
			Timer::Connection                 timer;

			Genode::Signal_context            state_change_signal_context;
			Ack_avail_signal_context          ack_avail_signal_context;
			Timeout_signal_context            timeout_signal_context;
			Wakeup_signal_context             wakeup_signal_context;
			Genode::Signal_context_capability wakeup_signal_context_cap;

			Genode::Signal_receiver           signal_receiver;

			Data(unsigned int vendor_id, unsigned int product_id)
			: _alloc(Genode::env()->heap()),
			  usb_connection(&_alloc, vendor_id, product_id)
			{
				/* wait until device and server are ready */

				Genode::Signal_context_capability state_change_signal_context_cap =
					signal_receiver.manage(&state_change_signal_context);

				usb_connection.sigh_state_change(state_change_signal_context_cap);

				signal_receiver.wait_for_signal();

				/* register the other signal handlers */

				Genode::Signal_context_capability ack_avail_signal_context_cap =
					signal_receiver.manage(&ack_avail_signal_context);
				usb_connection.tx_channel()->sigh_ack_avail(ack_avail_signal_context_cap);

				Genode::Signal_context_capability timeout_signal_context_cap =
					signal_receiver.manage(&timeout_signal_context);
				timer.sigh(timeout_signal_context_cap);

				wakeup_signal_context_cap = signal_receiver.manage(&wakeup_signal_context);
			}

	};

	/* keep a reference to the VirtualBox URB in a packet descriptor */
	struct Urb_preserve_completion : Usb::Completion
	{
		PVUSBURB pUrb;

		void complete(Usb::Packet_descriptor &p) { }

		Urb_preserve_completion(PVUSBURB pUrb) : pUrb(pUrb) { }
		virtual ~Urb_preserve_completion() { }
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

	return VINF_SUCCESS;
}

int Usb_proxy_device_genode::init(PUSBPROXYDEV pProxyDev)
{
	PDBG("not implemented, returning VINF_SUCCESS anyway");
	return VINF_SUCCESS;
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
	PDBG("not implemented, returning VINF_SUCCESS anyway, return address = %p", __builtin_return_address(0));
	/* TODO */
	return VINF_SUCCESS;
}

int Usb_proxy_device_genode::set_config(PUSBPROXYDEV pProxyDev, int iCfg)
{
	PDBG("set_config(%d): return address = %p", iCfg, __builtin_return_address(0));

#if 0
	Usb_proxy_device_genode::Data *data =
		USBPROXYDEV_2_DATA(pProxyDev, Usb_proxy_device_genode::Data*);

	Usb::Packet_descriptor p = data->usb_connection.source()->alloc_packet(1);
	p.type = Usb::Packet_descriptor::CONFIG;
	p.number = iCfg;
	p.succeded = false;

	data->usb_connection.source()->submit_packet(p);
	p = data->usb_connection.source()->get_acked_packet();

	if (p.succeded)
		return VINF_SUCCESS;
	else
		return -1;
#endif
	return VINF_SUCCESS;
}

int Usb_proxy_device_genode::claim_interface(PUSBPROXYDEV pProxyDev, int iIf)
{
	PDBG("not implemented");
	return VINF_SUCCESS;
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
	PDBG("not implemented, returning VINF_SUCCESS anyway");
	return VINF_SUCCESS;
}

int Usb_proxy_device_genode::urb_queue(PUSBPROXYDEV pProxyDev, PVUSBURB pUrb)
{
	PDBG("EndPt = %u, enmType = %d, enmDir = %d, cbData = %u", pUrb->EndPt, pUrb->enmType, pUrb->enmDir, pUrb->cbData);

	Usb_proxy_device_genode::Data *data =
		USBPROXYDEV_2_DATA(pProxyDev, Usb_proxy_device_genode::Data*);

	if (pUrb->enmType == VUSBXFERTYPE_MSG) { 

		if (pUrb->cbData < sizeof(VUSBSETUP))
			return VERR_BUFFER_UNDERFLOW;

		PVUSBSETUP pSetup = (PVUSBSETUP)pUrb->abData;

		PDBG("control transfer: request_type = 0x%x, request = 0x%x, value = 0x%x, index = 0x%x, length = %u, cbData = %u",
			pSetup->bmRequestType, pSetup->bRequest, pSetup->wValue, pSetup->wIndex, pSetup->wLength, pUrb->cbData);

		Usb::Packet_descriptor p = data->usb_connection.source()->alloc_packet(pSetup->wLength);

		p.type                 = Usb::Packet_descriptor::CTRL;
		p.succeded             = false;
		p.control.request      = pSetup->bRequest;
		p.control.request_type = pSetup->bmRequestType;
		p.control.value        = pSetup->wValue;
		p.control.index        = pSetup->wIndex;
		p.control.timeout      = 100;
		p.completion           = new Urb_preserve_completion(pUrb);

		if ((pUrb->enmDir == VUSBDIRECTION_OUT) && (pSetup->wLength > 0)) {
			char *packet_content = data->usb_connection.source()->packet_content(p);
			Genode::memcpy(packet_content, (pSetup + 1), pSetup->wLength);
		}

		data->usb_connection.source()->submit_packet(p);

		return VINF_SUCCESS;

	} else if (pUrb->enmType == VUSBXFERTYPE_INTR) {

		PDBG("interrupt transfer");

		Usb::Packet_descriptor p = data->usb_connection.source()->alloc_packet(pUrb->cbData);

		p.type             = Usb::Packet_descriptor::IRQ;
		p.succeded         = false;
		p.transfer.ep      = pUrb->EndPt | (pUrb->enmDir == VUSBDIRECTION_IN ? 0x80 : 0);
		p.transfer.timeout = 100;
		p.completion       = new Urb_preserve_completion(pUrb);

		if ((pUrb->enmDir == VUSBDIRECTION_OUT) && (pUrb->cbData > 0)) {
			char *packet_content = data->usb_connection.source()->packet_content(p);
			Genode::memcpy(packet_content, pUrb->abData, pUrb->cbData);
		}

		data->usb_connection.source()->submit_packet(p);

		return VINF_SUCCESS;

	} else
		PERR("%s: unsupported transfer type %d", __PRETTY_FUNCTION__, (int)pUrb->enmType);

	return -1;
}

int Usb_proxy_device_genode::urb_cancel(PUSBPROXYDEV pProxyDev, PVUSBURB pUrb)
{
	PDBG("not implemented");
	return -1;
}

PVUSBURB Usb_proxy_device_genode::urb_reap(PUSBPROXYDEV pProxyDev, RTMSINTERVAL cMillies)
{
	PDBG("cMillies = %u, return address = %p", cMillies, __builtin_return_address(0));

	Usb_proxy_device_genode::Data *data =
		USBPROXYDEV_2_DATA(pProxyDev, Usb_proxy_device_genode::Data*);

	unsigned long elapsed_ms_start = data->timer.elapsed_ms();

	if (cMillies != RT_INDEFINITE_WAIT) 
		data->timer.trigger_once(cMillies * 1000);

	for (;;) {

		PDBG("ack avail = %d", data->usb_connection.source()->ack_avail());

		Genode::Signal signal = data->signal_receiver.wait_for_signal();

		Genode::Signal_context *context = signal.context();

		if (dynamic_cast<Timeout_signal_context*>(context)) {

			if (cMillies == RT_INDEFINITE_WAIT) {
				/* received an old signal */
				PDBG("old timeout signal received");
				continue;
			}

			unsigned long elapsed_ms_now = data->timer.elapsed_ms();

			if (elapsed_ms_now - elapsed_ms_start < cMillies) {
				/* received an old signal */
				PDBG("old timeout signal received");
				continue;
			}

			PDBG("timeout signal received");

			return 0;

		} else if (dynamic_cast<Ack_avail_signal_context*>(context)) {

			PDBG("got ack_avail signal, ack_avail = %d",
			     data->usb_connection.source()->ack_avail());

			if (!data->usb_connection.source()->ack_avail()) {
				while (!data->usb_connection.source()->ack_avail()) ;
				PDBG("avail");
				while(1);
			}

			Usb::Packet_descriptor p = data->usb_connection.source()->get_acked_packet();

			static int count = 0;

			PDBG("got packet: %d", ++count);

			Urb_preserve_completion *completion = static_cast<Urb_preserve_completion*>(p.completion);
			PVUSBURB pUrb = completion->pUrb;
			delete completion;

			if (pUrb->enmType == VUSBXFERTYPE_MSG) {

				PVUSBSETUP pSetup = (PVUSBSETUP)pUrb->abData;

				PDBG("pSetup->wLength = %u, pUrb->cbData = %u", pSetup->wLength, pUrb->cbData);

				if ((pUrb->enmDir == VUSBDIRECTION_IN) && (pSetup->wLength > 0)) {
					char *packet_content = data->usb_connection.source()->packet_content(p);
					Genode::memcpy((pSetup + 1), packet_content, pSetup->wLength);
				}

				pUrb->enmStatus = VUSBSTATUS_OK;

			} else if (pUrb->enmType == VUSBXFERTYPE_INTR) {

				if ((pUrb->enmDir == VUSBDIRECTION_IN) && (pUrb->cbData > 0)) {
					char *packet_content = data->usb_connection.source()->packet_content(p);
					Genode::memcpy(pUrb->abData, packet_content, pUrb->cbData);
					unsigned int data = *(unsigned int*)pUrb->abData;
					signed char x = (data & 0x0000ff00) >> 8;
					signed char y = (data & 0x00ff0000) >> 16;
					PDBG("x = %d, y = %d", x, y);
				}

				pUrb->enmStatus = VUSBSTATUS_OK;

			} else {
				PERR("%s: unsupported transfer type %d", __PRETTY_FUNCTION__, (int)pUrb->enmType);
			}

			return pUrb;

		} else {
			/* wakeup */
			PDBG("wakeup signal received");
			return 0;
		}
	}

}

int Usb_proxy_device_genode::wakeup(PUSBPROXYDEV pProxyDev)
{
	PDBG("wakeup(): return address = %p", __builtin_return_address(0));

	Usb_proxy_device_genode::Data *data =
		USBPROXYDEV_2_DATA(pProxyDev, Usb_proxy_device_genode::Data*);

	Genode::Signal_transmitter(data->wakeup_signal_context_cap).submit();

	return VINF_SUCCESS;
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
