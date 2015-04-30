#ifndef ___VBox_com_defs_h
#define ___VBox_com_defs_h

#include <iprt/types.h>


/* from C binding header? */

typedef       bool                 PRBool;
typedef       uint8_t              PRUint8;
typedef       int16_t              PRInt16;
typedef       uint16_t             PRUint16;
typedef       int32_t              PRInt32;
typedef       uint32_t             PRUint32;
typedef       int64_t              PRInt64;
typedef       uint64_t             PRUint64;
typedef       PRUint16             PRUnichar;

/* nscore.h */
typedef       PRUint32             nsresult;

/* nsID.h */
typedef struct { char x [sizeof(RTUUID)]; } nsID; /* differs from original */

/* nsBase.h */

/* Standard "it worked" return value */
#define NS_OK                              0

#define NS_ERROR_BASE                      ((nsresult) 0xC1F30000)

/* Returned by a not implemented function */
#define NS_ERROR_NOT_IMPLEMENTED           ((nsresult) 0x80004001L)

/* Returned when a given interface is not supported. */
#define NS_NOINTERFACE                     ((nsresult) 0x80004002L)
#define NS_ERROR_NO_INTERFACE              NS_NOINTERFACE

#define NS_ERROR_INVALID_POINTER           ((nsresult) 0x80004003L)
#define NS_ERROR_NULL_POINTER              NS_ERROR_INVALID_POINTER

/* Returned when a function aborts */
#define NS_ERROR_ABORT                     ((nsresult) 0x80004004L)

/* Returned when a function fails */
#define NS_ERROR_FAILURE                   ((nsresult) 0x80004005L)

/* Returned when an IPC fails */
#define NS_ERROR_CALL_FAILED               ((nsresult) 0x800706beL)

/* Returned when an unexpected error occurs */
#define NS_ERROR_UNEXPECTED                ((nsresult) 0x8000ffffL)

/* Returned when a memory allocation fails */
#define NS_ERROR_OUT_OF_MEMORY             ((nsresult) 0x8007000eL)

/* Returned when an illegal value is passed */
#define NS_ERROR_ILLEGAL_VALUE             ((nsresult) 0x80070057L)
#define NS_ERROR_INVALID_ARG               NS_ERROR_ILLEGAL_VALUE


/* defs.h */

#define ATL_NO_VTABLE
#define DECLARE_CLASSFACTORY()
#define DECLARE_CLASSFACTORY_SINGLETON(X)
#define DECLARE_REGISTRY_RESOURCEID(X)
#define DECLARE_NOT_AGGREGATABLE(X)
#define DECLARE_PROTECT_FINAL_CONSTRUCT(Y)
#define BEGIN_COM_MAP(X)
#define COM_INTERFACE_ENTRY(X)
#define COM_INTERFACE_ENTRY2(X,Y)
#define END_COM_MAP()

#define HRESULT nsresult
#define SUCCEEDED(X) ((X) == VINF_SUCCESS)
#define FAILED(X) ((X) != VINF_SUCCESS)

#define FAILED_DEAD_INTERFACE(rc)  (   (rc) == NS_ERROR_ABORT \
                                    || (rc) == NS_ERROR_CALL_FAILED \
                                   )

typedef       PRBool               BOOL;
typedef       PRUint8              BYTE;
typedef       PRInt16              SHORT;
typedef       PRUint16             USHORT;
typedef       PRInt32              LONG;
typedef       PRUint32             ULONG;
typedef       PRInt64              LONG64;
typedef       PRUint64             ULONG64;

#define       FALSE                false
#define       TRUE                 true

typedef       wchar_t              OLECHAR;
typedef       PRUnichar           *BSTR;
typedef const PRUnichar           *CBSTR;
typedef       CBSTR                IN_BSTR;

#define       GUID                 nsID
#define       IN_GUID              const nsID &
#define       OUT_GUID             nsID **

#define COMGETTER(n)    Get##n
#define COMSETTER(n)    Set##n

