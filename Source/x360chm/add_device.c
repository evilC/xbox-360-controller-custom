
//	HID Minidriver for Microsoft Xbox 360 Controller

#include	"driver.h"

//----------------------------------------------------------------------------------------------
//	AddDevice
//----------------------------------------------------------------------------------------------
NTSTATUS AddDevice(
	 IN PDRIVER_OBJECT	DriverObject
	,IN PDEVICE_OBJECT	DeviceObject )
{
	//	変数宣言
	PDEVICE_EXTENSION	DeviceExtension	= GET_MINIDRIVER_DEVICE_EXTENSION( DeviceObject );

	//	デバイス エクステンションの内容を初期化する
	RtlZeroMemory( DeviceExtension, sizeof( DEVICE_EXTENSION ) );

	//	I/O 要求制御
	DeviceExtension->DriverState	= Enable;
	DeviceExtension->DeviceState	= Disable;
	DeviceExtension->ReferenceCount	= 1;
	KeInitializeEvent( &DeviceExtension->ReferenceEnd, NotificationEvent, FALSE );

	//	ワーカ スレッド
	DeviceExtension->ThreadState	= Disable;
	KeInitializeEvent( &DeviceExtension->ThreadExitingIn, SynchronizationEvent, FALSE );
	KeInitializeEvent( &DeviceExtension->ThreadExitingOut, SynchronizationEvent, FALSE );

	//	USB
	DeviceExtension->NextDeviceObject	= GET_NEXT_DEVICE_OBJECT( DeviceObject );

	//	レポート
	KeInitializeSpinLock( &DeviceExtension->LockReport );

	//	デバイス エクステンションの初期化完了を通知する
	DeviceObject->Flags	&= ~DO_DEVICE_INITIALIZING;

	return( STATUS_SUCCESS );
}
