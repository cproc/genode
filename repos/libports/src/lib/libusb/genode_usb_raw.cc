/*
 * \brief  Genode backend for libusb
 * \author Christian Prochaska
 * \date   2016-09-19
 */

/*
 * Copyright (C) 2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <base/log.h>
#include <base/allocator_avl.h>
#include <base/signal.h>
#include <usb/usb.h>
#include <usb_session/connection.h>

#include <fcntl.h>
#include <time.h>

#include "libusbi.h"

static Genode::Signal_receiver &signal_receiver()
{
	static Genode::Signal_receiver instance;
	return instance;
}


struct Completion : Usb::Completion
{
	struct usbi_transfer *itransfer;

	Completion(struct usbi_transfer *itransfer)
	: itransfer(itransfer) { }

	void complete(Usb::Packet_descriptor &p) override { }
};


struct Usb_device
{
	Genode::Allocator_avl   alloc { Genode::env()->heap() };

#if 0
	Genode::Signal_context  state_changed_signal_context;
	Genode::Signal_receiver state_changed_signal_receiver;
	Usb::Connection         usb_connection { &alloc,
	                                         "usb_device",
	                                         512*1024/*,
	                                         state_changed_signal_receiver.manage(
	                                         	&state_changed_signal_context)*/ };

	Genode::Signal_dispatcher<Usb_device> ack_avail_signal_dispatcher
	                                      { signal_receiver(), *this,
	                                        &Usb_device::ack_avail };
#endif
	Usb::Device_descriptor  device_descriptor;
	Usb::Config_descriptor  config_descriptor;
	char                   *raw_config_descriptor = nullptr;

	Usb_device()
	{
		int state_fd = open("/usb/state", O_RDONLY);
		//Genode::log("state_fd: ", state_fd);

		char state[10];
		ssize_t state_bytes_read = read(state_fd, state, sizeof(state));

		//Genode::log("state_bytes_read: ", state_bytes_read);
		//Genode::log("state: ", Genode::Cstring(state));

		while (Genode::strcmp(state, "plugged") != 0) {

			Genode::log("libusb: waiting until device is plugged...");

			fd_set readfds;
			FD_ZERO(&readfds);
			FD_SET(state_fd, &readfds);

			//Genode::log("calling select()");

			select(state_fd + 1, &readfds, nullptr, nullptr, nullptr);

			//Genode::log("select() returned");

			state_bytes_read = read(state_fd, state, sizeof(state));

			//Genode::log("state_bytes_read: ", state_bytes_read);
			//Genode::log("state: ", Genode::Cstring(state));
		}

		Genode::log("libusb: device is plugged");

		/* read device descriptor */

		int ep0_fd = open("/usb/ep0", O_RDONLY);

		struct libusb_device_descriptor device_descriptor;

		struct libusb_control_setup setup;
		libusb_fill_control_setup((unsigned char*)&setup,
		                          LIBUSB_ENDPOINT_IN,
		                          LIBUSB_REQUEST_GET_DESCRIPTOR,
		                          (LIBUSB_DT_DEVICE << 8) | 0,
		                          0,
		                          sizeof(device_descriptor));

		write(ep0_fd, &setup, sizeof(setup));

		read(ep0_fd, &device_descriptor, sizeof(device_descriptor));

#if 0
		usb_connection.config_descriptor(&device_descriptor, &config_descriptor);

		raw_config_descriptor = (char*)malloc(config_descriptor.total_length);

		Usb::Packet_descriptor p =
			usb_connection.source()->alloc_packet(config_descriptor.total_length);

		p.type                 = Usb::Packet_descriptor::CTRL;
		p.control.request      = LIBUSB_REQUEST_GET_DESCRIPTOR;
		p.control.request_type = LIBUSB_ENDPOINT_IN;
		p.control.value        = (LIBUSB_DT_CONFIG << 8) | 0;
		p.control.index        = 0;

		usb_connection.source()->submit_packet(p);

		p = usb_connection.source()->get_acked_packet();

		if (!p.succeded)
			Genode::error(__PRETTY_FUNCTION__,
			              ": could not read raw configuration descriptor");

		if (p.control.actual_size != config_descriptor.total_length)
			Genode::error(__PRETTY_FUNCTION__,
			              ": received configuration descriptor of unexpected size");

		char *packet_content = usb_connection.source()->packet_content(p);
		Genode::memcpy(raw_config_descriptor, packet_content,
		               config_descriptor.total_length);

		usb_connection.tx_channel()->sigh_ack_avail(ack_avail_signal_dispatcher);
#endif
	}

	~Usb_device()
	{
		free(raw_config_descriptor);
	}