#define ComSafeArrayIn(aType, aArg)         unsigned aArg##Size, aType *aArg
#define ComSafeArrayInIsNull(aArg)          ((aArg) == NULL)
#define ComSafeArrayInArg(aArg)             aArg##Size, aArg

#define ComSafeArrayOut(aType, aArg)        PRUint32 *aArg##Size, aType **aArg
#define ComSafeArrayOutIsNull(aArg)         ((aArg) == NULL)
#define ComSafeArrayOutArg(aArg)            aArg##Size, aArg

#define ComSafeGUIDArrayIn(aArg)            PRUint32 aArg##Size, const nsID **aArg
#define ComSafeGUIDArrayInArg(aArg)         ComSafeArrayInArg(aArg)

/* OLE error codes */
#define S_OK                ((nsresult)NS_OK)
#define E_UNEXPECTED        NS_ERROR_UNEXPECTED
#define E_NOTIMPL           NS_ERROR_NOT_IMPLEMENTED
#define E_OUTOFMEMORY       NS_ERROR_OUT_OF_MEMORY
#define E_INVALIDARG        NS_ERROR_INVALID_ARG
#define E_NOINTERFACE       NS_ERROR_NO_INTERFACE
#define E_POINTER           NS_ERROR_NULL_POINTER
#define E_ABORT             NS_ERROR_ABORT
#define E_FAIL              NS_ERROR_FAILURE
/* Note: a better analog for E_ACCESSDENIED would probably be
 * NS_ERROR_NOT_AVAILABLE, but we want binary compatibility for now. */
#define E_ACCESSDENIED      ((nsresult)0x80070005L)

#define STDMETHOD(X) virtual HRESULT X
#define STDMETHODIMP HRESULT

inline GUID& stuffstuff() {
	static GUID stuff;
	return stuff;
}
#define COM_IIDOF(X) stuffstuff()

#define COM_STRUCT_OR_CLASS(I) class I

extern "C"
{
	BSTR SysAllocString(const OLECHAR* sz);
	BSTR SysAllocStringByteLen(char *psz, unsigned int len);
	BSTR SysAllocStringLen(const OLECHAR *pch, unsigned int cch);
	void SysFreeString(BSTR bstr);
	unsigned int SysStringByteLen(BSTR bstr);
	unsigned int SysStringLen(BSTR bstr);
}

namespace com {

#define VBOX_SCRIPTABLE_IMPL(iface)                     \
    public iface

#define VBOX_DEFAULT_INTERFACE_ENTRIES(X)

} /* namespace com */

#if 0
#define FAILED_DEAD_INTERFACE(rc)  (   (rc) != VINF_SUCCESS )



#define getStaticClassIID() stuffstuff()



/* capiidl.xsl */
typedef enum VARTYPE
{
    VT_I2 = 2,
    VT_I4 = 3,
    VT_BSTR = 8,
    VT_DISPATCH = 9,
    VT_BOOL = 11,
    VT_UNKNOWN = 13,
    VT_I1 = 16,
    VT_UI1 = 17,
    VT_UI2 = 18,
    VT_UI4 = 19,
    VT_I8 = 20,
    VT_UI8 = 21,
    VT_HRESULT = 25
} VARTYPE;

typedef struct SAFEARRAY
{
    void *pv;
    ULONG c;
} SAFEARRAY;
#endif

#if 0
enum AccessMode_T
{
	AccessMode_ReadOnly,
	AccessMode_ReadWrite,
};

enum AdditionsFacilityClass_T
{
	AdditionsFacilityClass_None,
	AdditionsFacilityClass_Driver,
	AdditionsFacilityClass_Feature,
	AdditionsFacilityClass_Program,
	AdditionsFacilityClass_Service,
};

enum AdditionsFacilityStatus_T
{
	AdditionsFacilityStatus_Unknown,
	AdditionsFacilityStatus_Active,
};

enum AdditionsFacilityType_T
{
	AdditionsFacilityType_None,
	AdditionsFacilityType_AutoLogon,
	AdditionsFacilityType_Graphics,
	AdditionsFacilityType_Seamless,
	AdditionsFacilityType_VBoxService,
	AdditionsFacilityType_VBoxGuestDriver,
	AdditionsFacilityType_VBoxTrayClient,
};

