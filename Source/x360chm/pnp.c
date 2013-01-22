
//	HID Minidriver for Microsoft Xbox 360 Controller

#include	"driver.h"

//----------------------------------------------------------------------------------------------
//	定数
//----------------------------------------------------------------------------------------------

#define	INTERVAL					-1
#define	INTERVAL_WAIT				-4 * 1000 * 10
#define	INTERVAL_STALL				-16 * 1000 * 10

//----------------------------------------------------------------------------------------------
//	関数
//----------------------------------------------------------------------------------------------

NTSTATUS StartDevice( IN PDEVICE_OBJECT, IN PDEVICE_EXTENSION, IN PIRP );
NTSTATUS QueryCapabilities( IN PDEVICE_OBJECT, IN PIRP, IN PIO_STACK_LOCATION );
NTSTATUS SurpriseRemoval( IN PDEVICE_OBJECT, IN PDEVICE_EXTENSION, IN PIRP );
NTSTATUS RemoveDevice( IN PDEVICE_OBJECT, IN PDEVICE_EXTENSION, IN PIRP );
KSTART_ROUTINE WorkerThreadIn;
KSTART_ROUTINE WorkerThreadOut;

//----------------------------------------------------------------------------------------------
//	PnP
//----------------------------------------------------------------------------------------------
NTSTATUS PnP(
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
		case IRP_MN_START_DEVICE:
			Status	= StartDevice( DeviceObject, DeviceExtension, Irp );
			break;

		case IRP_MN_QUERY_CAPABILITIES:
			Status	= QueryCapabilities( DeviceObject, Irp, CurrentIrpStack );
			break;

		case IRP_MN_SURPRISE_REMOVAL:
			Status	= SurpriseRemoval( DeviceObject, DeviceExtension, Irp );
			break;

		case IRP_MN_REMOVE_DEVICE:
			Status	= RemoveDevice( DeviceObject, DeviceExtension, Irp );
			break;

		default:
			//	IRP_MJ_PNP 要求を下位ドライバに送る
			Status	= SendRequest( DeviceObject, Irp );
			break;
	}

	//	参照カウンタを減算する
	DecrementReference( DeviceExtension );

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	StartDevice
//----------------------------------------------------------------------------------------------
NTSTATUS StartDevice(
	 IN PDEVICE_OBJECT		DeviceObject
	,IN PDEVICE_EXTENSION	DeviceExtension
	,IN PIRP				Irp )
{
	//	変数宣言
	NTSTATUS	Status;

	//	先に IRP_MN_START_DEVICE 要求を下位ドライバに送る
	Status	= SendAndWaitRequest( DeviceObject, Irp );
	if( !NT_SUCCESS( Status ) )
	{
		CompleteRequest( Irp, Status, 0 );
		return( Status );
	}

	//	デバイスを有効にする
	Status	= EnableDevice( DeviceExtension );

	//	IRP_MN_START_DEVICE 要求を完了させる
	CompleteRequest( Irp, Status, 0 );

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	QueryCapabilities
//----------------------------------------------------------------------------------------------
NTSTATUS QueryCapabilities(
	 IN PDEVICE_OBJECT		DeviceObject
	,IN PIRP				Irp
	,IN PIO_STACK_LOCATION	CurrentIrpStack )
{
	//	変数宣言
	NTSTATUS				Status;
	PDEVICE_CAPABILITIES	DeviceCapabilities;

	//	先に IRP_MN_QUERY_CAPABILITIES 要求を下位ドライバに送る
	Status	= SendAndWaitRequest( DeviceObject, Irp );
	if( !NT_SUCCESS( Status ) )
	{
		return( Status );
	}

	//	下位ドライバで設定されたデバイスの機能を変更する
	DeviceCapabilities	= CurrentIrpStack->Parameters.DeviceCapabilities.Capabilities;
	DeviceCapabilities->SurpriseRemovalOK	= TRUE;

	//	IRP_MN_QUERY_CAPABILITIES 要求を完了させる
	CompleteRequest( Irp, Status, 0 );

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	SurpriseRemoval
//----------------------------------------------------------------------------------------------
NTSTATUS SurpriseRemoval(
	 IN PDEVICE_OBJECT		DeviceObject
	,IN PDEVICE_EXTENSION	DeviceExtension
	,IN PIRP				Irp )
{
	//	デバイスを無効にする
	DisableDevice( DeviceExtension );

	//	IRP_MN_SURPRISE_REMOVAL 要求を下位ドライバに送る
	return( SendRequest( DeviceObject, Irp ) );
}

//----------------------------------------------------------------------------------------------
//	RemoveDevice
//----------------------------------------------------------------------------------------------
NTSTATUS RemoveDevice(
	 IN PDEVICE_OBJECT		DeviceObject
	,IN PDEVICE_EXTENSION	DeviceExtension
	,IN PIRP				Irp )
{
	//	デバイスを無効にする
	DisableDevice( DeviceExtension );

	//	IRP_MN_REMOVE_DEVICE 要求を下位ドライバに送る
	return( SendRequest( DeviceObject, Irp ) );
}

//----------------------------------------------------------------------------------------------
//	EnableDevice
//----------------------------------------------------------------------------------------------
NTSTATUS EnableDevice( IN PDEVICE_EXTENSION DeviceExtension )
{
	//	変数宣言
	NTSTATUS	Status;
	HANDLE		ThreadHandle;

	//	デバイスを有効にする
	if( DeviceExtension->DeviceState == Disable )
	{
		//	USB ポートを初期化する
		Status	= SetupPort( DeviceExtension->NextDeviceObject );
		if( !NT_SUCCESS( Status ) )
		{
			return( Status );
		}

		//	デバイス記述子を取得する
		Status	= GetDeviceDescriptor(
					 DeviceExtension->NextDeviceObject
					,&DeviceExtension->DeviceDescriptor );
		if( !NT_SUCCESS( Status ) )
		{
			return( Status );
		}

		//	コンフィギュレーション記述子を取得する
		Status	= GetConfigurationDescriptor(
					 DeviceExtension->NextDeviceObject
					,&DeviceExtension->ConfigurationDescriptor );
		if( !NT_SUCCESS( Status ) )
		{
			return( Status );
		}

		//	インターフェイス記述子を取得する
		Status	= GetInterfaceDescriptor(
					 DeviceExtension->ConfigurationDescriptor
					,&DeviceExtension->InterfaceDescriptor );
		if( !NT_SUCCESS( Status ) )
		{
			return( Status );
		}

		//	コンフィギュレーションを選択する
		Status	= SelectConfiguration(
					 DeviceExtension->NextDeviceObject
					,DeviceExtension->ConfigurationDescriptor
					,DeviceExtension->InterfaceDescriptor
					,&DeviceExtension->InputPipeHandle
					,&DeviceExtension->OutputPipeHandle );
		if( !NT_SUCCESS( Status ) )
		{
			return( Status );
		}

		//	設定を読み込ませる
		DeviceExtension->ResetDevice	= TRUE;

		//	デバイスの状態を有効にする
		DeviceExtension->DeviceState	= Enable;
	}

	//	ワーカ スレッドを開始する
	if( DeviceExtension->ThreadState == Disable )
	{
		//	入力用ワーカ スレッドを開始する
		DeviceExtension->ThreadState	= Enable;
		PsCreateSystemThread(
			 &ThreadHandle
			,THREAD_ALL_ACCESS
			,NULL
			,NULL
			,NULL
			,WorkerThreadIn
			,DeviceExtension );
		ObReferenceObjectByHandle(
			 ThreadHandle
			,THREAD_ALL_ACCESS
			,NULL
			,KernelMode
			,&DeviceExtension->ThreadObjectIn
			,NULL );
		ZwClose( ThreadHandle );

		//	出力用ワーカ スレッドを開始する
		if( DeviceExtension->OutputPipeHandle != NULL )
		{
			PsCreateSystemThread(
				 &ThreadHandle
				,THREAD_ALL_ACCESS
				,NULL
				,NULL
				,NULL
				,WorkerThreadOut
				,DeviceExtension );
			ObReferenceObjectByHandle(
				 ThreadHandle
				,THREAD_ALL_ACCESS
				,NULL
				,KernelMode
				,&DeviceExtension->ThreadObjectOut
				,NULL );
			ZwClose( ThreadHandle );
		}
	}

	return( Status );
}

//----------------------------------------------------------------------------------------------
//	WorkerThreadIn
//----------------------------------------------------------------------------------------------
VOID WorkerThreadIn( IN PVOID Context )
{
	//	変数宣言
	NTSTATUS			Status;
	PDEVICE_EXTENSION	DeviceExtension	= Context;
	SETTING				Setting;
	KIRQL				Irql;
	INPUT_REPORT		ControllerReport;
	LARGE_INTEGER		TimeOut;

	//	スレッドの優先順位を下げる
	KeSetPriorityThread( KeGetCurrentThread(), LOW_REALTIME_PRIORITY );

	for(;;)
	{
		//	スレッドの終了をチェックする
		if( DeviceExtension->ThreadState == Disable )
		{
			break;
		}

		//	コントローラーの設定を読み込む
		if(	DeviceExtension->ResetDevice		== TRUE
		&&	DeviceExtension->OutputPipeHandle	== NULL )
		{
			//	設定を読み込む
			LoadSetting( &Setting );

			//	入力レポートからの割り込みを禁止する
			KeAcquireSpinLock( &DeviceExtension->LockReport, &Irql );

			//	設定を反映する
			if( DeviceExtension->ResetDevice == TRUE )
			{
				RtlCopyMemory( &DeviceExtension->Setting, &Setting, sizeof( SETTING ) );
				DeviceExtension->ResetDevice	= FALSE;
			}

			//	入力レポートからの割り込みを許可する
			KeReleaseSpinLock( &DeviceExtension->LockReport, Irql );
		}

		//	コントローラー レポートの入力を行う
		RtlZeroMemory( &ControllerReport, sizeof( INPUT_REPORT ) );
		ControllerReport.Length	= sizeof( INPUT_REPORT );
		Status	= InputReport(
					 DeviceExtension->NextDeviceObject
					,DeviceExtension->InputPipeHandle
					,&ControllerReport );
		if( NT_SUCCESS( Status ) )
		{
			//	入力レポートからの割り込みを禁止する
			KeAcquireSpinLock( &DeviceExtension->LockReport, &Irql );

			//	コントローラー レポートを反映する
			if( ControllerReport.Length == sizeof( INPUT_REPORT ) )
			{
				RtlCopyMemory(
					 &DeviceExtension->ControllerReport
					,&ControllerReport
					,sizeof( INPUT_REPORT ) );
			}
			TimeOut.QuadPart	= INTERVAL;

			//	入力レポートからの割り込みを許可する
			KeReleaseSpinLock( &DeviceExtension->LockReport, Irql );
		} else {
			//	エラーが発生したらパイプをリセットする
			if( Status != STATUS_CANCELLED && Status != STATUS_DEVICE_NOT_CONNECTED )
			{
				ResetPipe(
					 DeviceExtension->NextDeviceObject
					,DeviceExtension->InputPipeHandle );
			}
			TimeOut.QuadPart	= INTERVAL_STALL;
		}
		//	待機する
		KeDelayExecutionThread( KernelMode, FALSE, &TimeOut );
	}

	//	スレッドを開放する
	ObDereferenceObject( DeviceExtension->ThreadObjectIn );
	DeviceExtension->ThreadObjectIn	= NULL;

	//	スレッドが終了したことを通知する
	KeSetEvent( &DeviceExtension->ThreadExitingIn, 0 ,FALSE );

	//	スレッドを終了する
	PsTerminateSystemThread( STATUS_SUCCESS );
}

//----------------------------------------------------------------------------------------------
//	WorkerThreadOut
//----------------------------------------------------------------------------------------------
VOID WorkerThreadOut( IN PVOID Context )
{
	//	変数宣言
	NTSTATUS			Status;
	PDEVICE_EXTENSION	DeviceExtension		= Context;
	SETTING				Setting;
	KIRQL				Irql;
	POUTPUT_REPORT		RingLightReport		= &DeviceExtension->RingLightReport;
	POUTPUT_REPORT		ActuatorsReport		= &DeviceExtension->ActuatorsReport;
	OUTPUT_REPORT		PrevActuatorsReport	= *ActuatorsReport;
	LONG				ReportCount			= 0;
	LARGE_INTEGER		TimeOut;

	//	スレッドの優先順位を下げる
	KeSetPriorityThread( KeGetCurrentThread(), LOW_REALTIME_PRIORITY );

	for(;;)
	{
		//	スレッドの終了をチェックする
		if( DeviceExtension->ThreadState == Disable )
		{
			break;
		}

		//	コントローラーの設定を読み込む
		if( DeviceExtension->ResetDevice == TRUE )
		{
			//	設定を読み込む
			LoadSetting( &Setting );

			//	入力レポートからの割り込みを禁止する
			KeAcquireSpinLock( &DeviceExtension->LockReport, &Irql );

			//	設定を反映する
			if( DeviceExtension->ResetDevice == TRUE )
			{
				RtlCopyMemory( &DeviceExtension->Setting, &Setting, sizeof( SETTING ) );
				DeviceExtension->ActuatorsReported	= FALSE;
				DeviceExtension->RingLightReported	= FALSE;
				DeviceExtension->ResetDevice		= FALSE;
			}

			//	入力レポートからの割り込みを許可する
			KeReleaseSpinLock( &DeviceExtension->LockReport, Irql );
		}

		//	リング ライト レポートの出力を行う
		if( DeviceExtension->RingLightReported == FALSE )
		{
			//	リング ライト レポートを作成する
			RingLightReport->Type		= 1;
			RingLightReport->Length		= 3;
			RingLightReport->Pattern	= (UCHAR)DeviceExtension->Setting.RingLightPattern;
			//	リング ライト レポートを出力する
			Status	= OutputReport(
						 DeviceExtension->NextDeviceObject
						,DeviceExtension->OutputPipeHandle
						,&DeviceExtension->RingLightReport );
			if( NT_SUCCESS( Status ) )
			{
				DeviceExtension->RingLightReported	= TRUE;
			}
		}

		//	アクチュエータ レポートを作成する
		ActuatorsReport->Type	= 0;
		ActuatorsReport->Length	= sizeof( OUTPUT_REPORT );
		if( DeviceExtension->ActuatorsReported == FALSE )
		{
			ActuatorsReport->Left	= 0;
			ActuatorsReport->Right	= 0;
		}

		//	同一の値でレポートした回数を初期化する
		if(	ActuatorsReport->Left				!= PrevActuatorsReport.Left
		||	ActuatorsReport->Right				!= PrevActuatorsReport.Right
		||	DeviceExtension->ActuatorsReported	== FALSE )
		{
			ReportCount	= 0;
		}

		//	アクチュエータ レポートを出力する
		if( ReportCount < 250 )
		{
			Status	= OutputReport(
						 DeviceExtension->NextDeviceObject
						,DeviceExtension->OutputPipeHandle
						,&DeviceExtension->ActuatorsReport );
			if( NT_SUCCESS( Status ) )
			{
				//	レポートの内容を退避する
				PrevActuatorsReport					= *ActuatorsReport;
				ReportCount ++;
				DeviceExtension->ActuatorsReported	= TRUE;
				TimeOut.QuadPart					= INTERVAL;
			} else {
				//	エラーが発生したらパイプをリセットする
				if( Status != STATUS_CANCELLED && Status != STATUS_DEVICE_NOT_CONNECTED )
				{
					ResetPipe(
						 DeviceExtension->NextDeviceObject
						,DeviceExtension->OutputPipeHandle );
				}
				TimeOut.QuadPart	= INTERVAL_STALL;
			}
		} else {
			TimeOut.QuadPart	= INTERVAL_WAIT;
		}
		//	待機する
		KeDelayExecutionThread( KernelMode, FALSE, &TimeOut );
	}

	//	スレッドを開放する
	ObDereferenceObject( DeviceExtension->ThreadObjectOut );
	DeviceExtension->ThreadObjectOut	= NULL;

	//	スレッドが終了したことを通知する
	KeSetEvent( &DeviceExtension->ThreadExitingOut, 0 ,FALSE );

	//	スレッドを終了する
	PsTerminateSystemThread( STATUS_SUCCESS );
}

//----------------------------------------------------------------------------------------------
//	DisableDevice
//----------------------------------------------------------------------------------------------
NTSTATUS DisableDevice( IN PDEVICE_EXTENSION DeviceExtension )
{
	//	ワーカ スレッドが終了するのを待つ
	if( DeviceExtension->ThreadState == Enable )
	{
		//	ワーカ スレッドを終了する
		DeviceExtension->ThreadState	= Disable;

		//	入力用パイプをアボートする
		AbortPipe( DeviceExtension->NextDeviceObject, DeviceExtension->InputPipeHandle );
		//	入力用ワーカ スレッドが完了するのを待つ
		KeWaitForSingleObject(
			 &DeviceExtension->ThreadExitingIn
			,Executive
			,KernelMode
			,FALSE
			,NULL );

		if( DeviceExtension->OutputPipeHandle != NULL )
		{
			//	出力用パイプをアボートする
			AbortPipe( DeviceExtension->NextDeviceObject, DeviceExtension->OutputPipeHandle );
			//	出力用ワーカ スレッドが完了するのを待つ
			KeWaitForSingleObject(
				 &DeviceExtension->ThreadExitingOut
				,Executive
				,KernelMode
				,FALSE
				,NULL );
		}
	}

	//	ドライバの状態を無効にする
	DeviceExtension->DriverState	= Disable;

	//	参照終了イベントを初期化する
	KeResetEvent( &DeviceExtension->ReferenceEnd );

	//	参照カウンタを減算する
	DecrementReference( DeviceExtension );
	DecrementReference( DeviceExtension );

	//	全ての I/O 要求が終了し、参照終了が通知されるまで待機する
	KeWaitForSingleObject( &DeviceExtension->ReferenceEnd, Executive, KernelMode, FALSE, NULL);

	//	デバイスを無効にする
	if( DeviceExtension->DeviceState == Enable )
	{
		//	コンフィギュレーションの選択を解除する
		DeselectConfiguration( DeviceExtension->NextDeviceObject );

		//	コンフィギュレーション記述子用バッファが確保されていれば開放する
		if( DeviceExtension->ConfigurationDescriptor != NULL )
		{
			ExFreePool( DeviceExtension->ConfigurationDescriptor );
			DeviceExtension->ConfigurationDescriptor	= NULL;
		}

		//	インターフェイス記述子用バッファが確保されていれば開放する
		if( DeviceExtension->InterfaceDescriptor != NULL )
		{
			ExFreePool( DeviceExtension->InterfaceDescriptor );
			DeviceExtension->InterfaceDescriptor	= NULL;
		}

		//	デバイスの状態を無効にする
		DeviceExtension->DeviceState	= Disable;
	}

	//	ドライバの状態を有効にする
	DeviceExtension->DriverState	= Enable;

	//	参照カウンタの数を合わせる
	IncrementReference( DeviceExtension );
	IncrementReference( DeviceExtension );

	return( STATUS_SUCCESS );
}
