
//	Status Area Icon for Microsoft Xbox 360 Controller

#include	"setting.h"
#include	"resource.h"

//----------------------------------------------------------------------------------------------
//	CSetting
//----------------------------------------------------------------------------------------------
CSetting::CSetting(
	 HINSTANCE	Instance
	,HWND		Wnd )
{
	//	インスタンス
	this->Instance	= Instance;
	//	ウィンドウ ハンドル
	this->Wnd		= Wnd;

	//	コントローラーの設定
	Setting			= NULL;
	//	コントローラーの設定の名称
	SettingName		= NULL;

	//	レジストリから設定を読み込む
	LoadRegistry();
}

//----------------------------------------------------------------------------------------------
//	~CSetting
//----------------------------------------------------------------------------------------------
CSetting::~CSetting()
{
	//	コントローラーの設定、設定の名称の領域を開放する
	if( Setting != NULL )
	{
		LocalFree( Setting );
	}
	if( SettingName != NULL )
	{
		for( LONG Index = 0; Index < SettingCount; Index ++ )
		{
			LocalFree( SettingName[Index] );
		}
		LocalFree( SettingName );
	}
}

//----------------------------------------------------------------------------------------------
//	LoadRegistry
//----------------------------------------------------------------------------------------------
VOID CSetting::LoadRegistry()
{
	//	コントローラーの設定、設定の名称の領域を開放する
	if( Setting != NULL )
	{
		LocalFree( Setting );
	}
	if( SettingName != NULL )
	{
		for( LONG Index = 0; Index < SettingCount; Index ++ )
		{
			LocalFree( SettingName[Index] );
		}
		LocalFree( SettingName );
	}

	//	現在のコントローラーの設定を取得する
	GetDefaultSetting( &CurrentSetting );
	GetRegistry( &CurrentSetting, REG_BINARY, sizeof( SETTING ), CURRENT_SETTING );

	//	現在のコントローラーの設定の順番を取得する
	CurrentSettingIndex	= 0;
	GetRegistry( &CurrentSettingIndex, REG_DWORD, sizeof( LONG ), CURRENT_SETTING_INDEX );

	//	コントローラーの設定数を取得する
	SettingCount	= 1;
	GetRegistry( &SettingCount, REG_DWORD, sizeof( LONG ), SETTING_COUNT );

	//	コントローラーの設定、設定の名称を保存するための領域を確保する
	Setting		= (SETTING *)LocalAlloc( LPTR, SettingCount * sizeof( SETTING ) );
	SettingName	= (WCHAR * *)LocalAlloc( LPTR, SettingCount * sizeof( WCHAR * ) );
	for( LONG Index = 0; Index < SettingCount; Index ++ )
	{
		SettingName[Index]	= (WCHAR *)LocalAlloc( LPTR, sizeof( WCHAR ) * MAX_PATH );
	}

	//	標準の設定を取得する
	GetDefaultSetting( &Setting[0] );
	LoadString( Instance, IDS_DEFAULT_SETTING, SettingName[0], sizeof( WCHAR ) * MAX_PATH );

	//	コントローラーの設定、設定の名称を取得する
	for( LONG Index = 1; Index < SettingCount; Index ++ )
	{
		GetSetting( Index, &Setting[Index] );
		GetSettingName( Index, SettingName[Index], sizeof( WCHAR ) * MAX_PATH );
	}

	//	変更済みフラグを取得する
	ModifiedFlag	= FALSE;
	GetRegistry( &ModifiedFlag, REG_BINARY, sizeof( BOOLEAN ), MODIFIED_FLAG );

	//	設定の自動切換えを取得する
	AutoSettingChange	= FALSE;
	GetRegistry( &AutoSettingChange, REG_BINARY, sizeof( BOOLEAN ), AUTO_SETTING_CHANGE );
}