enum CopyFileFlag_T { };
enum DeviceActivity_T  { };
enum FsObjType_T { };
enum FileStatus_T { };
enum FileSeekType_T { };
enum DragAndDropAction_T { };
enum GuestSessionStatus_T { };
enum GuestSessionWaitForFlag_T { };
enum GuestSessionWaitResult_T { };
enum DirectoryCreateFlag_T { };
enum DirectoryOpenFlag_T { };
enum DirectoryRemoveRecFlag_T { };
enum PathRenameFlag_T { };
enum SymlinkType_T { };
enum SymlinkReadFlag_T { };
enum AdditionsUpdateFlag_T { };
enum AdditionsRunLevelType_T
{
	AdditionsRunLevelType_None,
	AdditionsRunLevelType_System,
	AdditionsRunLevelType_Desktop,
	AdditionsRunLevelType_Userland,
};

enum GuestUserState_T {
};

enum MouseButtonState
{
	MouseButtonState_LeftButton = 0x01,
	MouseButtonState_RightButton = 0x02,
	MouseButtonState_MiddleButton = 0x04,
	MouseButtonState_WheelUp = 0x08,
	MouseButtonState_WheelDown = 0x10,
	MouseButtonState_XButton1 = 0x20,
	MouseButtonState_XButton2 = 0x40,
	MouseButtonState_MouseStateMask = 0x7F
};

enum GuestMouseEventMode_T
{
	GuestMouseEventMode_Absolute,
	GuestMouseEventMode_Relative,
};

enum ProcessPriority_T
{
	ProcessPriority_Default,
};

enum FramebufferPixelFormat
{
	FramebufferPixelFormat_Opaque = 0,
	FramebufferPixelFormat_FOURCC_RGB = 0x32424752,
};

enum GuestMonitorChangedEventType
{
	GuestMonitorChangedEventType_Enabled,
	GuestMonitorChangedEventType_Disabled,
	GuestMonitorChangedEventType_NewOrigin,
};

enum VBoxEventType_T
{
	VBoxEventType_Invalid = 0,
	VBoxEventType_Any = 1,
	VBoxEventType_Vetoable = 2,
	VBoxEventType_MachineEvent = 3,
	VBoxEventType_SnapshotEvent = 4,
	VBoxEventType_InputEvent = 5,
	VBoxEventType_LastWildcard = 31,
	VBoxEventType_OnMachineStateChanged = 32,
	VBoxEventType_OnMachineDataChanged = 33,
	VBoxEventType_OnExtraDataChanged = 34,
	VBoxEventType_OnExtraDataCanChange = 35,
	VBoxEventType_OnMediumRegistered = 36,
	VBoxEventType_OnMachineRegistered = 37,
	VBoxEventType_OnSessionStateChanged = 38,
	VBoxEventType_OnSnapshotTaken = 39,
	VBoxEventType_OnSnapshotDeleted = 40,
	VBoxEventType_OnSnapshotChanged = 41,
	VBoxEventType_OnGuestPropertyChanged = 42,
	VBoxEventType_OnMousePointerShapeChanged = 43,
	VBoxEventType_OnMouseCapabilityChanged = 44,
	VBoxEventType_OnKeyboardLedsChanged = 45,
	VBoxEventType_OnStateChanged = 46,
	VBoxEventType_OnAdditionsStateChanged = 47,
	VBoxEventType_OnNetworkAdapterChanged = 48,
	VBoxEventType_OnSerialPortChanged = 49,
	VBoxEventType_OnParallelPortChanged = 50,
	VBoxEventType_OnStorageControllerChanged = 51,
	VBoxEventType_OnMediumChanged = 52,
	VBoxEventType_OnVRDEServerChanged = 53,
	VBoxEventType_OnUSBControllerChanged = 54,
	VBoxEventType_OnUSBDeviceStateChanged = 55,
	VBoxEventType_OnSharedFolderChanged = 56,
	VBoxEventType_OnRuntimeError = 57,
	VBoxEventType_OnCanShowWindow = 58,
	VBoxEventType_OnShowWindow = 59,
	VBoxEventType_OnCPUChanged = 60,
	VBoxEventType_OnVRDEServerInfoChanged = 61,
	VBoxEventType_OnEventSourceChanged = 62,
	VBoxEventType_OnCPUExecutionCapChanged = 63,
	VBoxEventType_OnGuestKeyboard = 64,
	VBoxEventType_OnGuestMouse = 65,
	VBoxEventType_OnNATRedirect = 66,
	VBoxEventType_OnHostPCIDevicePlug = 67,
	VBoxEventType_OnVBoxSVCAvailabilityChanged = 68,
	VBoxEventType_OnBandwidthGroupChanged = 69,
	VBoxEventType_OnGuestMonitorChanged = 70,
	VBoxEventType_OnStorageDeviceChanged = 71,
	VBoxEventType_OnClipboardModeChanged = 72,
	VBoxEventType_OnDragAndDropModeChanged = 73,
	VBoxEventType_OnGuestMultiTouch = 74,
	VBoxEventType_Last = 75
};

