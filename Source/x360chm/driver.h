
//	HID Minidriver for Microsoft Xbox 360 Controller

#include	<wdm.h>
#include	<hidport.h>
#include	<usbdi.h>
#include	<usbdlib.h>

//----------------------------------------------------------------------------------------------
//	定数
//----------------------------------------------------------------------------------------------

#define	REGISTRY_PATH				L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\Xbox360Controller\\Parameters"

#define	CURRENT_SETTING				L"Current Setting"

#define	LIMITATION_NONE				0
#define	LIMITATION_H1B16			1
#define	LIMITATION_NO_RX_RY			2
#define	LIMITATION_NO_SLIDER		3
#define	MAX_LIMITATION_MODE			3

#define	DIRECTIONAL_PAD_UP			0x0001
#define	DIRECTIONAL_PAD_DOWN		0x0002
#define	DIRECTIONAL_PAD_LEFT		0x0004
#define	DIRECTIONAL_PAD_RIGHT		0x0008
#define	START_BUTTON				0x0010
#define	BACK_BUTTON					0x0020
#define	LEFT_STICK_BUTTON			0x0040
#define	RIGHT_STICK_BUTTON			0x0080
#define	LB_BUTTON					0x0100
#define	RB_BUTTON					0x0200
#define	XBOX_GUIDE_BUTTON			0x0400
#define	A_BUTTON					0x1000
#define	B_BUTTON					0x2000
#define	X_BUTTON					0x4000
#define	Y_BUTTON					0x8000

#define	X_AXIS						0
#define	Y_AXIS						1
#define	X_ROTATION					2
#define	Y_ROTATION					3

#define	Z_AXIS						0
#define	Z_ROTATION					1
#define	SLIDER1						2
#define	SLIDER2						3

#define	HAT_SWITCH1					0
#define	HAT_SWITCH2					1
#define	HAT_SWITCH3					2
#define	HAT_SWITCH4					3

#define	INDEX_LEFT_STICK			0
#define	INDEX_RIGHT_STICK			1

#define	INDEX_WAY_UP				0
#define	INDEX_WAY_DOWN				1
#define	INDEX_WAY_LEFT				2
#define	INDEX_WAY_RIGHT				3

#define	INDEX_LEFT_TRIGGER			0
#define	INDEX_RIGHT_TRIGGER			1

#define	INDEX_A_BUTTON				0
#define	INDEX_B_BUTTON				1
#define	INDEX_X_BUTTON				2
#define	INDEX_Y_BUTTON				3
#define	INDEX_LB_BUTTON				4
#define	INDEX_RB_BUTTON				5
#define	INDEX_START_BUTTON			6
#define	INDEX_BACK_BUTTON			7
#define	INDEX_LEFT_STICK_BUTTON		8
#define	INDEX_RIGHT_STICK_BUTTON	9
#define	INDEX_XBOX_GUIDE_BUTTON		10

#define	INDEX_LEFT_ACTUATOR			0
#define	INDEX_RIGHT_ACTUATOR		1

#define	USAGE_NONE					0
#define	USAGE_X_AXIS_Y_AXIS			1
#define	USAGE_X_AXIS_NONE			2
#define	USAGE_NONE_X_AXIS			3
#define	USAGE_Y_AXIS_X_AXIS			4
#define	USAGE_Y_AXIS_NONE			5
#define	USAGE_NONE_Y_AXIS			6
#define	USAGE_X_ROTATION_Y_ROTATION	7
#define	USAGE_X_ROTATION_NONE		8
#define	USAGE_NONE_X_ROTATION		9
#define	USAGE_Y_ROTATION_X_ROTATION	10
#define	USAGE_Y_ROTATION_NONE		11
#define	USAGE_NONE_Y_ROTATION		12
#define	USAGE_Z_AXIS_Z_ROTAION		13
#define	USAGE_Z_AXIS_SLIDER1		14
#define	USAGE_Z_AXIS_SLIDER2		15
#define	USAGE_Z_AXIS_NONE			16
#define	USAGE_NONE_Z_AXIS			17
#define	USAGE_Z_ROTATION_Z_AXIS		18
#define	USAGE_Z_ROTATION_SLIDER1	19
#define	USAGE_Z_ROTATION_SLIDER2	20
#define	USAGE_Z_ROTATION_NONE		21
#define	USAGE_NONE_Z_ROTATION		22
#define	USAGE_SLIDER1_Z_AXIS		23
#define	USAGE_SLIDER1_Z_ROTATION	24
#define	USAGE_SLIDER1_SLIDER2		25
#define	USAGE_SLIDER1_NONE			26
#define	USAGE_NONE_SLIDER1			27
#define	USAGE_SLIDER2_Z_AXIS		28
#define	USAGE_SLIDER2_Z_ROTATION	29
#define	USAGE_SLIDER2_SLIDER1		30
#define	USAGE_SLIDER2_NONE			31
#define	USAGE_NONE_SLIDER2			32
#define	USAGE_HAT_SWITCH1			33
#define	USAGE_HAT_SWITCH2			34
#define	USAGE_HAT_SWITCH3			35
#define	USAGE_HAT_SWITCH4			36
#define	USAGE_4WAY_BUTTON			37

