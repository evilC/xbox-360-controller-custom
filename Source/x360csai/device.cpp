
//	Status Area Icon for Microsoft Xbox 360 Controller

#include	"device.h"

//----------------------------------------------------------------------------------------------
//	CDevice
//----------------------------------------------------------------------------------------------
CDevice::CDevice()
{
	//	サポートしているデバイスの数を取得する
	SupportedDeviceCount	= 0;
	GetRegistry(
		 &SupportedDeviceCount
		,REG_DWORD
		,sizeof( LONG )
		,SUPPORTED_DEVICE_COUNT );
	SupportedDeviceCount	= max( 0, min( SupportedDeviceCount, 256 ) );

	//	サポートしているデバイスの属性を取得する
	if( SupportedDeviceCount > 0 )
	{
		SupportedDeviceAttribute	= (ULONG *)LocalAlloc(
										  LPTR
										 ,SupportedDeviceCount * sizeof( ULONG ) );
		for( LONG Index = 0; Index < SupportedDeviceCount; Index ++ )
		{
			//	レジストリからデバイスの属性を取得する
			WCHAR	Attribute[32];
			swprintf_s( Attribute, SUPPORTED_DEVICE_ATTRIBUTE, Index );
			GetRegistry(
				 &SupportedDeviceAttribute[Index]
				,REG_DWORD
				,sizeof( ULONG )
				,Attribute );
		}
	} else {
		SupportedDeviceAttribute	= NULL;
	}
}

//----------------------------------------------------------------------------------------------
//	~CDevice
//----------------------------------------------------------------------------------------------
CDevice::~CDevice()
{
	//	サポートしているデバイスの属性を開放する
	if( SupportedDeviceAttribute != NULL )
	{
		LocalFree( SupportedDeviceAttribute );
	}
}

//----------------------------------------------------------------------------------------------
//	GetRegistry
//----------------------------------------------------------------------------------------------
VOID CDevice::GetRegistry(
	 VOID *		Value
	,ULONG		ValueType
	,ULONG		ValueSize
	,WCHAR *	ValueName )
{
	//	変数宣言
	HKEY	Key;
	DWORD	Disposition;
	LONG	Result;

	//	レジストリを開く
	Result	= RegCreateKeyEx(
				 HKEY_LOCAL_MACHINE
				,REGISTRY_KEY
				,0
				,NULL
				,REG_OPTION_NON_VOLATILE
				,KEY_ALL_ACCESS
				,NULL
				,&Key
				,&Disposition  );
	if( Result == ERROR_SUCCESS )
	{
		//	レジストリから読み込む
		RegQueryValueEx( Key, ValueName, NULL, &ValueType, (LPBYTE)Value, &ValueSize );
		//	レジストリを閉じる
		RegCloseKey( Key );
	}
}

//----------------------------------------------------------------------------------------------
//	IsConnected
//----------------------------------------------------------------------------------------------
BOOL CDevice::IsConnected()
{
	//	デバイスが接続されているかどうかを返す
	return( IoDevice( IO_MODE_IS_CONNECTED, NULL, NULL ) );
}

//----------------------------------------------------------------------------------------------
//	GetState
//----------------------------------------------------------------------------------------------
BOOL CDevice::GetState( FEATURE_REPORT_ID * FeatureReport )
{
	//	デバイスの状態を取得する
	return( IoDevice( IO_MODE_GET_STATE, NULL, FeatureReport ) );
}

//----------------------------------------------------------------------------------------------
//	SetState
//----------------------------------------------------------------------------------------------
BOOL CDevice::SetState( HID_WRITE_REPORT_ID * WriteReport )
{
	//	デバイスの状態を設定する
	return( IoDevice( IO_MODE_SET_STATE, WriteReport, NULL ) );
}

//----------------------------------------------------------------------------------------------
//	Apply
//----------------------------------------------------------------------------------------------
BOOL CDevice::Apply()
{
	//	デバイスに変更を通知する
	return( IoDevice( IO_MODE_APPLY, NULL, NULL ) );
}