enum ProcessStatus_T { };
enum ProcessInputStatus_T { };
enum ProcessInputFlag_T { };
enum ProcessWaitResult_T { };
enum ProcessWaitForFlag_T { };

enum ProcessCreateFlag_T
{
	ProcessCreateFlag_None,
};

enum SessionType_T
{
	SessionType_Null,
	SessionType_WriteLock,
	SessionType_Remote,
	SessionType_Shared,
};

enum MachineState_T {
	MachineState_Null,
	MachineState_Aborted,
	MachineState_Running,
	MachineState_Paused,
	MachineState_Teleporting,
	MachineState_LiveSnapshotting,
	MachineState_Stuck,
	MachineState_Starting,
	MachineState_Stopping,
	MachineState_Saving,
	MachineState_Restoring,
	MachineState_TeleportingPausedVM,
	MachineState_TeleportingIn,
	MachineState_RestoringSnapshot,
	MachineState_DeletingSnapshot,
	MachineState_SettingUp,
	MachineState_FaultTolerantSyncing,
	MachineState_PoweredOff,
	MachineState_Teleported,
	MachineState_Saved,
	MachineState_DeletingSnapshotOnline,
	MachineState_DeletingSnapshotPaused,
};

enum CleanupMode_T {
	CleanupMode_UnregisterOnly,
	CleanupMode_DetachAllReturnHardDisksOnly,
	CleanupMode_Full,
 };

enum CloneMode_T {
	CloneMode_MachineState,
	CloneMode_AllStates,
	CloneMode_MachineAndChildStates,
};

enum CloneOptions_T {
	CloneOptions_Link,
	CloneOptions_KeepAllMACs,
	CloneOptions_KeepNATMACs,
	CloneOptions_KeepDiskNames,
};

enum LockType_T {
	LockType_Shared,
	LockType_Write,
	LockType_VM,
};

enum SessionState_T {
	SessionState_Null,
	SessionState_Locked,
	SessionState_Spawning,
	SessionState_Unlocking,
	SessionState_Unlocked,
};

enum Reason_T
{
	Reason_Unspecified,
	Reason_HostSuspend,
	Reason_HostResume,
	Reason_HostBatteryLow,
};

enum MediumFormatCapabilities_T
{
	MediumFormatCapabilities_Uuid          = 0x01,
	MediumFormatCapabilities_CreateFixed   = 0x02,
	MediumFormatCapabilities_CreateDynamic = 0x04,
	MediumFormatCapabilities_Differencing  = 0x10,
	MediumFormatCapabilities_File          = 0x40
};

enum DataType_T {
	DataType_Int32,
	DataType_Int8,
	DataType_String,
};

enum DataFlags_T {
	DataFlags_Array, 
};

