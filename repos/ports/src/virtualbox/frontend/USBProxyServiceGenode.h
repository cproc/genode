#ifndef ____H_USBPROXYSERVICEGENODE
#define ____H_USBPROXYSERVICEGENODE

#include <os/timed_semaphore.h>

#include <USBProxyService.h>

class USBProxyServiceGenode : public USBProxyService
{
	private:

		Genode::Timed_semaphore _wait_sem;

	public:

		USBProxyServiceGenode(Host *aHost) : USBProxyService(aHost) { }

		HRESULT	init()
		{
			RTLogPrintf("USBProxyServiceGenode::init()\n");

			/*
 	 	 	 * Start the poller thread.
 	 	 	 */
			return (HRESULT)start();
        }

        PUSBDEVICE getDevices()
        {
			RTLogPrintf("USBProxyServiceGenode::getDevices()\n");

	        USBDEVICE *Dev = (USBDEVICE *)RTMemAllocZ(sizeof(USBDEVICE));
			
			Dev->pNext = 0;
			Dev->pPrev = 0;
			Dev->pszManufacturer = 0;
			Dev->pszProduct = "USB Optical Mouse";
			Dev->pszSerialNumber = 0;
			Dev->pszAddress = "/dev/mouse";
			Dev->idVendor = 0x4b3;
			Dev->idProduct = 0x310c;
			Dev->bcdDevice = 512;
			Dev->bcdUSB = 512;
			Dev->bDeviceClass = 0x0;
			Dev->bDeviceSubClass = 0x0;
			Dev->bDeviceProtocol = 0x0;
			Dev->bNumConfigurations = 1;
			Dev->enmState = USBDEVICESTATE_UNUSED;
			Dev->enmSpeed = USBDEVICESPEED_LOW;
			Dev->u64SerialHash = 0;
			Dev->bBus = 1;
			Dev->bPort = 1;
			Dev->bDevNum = 3;

			return Dev;
        }

		int wait(RTMSINTERVAL aMillies)
		{
			RTLogPrintf("USBProxyServiceGenode::wait(): aMillies = %u\n", aMillies);

			/* TODO: wait for report signal */

			if (aMillies == RT_INDEFINITE_WAIT)
				_wait_sem.down();
			else
				try {
					_wait_sem.down(aMillies);
				} catch (...) { }

			return VINF_SUCCESS;
		}

		int interruptWait()
		{
			RTLogPrintf("USBProxyServiceGenode::interruptWait()\n");

			_wait_sem.up();

			return VINF_SUCCESS;
		}

		int captureDevice(HostUSBDevice *aDevice)
		{
			RTLogPrintf("USBProxyServiceGenode::captureDevice()\n");

			interruptWait();

			return VINF_SUCCESS;
		}

		bool updateDeviceState(HostUSBDevice *aDevice,
		                       PUSBDEVICE aUSBDevice,
		                       bool *aRunFilters,
		                       SessionMachine **aIgnoreMachine)
		{
			RTLogPrintf("USBProxyServiceGenode::updateDeviceState()\n");

    		return updateDeviceStateFake(aDevice, aUSBDevice, aRunFilters, aIgnoreMachine);
		}

};

#endif // ____H_USBPROXYSERVICEGENODE