#define	USAGE_X_AXIS_PLUS			1
#define	USAGE_Y_AXIS_PLUS			2
#define	USAGE_X_ROTATION_PLUS		3
#define	USAGE_Y_ROTATION_PLUS		4
#define	USAGE_X_AXIS_MINUS			5
#define	USAGE_Y_AXIS_MINUS			6
#define	USAGE_X_ROTATION_MINUS		7
#define	USAGE_Y_ROTATION_MINUS		8
#define	USAGE_Z_AXIS_PLUS			9
#define	USAGE_Z_ROTATION_PLUS		10
#define	USAGE_SLIDER1_PLUS			11
#define	USAGE_SLIDER2_PLUS			12
#define	USAGE_Z_AXIS_MINUS			13
#define	USAGE_Z_ROTATION_MINUS		14
#define	USAGE_SLIDER1_MINUS			15
#define	USAGE_SLIDER2_MINUS			16
#define	USAGE_BUTTON_1				17
#define	USAGE_BUTTON_2				18
#define	USAGE_BUTTON_3				19
#define	USAGE_BUTTON_4				20
#define	USAGE_BUTTON_5				21
#define	USAGE_BUTTON_6				22
#define	USAGE_BUTTON_7				23
#define	USAGE_BUTTON_8				24
#define	USAGE_BUTTON_9				25
#define	USAGE_BUTTON_10				26
#define	USAGE_BUTTON_11				27
#define	USAGE_BUTTON_12				28
#define	USAGE_BUTTON_13				29
#define	USAGE_BUTTON_14				30
#define	USAGE_BUTTON_15				31
#define	USAGE_BUTTON_16				32
#define	USAGE_BUTTON_17				33
#define	USAGE_BUTTON_18				34
#define	USAGE_BUTTON_19				35
#define	USAGE_BUTTON_20				36
#define	USAGE_BUTTON_21				37
#define	USAGE_BUTTON_22				38
#define	USAGE_BUTTON_23				39
#define	USAGE_BUTTON_24				40
#define	USAGE_BUTTON_25				41
#define	USAGE_BUTTON_26				42
#define	USAGE_BUTTON_27				43
#define	USAGE_BUTTON_28				44
#define	USAGE_BUTTON_29				45
#define	USAGE_BUTTON_30				46
#define	USAGE_BUTTON_31				47
#define	USAGE_BUTTON_32				48

#define	DEAD_ZONE_TYPE_SQUARE		0
#define	DEAD_ZONE_TYPE_CIRCLE		1

#define	TRANSFORM_TYPE_NONE			0
#define	TRANSFORM_TYPE_SQUARE		1
#define	TRANSFORM_TYPE_CIRCLE		2

#define	REPORT_TYPE_NORMAL			0
#define	REPORT_TYPE_DIRECT			1

#define	Enable						TRUE
#define	Disable						FALSE

//----------------------------------------------------------------------------------------------
//	構造体
//----------------------------------------------------------------------------------------------

//	設定
typedef struct _SETTING
{
	UCHAR	StickUsage[2];
	BOOLEAN	StickReverseX[2];
	BOOLEAN	StickReverseY[2];
	SHORT	StickAngle[2];
	UCHAR	StickDeadZoneType[2];
	UCHAR	StickMinThreshold[2];
	UCHAR	StickMaxThreshold[2];
	UCHAR	StickHatSwitchThreshold[2];
	UCHAR	StickWayUsage[2][4];
	BOOLEAN	StickWayRapidFire[2][4];
	UCHAR	StickWayRapidFireSpeed[2][4];

	UCHAR	DirectionalPadUsage;
	BOOLEAN	DirectionalPadReverseX;
	BOOLEAN	DirectionalPadReverseY;
	SHORT	DirectionalPadAngle;
	UCHAR	DirectionalPadWayUsage[4];
	BOOLEAN	DirectionalPadWayRapidFire[4];
	UCHAR	DirectionalPadWayRapidFireSpeed[4];

	UCHAR	TriggerUsage[2];
	BOOLEAN	TriggerRapidFire[2];
	UCHAR	TriggerRapidFireSpeed[2];
	UCHAR	TriggerMinThreshold[2];
	UCHAR	TriggerMaxThreshold[2];
	UCHAR	TriggerButtonThreshold[2];

	UCHAR	ButtonUsage[11];
	BOOLEAN	ButtonRapidFire[11];
	UCHAR	ButtonRapidFireSpeed[11];

	UCHAR	RingLightPattern;

	BOOLEAN	Actuator;
	UCHAR	ActuatorLevel[2];
	UCHAR	ActuatorMinThreshold;
	UCHAR	ActuatorMaxThreshold;

	BOOLEAN	SlideSliderValue[4];
	BOOLEAN	SlideSliderCenterValue[4];
	BOOLEAN	ReverseSliderValue[4];
	BOOLEAN	DevicesArePolled;
	UCHAR	LimitationMode;

	UCHAR	StickTransformType[2];
	UCHAR	DirectionalPadTransformType;

} SETTING, *PSETTING;

