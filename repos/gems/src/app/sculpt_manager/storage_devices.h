/*
 * \brief  Registry of known storage devices
 * \author Norman Feske
 * \date   2018-05-03
 */

/*
 * Copyright (C) 2018 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _STORAGE_DEVICES_
#define _STORAGE_DEVICES_

#include "types.h"
#include "block_device.h"
#include "usb_storage_device.h"

namespace Sculpt_manager { struct Storage_devices; }


struct Sculpt_manager::Storage_devices
{
	Block_devices       block_devices       { };
	Usb_storage_devices usb_storage_devices { };

	template <typename FN>
	void for_each(FN const &fn) const
	{
		block_devices.for_each([&] (Block_device const &dev) {
			fn(dev); });

		usb_storage_devices.for_each([&] (Usb_storage_device const &dev) {
			fn(dev); });
	}

	template <typename FN>
	void for_each(FN const &fn)
	{
		block_devices.for_each([&] (Block_device &dev) {
			fn(dev); });

		usb_storage_devices.for_each([&] (Usb_storage_device &dev) {
			fn(dev); });
	}
};

#endif /* _STORAGE_DEVICES_ */
