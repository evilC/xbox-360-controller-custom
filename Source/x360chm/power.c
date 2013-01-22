
//	HID Minidriver for Microsoft Xbox 360 Controller

#include	"driver.h"

//----------------------------------------------------------------------------------------------
//	関数
//----------------------------------------------------------------------------------------------

NTSTATUS SetPower( IN PDEVICE_OBJECT, IN PDEVICE_EXTENSION, IN PIRP, IN PIO_STACK_LOCATION );

//----------------------------------------------------------------------------------------------
//	Power
//----------------------------------------------------------------------------------------------
NTSTATUS Power(
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
	switch( CurrentIrpStack->MinorFunction )
	{
		case IRP_MN_SET_POWER:
			Status = SetPower( DeviceObject, DeviceExtension, Irp, CurrentIrpStack );
			break;

		default:
			//	IRP_MJ_POWER 要求を下位ドライバに送る
			Status	= SendPowerRequest( DeviceObject, Irp );
			break;
	}

	//	参照カウンタを減算する
	DecrementReference( DeviceExtension );

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	SetPower
//----------------------------------------------------------------------------------------------
NTSTATUS SetPower(
	 IN PDEVICE_OBJECT		DeviceObject
	,IN PDEVICE_EXTENSION	DeviceExtension
	,IN PIRP				Irp
	,IN PIO_STACK_LOCATION	CurrentIrpStack )
{
	//	変数宣言
	NTSTATUS	Status;

	//	デバイスの電源状態が変更された場合にデバイスの状態を変更する
#if (NTDDI_VERSION < NTDDI_LONGHORN)
	switch( CurrentIrpStack->Parameters.Power.Type )
	{
		case SystemPowerState:
			if( CurrentIrpStack->Parameters.Power.State.SystemState == PowerSystemWorking )
			{
				//	デバイスをリセットする
				DeviceExtension->ResetDevice	= TRUE;
			}
		break;
	}
	//	IRP_MJ_POWER 要求を下位ドライバに送る
	Status	= SendPowerRequest( DeviceObject, Irp );
#else
	if( CurrentIrpStack->Parameters.Power.Type == DevicePowerState )
	{
		switch( CurrentIrpStack->Parameters.Power.State.DeviceState )
		{
			case PowerDeviceD0:
				//	先に IRP_MN_SET_POWER 要求を下位ドライバに送る
				Status = SendAndWaitPowerRequest( DeviceObject, Irp );
				if( !NT_SUCCESS( Status ) )
				{
					CompleteRequest( Irp, Status, 0 );
					return( Status );
				}

				//	デバイスを有効にする
				Status	= EnableDevice( DeviceExtension );

				//	IRP_MN_SET_POWER 要求を完了させる
				CompleteRequest( Irp, Status, 0 );
				break;

			case PowerDeviceD1:
			case PowerDeviceD2:
			case PowerDeviceD3:
				//	デバイスを無効にする
				DisableDevice( DeviceExtension );

				//	IRP_MJ_POWER 要求を下位ドライバに送る
				Status	= SendPowerRequest( DeviceObject, Irp );
				break;

			default:
				//	IRP_MJ_POWER 要求を下位ドライバに送る
				Status	= SendPowerRequest( DeviceObject, Irp );
				break;
		}
	} else {
		//	IRP_MJ_POWER 要求を下位ドライバに送る
		Status	= SendPowerRequest( DeviceObject, Irp );
	}
#endif

	return( Status );
}