#if 0
	void ack_avail(unsigned)
	{
		while (usb_connection.source()->ack_avail()) {

			Usb::Packet_descriptor p =
				usb_connection.source()->get_acked_packet();

			Completion *completion = static_cast<Completion*>(p.completion);
			struct usbi_transfer *itransfer = completion->itransfer;
			destroy(Genode::env()->heap(), completion);

			if (!p.succeded) {
				Genode::error("USB transfer failed");
				itransfer->transferred = -1;
				usb_connection.source()->release_packet(p);
				usbi_signal_transfer_completion(itransfer);
				continue;
			}

			char *packet_content = usb_connection.source()->packet_content(p);

			struct libusb_transfer *transfer =
				USBI_TRANSFER_TO_LIBUSB_TRANSFER(itransfer);

			switch (transfer->type) {

				case LIBUSB_TRANSFER_TYPE_CONTROL: {

					itransfer->transferred = p.control.actual_size;

					struct libusb_control_setup *setup =
						(struct libusb_control_setup*)transfer->buffer;

					if ((setup->bmRequestType & LIBUSB_ENDPOINT_DIR_MASK) ==
					    LIBUSB_ENDPOINT_IN) {
						Genode::memcpy(transfer->buffer + LIBUSB_CONTROL_SETUP_SIZE,
						               packet_content, p.control.actual_size);
					}

					break;
				}

				case LIBUSB_TRANSFER_TYPE_BULK:
				case LIBUSB_TRANSFER_TYPE_BULK_STREAM:
				case LIBUSB_TRANSFER_TYPE_INTERRUPT: {

					itransfer->transferred = p.transfer.actual_size;

					if (IS_XFERIN(transfer))
						Genode::memcpy(transfer->buffer, packet_content,
						               p.transfer.actual_size);

					break;
				}

				default:
					Genode::error(__PRETTY_FUNCTION__,
					              ": unsupported transfer type");
					usb_connection.source()->release_packet(p);
					continue;
			}

			usb_connection.source()->release_packet(p);

			usbi_signal_transfer_completion(itransfer);
		}
	}
#endif
};

#if 0
static void *signal_handler_thread_entry(void *)
{
	for (;;) {

		Genode::Signal signal = signal_receiver().wait_for_signal();	

		Genode::Signal_dispatcher_base *dispatcher =
			static_cast<Genode::Signal_dispatcher_base *>(signal.context());

		dispatcher->dispatch(signal.num());
	}
}
#endif

static int genode_init(struct libusb_context* ctx)
{
#if 0
	pthread_t signal_handler_thread;
	if (pthread_create(&signal_handler_thread, nullptr,
	                   signal_handler_thread_entry, nullptr) < 0) {
	    Genode::error("Could not create signal handler thread");
		return LIBUSB_ERROR_OTHER;
	}
#endif
	return LIBUSB_SUCCESS;
}


static void genode_exit(void) { }


int genode_get_device_list(struct libusb_context *ctx,
                           struct discovered_devs **discdevs)
{
	unsigned long session_id;
	struct libusb_device *dev;

	uint8_t busnum = 1;
	uint8_t devaddr = 1;

	session_id = busnum << 8 | devaddr;
	usbi_dbg("busnum %d devaddr %d session_id %ld", busnum, devaddr,
		session_id);

	dev = usbi_get_device_by_session_id(ctx, session_id);

