
//	HID Minidriver for Microsoft Xbox 360 Controller

#include	"driver.h"

//----------------------------------------------------------------------------------------------
//	構造体
//----------------------------------------------------------------------------------------------

typedef struct _READ_TIMER
{
	KDPC				ReadTimerDpc;
	KTIMER				ReadTimer;
	PIRP				Irp;
	PDEVICE_EXTENSION	DeviceExtension;
} READ_TIMER, *PREAD_TIMER;

//----------------------------------------------------------------------------------------------
//	グローバル変数
//----------------------------------------------------------------------------------------------

CHAR	ReportDescriptorNormal[]	=
{
	0x05, 0x01,			//	USAGE_PAGE (Generic Desktop)
	0x09, 0x05,			//	USAGE (Game Pad)
	0xa1, 0x01,			//	COLLECTION (Application)

	0x05, 0x01,			//		USAGE_PAGE (Generic Desktop)

	0x09, 0x30,			//		USAGE (X)
	0x09, 0x31,			//		USAGE (Y)
	0x09, 0x33,			//		USAGE (Rx)
	0x09, 0x34,			//		USAGE (Ry)
	0x16, 0x00, 0x80,	//		LOGICAL_MINIMUM (-32768)
	0x26, 0xff, 0x7f,	//		LOGICAL_MAXIMUM (32767)
	0x75, 0x10,			//		REPORT_SIZE (16)
	0x95, 0x04,			//		REPORT_COUNT (4)
	0x81, 0x02,			//		INPUT (Data,Var,Abs)

	0x09, 0x32,			//		USAGE (Z)
	0x09, 0x35,			//		USAGE (Rz)
	0x09, 0x36,			//		USAGE (Slider)
	0x09, 0x36,			//		USAGE (Slider)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x00,	//		LOGICAL_MAXIMUM (255)
	0x75, 0x08,			//		REPORT_SIZE (8)
	0x95, 0x04,			//		REPORT_COUNT (4)
	0x81, 0x02,			//		INPUT (Data,Var,Abs)

	0x09, 0x39,			//		USAGE (Hat switch)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x25, 0x07,			//		LOGICAL_MAXIMUM (7)
	0x75, 0x04,			//		REPORT_SIZE (4)
	0x95, 0x01,			//		REPORT_COUNT (1)
	0x81, 0x42,			//		INPUT (Data,Var,Abs,Null)

	0x75, 0x01,			//		REPORT_SIZE (1)
	0x95, 0x04,			//		REPORT_COUNT (4)
	0x81, 0x03,			//		INPUT (Cnst,Var,Abs)

	0x09, 0x39,			//		USAGE (Hat switch)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x25, 0x07,			//		LOGICAL_MAXIMUM (7)
	0x75, 0x04,			//		REPORT_SIZE (4)
	0x95, 0x01,			//		REPORT_COUNT (1)
	0x81, 0x42,			//		INPUT (Data,Var,Abs,Null)

	0x75, 0x01,			//		REPORT_SIZE (1)
	0x95, 0x04,			//		REPORT_COUNT (4)
	0x81, 0x03,			//		INPUT (Cnst,Var,Abs)

	0x09, 0x39,			//		USAGE (Hat switch)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x25, 0x07,			//		LOGICAL_MAXIMUM (7)
	0x75, 0x04,			//		REPORT_SIZE (4)
	0x95, 0x01,			//		REPORT_COUNT (1)
	0x81, 0x42,			//		INPUT (Data,Var,Abs,Null)

	0x75, 0x01,			//		REPORT_SIZE (1)
	0x95, 0x04,			//		REPORT_COUNT (4)
	0x81, 0x03,			//		INPUT (Cnst,Var,Abs)

	0x09, 0x39,			//		USAGE (Hat switch)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x25, 0x07,			//		LOGICAL_MAXIMUM (7)
	0x75, 0x04,			//		REPORT_SIZE (4)
	0x95, 0x01,			//		REPORT_COUNT (1)
	0x81, 0x42,			//		INPUT (Data,Var,Abs,Null)

	0x75, 0x01,			//		REPORT_SIZE (1)
	0x95, 0x04,			//		REPORT_COUNT (4)
	0x81, 0x03,			//		INPUT (Cnst,Var,Abs)

	0x05, 0x09,			//		USAGE_PAGE (Button)

	0x19, 0x01,			//		USAGE_MINIMUM (Button 1)
	0x29, 0x20,			//		USAGE_MAXIMUM (Button 32)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x25, 0x01,			//		LOGICAL_MAXIMUM (1)
	0x75, 0x01,			//		REPORT_SIZE (1)
	0x95, 0x20,			//		REPORT_COUNT (32)
	0x81, 0x02,			//		INPUT (Data,Var,Abs)

	0x06, 0x00, 0xff,	//		USAGE_PAGE (Vendor Defined Page 1)
	0x09, 0x01,			//		USAGE (Vendor Usage 1)
	0x09, 0x01,			//		USAGE (Vendor Usage 1)
	0x09, 0x01,			//		USAGE (Vendor Usage 1)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x00,	//		LOGICAL_MAXIMUM (255)
	0x75, 0x08,			//		REPORT_SIZE (8)
	0x95, 0x03,			//		REPORT_COUNT (3)
	0x91, 0x02,			//		OUTPUT (Data,Var,Abs)

	0x06, 0x00, 0xff,	//		USAGE_PAGE (Vendor Defined Page 1)
	0x09, 0x01,			//		USAGE (Vendor Usage 1)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x00,	//		LOGICAL_MAXIMUM (255)
	0x75, 0x08,			//		REPORT_SIZE (8)
	0x95, 0x14,			//		REPORT_COUNT (20)
	0xb1, 0x00,			//		FEATURE (Data,Ary,Abs)

	0xc0				//	END_COLLECTION
};