//----------------------------------------------------------------------------------------------
//	SaveRegistry
//----------------------------------------------------------------------------------------------
VOID CSetting::SaveRegistry()
{
	//	現在のコントローラーの設定を更新する
	SetRegistry( &CurrentSetting, REG_BINARY, sizeof( SETTING ), CURRENT_SETTING );

	//	現在のコントローラーの設定の順番を更新する
	SetRegistry( &CurrentSettingIndex, REG_DWORD, sizeof( LONG ), CURRENT_SETTING_INDEX );

	//	コントローラーの設定数を取得しておく
	LONG	OldSettingCount	= 0;
	GetRegistry( &OldSettingCount, REG_DWORD, sizeof( LONG ), SETTING_COUNT );

	//	コントローラーの設定数を更新する
	SetRegistry( &SettingCount, REG_DWORD, sizeof( LONG ), SETTING_COUNT );

	//	コントローラーの設定、設定の名称を更新する
	for( LONG Index = 1; Index < SettingCount; Index ++ )
	{
		SetSetting( Index, &Setting[Index] );
		SetSettingName( Index, SettingName[Index] );
	}

	//	余分なコントローラーの設定、設定の名称を削除する
	if( OldSettingCount > SettingCount )
	{
		for( LONG Index = SettingCount; Index < OldSettingCount; Index ++ )
		{
			DelSetting( Index );
			DelSettingName( Index );
		}
	}

	//	変更済みフラグを更新する
	SetRegistry( &ModifiedFlag, REG_BINARY, sizeof( BOOLEAN ), MODIFIED_FLAG );

	//	設定の自動切換えを更新する
	SetRegistry( &AutoSettingChange, REG_BINARY, sizeof( BOOLEAN ), AUTO_SETTING_CHANGE );
}

//----------------------------------------------------------------------------------------------
//	SaveAs
//----------------------------------------------------------------------------------------------
VOID CSetting::SaveAs( WCHAR * SaveName )
{
	//	現在のコントローラーの設定の順番を更新する
	CurrentSettingIndex	= SettingCount;

	//	コントローラーの設定数を更新する
	SettingCount ++;

	//	コントローラーの設定を追加する
	Setting	= (SETTING *)LocalReAlloc(
				 Setting
				,SettingCount * sizeof( SETTING )
				,LMEM_MOVEABLE );
	CopyMemory( &Setting[CurrentSettingIndex], &CurrentSetting, sizeof( SETTING ) );
	//	コントローラーの設定の名称を追加する
	SettingName	= (WCHAR * *)LocalReAlloc(
					 SettingName
					,SettingCount * sizeof( WCHAR * )
					,LMEM_MOVEABLE );
	SettingName[CurrentSettingIndex]	= (WCHAR *)LocalAlloc(
											  LPTR
											 ,sizeof( WCHAR ) * MAX_PATH );
	wcscpy_s( SettingName[CurrentSettingIndex], MAX_PATH, SaveName );

	//	変更済みフラグを更新する
	ModifiedFlag	= FALSE;
}

//----------------------------------------------------------------------------------------------
//	Save
//----------------------------------------------------------------------------------------------
VOID CSetting::Save( LONG Index )
{
	//	現在のコントローラーの設定の順番を更新する
	CurrentSettingIndex	= Index;

	//	コントローラーの設定を更新する
	CopyMemory( &Setting[CurrentSettingIndex], &CurrentSetting, sizeof( SETTING ) );

	//	変更済みフラグを更新する
	ModifiedFlag	= FALSE;
}

