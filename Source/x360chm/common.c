
//	HID Minidriver for Microsoft Xbox 360 Controller

#include	"driver.h"

//----------------------------------------------------------------------------------------------
//	関数
//----------------------------------------------------------------------------------------------

IO_COMPLETION_ROUTINE IoCompletionRoutine;

//----------------------------------------------------------------------------------------------
//	CompleteRequest
//----------------------------------------------------------------------------------------------
VOID CompleteRequest(
	 IN PIRP		Irp
	,IN NTSTATUS	Status
	,IN ULONG		Information )
{
	//	I/O 要求を完了させる
	Irp->IoStatus.Status		= Status;
	Irp->IoStatus.Information	= Information;
	IoCompleteRequest( Irp, IO_NO_INCREMENT );
}

//----------------------------------------------------------------------------------------------
//	SendRequest
//----------------------------------------------------------------------------------------------
NTSTATUS SendRequest(
	 IN PDEVICE_OBJECT	DeviceObject
	,IN PIRP			Irp )
{
	//	下位ドライバに I/O 要求を発行する
	IoSkipCurrentIrpStackLocation( Irp );
	return( IoCallDriver( GET_NEXT_DEVICE_OBJECT( DeviceObject ), Irp ) );
}

//----------------------------------------------------------------------------------------------
//	SendAndWaitRequest
//----------------------------------------------------------------------------------------------
NTSTATUS SendAndWaitRequest(
	 IN PDEVICE_OBJECT	DeviceObject
	,IN PIRP			Irp )
{
	//	変数宣言
	NTSTATUS	Status;
	KEVENT		WaitEvent;

	//	下位ドライバに I/O 要求を発行する
	KeInitializeEvent( &WaitEvent, NotificationEvent, FALSE );
	IoCopyCurrentIrpStackLocationToNext( Irp );
	IoSetCompletionRoutine(
		 Irp
		,(PIO_COMPLETION_ROUTINE)IoCompletionRoutine
		,(PVOID)&WaitEvent
		,TRUE
		,TRUE
		,TRUE );
	Status	= IoCallDriver( GET_NEXT_DEVICE_OBJECT( DeviceObject ), Irp );

	//	下位ドライバの処理が完了するまで待機する
	if( Status == STATUS_PENDING )
	{
		KeWaitForSingleObject( &WaitEvent, Executive, KernelMode, FALSE, NULL );
		Status	= Irp->IoStatus.Status;
	}

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	SendPowerRequest
//----------------------------------------------------------------------------------------------
NTSTATUS SendPowerRequest(
	 IN PDEVICE_OBJECT	DeviceObject
	,IN PIRP			Irp )
{
	//	下位ドライバに I/O 要求を発行する
	PoStartNextPowerIrp( Irp );
	IoSkipCurrentIrpStackLocation( Irp );
#if (NTDDI_VERSION < NTDDI_LONGHORN)
	return( PoCallDriver( GET_NEXT_DEVICE_OBJECT( DeviceObject ), Irp ) );
#else
	return( IoCallDriver( GET_NEXT_DEVICE_OBJECT( DeviceObject ), Irp ) );
#endif
}

//----------------------------------------------------------------------------------------------
//	SendAndWaitPowerRequest
//----------------------------------------------------------------------------------------------
NTSTATUS SendAndWaitPowerRequest(
	 IN PDEVICE_OBJECT	DeviceObject
	,IN PIRP			Irp )
{
	//	変数宣言
	NTSTATUS	Status;
	KEVENT		WaitEvent;

	//	下位ドライバに I/O 要求を発行する
	KeInitializeEvent( &WaitEvent, NotificationEvent, FALSE );
	PoStartNextPowerIrp( Irp );
	IoCopyCurrentIrpStackLocationToNext( Irp );
	IoSetCompletionRoutine(
		 Irp
		,(PIO_COMPLETION_ROUTINE)IoCompletionRoutine
		,(PVOID)&WaitEvent
		,TRUE
		,TRUE
		,TRUE );
#if (NTDDI_VERSION < NTDDI_LONGHORN)
	Status	= PoCallDriver( GET_NEXT_DEVICE_OBJECT( DeviceObject ), Irp );
#else
	Status	= IoCallDriver( GET_NEXT_DEVICE_OBJECT( DeviceObject ), Irp );
#endif

	//	下位ドライバの処理が完了するまで待機する
	if( Status == STATUS_PENDING )
	{
		KeWaitForSingleObject( &WaitEvent, Executive, KernelMode, FALSE, NULL );
		Status	= Irp->IoStatus.Status;
	}

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	IoCompletionRoutine
//----------------------------------------------------------------------------------------------
NTSTATUS IoCompletionRoutine(
	 IN PDEVICE_OBJECT	DeviceObject
	,IN PIRP			Irp
	,IN PKEVENT			Event )
{
	//	I/O 要求の完了を通知する
	KeSetEvent( Event, 0, FALSE );

	return( STATUS_MORE_PROCESSING_REQUIRED );
}

//----------------------------------------------------------------------------------------------
//	IncrementReference
//----------------------------------------------------------------------------------------------
NTSTATUS IncrementReference( IN PDEVICE_EXTENSION DeviceExtension )
{
	//	変数宣言
	NTSTATUS	Status	= STATUS_SUCCESS;

	//	参照カウンタを加算する
	InterlockedIncrement( &DeviceExtension->ReferenceCount );

	//	ドライバの状態は有効・無効？
	if( DeviceExtension->DriverState == Disable )
	{
		if( InterlockedDecrement( &DeviceExtension->ReferenceCount ) == 0 )
		{
			//	参照終了イベントを送る
			KeSetEvent( &DeviceExtension->ReferenceEnd, 0, FALSE );
			Status	= STATUS_DELETE_PENDING;
		}
	}

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	DecrementReference
//----------------------------------------------------------------------------------------------
VOID DecrementReference( IN PDEVICE_EXTENSION DeviceExtension )
{
	//	参照カウンタを減算する
	if( InterlockedDecrement( &DeviceExtension->ReferenceCount ) == 0 )
	{
		//	参照終了イベントを送る
		KeSetEvent( &DeviceExtension->ReferenceEnd, 0, FALSE );
	}
}

//----------------------------------------------------------------------------------------------
//	GetRegistryBinary
//----------------------------------------------------------------------------------------------
VOID GetRegistryBinary(
	 IN PWSTR	Path
	,IN PWSTR	Value
	,IN CHAR *	Buf
	,IN LONG	BufSize )
{
	//	変数宣言
	NTSTATUS						Status;
	UNICODE_STRING					RegistryPath;
	UNICODE_STRING					RegistryValue;
	OBJECT_ATTRIBUTES				ObjAttribute;
	HANDLE							Key;
	ULONG							ValueSize	= 0;
	PKEY_VALUE_PARTIAL_INFORMATION	ValueInfo;

	//	レジストリ パスを作成する
	RtlInitUnicodeString( &RegistryPath, Path );
	//	オブジェクト属性を作成する
	InitializeObjectAttributes(
		 &ObjAttribute
		,&RegistryPath
		,OBJ_CASE_INSENSITIVE
		,NULL
		,NULL );

	//	レジストリを開く
	Status	= ZwOpenKey( &Key, KEY_QUERY_VALUE, &ObjAttribute );
	if( NT_SUCCESS( Status ) )
	{
		//	レジストリ値名を作成する
		RtlInitUnicodeString( &RegistryValue, Value );

		//	レジストリ値のサイズを取得する
		Status	= ZwQueryValueKey(
					 Key
					,&RegistryValue
					,KeyValuePartialInformation
					,NULL
					,0
					,&ValueSize );
		if( ( Status == STATUS_OBJECT_NAME_NOT_FOUND ) || ( ValueSize == 0 ) )
		{
			ZwClose( Key );
			return;
		}

		//	レジストリ値を取得するバッファを作成する
		ValueInfo	= (PKEY_VALUE_PARTIAL_INFORMATION)ExAllocatePoolWithTag(
						 NonPagedPool
						,ValueSize
						,'100T' );
		if( ValueInfo == NULL )
		{
			ZwClose( Key );
			return;
		}

		//	レジストリ値を取得する
		Status	= ZwQueryValueKey(
					 Key
					,&RegistryValue
					,KeyValuePartialInformation
					,ValueInfo
					,ValueSize
					,&ValueSize );
		if( !NT_SUCCESS( Status ) )
		{
			ExFreePool( ValueInfo );
			ZwClose( Key );
			return;
		}

		//	取得した内容を退避する
		RtlCopyMemory( Buf, ValueInfo->Data, BufSize );

		//	バッファを開放する
		ExFreePool( ValueInfo );

		//	レジストリを閉じる
		ZwClose( Key );
	}
}
