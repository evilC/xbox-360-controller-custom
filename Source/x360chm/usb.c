
//	HID Minidriver for Microsoft Xbox 360 Controller

#include	"driver.h"

//----------------------------------------------------------------------------------------------
//	関数
//----------------------------------------------------------------------------------------------

NTSTATUS GetPortStatus( IN PDEVICE_OBJECT );
NTSTATUS ResetPort( IN PDEVICE_OBJECT );
NTSTATUS PipeRequest( IN PDEVICE_OBJECT, IN USHORT, IN USBD_PIPE_HANDLE );
NTSTATUS CallUSBDI( IN PDEVICE_OBJECT, IN ULONG, IN OUT PVOID );

//----------------------------------------------------------------------------------------------
//	SetupPort
//----------------------------------------------------------------------------------------------
NTSTATUS SetupPort( IN PDEVICE_OBJECT NextDeviceObject )
{
	//	変数宣言
	NTSTATUS	Status;

	//	USB ポートの状態を取得する
	Status	= GetPortStatus( NextDeviceObject );
	if( !NT_SUCCESS( Status ) )
	{
		//	USB ポートを初期化する
		Status	= ResetPort( NextDeviceObject );
		if( !NT_SUCCESS( Status ) )
		{
			return( Status );
		}

		//	再度 USB ポートの状態を取得する
		Status	= GetPortStatus( NextDeviceObject );
	}

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	GetPortStatus
//----------------------------------------------------------------------------------------------
NTSTATUS GetPortStatus( IN PDEVICE_OBJECT NextDeviceObject )
{
	//	変数宣言
	NTSTATUS	Status;
	ULONG		PortStatus;

	//	USB ポートの状態を取得する
	Status	= CallUSBDI( NextDeviceObject, IOCTL_INTERNAL_USB_GET_PORT_STATUS, &PortStatus );
	if( !NT_SUCCESS( Status ) )
	{
		return( Status );
	}

	//	デバイスが USB ポートに接続されているか調べる
	if( ( PortStatus & USBD_PORT_CONNECTED ) != USBD_PORT_CONNECTED )
	{
		return( STATUS_NO_SUCH_DEVICE );
	}

	//	USB ポートが有効か調べる
	if( ( PortStatus & USBD_PORT_ENABLED ) != USBD_PORT_ENABLED )
	{
		return( STATUS_DEVICE_NOT_READY );
	}

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	ResetPort
//----------------------------------------------------------------------------------------------
NTSTATUS ResetPort( IN PDEVICE_OBJECT NextDeviceObject )
{
	//	USB ポートをリセットする
	return( CallUSBDI( NextDeviceObject, IOCTL_INTERNAL_USB_RESET_PORT, NULL ) );
}

//----------------------------------------------------------------------------------------------
//	GetDeviceDescriptor
//----------------------------------------------------------------------------------------------
NTSTATUS GetDeviceDescriptor(
	 IN PDEVICE_OBJECT			NextDeviceObject
	,OUT PUSB_DEVICE_DESCRIPTOR	DeviceDescriptor )
{
	//	変数宣言
	NTSTATUS	Status;
	USHORT		UrbSize	= sizeof( struct _URB_CONTROL_DESCRIPTOR_REQUEST );
	PURB		Urb;

	//	URB 用バッファを作成する
	Urb	= (PURB)ExAllocatePoolWithTag( NonPagedPool, UrbSize, '300T' );
	if( Urb == NULL )
	{
		return( STATUS_INSUFFICIENT_RESOURCES );
	}

	//	デバイス記述子を取得する為の URB を作成する
	UsbBuildGetDescriptorRequest(
		 Urb
		,UrbSize
		,USB_DEVICE_DESCRIPTOR_TYPE
		,0
		,0
		,DeviceDescriptor
		,NULL
		,sizeof( USB_DEVICE_DESCRIPTOR )
		,NULL );

	//	デバイス記述子を取得する為の URB を発行する
	Status	= CallUSBDI( NextDeviceObject, IOCTL_INTERNAL_USB_SUBMIT_URB, Urb );
	if( !NT_SUCCESS( Status ) || !USBD_SUCCESS( Urb->UrbHeader.Status ) )
	{
		Status	= STATUS_UNSUCCESSFUL;
	}

	//	作成した URB を解放する
	ExFreePool( Urb );

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	GetConfigurationDescriptor
//----------------------------------------------------------------------------------------------
NTSTATUS GetConfigurationDescriptor(
	 IN PDEVICE_OBJECT						NextDeviceObject
	,OUT PUSB_CONFIGURATION_DESCRIPTOR *	ConfigurationDescriptor )
{
	//	変数宣言
	NTSTATUS	Status;
	USHORT		UrbSize	= sizeof( struct _URB_CONTROL_DESCRIPTOR_REQUEST );
	PURB		Urb;
	LONG		ConfigurationDescriptorSize
						= sizeof( USB_CONFIGURATION_DESCRIPTOR );

	//	URB 用バッファを作成する
	Urb	= (PURB)ExAllocatePoolWithTag( NonPagedPool, UrbSize, '400T' );
	if( Urb == NULL )
	{
		return( STATUS_INSUFFICIENT_RESOURCES );
	}

	//	コンフィギュレーション記述子はサイズが未確定なので調整する必要がある
	for(;;)
	{
		//	コンフィギュレーション記述子用バッファを作成する
		*ConfigurationDescriptor	= ExAllocatePoolWithTag(
										 NonPagedPool
										,ConfigurationDescriptorSize
										,'500T' );
		if( *ConfigurationDescriptor == NULL )
		{
			ExFreePool( Urb );
			return( STATUS_INSUFFICIENT_RESOURCES );
		}

		//	コンフィギュレーション記述子を取得する為の URB を作成する
		UsbBuildGetDescriptorRequest(
			 Urb
			,UrbSize
			,USB_CONFIGURATION_DESCRIPTOR_TYPE
			,0
			,0
			,*ConfigurationDescriptor
			,NULL
			,ConfigurationDescriptorSize
			,NULL );

		//	コンフィギュレーション記述子を取得する為の URB を発行する
		Status	= CallUSBDI( NextDeviceObject, IOCTL_INTERNAL_USB_SUBMIT_URB, Urb );
		if( !NT_SUCCESS( Status ) || !USBD_SUCCESS( Urb->UrbHeader.Status ) )
		{
			Status	= STATUS_UNSUCCESSFUL;
			break;
		}

		//	作成したコンフィギュレーション記述子のサイズが小さい場合、再作成する
		if( ConfigurationDescriptorSize < (*ConfigurationDescriptor)->wTotalLength )
		{
			ConfigurationDescriptorSize	= (*ConfigurationDescriptor)->wTotalLength;
			ExFreePool( *ConfigurationDescriptor );
			*ConfigurationDescriptor	= NULL;
		} else {
			break;
		}
	}

	//	作成した URB を解放する
	ExFreePool( Urb );

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	GetInterfaceDescriptor
//----------------------------------------------------------------------------------------------
NTSTATUS GetInterfaceDescriptor(
	 IN PUSB_CONFIGURATION_DESCRIPTOR	ConfigurationDescriptor
	,OUT PUSB_INTERFACE_DESCRIPTOR * *	InterfaceDescriptor )
{
	//	変数宣言
	NTSTATUS	Status	= STATUS_SUCCESS;
	LONG		Index;

	//	インターフェイス記述子用バッファを作成する
	*InterfaceDescriptor	= ExAllocatePoolWithTag(
								 NonPagedPool
								,ConfigurationDescriptor->bNumInterfaces
									* sizeof( PUSB_INTERFACE_DESCRIPTOR )
								,'600T' );
	if( *InterfaceDescriptor == NULL )
	{
		return( STATUS_INSUFFICIENT_RESOURCES );
	}

	//	全てのインターフェイス記述子を取得する
	(*InterfaceDescriptor)[0]	= (PUSB_INTERFACE_DESCRIPTOR)ConfigurationDescriptor;
	for( Index = 0; Index < ConfigurationDescriptor->bNumInterfaces; Index ++ )
	{
		//	インターフェイス記述子を取得する
		(*InterfaceDescriptor)[Index]	= USBD_ParseConfigurationDescriptorEx(
											 ConfigurationDescriptor
											,(*InterfaceDescriptor)[max( 0, Index - 1 )]
											,Index
											,0
											,-1
											,-1
											,-1 );
		if( (*InterfaceDescriptor)[Index] == NULL )
		{
			Status	= STATUS_UNSUCCESSFUL;
			break;
		}
	}

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	SelectConfiguration
//----------------------------------------------------------------------------------------------
NTSTATUS SelectConfiguration(
	 IN PDEVICE_OBJECT					NextDeviceObject
	,IN PUSB_CONFIGURATION_DESCRIPTOR	ConfigurationDescriptor
	,IN PUSB_INTERFACE_DESCRIPTOR *		InterfaceDescriptor
	,OUT USBD_PIPE_HANDLE *				InputPipeHandle
	,OUT USBD_PIPE_HANDLE *				OutputPipeHandle )
{
	//	変数宣言
	NTSTATUS					Status;
	LONG						InterfaceListEntrySize
									= ( ConfigurationDescriptor->bNumInterfaces + 1 )
										* sizeof( USBD_INTERFACE_LIST_ENTRY );
	PUSBD_INTERFACE_LIST_ENTRY	InterfaceListEntry;
	LONG						Index;
	PURB						Urb;
	PUSBD_INTERFACE_INFORMATION	InterfaceInformation;

	//	インターフェイス リスト用バッファを作成する
	InterfaceListEntry	= ExAllocatePoolWithTag(
							  NonPagedPool
							 ,InterfaceListEntrySize
							 ,'700T' );
	if( InterfaceListEntry == NULL )
	{
		return( STATUS_INSUFFICIENT_RESOURCES );
	}

	//	インターフェイス リストを設定する
	RtlZeroMemory( InterfaceListEntry, InterfaceListEntrySize );
	for( Index = 0; Index < ConfigurationDescriptor->bNumInterfaces; Index ++ )
	{
		InterfaceListEntry[Index].InterfaceDescriptor	= InterfaceDescriptor[Index];
	}

	//	コンフィギュレーションを選択するための URB を作成する
	Urb	= USBD_CreateConfigurationRequestEx(
			 ConfigurationDescriptor
			,InterfaceListEntry );
	if( Urb == NULL )
	{
		ExFreePool( InterfaceListEntry );
		return( STATUS_INSUFFICIENT_RESOURCES );
	}

	//	コンフィギュレーションを選択する為の URB を発行する
	Status	= CallUSBDI( NextDeviceObject, IOCTL_INTERNAL_USB_SUBMIT_URB, Urb );
	if( NT_SUCCESS( Status ) )
	{
		//	インターフェイス インフォメーションを取得する
		InterfaceInformation	= &Urb->UrbSelectConfiguration.Interface;
		//	パイプを取得する
		switch( InterfaceInformation->NumberOfPipes )
		{
			case 0:
				Status	= STATUS_UNSUCCESSFUL;
				break;

			case 1:
				*InputPipeHandle	= InterfaceInformation->Pipes[0].PipeHandle;
				break;

			default:
				*InputPipeHandle	= InterfaceInformation->Pipes[0].PipeHandle;
				*OutputPipeHandle	= InterfaceInformation->Pipes[1].PipeHandle;
				break;
		}
	}

	//	インターフェイス リスト用バッファを解放する
	ExFreePool( InterfaceListEntry );

	//	作成した URB を解放する
	ExFreePool( Urb );

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	DeselectConfiguration
//----------------------------------------------------------------------------------------------
NTSTATUS DeselectConfiguration( IN PDEVICE_OBJECT NextDeviceObject )
{
	//	変数宣言
	NTSTATUS	Status;
	USHORT		UrbSize	= sizeof( struct _URB_CONTROL_DESCRIPTOR_REQUEST );
	PURB		Urb;

	//	URB 用バッファを作成する
	Urb	= (PURB)ExAllocatePoolWithTag( NonPagedPool, UrbSize, '800T' );
	if( Urb == NULL )
	{
		return( STATUS_INSUFFICIENT_RESOURCES );
	}

	//	コンフィギュレーション選択を解除する為の URB を作成する
	UsbBuildSelectConfigurationRequest( Urb, UrbSize, NULL );

	//	コンフィギュレーション選択を解除する為の URB を発行する
	Status	= CallUSBDI( NextDeviceObject, IOCTL_INTERNAL_USB_SUBMIT_URB, Urb );
	if( !NT_SUCCESS( Status ) || !USBD_SUCCESS( Urb->UrbHeader.Status ) )
	{
		ExFreePool( Urb );
		return( STATUS_UNSUCCESSFUL );
	}

	//	作成した URB を解放する
	ExFreePool( Urb );

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	ResetPipe
//----------------------------------------------------------------------------------------------
NTSTATUS ResetPipe(
	 IN PDEVICE_OBJECT		NextDeviceObject
	,IN USBD_PIPE_HANDLE	PipeHandle )
{
	//	パイプをリセットする
	return( PipeRequest( NextDeviceObject, URB_FUNCTION_RESET_PIPE, PipeHandle ) );
}

//----------------------------------------------------------------------------------------------
//	AbortPipe
//----------------------------------------------------------------------------------------------
NTSTATUS AbortPipe(
	 IN PDEVICE_OBJECT		NextDeviceObject
	,IN USBD_PIPE_HANDLE	PipeHandle )
{
	//	パイプをアボートする
	return( PipeRequest( NextDeviceObject, URB_FUNCTION_ABORT_PIPE, PipeHandle ) );
}

//----------------------------------------------------------------------------------------------
//	PipeRequest
//----------------------------------------------------------------------------------------------
NTSTATUS PipeRequest(
	 IN PDEVICE_OBJECT		NextDeviceObject
	,IN USHORT				Function
	,IN USBD_PIPE_HANDLE	PipeHandle )
{
	//	変数宣言
	NTSTATUS	Status;
	USHORT		UrbSize	= sizeof( struct _URB_PIPE_REQUEST );
	PURB		Urb;

	//	URB 用バッファを作成する
	Urb	= (PURB)ExAllocatePoolWithTag( NonPagedPool, UrbSize, '900T' );
	if( Urb == NULL )
	{
		return( STATUS_INSUFFICIENT_RESOURCES );
	}

	//	パイプへの URB を作成する
	Urb->UrbHeader.Length			= (USHORT)sizeof( struct _URB_PIPE_REQUEST );
	Urb->UrbHeader.Function			= Function;
	Urb->UrbPipeRequest.PipeHandle	= PipeHandle;

	//	パイプへの URB を発行する
	Status	= CallUSBDI( NextDeviceObject, IOCTL_INTERNAL_USB_SUBMIT_URB, Urb );
	if( !NT_SUCCESS( Status ) || !USBD_SUCCESS( Urb->UrbHeader.Status ) )
	{
		Status	= STATUS_UNSUCCESSFUL;
	}

	//	作成した URB を解放する
	ExFreePool( Urb );

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	InputReport
//----------------------------------------------------------------------------------------------
NTSTATUS InputReport(
	 IN PDEVICE_OBJECT		NextDeviceObject
	,IN USBD_PIPE_HANDLE	PipeHandle
	,OUT INPUT_REPORT *		InputReport )
{
	//	変数宣言
	NTSTATUS	Status;
	USHORT		UrbSize	= sizeof( struct _URB_BULK_OR_INTERRUPT_TRANSFER );
	PURB		Urb;

	//	URB 用バッファを作成する
	Urb	= (PURB)ExAllocatePoolWithTag( NonPagedPool, UrbSize, '010T' );
	if( Urb == NULL )
	{
		return( STATUS_INSUFFICIENT_RESOURCES );
	}

	//	入力レポートを取得する為の URB を作成する
	UsbBuildInterruptOrBulkTransferRequest(
		 Urb
		,UrbSize
		,PipeHandle
		,InputReport
		,NULL
		,InputReport->Length
		,USBD_TRANSFER_DIRECTION_IN | USBD_SHORT_TRANSFER_OK
		,NULL );

	//	入力レポートを取得する為の URB を発行する
	Status	= CallUSBDI( NextDeviceObject, IOCTL_INTERNAL_USB_SUBMIT_URB, Urb );
	if( !NT_SUCCESS( Status ) || !USBD_SUCCESS( Urb->UrbHeader.Status ) )
	{
		Status	= STATUS_UNSUCCESSFUL;
	}

	//	作成した URB を解放する
	ExFreePool( Urb );

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	OutputReport
//----------------------------------------------------------------------------------------------
NTSTATUS OutputReport(
	 IN PDEVICE_OBJECT		NextDeviceObject
	,IN USBD_PIPE_HANDLE	PipeHandle
	,OUT OUTPUT_REPORT *	OutputReport )
{
	//	変数宣言
	NTSTATUS	Status;
	USHORT		UrbSize	= sizeof( struct _URB_BULK_OR_INTERRUPT_TRANSFER );
	PURB		Urb;

	//	URB 用バッファを作成する
	Urb	= (PURB)ExAllocatePoolWithTag( NonPagedPool, UrbSize, '110T' );
	if( Urb == NULL )
	{
		return( STATUS_INSUFFICIENT_RESOURCES );
	}

	//	出力レポートを設定する為の URB を作成する
	UsbBuildInterruptOrBulkTransferRequest(
		 Urb
		,UrbSize
		,PipeHandle
		,OutputReport
		,NULL
		,OutputReport->Length
		,USBD_SHORT_TRANSFER_OK
		,NULL );
	//	出力レポートを設定する為の URB を発行する
	Status	= CallUSBDI( NextDeviceObject, IOCTL_INTERNAL_USB_SUBMIT_URB, Urb );
	if( !NT_SUCCESS( Status ) || !USBD_SUCCESS( Urb->UrbHeader.Status ) )
	{
		Status	= STATUS_UNSUCCESSFUL;
	}

	//	作成した URB を解放する
	ExFreePool( Urb );

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	CallUSBDI
//----------------------------------------------------------------------------------------------
NTSTATUS CallUSBDI(
	 IN PDEVICE_OBJECT	NextDeviceObject
	,IN ULONG			IOCTLCode
	,IN OUT PVOID		Arg1 )
{
	//	変数宣言
	NTSTATUS			Status;
	PIRP				Irp;
	KEVENT				WaitEvent;
	IO_STATUS_BLOCK		IoStatus;
	PIO_STACK_LOCATION	NextIrpStack;

	//	I/O 要求を作成する
	KeInitializeEvent( &WaitEvent, NotificationEvent, FALSE );
	Irp	= IoBuildDeviceIoControlRequest(
			 IOCTLCode
			,NextDeviceObject
			,NULL
			,0
			,NULL
			,0
			,TRUE
			,&WaitEvent
			,&IoStatus );
	NextIrpStack	= IoGetNextIrpStackLocation( Irp );
	NextIrpStack->Parameters.Others.Argument1	= Arg1;

	//	下位ドライバに I/O 要求を発行する
	Status	= IoCallDriver( NextDeviceObject, Irp );
	if( Status == STATUS_PENDING )
	{
		//	下位ドライバの処理が完了するまで待機する
		KeWaitForSingleObject(
			 &WaitEvent
			,Executive
			,KernelMode
			,FALSE
			,NULL );
		Status	= IoStatus.Status;
	}

	return( Status );
}