enum MediumVariant_T {
	MediumVariant_Standard,
	MediumVariant_Fixed,
	MediumVariant_Diff,
	MediumVariant_VmdkStreamOptimized,
	MediumVariant_NoCreateDir,
};
enum HostNetworkInterfaceType_T { };

enum NATAliasMode_T
{
	NATAliasMode_AliasLog = 0x1,
	NATAliasMode_AliasProxyOnly = 0x02,
	NATAliasMode_AliasUseSamePorts = 0x04,
};

enum MediumState_T {
    MediumState_NotCreated = 0,
    MediumState_Created = 1,
    MediumState_LockedRead = 2,
    MediumState_LockedWrite = 3,
    MediumState_Inaccessible = 4,
    MediumState_Creating = 5,
    MediumState_Deleting = 6
};

enum AuthType_T {
	AuthType_Null,
	AuthType_Guest,
	AuthType_External,
};

enum BIOSBootMenuMode_T {
	BIOSBootMenuMode_MessageAndMenu,
	BIOSBootMenuMode_Disabled,
	BIOSBootMenuMode_MenuOnly,
};

enum USBControllerType_T {
	USBControllerType_Null,
	USBControllerType_OHCI,
	USBControllerType_EHCI,
	USBControllerType_Last,
};

enum USBDeviceFilterAction_T {
	USBDeviceFilterAction_Null,
	USBDeviceFilterAction_Ignore,
	USBDeviceFilterAction_Hold,
};

enum DeviceType_T {
	DeviceType_Null,
	DeviceType_HardDisk,
	DeviceType_DVD,
	DeviceType_Floppy,
	DeviceType_Network,
	DeviceType_USB,
	DeviceType_SharedFolder,
};

enum MediumType_T {
	MediumType_Normal,
	MediumType_Immutable,
	MediumType_Writethrough,
	MediumType_Shareable,
	MediumType_Readonly,
	MediumType_MultiAttach,
};

enum NATProtocol_T {
	NATProtocol_TCP,
	NATProtocol_UDP,
};

enum NetworkAdapterType_T {
	NetworkAdapterType_Am79C970A,
	NetworkAdapterType_Am79C973,
	NetworkAdapterType_I82540EM,
	NetworkAdapterType_I82543GC,
	NetworkAdapterType_I82545EM,
	NetworkAdapterType_Virtio,
};

enum ProcessorFeature_T
{
	ProcessorFeature_HWVirtEx,
	ProcessorFeature_LongMode,
	ProcessorFeature_NestedPaging,
	ProcessorFeature_PAE,
};

enum CPUPropertyType_T
{
	CPUPropertyType_Null,
	CPUPropertyType_PAE,
	CPUPropertyType_Synthetic,
	CPUPropertyType_LongMode,
	CPUPropertyType_TripleFaultReset,
};

/* End of enum CPUPropertyType Declaration */
enum AudioDriverType_T {
	AudioDriverType_Null,
	AudioDriverType_WinMM,
	AudioDriverType_DirectSound,
	AudioDriverType_SolAudio,
	AudioDriverType_ALSA,
	AudioDriverType_Pulse,
	AudioDriverType_OSS,
	AudioDriverType_CoreAudio,
	AudioDriverType_MMPM,
};

enum PortMode_T {
	PortMode_Disconnected,
	PortMode_HostPipe,
	PortMode_HostDevice,
	PortMode_RawFile,
};

enum BandwidthGroupType_T {
	BandwidthGroupType_Null,
	BandwidthGroupType_Disk,
	BandwidthGroupType_Network,
};

enum ClipboardMode_T {
	ClipboardMode_Disabled,
	ClipboardMode_HostToGuest,
	ClipboardMode_GuestToHost,
	ClipboardMode_Bidirectional,
};

enum FaultToleranceState_T {
	FaultToleranceState_Inactive,
	FaultToleranceState_Master,
	FaultToleranceState_Standby,
};

enum AudioControllerType_T {
	AudioControllerType_AC97,
	AudioControllerType_HDA,
	AudioControllerType_SB16,
};