CHAR	ReportDescriptorH1B16[]	=
{
	0x05, 0x01,			//	USAGE_PAGE (Generic Desktop)
	0x09, 0x05,			//	USAGE (Game Pad)
	0xa1, 0x01,			//	COLLECTION (Application)

	0x05, 0x01,			//		USAGE_PAGE (Generic Desktop)

	0x09, 0x30,			//		USAGE (X)
	0x09, 0x31,			//		USAGE (Y)
	0x09, 0x33,			//		USAGE (Rx)
	0x09, 0x34,			//		USAGE (Ry)
	0x16, 0x00, 0x80,	//		LOGICAL_MINIMUM (-32768)
	0x26, 0xff, 0x7f,	//		LOGICAL_MAXIMUM (32767)
	0x75, 0x10,			//		REPORT_SIZE (16)
	0x95, 0x04,			//		REPORT_COUNT (4)
	0x81, 0x02,			//		INPUT (Data,Var,Abs)

	0x09, 0x32,			//		USAGE (Z)
	0x09, 0x35,			//		USAGE (Rz)
	0x09, 0x36,			//		USAGE (Slider)
	0x09, 0x36,			//		USAGE (Slider)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x00,	//		LOGICAL_MAXIMUM (255)
	0x75, 0x08,			//		REPORT_SIZE (8)
	0x95, 0x04,			//		REPORT_COUNT (4)
	0x81, 0x02,			//		INPUT (Data,Var,Abs)

	0x09, 0x39,			//		USAGE (Hat switch)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x25, 0x07,			//		LOGICAL_MAXIMUM (7)
	0x75, 0x04,			//		REPORT_SIZE (4)
	0x95, 0x01,			//		REPORT_COUNT (1)
	0x81, 0x42,			//		INPUT (Data,Var,Abs,Null)

	0x75, 0x01,			//		REPORT_SIZE (1)
	0x95, 0x1c,			//		REPORT_COUNT (28)
	0x81, 0x03,			//		INPUT (Cnst,Var,Abs)

	0x05, 0x09,			//		USAGE_PAGE (Button)

	0x19, 0x01,			//		USAGE_MINIMUM (Button 1)
	0x29, 0x10,			//		USAGE_MAXIMUM (Button 16)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x25, 0x01,			//		LOGICAL_MAXIMUM (1)
	0x75, 0x01,			//		REPORT_SIZE (1)
	0x95, 0x10,			//		REPORT_COUNT (16)
	0x81, 0x02,			//		INPUT (Data,Var,Abs)

	0x75, 0x01,			//		REPORT_SIZE (1)
	0x95, 0x10,			//		REPORT_COUNT (16)
	0x81, 0x03,			//		INPUT (Cnst,Var,Abs)

	0x06, 0x00, 0xff,	//		USAGE_PAGE (Vendor Defined Page 1)
	0x09, 0x01,			//		USAGE (Vendor Usage 1)
	0x09, 0x01,			//		USAGE (Vendor Usage 1)
	0x09, 0x01,			//		USAGE (Vendor Usage 1)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x00,	//		LOGICAL_MAXIMUM (255)
	0x75, 0x08,			//		REPORT_SIZE (8)
	0x95, 0x03,			//		REPORT_COUNT (3)
	0x91, 0x02,			//		OUTPUT (Data,Var,Abs)

	0x06, 0x00, 0xff,	//		USAGE_PAGE (Vendor Defined Page 1)
	0x09, 0x01,			//		USAGE (Vendor Usage 1)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x00,	//		LOGICAL_MAXIMUM (255)
	0x75, 0x08,			//		REPORT_SIZE (8)
	0x95, 0x14,			//		REPORT_COUNT (20)
	0xb1, 0x00,			//		FEATURE (Data,Ary,Abs)

	0xc0				//	END_COLLECTION
};