	if (!dev) {
	
		usbi_dbg("allocating new device for %d/%d (session %ld)",
		 	 busnum, devaddr, session_id);
		dev = usbi_alloc_device(ctx, session_id);
		if (!dev)
			return LIBUSB_ERROR_NO_MEM;

		/* initialize device structure */
		dev->bus_number = busnum;
		dev->device_address = devaddr;

		/* FIXME: find place to free the allocated memory */
		Usb_device *usb_device = new (Genode::env()->heap()) Usb_device;
		*(Usb_device**)dev->os_priv = usb_device;

		int result = usbi_sanitize_device(dev);
		if (result < 0) {
			libusb_unref_device(dev);
			return result;
		}

	} else {
		usbi_dbg("session_id %ld already exists", session_id);
	}

	if (discovered_devs_append(*discdevs, dev) == NULL) {
		libusb_unref_device(dev);
		return LIBUSB_ERROR_NO_MEM;
	}

	libusb_unref_device(dev);

	return LIBUSB_SUCCESS;
}


static int genode_open(struct libusb_device_handle *dev_handle)
{
	return LIBUSB_SUCCESS;
}


static void genode_close(struct libusb_device_handle *dev_handle)
{
}


static int genode_get_device_descriptor(struct libusb_device *device,
                                        unsigned char* buffer,
                                        int *host_endian)
{
Genode::log(__PRETTY_FUNCTION__);
	Usb_device *usb_device = *(Usb_device**)device->os_priv;

	Genode::memcpy(buffer, &usb_device->device_descriptor,
	               sizeof(libusb_device_descriptor));

	*host_endian = 0;

	return LIBUSB_SUCCESS;
}


static int genode_get_config_descriptor(struct libusb_device *device,
                                        uint8_t config_index,
                                        unsigned char *buffer,
                                        size_t len,
                                        int *host_endian)
{
Genode::log(__PRETTY_FUNCTION__);

	if (config_index != 0) {
		Genode::error(__PRETTY_FUNCTION__,
		              ": only the first configuration is supported");
		return LIBUSB_ERROR_NOT_SUPPORTED;
	}

	Usb_device *usb_device = *(Usb_device**)device->os_priv;

	Genode::memcpy(buffer, usb_device->raw_config_descriptor, len);

	*host_endian = 0;

	return len;
}


static int genode_get_active_config_descriptor(struct libusb_device *device,
                                               unsigned char *buffer,
                                               size_t len,
                                               int *host_endian)
{
	/* only configuration 0 is currently supported */
	return genode_get_config_descriptor(device, 0, buffer, len, host_endian);
}


static int genode_set_configuration(struct libusb_device_handle *dev_handle,
                                    int config)
{
	Genode::error(__PRETTY_FUNCTION__,
	              ": not implemented (return address: ",
	              Genode::Hex((Genode::addr_t)__builtin_return_address(0)),
	              ") \n");
	return LIBUSB_ERROR_NOT_SUPPORTED;
}


static int genode_claim_interface(struct libusb_device_handle *dev_handle,
                                  int interface_number)
{
Genode::log(__PRETTY_FUNCTION__);
#if 0
	Usb_device *usb_device = *(Usb_device**)dev_handle->dev->os_priv;

	try {
		usb_device->usb_connection.claim_interface(interface_number);
	} catch (Usb::Session::Interface_not_found) {
		Genode::error(__PRETTY_FUNCTION__, ": interface not found");
		return LIBUSB_ERROR_NOT_FOUND;
	} catch (Usb::Session::Interface_already_claimed) {
		Genode::error(__PRETTY_FUNCTION__, ": interface already claimed");
		return LIBUSB_ERROR_BUSY;
	} catch (...) {
		Genode::error(__PRETTY_FUNCTION__, ": unknown exception");
		return LIBUSB_ERROR_OTHER;
	}
#endif
	return LIBUSB_SUCCESS;
}


