#include <stdio.h>
#include <unistd.h>

#include <libusb.h>

int main()
{
	sleep(5);

	int r;
	libusb_context *ctx = NULL;
	libusb_device **devs;
	ssize_t cnt;
	int i, j, k, l;

	r = libusb_init(&ctx);

	printf("libusb_init(): %d\n", r);

	libusb_set_debug(ctx, 3);

	cnt = libusb_get_device_list(ctx, &devs);

	printf("libusb_get_device_list(): %zd\n", cnt);

	for (i = 0; i < cnt; i++) {

		struct libusb_device_descriptor device_descriptor;
		struct libusb_config_descriptor *config;
		const struct libusb_interface *interface;
		const struct libusb_interface_descriptor *interface_descriptor;
		const struct libusb_endpoint_descriptor *endpoint_descriptor;

		r = libusb_get_device_descriptor(devs[i], &device_descriptor);

		printf("\n");

		printf("libusb_get_device_descriptor(): %d\n", r);

		printf("\n");

		printf("device %d: bNumConfigurations: %d\n", i, (int)device_descriptor.bNumConfigurations);
		printf("device %d: bDeviceClass: %d\n", i, device_descriptor.bDeviceClass);
		printf("device %d: idVendor: 0x%04x\n", i, device_descriptor.idVendor);
		printf("device %d: idProduct: 0x%04x\n", i, device_descriptor.idProduct);

		r = libusb_get_config_descriptor(devs[i], 0, &config);

		printf("libusb_get_config_descriptor(): %d\n", r);

		if (r == LIBUSB_SUCCESS) {

			printf("device %d: bNumInterfaces: %d\n", i, config->bNumInterfaces);

			for (j = 0; j < config->bNumInterfaces; j++) {

				interface = &config->interface[j];

				printf("device %d interface %d: num_altsetting: %d\n", i, j, interface->num_altsetting);

				for (k = 0; k < interface->num_altsetting; k++) {

					interface_descriptor = &interface->altsetting[k];

					printf("device %d interface %d setting %d: bInterfaceNumber: %d\n", i, j, k, interface_descriptor->bInterfaceNumber);
					printf("device %d interface %d setting %d: bNumEndpoints: %d\n", i, j, k, interface_descriptor->bNumEndpoints);

					for (l = 0; l < interface_descriptor->bNumEndpoints; l++) {

						endpoint_descriptor = &interface_descriptor->endpoint[l];

						printf("device %d interface %d setting %d endpoint %d: bDescriptorType: %d\n", i, j, k, l, endpoint_descriptor->bDescriptorType);
						printf("device %d interface %d setting %d endpoint %d: bEndpointAddress: 0x%x\n", i, j, k, l, endpoint_descriptor->bEndpointAddress);

					}

				}

			}

			libusb_free_config_descriptor(config);
		}
	}

	/* ---------- */

	struct libusb_device_handle *device_handle;

	device_handle = libusb_open_device_with_vid_pid(ctx, 0x04e6/*0x1d6b*/, 0x5116/*0x0002*/);

	printf("libusb_open_device_with_vid_pid(): %p\n", device_handle);

	if (device_handle) {

		r = libusb_kernel_driver_active(device_handle, 0);

		printf("libusb_kernel_driver_active(): %d\n", r);

		r = libusb_claim_interface(device_handle, 0);

		printf("libusb_claim_interface(): %d\n", r);

		r = libusb_release_interface(device_handle, 0);

		printf("libusb_release_interface(): %d\n", r);

		libusb_close(device_handle);
	}

	/* ---------- */

	libusb_free_device_list(devs, 1);

	libusb_exit(ctx);

	return 0;
}