CHAR	ReportDescriptorNoRxRy[]	=
{
	0x05, 0x01,			//	USAGE_PAGE (Generic Desktop)
	0x09, 0x05,			//	USAGE (Game Pad)
	0xa1, 0x01,			//	COLLECTION (Application)

	0x05, 0x01,			//		USAGE_PAGE (Generic Desktop)

	0x09, 0x30,			//		USAGE (X)
	0x09, 0x31,			//		USAGE (Y)
	0x16, 0x00, 0x80,	//		LOGICAL_MINIMUM (-32768)
	0x26, 0xff, 0x7f,	//		LOGICAL_MAXIMUM (32767)
	0x75, 0x10,			//		REPORT_SIZE (16)
	0x95, 0x02,			//		REPORT_COUNT (2)
	0x81, 0x02,			//		INPUT (Data,Var,Abs)

	0x75, 0x10,			//		REPORT_SIZE (16)
	0x95, 0x02,			//		REPORT_COUNT (2)
	0x81, 0x03,			//		INPUT (Cnst,Var,Abs)

	0x09, 0x32,			//		USAGE (Z)
	0x09, 0x35,			//		USAGE (Rz)
	0x09, 0x36,			//		USAGE (Slider)
	0x09, 0x36,			//		USAGE (Slider)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x00,	//		LOGICAL_MAXIMUM (255)
	0x75, 0x08,			//		REPORT_SIZE (8)
	0x95, 0x04,			//		REPORT_COUNT (4)
	0x81, 0x02,			//		INPUT (Data,Var,Abs)

	0x09, 0x39,			//		USAGE (Hat switch)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x25, 0x07,			//		LOGICAL_MAXIMUM (7)
	0x75, 0x04,			//		REPORT_SIZE (4)
	0x95, 0x01,			//		REPORT_COUNT (1)
	0x81, 0x42,			//		INPUT (Data,Var,Abs,Null)

	0x75, 0x01,			//		REPORT_SIZE (1)
	0x95, 0x04,			//		REPORT_COUNT (4)
	0x81, 0x03,			//		INPUT (Cnst,Var,Abs)

	0x09, 0x39,			//		USAGE (Hat switch)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x25, 0x07,			//		LOGICAL_MAXIMUM (7)
	0x75, 0x04,			//		REPORT_SIZE (4)
	0x95, 0x01,			//		REPORT_COUNT (1)
	0x81, 0x42,			//		INPUT (Data,Var,Abs,Null)

	0x75, 0x01,			//		REPORT_SIZE (1)
	0x95, 0x04,			//		REPORT_COUNT (4)
	0x81, 0x03,			//		INPUT (Cnst,Var,Abs)

	0x09, 0x39,			//		USAGE (Hat switch)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x25, 0x07,			//		LOGICAL_MAXIMUM (7)
	0x75, 0x04,			//		REPORT_SIZE (4)
	0x95, 0x01,			//		REPORT_COUNT (1)
	0x81, 0x42,			//		INPUT (Data,Var,Abs,Null)

	0x75, 0x01,			//		REPORT_SIZE (1)
	0x95, 0x04,			//		REPORT_COUNT (4)
	0x81, 0x03,			//		INPUT (Cnst,Var,Abs)

	0x09, 0x39,			//		USAGE (Hat switch)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x25, 0x07,			//		LOGICAL_MAXIMUM (7)
	0x75, 0x04,			//		REPORT_SIZE (4)
	0x95, 0x01,			//		REPORT_COUNT (1)
	0x81, 0x42,			//		INPUT (Data,Var,Abs,Null)

	0x75, 0x01,			//		REPORT_SIZE (1)
	0x95, 0x04,			//		REPORT_COUNT (4)
	0x81, 0x03,			//		INPUT (Cnst,Var,Abs)

	0x05, 0x09,			//		USAGE_PAGE (Button)

	0x19, 0x01,			//		USAGE_MINIMUM (Button 1)
	0x29, 0x20,			//		USAGE_MAXIMUM (Button 32)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x25, 0x01,			//		LOGICAL_MAXIMUM (1)
	0x75, 0x01,			//		REPORT_SIZE (1)
	0x95, 0x20,			//		REPORT_COUNT (32)
	0x81, 0x02,			//		INPUT (Data,Var,Abs)

	0x06, 0x00, 0xff,	//		USAGE_PAGE (Vendor Defined Page 1)
	0x09, 0x01,			//		USAGE (Vendor Usage 1)
	0x09, 0x01,			//		USAGE (Vendor Usage 1)
	0x09, 0x01,			//		USAGE (Vendor Usage 1)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x00,	//		LOGICAL_MAXIMUM (255)
	0x75, 0x08,			//		REPORT_SIZE (8)
	0x95, 0x03,			//		REPORT_COUNT (3)
	0x91, 0x02,			//		OUTPUT (Data,Var,Abs)

	0x06, 0x00, 0xff,	//		USAGE_PAGE (Vendor Defined Page 1)
	0x09, 0x01,			//		USAGE (Vendor Usage 1)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x00,	//		LOGICAL_MAXIMUM (255)
	0x75, 0x08,			//		REPORT_SIZE (8)
	0x95, 0x14,			//		REPORT_COUNT (20)
	0xb1, 0x00,			//		FEATURE (Data,Ary,Abs)

	0xc0				//	END_COLLECTION
};