enum NetworkAttachmentType_T {
	NetworkAttachmentType_Null,
	NetworkAttachmentType_NAT,
	NetworkAttachmentType_Bridged,
	NetworkAttachmentType_Internal,
	NetworkAttachmentType_HostOnly,
	NetworkAttachmentType_Generic,
	NetworkAttachmentType_NATNetwork,
};

enum NetworkAdapterPromiscModePolicy_T {
	NetworkAdapterPromiscModePolicy_Deny,
	NetworkAdapterPromiscModePolicy_AllowNetwork,
	NetworkAdapterPromiscModePolicy_AllowAll,
};

enum StorageBus_T {
	StorageBus_Null,
	StorageBus_IDE,
	StorageBus_SATA,
	StorageBus_SAS,
	StorageBus_SCSI,
	StorageBus_Floppy,
};

enum FirmwareType_T {
	FirmwareType_BIOS,
	FirmwareType_EFI,
	FirmwareType_EFI32,
	FirmwareType_EFI64,
	FirmwareType_EFIDUAL,
};

enum GraphicsControllerType_T {
	GraphicsControllerType_Null,
	GraphicsControllerType_VBoxVGA,
	GraphicsControllerType_VMSVGA,
};

enum AutostopType_T {
	AutostopType_Disabled,
	AutostopType_SaveState,
	AutostopType_PowerOff,
	AutostopType_AcpiShutdown,
};

enum DragAndDropMode_T {
	DragAndDropMode_Disabled,
	DragAndDropMode_HostToGuest,
	DragAndDropMode_GuestToHost,
	DragAndDropMode_Bidirectional,
};

enum StorageControllerType_T {
	StorageControllerType_PIIX3,
	StorageControllerType_IntelAhci,
	StorageControllerType_LsiLogic,
	StorageControllerType_BusLogic,
	StorageControllerType_PIIX4,
	StorageControllerType_ICH6,
	StorageControllerType_I82078,
	StorageControllerType_LsiLogicSas,
};

enum KeyboardHIDType_T {
	KeyboardHIDType_None,
	KeyboardHIDType_PS2Keyboard,
	KeyboardHIDType_USBKeyboard,
	KeyboardHIDType_ComboKeyboard,
};

enum PointingHIDType_T {
	PointingHIDType_None,
	PointingHIDType_PS2Mouse,
	PointingHIDType_USBMouse,
	PointingHIDType_USBTablet,
	PointingHIDType_ComboMouse,
	PointingHIDType_USBMultiTouch,
};

enum ChipsetType_T {
	ChipsetType_Null,
	ChipsetType_ICH9,
	ChipsetType_PIIX3,
};

enum DhcpOpt_T {
	DhcpOpt_SubnetMask,
	DhcpOpt_Router,
};

enum SettingsVersion_T {
	SettingsVersion_Null,
	SettingsVersion_v1_3,
	SettingsVersion_v1_4,
	SettingsVersion_v1_5,
	SettingsVersion_v1_6,
	SettingsVersion_v1_7,
	SettingsVersion_v1_8,
	SettingsVersion_v1_9,
	SettingsVersion_v1_10,
	SettingsVersion_v1_11,
	SettingsVersion_v1_12,
	SettingsVersion_v1_13,
	SettingsVersion_v1_14,
	SettingsVersion_Future,
};

enum HWVirtExPropertyType_T
{
	HWVirtExPropertyType_Enabled,
	HWVirtExPropertyType_Force,
	HWVirtExPropertyType_NestedPaging,
	HWVirtExPropertyType_LargePages,
	HWVirtExPropertyType_VPID,
	HWVirtExPropertyType_UnrestrictedExecution,
};

enum USBDeviceState_T
{
    USBDeviceState_NotSupported = 0,
    USBDeviceState_Unavailable = 1,
    USBDeviceState_Busy = 2,
    USBDeviceState_Available = 3,
    USBDeviceState_Held = 4,
    USBDeviceState_Captured = 5
};
#endif

#endif /* !___VBox_com_defs_h */