//----------------------------------------------------------------------------------------------
//	Delete
//----------------------------------------------------------------------------------------------
VOID CSetting::Delete()
{
	//	コントローラーの設定数を更新する
	SettingCount --;
	if( CurrentSettingIndex < SettingCount )
	{
		//	コントローラーの設定、設定の名称を１つずつスライドする
		MoveMemory(
			 &Setting[CurrentSettingIndex]
			,&Setting[CurrentSettingIndex + 1]
			,sizeof( SETTING ) * ( SettingCount - CurrentSettingIndex ) );
		LocalFree( SettingName[CurrentSettingIndex] );
		MoveMemory(
			 &SettingName[CurrentSettingIndex]
			,&SettingName[CurrentSettingIndex + 1]
			,sizeof( WCHAR * ) * ( SettingCount - CurrentSettingIndex ) );
	} else {
		//	現在のコントローラーの設定の順番を更新する
		CurrentSettingIndex --;
	}
	//	現在のコントローラーの設定を更新する
	CopyMemory( &CurrentSetting, &Setting[CurrentSettingIndex], sizeof( SETTING ) );
	//	不要なメモリを開放する
	Setting	= (SETTING *)LocalReAlloc(
				 Setting
				,SettingCount * sizeof( SETTING )
				,LMEM_MOVEABLE );
	SettingName	= (WCHAR * *)LocalReAlloc(
					 SettingName
					,SettingCount * sizeof( WCHAR * )
					,LMEM_MOVEABLE );

	//	変更済みフラグを更新する
	ModifiedFlag	= FALSE;
}

//----------------------------------------------------------------------------------------------
//	Change
//----------------------------------------------------------------------------------------------
VOID CSetting::Change(
	 LONG	Index
	,BOOL	Auto )
{
	//	変更済みなら確認する
	LONG	MsgResult	= IDYES;
	if( ModifiedFlag == TRUE )
	{
		if( Auto == TRUE )
		{
			MsgResult	= IDNO;
		} else {
			MsgResult	= ShowMessageBox(
							 Wnd
							,IDS_WARNING_TITLE
							,IDS_WARNING_MESSAGE
							,MB_YESNO | MB_ICONQUESTION );
		}
	}

	//	コントローラーの設定を変更する
	if( MsgResult == IDYES )
	{
		//	互換性の設定が変わっていればダイアログを表示する
		if( Auto == FALSE )
		{
			if(	CurrentSetting.DevicesArePolled	!= Setting[Index].DevicesArePolled
			||	CurrentSetting.LimitationMode	!= Setting[Index].LimitationMode )
			{
				ShowMessageBox(
					 Wnd
					,IDS_RESTART_TITLE
					,IDS_RESTART_MESSAGE
					,MB_OK | MB_ICONINFORMATION );
			}
		}

		//	現在のコントローラーの設定を更新する
		CopyMemory( &CurrentSetting, &Setting[Index], sizeof( SETTING ) );
		//	現在のコントローラーの設定の順番を更新する
		CurrentSettingIndex	= Index;

		//	変更済みフラグを更新する
		ModifiedFlag	= FALSE;
	}
}

//----------------------------------------------------------------------------------------------
//	ShowMessageBox
//----------------------------------------------------------------------------------------------
LONG CSetting::ShowMessageBox(
	 HWND	Wnd
	,ULONG	TitleID
	,ULONG	MessageID
	,ULONG	Type )
{
	//	メッセージを読み込む
	WCHAR	Title[MAX_PATH];
	LoadString( Instance, TitleID, Title, MAX_PATH );
	WCHAR	Message[MAX_PATH];
	LoadString( Instance, MessageID, Message, MAX_PATH );

	//	メッセージ ボックスを表示する
	return( MessageBox( Wnd, Message, Title, Type ) );
}

//----------------------------------------------------------------------------------------------
//	GetSetting
//----------------------------------------------------------------------------------------------
VOID CSetting::GetSetting(
	 LONG		Index
	,SETTING *	Setting )
{
	//	コントローラーの設定を取得する
	WCHAR	KeyName[MAX_PATH];
	swprintf_s( KeyName, SETTING_DATA, Index );
	GetRegistry( Setting, REG_BINARY, sizeof( SETTING ), KeyName );
}