//----------------------------------------------------------------------------------------------
//	IoDevice
//----------------------------------------------------------------------------------------------
BOOL CDevice::IoDevice(
	 LONG					IoMode
	,HID_WRITE_REPORT_ID *	WriteReport
	,FEATURE_REPORT_ID *	FeatureReport )
{
	//	変数宣言
	GUID				GUID_HID;
	LONG				InterfaceIndex	= 0;
	BOOL				DeviceFound		= FALSE;
	FEATURE_REPORT_ID	Report;

	//	デバイス クラスのハンドルを取得する
	HidD_GetHidGuid( &GUID_HID );
	HDEVINFO	DevInfo	= SetupDiGetClassDevs(
							 &GUID_HID
							,NULL
							,NULL
							,DIGCF_PRESENT | DIGCF_INTERFACEDEVICE );
	if( DevInfo == INVALID_HANDLE_VALUE )
	{
		return( FALSE );
	}

	//	対象のインターフェイスを探す
	for(;;)
	{
		//	インターフェイスを取得する
		SP_INTERFACE_DEVICE_DATA	Interface;
		Interface.cbSize	= sizeof( SP_INTERFACE_DEVICE_DATA );
		BOOL	Result	= SetupDiEnumDeviceInterfaces(
							 DevInfo
							,NULL
							,&GUID_HID
							,InterfaceIndex
							,&Interface );
		if( Result == FALSE )
		{
			SetupDiDestroyDeviceInfoList( DevInfo );
			return( DeviceFound );
		}

		//	インターフェイスの詳細情報のサイズを取得する
		DWORD	DetailSize;
		SetupDiGetDeviceInterfaceDetail( DevInfo, &Interface, NULL, 0, &DetailSize, NULL );

		//	インターフェイスの詳細情報を取得する
		PSP_INTERFACE_DEVICE_DETAIL_DATA	Detail;
		Detail	= (PSP_INTERFACE_DEVICE_DETAIL_DATA)LocalAlloc( LPTR, DetailSize );
		if( Detail == NULL )
		{
			SetupDiDestroyDeviceInfoList( DevInfo );
			return( DeviceFound );
		}
		Detail->cbSize	= sizeof( SP_INTERFACE_DEVICE_DETAIL_DATA );
		Result	= SetupDiGetDeviceInterfaceDetail(
					 DevInfo
					,&Interface
					,Detail
					,DetailSize
					,NULL
					,NULL );
		if( Result == FALSE )
		{
			SetupDiDestroyDeviceInfoList( DevInfo );
			LocalFree( Detail );
			return( DeviceFound );
		}

		//	デバイス インターフェイスを開く
		HANDLE	Device	= CreateFile(
							 Detail->DevicePath
							,GENERIC_READ | GENERIC_WRITE
							,FILE_SHARE_READ | FILE_SHARE_WRITE
							,NULL
							,OPEN_EXISTING
							,FILE_ATTRIBUTE_NORMAL
							,NULL );
		if( Device == INVALID_HANDLE_VALUE )
		{
			LocalFree( Detail );
			InterfaceIndex	++;
			continue;
		}

		//	インターフェイスの詳細にベンダ ID、プロダクト ID が含まれるかチェックする
		HIDD_ATTRIBUTES	HidAttribute;
		HidD_GetAttributes( Device, &HidAttribute );
		BOOL	TargetDeviceFound	= FALSE;
		for( LONG DeviceIndex = 0; DeviceIndex < SupportedDeviceCount; DeviceIndex ++ )
		{
			if(	HidAttribute.VendorID	== HIWORD( SupportedDeviceAttribute[DeviceIndex] )
			&&	HidAttribute.ProductID	== LOWORD( SupportedDeviceAttribute[DeviceIndex] ) )
			{
				TargetDeviceFound	= TRUE;
				break;
			}
		}

		//	デバイスを発見した場合
		if( TargetDeviceFound == TRUE )
		{
			switch( IoMode )
			{
				case IO_MODE_IS_CONNECTED:
					;
					break;

				case IO_MODE_GET_STATE:
					//	デバイスの状態を取得する
					ZeroMemory( &Report, sizeof( FEATURE_REPORT_ID ) );
					Result	= HidD_GetFeature( Device, &Report, sizeof( FEATURE_REPORT_ID ) );
					if( Result == TRUE )
					{
						//	複数コントローラに対処するため、データを加算する
						//	デジタル ボタン
						FeatureReport->DigitalButton	|= Report.DigitalButton;
						//	左トリガー
						FeatureReport->LeftTrigger		= ADD_UCHAR(
															 FeatureReport->LeftTrigger
															,Report.LeftTrigger );
						//	右トリガー
						FeatureReport->RightTrigger		= ADD_UCHAR(
															 FeatureReport->RightTrigger
															,Report.RightTrigger );
						//	左スティック X
						FeatureReport->LeftStickX		= ADD_SHORT(
															 FeatureReport->LeftStickX
															,Report.LeftStickX );
						//	左スティック Y
						FeatureReport->LeftStickY		= ADD_SHORT(
															 FeatureReport->LeftStickY
															,Report.LeftStickY );
						//	左スティック X
						FeatureReport->RightStickX		= ADD_SHORT(
															 FeatureReport->RightStickX
															,Report.RightStickX );
						//	左スティック Y
						FeatureReport->RightStickY		= ADD_SHORT(
															 FeatureReport->RightStickY
															,Report.RightStickY );
					}
					break;

				case IO_MODE_SET_STATE:
					//	デバイスの状態を設定する
					DWORD	ReqLen;
					WriteFile(
						 Device
						,WriteReport
						,sizeof( HID_WRITE_REPORT_ID )
						,&ReqLen
						,NULL );
					break;

				case IO_MODE_APPLY:
					//	デバイスに変更を通知する
					ZeroMemory( &Report, sizeof( FEATURE_REPORT_ID ) );
					HidD_SetFeature( Device, &Report, sizeof( FEATURE_REPORT_ID ) );
					break;
			}
			DeviceFound	= TRUE;
		}

		//	デバイス インターフェイスを閉じる
		CloseHandle( Device );

		//	次のインターフェイスへ
		LocalFree( Detail );
		InterfaceIndex	++;
	}

	//	デバイス クラスのハンドルを開放する
	SetupDiDestroyDeviceInfoList( DevInfo );

	return( DeviceFound );
}
