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
			Dev->pszSerialNumber = 0;
#if 0
			Dev->pszProduct = "USB Printer";
			Dev->pszAddress = "0482:0015";
			Dev->idVendor = 0x0482;
			Dev->idProduct = 0x0015;
#endif
#if 0
			Dev->pszProduct = "USB Scanner";
			Dev->pszAddress = "04a9:2220";
			Dev->idVendor = 0x04a9;
			Dev->idProduct = 0x2220;
#endif
#if 0
			Dev->pszProduct = "USB Stick";
			Dev->pszAddress = "05e3:0738";
			Dev->idVendor = 0x05e3;
			Dev->idProduct = 0x0738;
#endif
#if 0
			Dev->pszProduct = "USB Stick 2";
			Dev->pszAddress = "058f:6387";
			Dev->idVendor = 0x058f;
			Dev->idProduct = 0x6387;
#endif
#if 1
			Dev->pszProduct = "USB Keyboard";
			Dev->pszAddress = "046a:0001";
			Dev->idVendor = 0x046a;
			Dev->idProduct = 0x0001;
#endif
#if 0
			Dev->pszProduct = "USB Optical Mouse";
			Dev->pszAddress = "04b3:310c";
			Dev->idVendor = 0x04b3;
			Dev->idProduct = 0x310c;
#endif
			Dev->bcdDevice = 0;
			Dev->bcdUSB = 0;
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