//----------------------------------------------------------------------------------------------
//	SetSetting
//----------------------------------------------------------------------------------------------
VOID CSetting::SetSetting(
	 LONG		Index
	,SETTING *	Setting )
{
	//	コントローラーの設定を設定する
	WCHAR	KeyName[MAX_PATH];
	swprintf_s( KeyName, SETTING_DATA, Index );
	SetRegistry( Setting, REG_BINARY, sizeof( SETTING ), KeyName );
}

//----------------------------------------------------------------------------------------------
//	DelSetting
//----------------------------------------------------------------------------------------------
VOID CSetting::DelSetting( LONG Index )
{
	//	コントローラーの設定を削除する
	WCHAR	KeyName[MAX_PATH];
	swprintf_s( KeyName, SETTING_DATA, Index );
	DelRegistry( KeyName );
}

//----------------------------------------------------------------------------------------------
//	GetSettingName
//----------------------------------------------------------------------------------------------
VOID CSetting::GetSettingName(
	 LONG		Index
	,WCHAR *	SettingName
	,LONG		Size )
{
	//	コントローラーの設定を取得する
	WCHAR	KeyName[MAX_PATH];
	swprintf_s( KeyName, SETTING_NAME, Index );
	GetRegistry( SettingName, REG_SZ, Size, KeyName );
}

//----------------------------------------------------------------------------------------------
//	SetSettingName
//----------------------------------------------------------------------------------------------
VOID CSetting::SetSettingName(
	 LONG		Index
	,WCHAR *	SettingName )
{
	//	コントローラーの設定を設定する
	WCHAR	KeyName[MAX_PATH];
	swprintf_s( KeyName, SETTING_NAME, Index );
	SetRegistry( SettingName, REG_SZ, sizeof( WCHAR ) * wcslen( SettingName ) + 1, KeyName );
}

//----------------------------------------------------------------------------------------------
//	DelSettingName
//----------------------------------------------------------------------------------------------
VOID CSetting::DelSettingName( LONG Index )
{
	//	コントローラーの設定を削除する
	WCHAR	KeyName[MAX_PATH];
	swprintf_s( KeyName, SETTING_NAME, Index );
	DelRegistry( KeyName );
}

//----------------------------------------------------------------------------------------------
//	GetRegistry
//----------------------------------------------------------------------------------------------
VOID CSetting::GetRegistry(
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
//	SetRegistry
//----------------------------------------------------------------------------------------------
VOID CSetting::SetRegistry(
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
		//	レジストリに書き込む
		RegSetValueEx( Key, ValueName, 0, ValueType, (LPBYTE)Value, ValueSize );
		//	レジストリを閉じる
		RegCloseKey( Key );
	}
}

//----------------------------------------------------------------------------------------------
//	DelRegistry
//----------------------------------------------------------------------------------------------
VOID CSetting::DelRegistry( WCHAR * ValueName )
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
		//	レジストリから削除する
		RegDeleteValue( Key, ValueName );
		//	レジストリを閉じる
		RegCloseKey( Key );
	}
}