static int genode_release_interface(struct libusb_device_handle *dev_handle,
                                    int interface_number)
{
Genode::log(__PRETTY_FUNCTION__);
#if 0
	Usb_device *usb_device = *(Usb_device**)dev_handle->dev->os_priv;

	try {
		usb_device->usb_connection.release_interface(interface_number);
	} catch (Usb::Session::Interface_not_found) {
		Genode::error(__PRETTY_FUNCTION__, ": interface not found");
		return LIBUSB_ERROR_NOT_FOUND;
	} catch (...) {
		Genode::error(__PRETTY_FUNCTION__, ": unknown exception");
		return LIBUSB_ERROR_OTHER;
	}
#endif
	return LIBUSB_SUCCESS;
}


static int genode_set_interface_altsetting(struct libusb_device_handle* dev_handle,
                                           int interface_number,
                                           int altsetting)
{
	Genode::error(__PRETTY_FUNCTION__,
	              ": not implemented (return address: ",
	              Genode::Hex((Genode::addr_t)__builtin_return_address(0)),
	              ") \n");
	return LIBUSB_ERROR_NOT_SUPPORTED;
}


static int genode_submit_transfer(struct usbi_transfer * itransfer)
{
Genode::log(__PRETTY_FUNCTION__);
#if 0
	struct libusb_transfer *transfer =
		USBI_TRANSFER_TO_LIBUSB_TRANSFER(itransfer);

	Usb_device *usb_device = *(Usb_device**)transfer->dev_handle->dev->os_priv;

	switch (transfer->type) {

		case LIBUSB_TRANSFER_TYPE_CONTROL: {

			struct libusb_control_setup *setup =
				(struct libusb_control_setup*)transfer->buffer;

			Usb::Packet_descriptor p;

			try {
				p = usb_device->usb_connection.source()->alloc_packet(transfer->length);
			} catch (Usb::Session::Tx::Source::Packet_alloc_failed) {
				Genode::error(__PRETTY_FUNCTION__, ": packet allocation failed");
				return LIBUSB_ERROR_BUSY;
			}

			p.completion           = new (Genode::env()->heap()) Completion(itransfer);

			p.type                 = Usb::Packet_descriptor::CTRL;
			p.control.request      = setup->bRequest;
			p.control.request_type = setup->bmRequestType;
			p.control.value        = setup->wValue;
			p.control.index        = setup->wIndex;
			p.control.timeout      = transfer->timeout;

			if ((setup->bmRequestType & LIBUSB_ENDPOINT_DIR_MASK) ==
			    LIBUSB_ENDPOINT_OUT) {

				char *packet_content =
					usb_device->usb_connection.source()->packet_content(p);

				Genode::memcpy(packet_content,
				               transfer->buffer + LIBUSB_CONTROL_SETUP_SIZE,
				               setup->wLength);
			}

			try {
				usb_device->usb_connection.source()->submit_packet(p);
			} catch (...) {
				Genode::error(__PRETTY_FUNCTION__,
				              ": could not submit packet");
			}

			return LIBUSB_SUCCESS;
		}

		case LIBUSB_TRANSFER_TYPE_BULK:
		case LIBUSB_TRANSFER_TYPE_BULK_STREAM:
		case LIBUSB_TRANSFER_TYPE_INTERRUPT: {
		
			if (IS_XFEROUT(transfer) &&
		    	transfer->flags & LIBUSB_TRANSFER_ADD_ZERO_PACKET) {
		    	Genode::error(__PRETTY_FUNCTION__,
		    	              ": zero packet not supported");
				return LIBUSB_ERROR_NOT_SUPPORTED;
			}

			Usb::Packet_descriptor p;
			
			try {
				p = usb_device->usb_connection.source()->alloc_packet(transfer->length);
			} catch (Usb::Session::Tx::Source::Packet_alloc_failed) {
				Genode::error(__PRETTY_FUNCTION__,
				              ": packet allocation failed");
				return LIBUSB_ERROR_BUSY;
			}

			if (transfer->type == LIBUSB_TRANSFER_TYPE_INTERRUPT) {
				p.type = Usb::Packet_descriptor::IRQ;
				p.transfer.polling_interval =
					Usb::Packet_descriptor::DEFAULT_POLLING_INTERVAL;
			} else
				p.type = Usb::Packet_descriptor::BULK;

			p.completion  = new (Genode::env()->heap()) Completion(itransfer);
			p.transfer.ep = transfer->endpoint;

			if (IS_XFEROUT(transfer)) {
				char *packet_content =
					usb_device->usb_connection.source()->packet_content(p);
				Genode::memcpy(packet_content, transfer->buffer,
				               transfer->length);
			}

			try {
				usb_device->usb_connection.source()->submit_packet(p);
			} catch (...) {
				Genode::error(__PRETTY_FUNCTION__,
				              ": could not submit packet");
			}

			return LIBUSB_SUCCESS;
		}

		case LIBUSB_TRANSFER_TYPE_ISOCHRONOUS:
			Genode::error(__PRETTY_FUNCTION__,
			              ": isochronous transfer - not implemented");
			return LIBUSB_ERROR_NOT_SUPPORTED;

		default:
			usbi_err(TRANSFER_CTX(transfer),
				"unknown endpoint type %d", transfer->type);
			return LIBUSB_ERROR_INVALID_PARAM;
	}
#endif
}


