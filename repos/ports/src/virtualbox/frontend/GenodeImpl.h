#ifndef ____H_GENODEIMPL
#define ____H_GENODEIMPL

/* miscellanious missing/unimplemented stuff */

/* nsISupportsImpl.h */

#define NS_DECL_CLASSINFO(x)
#define NS_IMPL_THREADSAFE_ADDREF(x)
#define NS_IMPL_THREADSAFE_RELEASE(x)
#define NS_IMPL_QUERY_INTERFACE1_CI(x, y)
#define NS_IMPL_CI_INTERFACE_GETTER1(x, y)

/* GuestControlSvc.h */

typedef struct sdfkkd { } VRDEUSBDEVICEDESC;
typedef struct sdfsdf3 { } VBOXGUESTCTRLHOSTCBCTX, *PVBOXGUESTCTRLHOSTCBCTX;
typedef struct sdffsd2 { } VBOXGUESTCTRLHOSTCALLBACK, *PVBOXGUESTCTRLHOSTCALLBACK;
enum GUEST_FILE_SEEKTYPE { };

/* VBoxClipboardSvc.h */

#define VBOX_SHARED_CLIPBOARD_HOST_FN_SET_HEADLESS  2

/* intnet.h */

#define INTNET_MAX_NETWORK_NAME     128

enum INTNETTRUNKTYPE
{
    kIntNetTrunkType_WhateverNone,
	kIntNetTrunkType_NetFlt,
};

#if 0

#include <VBox/vmm/vmapi.h>
#include <VBox/com/com.h>
#include <VBox/com/string.h>
#include <VBox/com/Guid.h>
#include <VBox/com/AutoLock.h>
#include <VBox/com/EventQueue.h>

class Progress;
typedef Progress       IProgress;

class VirtualBoxErrorInfo;
typedef VirtualBoxErrorInfo IVirtualBoxErrorInfo;

#include <VBox/com/ErrorInfo.h>

#include <VBox/vmm/pdmifs.h>
#include <VBox/settings.h>

#include <iprt/uuid.h>
#include <iprt/fs.h>
#include <iprt/file.h>
#include <iprt/semaphore.h>

class Console;

#include "AutoCaller.h"
#include "VMMDev.h"

#include <map>
#include <list>
#include <vector>

#define TRUNKTYPE_WHATEVER "whatever"
#define TRUNKTYPE_NETFLT   "netflt"


class IHostNetworkInterface;
class IFramebufferOverlay;
class IHostUSBDeviceFilter;
class IHostVideoInputDevice;
class IVetoEvent;

class AdditionsFacility;
class AudioAdapter;
class BandwidthControl;
class BandwidthGroup;
class BIOSSettings;
class ConsoleMouseInterface;
class Display;
class DHCPServer;
class EmulatedUSB;
class EventSource;
class Framebuffer;
class Guest; 
class GuestDirectory;
class GuestFile;
class GuestFsObjInfo;
class GuestOSType;
class GuestProcess;
class GuestSession;
class Host;
class HostUSBDevice;
class Keyboard;
class Machine;
class MachineDebugger;
class Medium;
class MediumAttachment;
class MediumFormat;
class MediumLockList;
class Mouse;
class NATEngine;
class NATNetwork;
class NetworkAdapter;
class OUSBDevice;
class ParallelPort;
class PCIDeviceAttachment;
class SerialPort;
class Session;
class SessionMachine;
class SharedFolder;
class Snapshot;
class StorageController;
class SystemProperties;
class Token;
class USBController;
class USBDeviceFilter;
class USBDeviceFilters;
class VBoxEvent;
class VBoxEventDesc;
class VBoxVetoEvent;
class VirtualBox;
class VRDEServer;
class VRDEServerInfo;

class IStateChangedEvent
{
	public:
		void get_State(MachineState_T*);
}; 

