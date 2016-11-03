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

enum { MAX_NUM_ENDPOINTS = 8 };

static int ep_fd[MAX_NUM_ENDPOINTS] = { -1, -1, -1, -1, -1, -1, -1, -1 };

struct Genode_transfer_priv
{
	int fd;
};

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

	struct libusb_device_descriptor  device_descriptor;
	struct libusb_config_descriptor  config_descriptor;
	char                            *raw_config_descriptor { nullptr };

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

		int ep0_fd = open("/usb/ep0", O_RDWR);

		/* XXX: temporary workaround while non-blocking mode is set as default */
		fcntl(ep0_fd, F_SETFL, O_RDWR);

		struct libusb_control_setup setup;
		
		/* read device descriptor */

		libusb_fill_control_setup((unsigned char*)&setup,
		                          LIBUSB_ENDPOINT_IN,
		                          LIBUSB_REQUEST_GET_DESCRIPTOR,
		                          (LIBUSB_DT_DEVICE << 8) | 0,
		                          0,
		                          sizeof(device_descriptor));

		ssize_t res;

		res = write(ep0_fd, &setup, sizeof(setup));

		Genode::log("write() returned ", res, ", calling read()");

		res = read(ep0_fd, &device_descriptor, sizeof(device_descriptor));

		Genode::log("read() returned ", res);

		/* read config descriptor */

		libusb_fill_control_setup((unsigned char*)&setup,
		                          LIBUSB_ENDPOINT_IN,
		                          LIBUSB_REQUEST_GET_DESCRIPTOR,
		                          (LIBUSB_DT_CONFIG << 8) | 0,
		                          0,
		                          sizeof(config_descriptor));
	
		res = write(ep0_fd, &setup, sizeof(setup));

		Genode::log("write() returned ", res, ", calling read()");

		res = read(ep0_fd, &config_descriptor, sizeof(config_descriptor));

		Genode::log("read() returned ", res);

		raw_config_descriptor = (char*)malloc(config_descriptor.wTotalLength);

		libusb_fill_control_setup((unsigned char*)&setup,
		                          LIBUSB_ENDPOINT_IN,
		                          LIBUSB_REQUEST_GET_DESCRIPTOR,
		                          (LIBUSB_DT_CONFIG << 8) | 0,
		                          0,
		                          config_descriptor.wTotalLength);
	
		res = write(ep0_fd, &setup, sizeof(setup));

		Genode::log("write() returned ", res, ", calling read()");

		res = read(ep0_fd, raw_config_descriptor, config_descriptor.wTotalLength);

		Genode::log("read() returned ", res);

		close(ep0_fd);
	}

	~Usb_device()
	{
		free(raw_config_descriptor);

		for (int i = 0; i < sizeof(ep_fd); i++)
			if (ep_fd[i] != -1)
				close(ep_fd[i]);
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

	struct libusb_transfer *transfer =
		USBI_TRANSFER_TO_LIBUSB_TRANSFER(itransfer);

	unsigned int ep = transfer->endpoint & LIBUSB_ENDPOINT_ADDRESS_MASK;

	Genode::log("submit_transfer(): itransfer: ", itransfer, ", ep: ", ep);

	if (ep > MAX_NUM_ENDPOINTS) {
		Genode::error("endpoint number ", ep, " not supported");
		return LIBUSB_ERROR_INVALID_PARAM;
	}

	if (ep_fd[ep] == -1) {

		char ep_filename[Genode::strlen("/usb/epX") + 1];

		Genode::snprintf(ep_filename, sizeof(ep_filename), "/usb/ep%c", '0' + ep);

		Genode::log("ep_filename: ", Genode::Cstring(ep_filename));

		ep_fd[ep] = open(ep_filename, O_RDWR);

		if (ep_fd[ep] == -1) {
			Genode::error("Could not open endpoint file ", Genode::Cstring(ep_filename));
			return LIBUSB_ERROR_IO;
		}

		/* XXX: temporary workaround while non-blocking mode is set as default */
		fcntl(ep_fd[ep], F_SETFL, O_RDONLY);

		usbi_add_pollfd(HANDLE_CTX(transfer->dev_handle),
		                ep_fd[ep], POLLIN | POLLOUT);
	}

	Usb_device *usb_device = *(Usb_device**)transfer->dev_handle->dev->os_priv;

	Genode_transfer_priv *transfer_priv = (Genode_transfer_priv*)usbi_transfer_get_os_priv(itransfer);
	transfer_priv->fd = ep_fd[ep];

	switch (transfer->type) {

		case LIBUSB_TRANSFER_TYPE_CONTROL: {
			Genode::log("control transfer");

			struct libusb_control_setup *setup =
				(struct libusb_control_setup*)transfer->buffer;

			ssize_t res = write(ep_fd[ep], transfer->buffer, transfer->length);

			Genode::log("write() returned ", res);

			return LIBUSB_SUCCESS;
		}
		case LIBUSB_TRANSFER_TYPE_BULK:
		case LIBUSB_TRANSFER_TYPE_BULK_STREAM:
		case LIBUSB_TRANSFER_TYPE_INTERRUPT: {
			Genode::log("bulk or interrupt transfer for ep ", ep);

			if (IS_XFEROUT(transfer) &&
		    	transfer->flags & LIBUSB_TRANSFER_ADD_ZERO_PACKET) {
		    	Genode::error(__PRETTY_FUNCTION__,
		    	              ": zero packet not supported");
				return LIBUSB_ERROR_NOT_SUPPORTED;
			}

			ssize_t res = write(ep_fd[ep], transfer->buffer, transfer->length);

			Genode::log("write() returned ", res);

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

static int genode_handle_events(struct libusb_context *ctx,
                                struct pollfd *fds, POLL_NFDS_TYPE nfds, int num_ready)
{
	Genode::log(__PRETTY_FUNCTION__);

	unsigned int i;
	struct usbi_transfer *itransfer = 0;

	usbi_mutex_lock(&ctx->open_devs_lock);

	Genode::log("handle_events(): nfds: ", nfds, ", num_ready: ", num_ready);

	for (i = 0; i < nfds && num_ready > 0; i++) {
		struct pollfd *pollfd = &fds[i];

		if (!pollfd->revents) {
			Genode::log("no poll event for fd ", pollfd->fd);
			continue;
		}

		Genode::log("poll event for fd ", pollfd->fd);

		num_ready--;		

		usbi_mutex_lock(&ctx->flying_transfers_lock);
		list_for_each_entry(itransfer, &ctx->flying_transfers, list, struct usbi_transfer) {
			Genode_transfer_priv *transfer_priv = (Genode_transfer_priv*)usbi_transfer_get_os_priv(itransfer);
			if (transfer_priv->fd == pollfd->fd) {
				Genode::log("handle_events(): found transfer ", itransfer, ", for fd ", pollfd->fd);
				break;
			}
		}
		usbi_mutex_unlock(&ctx->flying_transfers_lock);

		if (itransfer) {

			struct libusb_transfer *transfer =
				USBI_TRANSFER_TO_LIBUSB_TRANSFER(itransfer);

			switch (transfer->type) {

				case LIBUSB_TRANSFER_TYPE_CONTROL: {

					struct libusb_control_setup *setup =
						(struct libusb_control_setup*)transfer->buffer;

					if ((setup->bmRequestType & LIBUSB_ENDPOINT_DIR_MASK) ==
					    LIBUSB_ENDPOINT_IN) {
					    int res = read(pollfd->fd, transfer->buffer + LIBUSB_CONTROL_SETUP_SIZE,
					                   setup->wLength);
					    Genode::log("received ", res, " bytes");
					    itransfer->transferred = res;
					}

					break;
				}
				case LIBUSB_TRANSFER_TYPE_BULK:
				case LIBUSB_TRANSFER_TYPE_BULK_STREAM:
				case LIBUSB_TRANSFER_TYPE_INTERRUPT: {
					break;
				}
				default:
					Genode::error(__PRETTY_FUNCTION__,
					              ": unsupported transfer type");
			}

			usbi_handle_transfer_completion(itransfer, LIBUSB_TRANSFER_COMPLETED);

		} else
			Genode::error("genode_handle_events(): could not find transfer object");
	}
	
	usbi_mutex_unlock(&ctx->open_devs_lock);

	return LIBUSB_SUCCESS;
}

/* XXX: remove */
static int genode_handle_transfer_completion(struct usbi_transfer * itransfer)
{
	Genode::log(__PRETTY_FUNCTION__);
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

	/*.handle_events =*/ genode_handle_events,
	/*.handle_transfer_completion =*/ genode_handle_transfer_completion,

	/*.clock_gettime =*/ genode_clock_gettime,

#ifdef USBI_TIMERFD_AVAILABLE
	/*.get_timerfd_clockid =*/ NULL,
#endif

	/*.device_priv_size =*/ sizeof(Usb_device*),
	/*.device_handle_priv_size =*/ 0,
	/*.transfer_priv_size =*/ sizeof(Genode_transfer_priv),
};
