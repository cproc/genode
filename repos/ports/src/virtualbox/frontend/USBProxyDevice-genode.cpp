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

#include <base/printf.h>

#include <VBox/vmm/pdm.h>

#include "USBProxyDevice.h"

namespace Usb_proxy_genode {

	struct Meta_data
	{
		/* dummy */

	};

	int open (PUSBPROXYDEV pProxyDev, const char *pszAddress, void *pvBackend);
	int init(PUSBPROXYDEV pProxyDev);
	void close(PUSBPROXYDEV pProxyDev);
	int reset(PUSBPROXYDEV pProxyDev, bool fResetOnLinux);
	int set_config(PUSBPROXYDEV pProxyDev, int iCfg);
	int claim_interface(PUSBPROXYDEV pProxyDev, int iIf);
	int release_interface(PUSBPROXYDEV pProxyDev, int iIf);
	int set_interface(PUSBPROXYDEV pProxyDev, int iIf, int iSetting);
	int clear_halted_endpoint(PUSBPROXYDEV  pDev, unsigned int iEp);
	int urb_queue(PUSBPROXYDEV pProxyDev, PVUSBURB pUrb);
	int urb_cancel(PUSBPROXYDEV pProxyDev, PVUSBURB pUrb);
	PVUSBURB urb_reap(PUSBPROXYDEV pProxyDev, RTMSINTERVAL cMillies);
	int wakeup(PUSBPROXYDEV pProxyDev);

};

int Usb_proxy_genode::open (PUSBPROXYDEV pProxyDev, const char *pszAddress, void *pvBackend)
{
	PDBG("not implemented");
	PDBG("pszAddress = %s", pszAddress);
	return VINF_SUCCESS;
}

int Usb_proxy_genode::init(PUSBPROXYDEV pProxyDev)
{
	PDBG("not implemented");
	return -1;
}

void Usb_proxy_genode::close(PUSBPROXYDEV pProxyDev)
{
	PDBG("not implemented");
}

int Usb_proxy_genode::reset(PUSBPROXYDEV pProxyDev, bool fResetOnLinux)
{
	PDBG("not implemented");
	return -1;
}

int Usb_proxy_genode::set_config(PUSBPROXYDEV pProxyDev, int iCfg)
{
	PDBG("not implemented");
	return -1;
}

int Usb_proxy_genode::claim_interface(PUSBPROXYDEV pProxyDev, int iIf)
{
	PDBG("not implemented");
	return -1;
}

int Usb_proxy_genode::release_interface(PUSBPROXYDEV pProxyDev, int iIf)
{
	PDBG("not implemented");
	return -1;
}

int Usb_proxy_genode::set_interface(PUSBPROXYDEV pProxyDev, int iIf, int iSetting)
{
	PDBG("not implemented");
	return -1;
}

int Usb_proxy_genode::clear_halted_endpoint(PUSBPROXYDEV  pDev, unsigned int iEp)
{
	PDBG("not implemented");
	return -1;
}

int Usb_proxy_genode::urb_queue(PUSBPROXYDEV pProxyDev, PVUSBURB pUrb)
{
	PDBG("not implemented");
	return -1;
}

int Usb_proxy_genode::urb_cancel(PUSBPROXYDEV pProxyDev, PVUSBURB pUrb)
{
	PDBG("not implemented");
	return -1;
}

PVUSBURB Usb_proxy_genode::urb_reap(PUSBPROXYDEV pProxyDev, RTMSINTERVAL cMillies)
{
	PDBG("not implemented");
	return 0;
}

int Usb_proxy_genode::wakeup(PUSBPROXYDEV pProxyDev)
{
	PDBG("not implemented");
	return 0;
}

extern const USBPROXYBACK g_USBProxyDeviceHost =
{
    "host",
    sizeof(Usb_proxy_genode::Meta_data),
    Usb_proxy_genode::open,
    Usb_proxy_genode::init,
    Usb_proxy_genode::close,
    Usb_proxy_genode::reset,
    Usb_proxy_genode::set_config,
    Usb_proxy_genode::claim_interface,
    Usb_proxy_genode::release_interface,
    Usb_proxy_genode::set_interface,
    Usb_proxy_genode::clear_halted_endpoint,
    Usb_proxy_genode::urb_queue,
    Usb_proxy_genode::urb_cancel,
    Usb_proxy_genode::urb_reap,
	Usb_proxy_genode::wakeup,
    0
};