static int genode_cancel_transfer(struct usbi_transfer * itransfer)
{
	return LIBUSB_ERROR_NOT_SUPPORTED;
}


static void genode_clear_transfer_priv(struct usbi_transfer * itransfer) { }


static int genode_handle_transfer_completion(struct usbi_transfer * itransfer)
{
	return usbi_handle_transfer_completion(itransfer,
	                                       LIBUSB_TRANSFER_COMPLETED);
}


static int genode_clock_gettime(int clkid, struct timespec *tp)
{
	switch (clkid) {
		case USBI_CLOCK_MONOTONIC:
			return clock_gettime(CLOCK_MONOTONIC, tp);
		case USBI_CLOCK_REALTIME:
			return clock_gettime(CLOCK_REALTIME, tp);
		default:
			return 	LIBUSB_ERROR_INVALID_PARAM;
	}
}


const struct usbi_os_backend genode_usb_raw_backend = {
	/*.name =*/ "Genode",
	/*.caps =*/ 0,
	/*.init =*/ genode_init,
	/*.exit =*/ genode_exit,
	/*.get_device_list =*/ genode_get_device_list,
	/*.hotplug_poll =*/ NULL,
	/*.open =*/ genode_open,
	/*.close =*/ genode_close,
	/*.get_device_descriptor =*/ genode_get_device_descriptor,
	/*.get_active_config_descriptor =*/ genode_get_active_config_descriptor,
	/*.get_config_descriptor =*/ genode_get_config_descriptor,
	/*.get_config_descriptor_by_value =*/ NULL,


	/*.get_configuration =*/ NULL,
	/*.set_configuration =*/ genode_set_configuration,
	/*.claim_interface =*/ genode_claim_interface,
	/*.release_interface =*/ genode_release_interface,

	/*.set_interface_altsetting =*/ genode_set_interface_altsetting,
	/*.clear_halt =*/ NULL,
	/*.reset_device =*/ NULL,

	/*.alloc_streams =*/ NULL,
	/*.free_streams =*/ NULL,

	/*.kernel_driver_active =*/ NULL,
	/*.detach_kernel_driver =*/ NULL,
	/*.attach_kernel_driver =*/ NULL,

	/*.destroy_device =*/ NULL,

	/*.submit_transfer =*/ genode_submit_transfer,
	/*.cancel_transfer =*/ genode_cancel_transfer,
	/*.clear_transfer_priv =*/ genode_clear_transfer_priv,

	/*.handle_events =*/ NULL,
	/*.handle_transfer_completion =*/ genode_handle_transfer_completion,

	/*.clock_gettime =*/ genode_clock_gettime,

#ifdef USBI_TIMERFD_AVAILABLE
	/*.get_timerfd_clockid =*/ NULL,
#endif

	/*.device_priv_size =*/ sizeof(Usb_device*),
	/*.device_handle_priv_size =*/ 0,
	/*.transfer_priv_size =*/ 0,
};