//----------------------------------------------------------------------------------------------
//	GetDefaultSetting
//----------------------------------------------------------------------------------------------
VOID CSetting::GetDefaultSetting( SETTING * Setting )
{
	//	変数宣言
	UCHAR	Index;
	UCHAR	WayIndex;

	//	標準の設定を作成する
	ZeroMemory( Setting, sizeof( SETTING ) );

	//	スティック
	Setting->StickUsage[INDEX_LEFT_STICK]	= USAGE_X_AXIS_Y_AXIS;
	Setting->StickUsage[INDEX_RIGHT_STICK]	= USAGE_X_ROTATION_Y_ROTATION;
	for( Index = INDEX_LEFT_STICK; Index <= INDEX_RIGHT_STICK; Index ++ )
	{
		Setting->StickReverseX[Index]			= FALSE;
		Setting->StickReverseY[Index]			= FALSE;
		Setting->StickAngle[Index]				= 0;
		Setting->StickDeadZoneType[Index]		= DEAD_ZONE_TYPE_SQUARE;
		Setting->StickTransformType[Index]		= TRANSFORM_TYPE_NONE;
		Setting->StickMinThreshold[Index]		= 15;
		Setting->StickMaxThreshold[Index]		= 95;
		Setting->StickHatSwitchThreshold[Index]	= 50;
		for( WayIndex = INDEX_WAY_UP; WayIndex <= INDEX_WAY_RIGHT; WayIndex ++ )
		{
			Setting->StickWayUsage[Index][WayIndex]				= USAGE_BUTTON_13 + WayIndex;
			Setting->StickWayRapidFire[Index][WayIndex]			= FALSE;
			Setting->StickWayRapidFireSpeed[Index][WayIndex]	= 100;
		}
	}

	//	方向パッド
	Setting->DirectionalPadUsage			= USAGE_HAT_SWITCH1;
	Setting->DirectionalPadReverseX			= FALSE;
	Setting->DirectionalPadReverseY			= FALSE;
	Setting->DirectionalPadAngle			= 0;
	Setting->DirectionalPadTransformType	= TRANSFORM_TYPE_NONE;
	for( WayIndex = INDEX_WAY_UP; WayIndex <= INDEX_WAY_RIGHT; WayIndex ++ )
	{
		Setting->DirectionalPadWayUsage[WayIndex]			= USAGE_BUTTON_13 + WayIndex;
		Setting->DirectionalPadWayRapidFire[WayIndex]		= FALSE;
		Setting->DirectionalPadWayRapidFireSpeed[WayIndex]	= 100;
	}

	//	トリガー
	for( Index = INDEX_LEFT_TRIGGER; Index <= INDEX_RIGHT_TRIGGER; Index ++ )
	{
		Setting->TriggerUsage[Index]			= USAGE_BUTTON_11 + Index;
		Setting->TriggerRapidFire[Index]		= FALSE;
		Setting->TriggerRapidFireSpeed[Index]	= 100;
		Setting->TriggerMinThreshold[Index]		= 15;
		Setting->TriggerMaxThreshold[Index]		= 95;
		Setting->TriggerButtonThreshold[Index]	= 85;
	}

	//	ボタン
	for( Index = INDEX_A_BUTTON; Index <= INDEX_RIGHT_STICK_BUTTON; Index ++ )
	{
		Setting->ButtonUsage[Index]				= USAGE_BUTTON_1 + Index;
		Setting->ButtonRapidFire[Index]			= FALSE;
		Setting->ButtonRapidFireSpeed[Index]	= 100;
	}

	//	Xbox ガイド ボタン
	Setting->ButtonUsage[INDEX_XBOX_GUIDE_BUTTON]			= USAGE_BUTTON_13;
	Setting->ButtonRapidFire[INDEX_XBOX_GUIDE_BUTTON]		= FALSE;
	Setting->ButtonRapidFireSpeed[INDEX_XBOX_GUIDE_BUTTON]	= 100;

	//	リング ライト
	Setting->RingLightPattern	= 0;

	//	振動
	Setting->Actuator								= TRUE;
	Setting->ActuatorLevel[INDEX_LEFT_ACTUATOR]		= 80;
	Setting->ActuatorLevel[INDEX_RIGHT_ACTUATOR]	= 80;
	Setting->ActuatorMinThreshold					= 20;
	Setting->ActuatorMaxThreshold					= 100;

	//	詳細設定
	for( Index = Z_AXIS; Index <= SLIDER2; Index ++ )
	{
		Setting->SlideSliderValue[Index]			= FALSE;
		Setting->SlideSliderCenterValue[Index]		= FALSE;
		Setting->ReverseSliderValue[Index]			= FALSE;
	}
	Setting->LimitationMode		= LIMITATION_NONE;
	Setting->DevicesArePolled	= TRUE;
}