//	入力用レポート
typedef struct _INPUT_REPORT
{
	UCHAR	Zero1;
	UCHAR	Length;
	USHORT	DigitalButton;
	UCHAR	LeftTrigger;
	UCHAR	RightTrigger;
	SHORT	LeftStickX;
	SHORT	LeftStickY;
	SHORT	RightStickX;
	SHORT	RightStickY;
	UCHAR	Zero2;
	UCHAR	Zero3;
	UCHAR	Zero4;
	UCHAR	Zero5;
	UCHAR	Zero6;
	UCHAR	Zero7;
} INPUT_REPORT, *PINPUT_REPORT;

//	出力用レポート
typedef struct _OUTPUT_REPORT
{
	UCHAR	Type;
	UCHAR	Length;
	UCHAR	Pattern;
	UCHAR	Left;
	UCHAR	Right;
	UCHAR	Zero1;
	UCHAR	Zero2;
	UCHAR	Zero3;
} OUTPUT_REPORT, *POUTPUT_REPORT;

//	HID 読込用レポート
typedef struct _HID_READ_REPORT
{
	SHORT	XAxis;
	SHORT	YAxis;
	SHORT	XRotation;
	SHORT	YRotation;
	UCHAR	ZAxis;
	UCHAR	ZRotation;
	UCHAR	Slider2;
	UCHAR	Slider1;
	CHAR	HatSwitch1;
	CHAR	HatSwitch2;
	CHAR	HatSwitch3;
	CHAR	HatSwitch4;
	ULONG	Buttons;
} HID_READ_REPORT, *PHID_READ_REPORT;

//	HID 書込用レポート
typedef struct _HID_WRITE_REPORT
{
	UCHAR	Type;
	UCHAR	Left;
	UCHAR	Right;
} HID_WRITE_REPORT, *PHID_WRITE_REPORT;

//	最後の状態
typedef struct _LAST_STATE
{
	BOOLEAN	StickWayLastState[2][4];
	UCHAR	StickWayRapidFireCount[2][4];
	BOOLEAN	DirectionalPadWayLastState[4];
	UCHAR	DirectionalPadWayRapidFireCount[4];
	BOOLEAN	TriggerLastState[2];
	UCHAR	TriggerRapidFireCount[2];
	BOOLEAN	ButtonLastState[11];
	UCHAR	ButtonRapidFireCount[11];
} LAST_STATE, *PLAST_STATE;

//	デバイス エクステンション
typedef struct _DEVICE_EXTENSION
{
	//	I/O 要求制御
	BOOLEAN							DriverState;
	BOOLEAN							DeviceState;
	LONG							ReferenceCount;
	KEVENT							ReferenceEnd;

	//	ワーカ スレッド
	BOOLEAN							ThreadState;
	PVOID							ThreadObjectIn;
	KEVENT							ThreadExitingIn;
	PVOID							ThreadObjectOut;
	KEVENT							ThreadExitingOut;

	//	USB
	PDEVICE_OBJECT					NextDeviceObject;
	USB_DEVICE_DESCRIPTOR			DeviceDescriptor;
	PUSB_CONFIGURATION_DESCRIPTOR	ConfigurationDescriptor;
	PUSB_INTERFACE_DESCRIPTOR *		InterfaceDescriptor;
	USBD_PIPE_HANDLE				InputPipeHandle;
	USBD_PIPE_HANDLE				OutputPipeHandle;

	//	レポート
	KSPIN_LOCK						LockReport;
	INPUT_REPORT					ControllerReport;
	OUTPUT_REPORT					ActuatorsReport;
	OUTPUT_REPORT					RingLightReport;
	BOOLEAN							ActuatorsReported;
	BOOLEAN							RingLightReported;

	//	設定
	BOOLEAN							ResetDevice;
	SETTING							Setting;

	//	連射
	LAST_STATE						LastState;

} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