typedef AdditionsFacility   IAdditionsFacility;
class IAppliance;
typedef AudioAdapter        IAudioAdapter;
typedef Console             IConsole;
typedef BandwidthControl    IBandwidthControl;
typedef BandwidthGroup      IBandwidthGroup;
typedef BIOSSettings        IBIOSSettings;
typedef Display             IDisplay;
typedef DHCPServer          IDHCPServer;
typedef EmulatedUSB         IEmulatedUSB;
typedef VBoxEvent           IEvent;
typedef VBoxVetoEvent       IExtraDataCanChangeEvent;
typedef EventSource         IEventSource;
typedef Framebuffer         IFramebuffer;
class IExtPackManager;
typedef Guest               IGuest;
typedef GuestDirectory      IGuestDirectory;
typedef GuestFile           IGuestFile;
typedef GuestFsObjInfo      IGuestFsObjInfo;
typedef GuestProcess        IGuestProcess;
typedef GuestSession        IGuestSession;
typedef GuestOSType         IGuestOSType;
typedef Host                IHost;
typedef Keyboard            IKeyboard;
typedef Machine             IMachine;
typedef MachineDebugger     IMachineDebugger;
typedef Medium              IMedium;
typedef MediumAttachment    IMediumAttachment;
typedef Mouse               IMouse;
typedef NetworkAdapter      INetworkAdapter;
typedef NATEngine           INATEngine;
typedef NATNetwork          INATNetwork;
typedef ParallelPort        IParallelPort;
class IPerformanceCollector;
typedef SerialPort          ISerialPort;
typedef Session             ISession;
typedef Session             IInternalSessionControl;
typedef SharedFolder        ISharedFolder;
typedef Snapshot            ISnapshot;
typedef StorageController   IStorageController;
typedef SystemProperties    ISystemProperties;
typedef Token               IToken;
typedef USBController       IUSBController;
typedef OUSBDevice          IUSBDevice;
typedef USBDeviceFilter     IUSBDeviceFilter;
typedef USBDeviceFilters    IUSBDeviceFilters;
typedef HostUSBDevice       IHostUSBDevice;
typedef VirtualBox          IVirtualBox;
typedef VRDEServer          IVRDEServer;
typedef VRDEServerInfo      IVRDEServerInfo;
typedef PCIDeviceAttachment IPCIDeviceAttachment;
typedef SessionMachine      IInternalMachineControl;

class IVirtualSystemDescription;

class IEventListener {

	public:

		HRESULT HandleEvent(IEvent * aEvent);
/*
	{
		VBoxEventType_T aType = VBoxEventType_Invalid;
		aEvent->COMGETTER(Type)(&aType);
		return mListener->HandleEvent(aType, aEvent);
	}
*/
 };

#if 0
template <typename X>
class DummyClass {
	public:
		void fireNATRedirectEvent(IEventSource *, CBSTR, ULONG, BOOL, CBSTR, NATProtocol_T, CBSTR, LONG, CBSTR, LONG);
		void fireNATNetworkChangedEvent(IEventSource*, CBSTR);
		void fireNATNetworkStartStopEvent(IEventSource*, CBSTR, BOOL);
		void fireNATNetworkSettingEvent(IEventSource*, CBSTR, BOOL, CBSTR, CBSTR, BOOL, BOOL);
		void fireNATNetworkPortForwardEvent(IEventSource*, CBSTR, BOOL, BOOL, CBSTR, NATProtocol_T, CBSTR, LONG, CBSTR, LONG);
		void fireHostNameResolutionConfigurationChangeEvent(IEventSource*);
		void fireHostPCIDevicePlugEvent(IEventSource*, CBSTR, BOOL, BOOL, IPCIDeviceAttachment*, CBSTR);
};

#define VBOX_SCRIPTABLE_IMPL(X) public DummyClass<X>
#endif

#include "BusAssignmentManager.h"

#include "MediumFormatImpl.h"
typedef MediumFormat IMediumFormat;

typedef struct IFsObjInfo { } IFsObjInfo;

#include "AdditionsFacilityImpl.h"
#include "BIOSSettingsImpl.h"
#include "EventImpl.h"
#include "FramebufferImpl.h"
#include "PCIDeviceAttachmentImpl.h"
#include "GuestImpl.h"
#include "GuestDirectoryImpl.h"
#include "GuestFsObjInfoImpl.h"
#include "GuestProcessImpl.h"
#include "GuestSessionImpl.h"
#include "NetworkServiceRunner.h"
#include "SnapshotImpl.h"
#include "ParallelPortImpl.h"
#include "MediumAttachmentImpl.h"
#include "StorageControllerImpl.h"
#include "USBControllerImpl.h"
#include "SerialPortImpl.h"

#include "NATEngineImpl.h"
#include "NetworkAdapterImpl.h"
#include "ProgressImpl.h"
#include "VRDEServerImpl.h"
#include "GuestOSTypeImpl.h"
#include "DHCPServerImpl.h"
#include "SystemPropertiesImpl.h"
#include "KeyboardImpl.h"
#include "DisplayImpl.h"
#include "NATNetworkImpl.h"
#include "VRDEServerImpl.h"
#include "MediumImpl.h"
#include "SessionImpl.h"
#include "HostImpl.h"
#include "USBDeviceImpl.h"

class VRDEServerInfo { };
class EmulatedUSB { };

class ConsoleVRDPServer
{
	public:
		void SendResize (void) {}
		void SendUpdate (unsigned uScreenId, void *pvUpdate, uint32_t cbUpdate) const;
		void SendUpdateBitmap (unsigned uScreenId, uint32_t x, uint32_t y, uint32_t w, uint32_t h) const;
		void Stop();
};

#endif

#endif // !____H_GENODEIMPL