CHAR	ReportDescriptorNoSlider[]	=
{
	0x05, 0x01,			//	USAGE_PAGE (Generic Desktop)
	0x09, 0x05,			//	USAGE (Game Pad)
	0xa1, 0x01,			//	COLLECTION (Application)

	0x05, 0x01,			//		USAGE_PAGE (Generic Desktop)

	0x09, 0x30,			//		USAGE (X)
	0x09, 0x31,			//		USAGE (Y)
	0x09, 0x33,			//		USAGE (Rx)
	0x09, 0x34,			//		USAGE (Ry)
	0x16, 0x00, 0x80,	//		LOGICAL_MINIMUM (-32768)
	0x26, 0xff, 0x7f,	//		LOGICAL_MAXIMUM (32767)
	0x75, 0x10,			//		REPORT_SIZE (16)
	0x95, 0x04,			//		REPORT_COUNT (4)
	0x81, 0x02,			//		INPUT (Data,Var,Abs)

	0x09, 0x32,			//		USAGE (Z)
	0x09, 0x35,			//		USAGE (Rz)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x00,	//		LOGICAL_MAXIMUM (255)
	0x75, 0x08,			//		REPORT_SIZE (8)
	0x95, 0x02,			//		REPORT_COUNT (2)
	0x81, 0x02,			//		INPUT (Data,Var,Abs)

	0x75, 0x08,			//		REPORT_SIZE (8)
	0x95, 0x02,			//		REPORT_COUNT (2)
	0x81, 0x03,			//		INPUT (Cnst,Var,Abs)

	0x09, 0x39,			//		USAGE (Hat switch)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x25, 0x07,			//		LOGICAL_MAXIMUM (7)
	0x75, 0x04,			//		REPORT_SIZE (4)
	0x95, 0x01,			//		REPORT_COUNT (1)
	0x81, 0x42,			//		INPUT (Data,Var,Abs,Null)

	0x75, 0x01,			//		REPORT_SIZE (1)
	0x95, 0x04,			//		REPORT_COUNT (4)
	0x81, 0x03,			//		INPUT (Cnst,Var,Abs)

	0x09, 0x39,			//		USAGE (Hat switch)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x25, 0x07,			//		LOGICAL_MAXIMUM (7)
	0x75, 0x04,			//		REPORT_SIZE (4)
	0x95, 0x01,			//		REPORT_COUNT (1)
	0x81, 0x42,			//		INPUT (Data,Var,Abs,Null)

	0x75, 0x01,			//		REPORT_SIZE (1)
	0x95, 0x04,			//		REPORT_COUNT (4)
	0x81, 0x03,			//		INPUT (Cnst,Var,Abs)

	0x09, 0x39,			//		USAGE (Hat switch)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x25, 0x07,			//		LOGICAL_MAXIMUM (7)
	0x75, 0x04,			//		REPORT_SIZE (4)
	0x95, 0x01,			//		REPORT_COUNT (1)
	0x81, 0x42,			//		INPUT (Data,Var,Abs,Null)

	0x75, 0x01,			//		REPORT_SIZE (1)
	0x95, 0x04,			//		REPORT_COUNT (4)
	0x81, 0x03,			//		INPUT (Cnst,Var,Abs)

	0x09, 0x39,			//		USAGE (Hat switch)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x25, 0x07,			//		LOGICAL_MAXIMUM (7)
	0x75, 0x04,			//		REPORT_SIZE (4)
	0x95, 0x01,			//		REPORT_COUNT (1)
	0x81, 0x42,			//		INPUT (Data,Var,Abs,Null)

	0x75, 0x01,			//		REPORT_SIZE (1)
	0x95, 0x04,			//		REPORT_COUNT (4)
	0x81, 0x03,			//		INPUT (Cnst,Var,Abs)

	0x05, 0x09,			//		USAGE_PAGE (Button)

	0x19, 0x01,			//		USAGE_MINIMUM (Button 1)
	0x29, 0x20,			//		USAGE_MAXIMUM (Button 32)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x25, 0x01,			//		LOGICAL_MAXIMUM (1)
	0x75, 0x01,			//		REPORT_SIZE (1)
	0x95, 0x20,			//		REPORT_COUNT (32)
	0x81, 0x02,			//		INPUT (Data,Var,Abs)

	0x06, 0x00, 0xff,	//		USAGE_PAGE (Vendor Defined Page 1)
	0x09, 0x01,			//		USAGE (Vendor Usage 1)
	0x09, 0x01,			//		USAGE (Vendor Usage 1)
	0x09, 0x01,			//		USAGE (Vendor Usage 1)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x00,	//		LOGICAL_MAXIMUM (255)
	0x75, 0x08,			//		REPORT_SIZE (8)
	0x95, 0x03,			//		REPORT_COUNT (3)
	0x91, 0x02,			//		OUTPUT (Data,Var,Abs)

	0x06, 0x00, 0xff,	//		USAGE_PAGE (Vendor Defined Page 1)
	0x09, 0x01,			//		USAGE (Vendor Usage 1)
	0x15, 0x00,			//		LOGICAL_MINIMUM (0)
	0x26, 0xff, 0x00,	//		LOGICAL_MAXIMUM (255)
	0x75, 0x08,			//		REPORT_SIZE (8)
	0x95, 0x14,			//		REPORT_COUNT (20)
	0xb1, 0x00,			//		FEATURE (Data,Ary,Abs)

	0xc0				//	END_COLLECTION
};