//----------------------------------------------------------------------------------------------
//	マクロ定義
//----------------------------------------------------------------------------------------------

#define	GET_PHYSICAL_DEVICE_OBJECT( DO ) \
		( ((PHID_DEVICE_EXTENSION)(DO)->DeviceExtension)->PhysicalDeviceObject )

#define	GET_NEXT_DEVICE_OBJECT( DO ) \
		( ((PHID_DEVICE_EXTENSION)(DO)->DeviceExtension)->NextDeviceObject )

#define	GET_MINIDRIVER_DEVICE_EXTENSION( DO ) \
		( (PDEVICE_EXTENSION) \
		(((PHID_DEVICE_EXTENSION)(DO)->DeviceExtension)->MiniDeviceExtension) )

//----------------------------------------------------------------------------------------------
//	グローバル変数
//----------------------------------------------------------------------------------------------

//	driver.c
extern BOOLEAN	DevicesArePolled;
extern UCHAR	LimitationMode;

//----------------------------------------------------------------------------------------------
//	関数
//----------------------------------------------------------------------------------------------

//	add_device.c
DRIVER_ADD_DEVICE AddDevice;

//	pnp.c
__drv_dispatchType(IRP_MJ_PNP) DRIVER_DISPATCH PnP;
NTSTATUS EnableDevice( IN PDEVICE_EXTENSION );
NTSTATUS DisableDevice( IN PDEVICE_EXTENSION );

//	usb.c
NTSTATUS SetupPort( IN PDEVICE_OBJECT );
NTSTATUS GetDeviceDescriptor( IN PDEVICE_OBJECT, OUT PUSB_DEVICE_DESCRIPTOR );
NTSTATUS GetConfigurationDescriptor( IN PDEVICE_OBJECT, OUT PUSB_CONFIGURATION_DESCRIPTOR * );
NTSTATUS GetInterfaceDescriptor( IN PUSB_CONFIGURATION_DESCRIPTOR, OUT PUSB_INTERFACE_DESCRIPTOR * * );
NTSTATUS SelectConfiguration( IN PDEVICE_OBJECT, IN PUSB_CONFIGURATION_DESCRIPTOR, IN PUSB_INTERFACE_DESCRIPTOR *, OUT USBD_PIPE_HANDLE *, OUT USBD_PIPE_HANDLE * );
NTSTATUS DeselectConfiguration( IN PDEVICE_OBJECT );
NTSTATUS ResetPipe( IN PDEVICE_OBJECT, IN USBD_PIPE_HANDLE );
NTSTATUS AbortPipe( IN PDEVICE_OBJECT, IN USBD_PIPE_HANDLE );
NTSTATUS InputReport( IN PDEVICE_OBJECT, IN USBD_PIPE_HANDLE, OUT INPUT_REPORT * );
NTSTATUS OutputReport( IN PDEVICE_OBJECT, IN USBD_PIPE_HANDLE, OUT OUTPUT_REPORT * );

//	internal_device_control.c
__drv_dispatchType(IRP_MJ_INTERNAL_DEVICE_CONTROL) DRIVER_DISPATCH InternalDeviceControl;

//	setting.c
VOID LoadSetting( IN SETTING * );

//	report.c
VOID GenerateReadReport( IN INPUT_REPORT *, IN SETTING *, OUT HID_READ_REPORT *, IN OUT LAST_STATE * );
VOID GenerateWriteReport( IN HID_WRITE_REPORT *, IN SETTING *, OUT POUTPUT_REPORT );

//	math.c
LONG Sin( IN LONG );
LONG Cos( IN LONG );
LONG ATan2( IN LONG, IN LONG );
LONGLONG Sqrt( IN LONGLONG );

//	power.c
__drv_dispatchType(IRP_MJ_POWER) DRIVER_DISPATCH Power;

//	unload.c
DRIVER_UNLOAD DriverUnload;

//	common.c
VOID CompleteRequest( IN PIRP, IN NTSTATUS, IN ULONG );
NTSTATUS SendRequest( IN PDEVICE_OBJECT, IN PIRP );
NTSTATUS SendAndWaitRequest( IN PDEVICE_OBJECT, IN PIRP );
NTSTATUS SendPowerRequest( IN PDEVICE_OBJECT, IN PIRP );
NTSTATUS SendAndWaitPowerRequest( IN PDEVICE_OBJECT, IN PIRP );
NTSTATUS IncrementReference( IN PDEVICE_EXTENSION );
VOID DecrementReference( IN PDEVICE_EXTENSION );
VOID GetRegistryBinary( IN PWSTR, IN PWSTR, IN CHAR *, IN LONG );