CHAR *	ReportDescriptor[]	= {
	 ReportDescriptorNormal
	,ReportDescriptorH1B16
	,ReportDescriptorNoRxRy
	,ReportDescriptorNoSlider
};

UCHAR	ReportDescriptorSize[]	= {
	 sizeof( ReportDescriptorNormal )
	,sizeof( ReportDescriptorH1B16 )
	,sizeof( ReportDescriptorNoRxRy )
	,sizeof( ReportDescriptorNoSlider )
};

//----------------------------------------------------------------------------------------------
//	関数
//----------------------------------------------------------------------------------------------

NTSTATUS HidGetDeviceAttributes( IN PDEVICE_EXTENSION, IN PIRP, IN PIO_STACK_LOCATION );
NTSTATUS HidGetDeviceDescriptor( IN PDEVICE_EXTENSION, IN PIRP, IN PIO_STACK_LOCATION );
NTSTATUS HidGetReportDescriptor( IN PDEVICE_EXTENSION, IN PIRP, IN PIO_STACK_LOCATION );
NTSTATUS HidReadReport( IN PDEVICE_EXTENSION, IN PIRP, IN PIO_STACK_LOCATION );
KDEFERRED_ROUTINE ReadReportDpcRoutine;
NTSTATUS HidWriteReport( IN PDEVICE_EXTENSION, IN PIRP, IN PIO_STACK_LOCATION );
NTSTATUS HidGetFeature( IN PDEVICE_EXTENSION, IN PIRP, IN PIO_STACK_LOCATION );
NTSTATUS HidSetFeature( IN PDEVICE_EXTENSION, IN PIRP, IN PIO_STACK_LOCATION );

//----------------------------------------------------------------------------------------------
//	InternalDeviceControl
//----------------------------------------------------------------------------------------------
NTSTATUS InternalDeviceControl(
	 IN PDEVICE_OBJECT	DeviceObject
	,IN PIRP			Irp )
{
	//	変数宣言
	NTSTATUS			Status;
	PDEVICE_EXTENSION	DeviceExtension	= GET_MINIDRIVER_DEVICE_EXTENSION( DeviceObject );
	PIO_STACK_LOCATION	CurrentIrpStack;

	//	参照カウンタを加算する
	Status	= IncrementReference( DeviceExtension );
	if( !NT_SUCCESS( Status ) )
	{
		CompleteRequest( Irp, Status, 0 );
		return( Status );
	}

	//	マイナー ファンクションによって処理を振り分ける
	CurrentIrpStack	= IoGetCurrentIrpStackLocation( Irp );
	switch( CurrentIrpStack->Parameters.DeviceIoControl.IoControlCode )
	{
		case IOCTL_HID_GET_DEVICE_ATTRIBUTES:
			Status	= HidGetDeviceAttributes( DeviceExtension, Irp, CurrentIrpStack );
			break;

		case IOCTL_HID_GET_DEVICE_DESCRIPTOR:
			Status	= HidGetDeviceDescriptor( DeviceExtension, Irp, CurrentIrpStack );
			break;

		case IOCTL_HID_GET_REPORT_DESCRIPTOR:
			Status	= HidGetReportDescriptor( DeviceExtension, Irp, CurrentIrpStack );
			break;

		case IOCTL_HID_READ_REPORT:
			Status	= HidReadReport( DeviceExtension, Irp, CurrentIrpStack );
			break;

		case IOCTL_HID_WRITE_REPORT:
			Status	= HidWriteReport( DeviceExtension, Irp, CurrentIrpStack );
			break;

		case IOCTL_HID_GET_FEATURE:
			Status	= HidGetFeature( DeviceExtension, Irp, CurrentIrpStack );
			break;

		case IOCTL_HID_SET_FEATURE:
			Status	= HidSetFeature( DeviceExtension, Irp, CurrentIrpStack );
			break;

		default:
			Status	= STATUS_NOT_SUPPORTED;
			CompleteRequest( Irp, Status, 0 );
			break;
	}

	//	参照カウンタを減算する
	DecrementReference( DeviceExtension );

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	HidGetDeviceAttributes
//----------------------------------------------------------------------------------------------
NTSTATUS HidGetDeviceAttributes(
	 IN PDEVICE_EXTENSION	DeviceExtension
	,IN PIRP				Irp
	,IN PIO_STACK_LOCATION	CurrentIrpStack )
{
	//	変数宣言
	NTSTATUS				Status			= STATUS_SUCCESS;
	UCHAR					BufferLength	= sizeof( HID_DEVICE_ATTRIBUTES );
	PHID_DEVICE_ATTRIBUTES	HidDeviceAttributes;

	//	デバイス属性用バッファのサイズをチェックする
	if( CurrentIrpStack->Parameters.DeviceIoControl.OutputBufferLength < BufferLength )
	{
		Status	= STATUS_BUFFER_TOO_SMALL;
		CompleteRequest( Irp, Status, 0 );
		return( Status );
	}

	//	デバイス属性を設定する
	HidDeviceAttributes					= (PHID_DEVICE_ATTRIBUTES)Irp->UserBuffer;
	RtlZeroMemory( HidDeviceAttributes, BufferLength );
	HidDeviceAttributes->Size			= BufferLength;
	HidDeviceAttributes->VendorID		= DeviceExtension->DeviceDescriptor.idVendor;
	HidDeviceAttributes->ProductID		= DeviceExtension->DeviceDescriptor.idProduct;
	HidDeviceAttributes->VersionNumber	= DeviceExtension->DeviceDescriptor.bcdDevice;

	//	IOCTL_HID_GET_DEVICE_ATTRIBUTES 要求を完了させる
	CompleteRequest( Irp, Status, BufferLength );

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	HidGetDeviceDescriptor
//----------------------------------------------------------------------------------------------
NTSTATUS HidGetDeviceDescriptor(
	 IN PDEVICE_EXTENSION	DeviceExtension
	,IN PIRP				Irp
	,IN PIO_STACK_LOCATION	CurrentIrpStack )
{
	//	変数宣言
	NTSTATUS		Status			= STATUS_SUCCESS;
	UCHAR			BufferLength	= sizeof( HID_DESCRIPTOR );
	PHID_DESCRIPTOR	HidDescriptor;

	//	HID 記述子用バッファのサイズをチェックする
	if( CurrentIrpStack->Parameters.DeviceIoControl.OutputBufferLength < BufferLength )
	{
		Status	= STATUS_BUFFER_TOO_SMALL;
		CompleteRequest( Irp, Status, 0 );
		return( Status );
	}

	//	HID 記述子を設定する
	HidDescriptor									= (PHID_DESCRIPTOR)Irp->UserBuffer;
	RtlZeroMemory( HidDescriptor, BufferLength );
	HidDescriptor->bLength							= BufferLength;
	HidDescriptor->bDescriptorType					= HID_HID_DESCRIPTOR_TYPE;
	HidDescriptor->bcdHID							= HID_REVISION;
	HidDescriptor->bCountry							= 0;
	HidDescriptor->bNumDescriptors					= 1;
	HidDescriptor->DescriptorList[0].bReportType	= HID_REPORT_DESCRIPTOR_TYPE;
	HidDescriptor->DescriptorList[0].wReportLength	= ReportDescriptorSize[LimitationMode];

	//	IOCTL_HID_GET_DEVICE_DESCRIPTOR 要求を完了させる
	CompleteRequest( Irp, Status, BufferLength );

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	HidGetReportDescriptor
//----------------------------------------------------------------------------------------------
NTSTATUS HidGetReportDescriptor(
	 IN PDEVICE_EXTENSION	DeviceExtension
	,IN PIRP				Irp
	,IN PIO_STACK_LOCATION	CurrentIrpStack )
{
	//	変数宣言
	NTSTATUS	Status			= STATUS_SUCCESS;
	UCHAR		BufferLength	= ReportDescriptorSize[LimitationMode];

	//	レポート記述子用バッファのサイズをチェックする
	if( CurrentIrpStack->Parameters.DeviceIoControl.OutputBufferLength < BufferLength )
	{
		Status	= STATUS_BUFFER_TOO_SMALL;
		CompleteRequest( Irp, Status, 0 );
		return( Status );
	}

	//	レポート記述子を設定する
	RtlCopyMemory( Irp->UserBuffer, ReportDescriptor[LimitationMode], BufferLength );

	//	IOCTL_HID_GET_REPORT_DESCRIPTOR 要求を完了させる
	CompleteRequest( Irp, Status, BufferLength );

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	HidReadReport
//----------------------------------------------------------------------------------------------
NTSTATUS HidReadReport(
	 IN PDEVICE_EXTENSION	DeviceExtension
	,IN PIRP				Irp
	,IN PIO_STACK_LOCATION	CurrentIrpStack )
{
	//	変数宣言
	NTSTATUS		Status			= STATUS_SUCCESS;
	UCHAR			BufferLength	= sizeof( HID_READ_REPORT );
	KIRQL			Irql;
	LARGE_INTEGER	TimeOut;
	PREAD_TIMER		ReadTimer;

	if( DevicesArePolled == FALSE )
	{
		//	タイマー用バッファを作成する
		ReadTimer	= (PREAD_TIMER)ExAllocatePoolWithTag(
						 NonPagedPool
						,sizeof( READ_TIMER )
						,'200T' );
		RtlZeroMemory( ReadTimer, sizeof( READ_TIMER ) );
		ReadTimer->Irp				= Irp;
		ReadTimer->DeviceExtension	= DeviceExtension;
		//	タイマーを初期化する
		KeInitializeDpc(
			 &ReadTimer->ReadTimerDpc
			,ReadReportDpcRoutine
			,(PVOID)ReadTimer );
		KeInitializeTimer( &ReadTimer->ReadTimer );
		//	タイマーを発行する
		TimeOut.QuadPart	= -1;
		KeSetTimer(
			 &ReadTimer->ReadTimer
			,TimeOut
			,&ReadTimer->ReadTimerDpc );
	} else {
		//	入力レポート用バッファのサイズをチェックする
		if( CurrentIrpStack->Parameters.DeviceIoControl.OutputBufferLength < BufferLength )
		{
			Status	= STATUS_BUFFER_TOO_SMALL;
			CompleteRequest( Irp, Status, 0 );
			return( Status );
		}

		//	ワーカー スレッドからの割り込みを禁止する
		KeAcquireSpinLock( &DeviceExtension->LockReport, &Irql );

		//	入力レポートを設定する
		RtlZeroMemory( Irp->UserBuffer, BufferLength );
		GenerateReadReport(
			 &DeviceExtension->ControllerReport
			,&DeviceExtension->Setting
			,(HID_READ_REPORT *)Irp->UserBuffer
			,&DeviceExtension->LastState );

		//	ワーカー スレッドからの割り込みを許可する
		KeReleaseSpinLock( &DeviceExtension->LockReport, Irql );

		//	IOCTL_HID_READ_REPORT 要求を完了させる
		CompleteRequest( Irp, Status, BufferLength );
	}

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	ReadReportDpcRoutine
//----------------------------------------------------------------------------------------------
VOID ReadReportDpcRoutine(
	 IN PKDPC	Dpc
	,IN PVOID	DeferredContext
	,IN PVOID	SystemArgument1
	,IN PVOID	SystemArgument2 )
{
	//	変数宣言
	NTSTATUS			Status			= STATUS_SUCCESS;
	PREAD_TIMER			ReadTimer		= (PREAD_TIMER)DeferredContext;
	PIRP				Irp				= ReadTimer->Irp;
	PDEVICE_EXTENSION	DeviceExtension	= ReadTimer->DeviceExtension;
	PIO_STACK_LOCATION	CurrentIrpStack;
	KIRQL				Irql;
	UCHAR				BufferLength	= sizeof( HID_READ_REPORT );

	//	入力レポート用バッファのサイズをチェックする
	CurrentIrpStack	= IoGetCurrentIrpStackLocation( Irp );
	if( CurrentIrpStack->Parameters.DeviceIoControl.OutputBufferLength < BufferLength )
	{
		Status	= STATUS_BUFFER_TOO_SMALL;
		CompleteRequest( Irp, Status, 0 );
	} else {

		//	ワーカー スレッドからの割り込みを禁止する
		KeAcquireSpinLock( &DeviceExtension->LockReport, &Irql );

		//	入力レポートを設定する
		RtlZeroMemory( Irp->UserBuffer, BufferLength );
		GenerateReadReport(
			 &DeviceExtension->ControllerReport
			,&DeviceExtension->Setting
			,(HID_READ_REPORT *)Irp->UserBuffer
			,&DeviceExtension->LastState );

		//	ワーカー スレッドからの割り込みを許可する
		KeReleaseSpinLock( &DeviceExtension->LockReport, Irql );

		//	IOCTL_HID_READ_REPORT 要求を完了させる
		CompleteRequest( Irp, Status, BufferLength );
	}

	//	タイマー用バッファを開放する
	ExFreePool( ReadTimer );
}

//----------------------------------------------------------------------------------------------
//	HidWriteReport
//----------------------------------------------------------------------------------------------
NTSTATUS HidWriteReport(
	 IN PDEVICE_EXTENSION	DeviceExtension
	,IN PIRP				Irp
	,IN PIO_STACK_LOCATION	CurrentIrpStack )
{
	//	変数宣言
	NTSTATUS			Status			= STATUS_SUCCESS;
	UCHAR				BufferLength	= sizeof( HID_WRITE_REPORT );
	PHID_XFER_PACKET	Report			= (PHID_XFER_PACKET)Irp->UserBuffer;

	//	出力レポート用バッファのサイズをチェックする
	if( Report->reportBufferLen < BufferLength )
	{
		Status	= STATUS_BUFFER_TOO_SMALL;
		CompleteRequest( Irp, Status, 0 );
		return( Status );
	}

	//	出力レポートを設定する
	GenerateWriteReport(
		 (HID_WRITE_REPORT *)Report->reportBuffer
		,&DeviceExtension->Setting
		,&DeviceExtension->ActuatorsReport );

	//	IOCTL_HID_WRITE_REPORT 要求を完了させる
	CompleteRequest( Irp, Status, BufferLength );

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	HidGetFeature
//----------------------------------------------------------------------------------------------
NTSTATUS HidGetFeature(
	 IN PDEVICE_EXTENSION	DeviceExtension
	,IN PIRP				Irp
	,IN PIO_STACK_LOCATION	CurrentIrpStack )
{
	//	変数宣言
	NTSTATUS			Status			= STATUS_SUCCESS;
	UCHAR				BufferLength	= sizeof( INPUT_REPORT );
	PHID_XFER_PACKET	Feature			= (PHID_XFER_PACKET)Irp->UserBuffer;

	//	フューチャー用バッファのサイズをチェックする
	if( Feature->reportBufferLen < BufferLength )
	{
		Status	= STATUS_BUFFER_TOO_SMALL;
		CompleteRequest( Irp, Status, 0 );
		return( Status );
	}

	//	コントローラーの状態を返す
	RtlCopyMemory(
		 Feature->reportBuffer + 1
		,&DeviceExtension->ControllerReport
		,BufferLength );

	//	IOCTL_HID_GET_FEATURE 要求を完了させる
	CompleteRequest( Irp, Status, BufferLength );

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	HidSetFeature
//----------------------------------------------------------------------------------------------
NTSTATUS HidSetFeature(
	 IN PDEVICE_EXTENSION	DeviceExtension
	,IN PIRP				Irp
	,IN PIO_STACK_LOCATION	CurrentIrpStack )
{
	//	変数宣言
	NTSTATUS	Status	= STATUS_SUCCESS;

	//	コントローラーの設定を読み直す
	DeviceExtension->ResetDevice	= TRUE;

	//	IOCTL_HID_SET_FEATURE 要求を完了させる
	CompleteRequest( Irp, Status, 0 );

	return( Status );
}
