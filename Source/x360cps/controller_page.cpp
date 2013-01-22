
//	Property Sheet for Microsoft Xbox 360 Controller

#include	"controller_page.h"
#include	"extension.h"
#include	"save_dialog.h"
#include	"resource.h"

//----------------------------------------------------------------------------------------------
//	CControllerPage
//----------------------------------------------------------------------------------------------
CControllerPage::CControllerPage( CSetting * Setting )
{
	//	設定
	this->Setting	= Setting;

	//	現在のコントローラーの設定
	CurrentSetting	= &Setting->CurrentSetting;

	//	デバイスに適用されているセッティングを退避する
	CopyMemory( &DeviceSetting, CurrentSetting, sizeof( SETTING ) );

	//	各インスタンスを作成する
	Device			= new CDevice();
	Report			= new CReport();
}

//----------------------------------------------------------------------------------------------
//	wmInitDialog
//----------------------------------------------------------------------------------------------
VOID CControllerPage::wmInitDialog( HWND Wnd )
{
	//	ウィンドウ ハンドル
	this->Wnd	= Wnd;

	//	ステータス エリア アイコンとの排他制御を行う
	Mutex	= CreateMutex( NULL, TRUE, X360CPS_MUTEX );

	//	「振動」
	InitTrackBar( IDC_FORCE_FEEDBACK, 0, 100, 10, 10 );

	//	ポーリング用タイマーを開始する
	SetTimer( Wnd, POLLING_TIMER_ID, POLLING_INTERVAL, NULL );
}

//----------------------------------------------------------------------------------------------
//	wmCommand
//----------------------------------------------------------------------------------------------
VOID CControllerPage::wmCommand(
	 HWND	Control
	,WORD	ControlID
	,WORD	NotifyMessage )
{
	//	コントロールによって処理を振り分ける
	switch( ControlID )
	{
		case IDC_SETTING:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case CBN_SELCHANGE:
					//	設定を変更する
					ChangeSetting();
					break;
			}
			break;

		case IDC_SETTING_SAVE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case BN_CLICKED:
					//	設定を保存する
					SaveSetting();
					break;
			}
			break;

		case IDC_SETTING_DELETE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case BN_CLICKED:
					//	設定を削除する
					DeleteSetting();
					break;
			}
			break;
	}
}

//----------------------------------------------------------------------------------------------
//	wmNotify
//----------------------------------------------------------------------------------------------
VOID CControllerPage::wmNotify( NMHDR * Notify )
{
	//	通知メッセージによって処理を振り分ける
	switch( Notify->code )
	{
		case PSN_APPLY:
			//	互換性の設定が変わっていればダイアログを表示する
			if( ( DeviceSetting.DevicesArePolled	!= CurrentSetting->DevicesArePolled )
			||	( DeviceSetting.LimitationMode		!= CurrentSetting->LimitationMode ) )
			{
				ShowMessageBox(
					 IDS_RESTART_TITLE
					,IDS_RESTART_MESSAGE
					,NULL
					,MB_OK | MB_ICONINFORMATION );
			}

			//	コントローラーの設定を変更する
			Setting->SaveRegistry();
			Device->Apply();

			//	ステータス エリア アイコンに変更を通知する
			SendMessage(
				 FindWindow( X360CSAI_CLASS_NAME, NULL )
				,WM_STATUS_AREA_ICON_NOTIFY
				,UPDATE_SETTING
				,NULL );

			//	プロパティ シートを閉じる
			if( ((PSHNOTIFY *)Notify)->lParam == TRUE )
			{
				ClosePage();
			}
			break;

		case PSN_RESET:
			//	プロパティ シートを閉じる
			ClosePage();
			break;
	}
}

//----------------------------------------------------------------------------------------------
//	wmTimer
//----------------------------------------------------------------------------------------------
VOID CControllerPage::wmTimer( WPARAM TimerId )
{
	switch( TimerId )
	{
		case POLLING_TIMER_ID:
			//	ポーリング用タイマーを停止する
			KillTimer( Wnd, TimerId );

			//	コントローラーの状態を表示する
			ShowControllerState();

			//	振動をテストする
			if( CurrentSetting->Actuator == TRUE )
			{
				//	テスト値を取得する
				LONG TestLevel	= (LONG)SendMessage(
									 GetDlgItem( Wnd, IDC_FORCE_FEEDBACK )
									,TBM_GETPOS
									,0
									,0 ) * 255 / 100;
				//	振動をテストする
				TestForceFeedback( TestLevel );
			} else {
				//	振動をテストする
				TestForceFeedback( 0 );
			}

			//	ポーリング用タイマーを開始する
			SetTimer( Wnd, POLLING_TIMER_ID, POLLING_INTERVAL, NULL );
			break;
	}
}

//----------------------------------------------------------------------------------------------
//	wmPageChanged
//----------------------------------------------------------------------------------------------
VOID CControllerPage::wmPageChanged()
{
	//	変更された設定を反映する
	ApplyChangedSetting();
}

//----------------------------------------------------------------------------------------------
//	ApplySetting
//----------------------------------------------------------------------------------------------
VOID CControllerPage::ApplySetting()
{
	//	変数宣言
	BOOL	Different	= FALSE;

	//	コンボ ボックスの内容が異なるかチェックする
	HWND	ComboBox		= GetDlgItem( Wnd, IDC_SETTING );
	LONG	ItemCount		= (LONG)SendMessage( ComboBox, CB_GETCOUNT, 0, 0 );
	if( Setting->ModifiedFlag == TRUE )
	{
		ItemCount --;
	}
	if( ItemCount == Setting->SettingCount )
	{
		//	アイテムの文字列をチェックする
		for( LONG Index = 0; Index < Setting->SettingCount; Index ++ )
		{
			//	コンボ ボックスの文字列を取得する
			WCHAR	Text[MAX_PATH]	= { '\n' };
			if( Setting->ModifiedFlag == TRUE )
			{
				SendMessage( ComboBox, CB_GETLBTEXT, Index + 1, (LPARAM)Text );
			} else {
				SendMessage( ComboBox, CB_GETLBTEXT, Index, (LPARAM)Text );
			}
			if( wcscmp( Setting->SettingName[Index], Text ) != 0 )
			{
				Different	= TRUE;
				break;
			}
		}
	} else {
		Different	= TRUE;
	}

	//	コンボ ボックスの内容を設定する
	if( Different == TRUE )
	{
		//	コンボ ボックスの内容をリセットする
		SendMessage( ComboBox, CB_RESETCONTENT, 0, 0 );

		//	設定名称をコンボ ボックスに挿入する
		for( LONG Index = 0; Index < Setting->SettingCount; Index ++ )
		{
			SendMessage(
				 ComboBox
				,CB_INSERTSTRING
				,Index
				,(LPARAM)Setting->SettingName[Index] );
		}

		//	設定が変更済みなら一番上に「～ (変更済み)」を挿入する
		if( Setting->ModifiedFlag == TRUE )
		{
			//	設定名称を取得する
			WCHAR	SettingName[MAX_PATH * 2];
			wcscpy_s( SettingName, Setting->SettingName[Setting->CurrentSettingIndex] );

			//	リソースから「(変更済み)」を取得し、設定名称に付加する
			WCHAR	Modified[MAX_PATH];
			LoadString( Instance, IDS_MODIFIED, Modified, MAX_PATH );
			wcscat_s( SettingName, Modified );

			//	変更済みの設定名称をコンボ ボックスの一番上に挿入する
			SendMessage( ComboBox, CB_INSERTSTRING, 0, (LPARAM)SettingName );
		}
	}

	//	コンボ ボックスの順番を設定する
	if( Setting->ModifiedFlag == FALSE )
	{
		SetComboBoxIndex( IDC_SETTING, (UCHAR)Setting->CurrentSettingIndex );
	} else {
		SetComboBoxIndex( IDC_SETTING, 0 );
	}
	//	削除ボタンの有効、無効を設定する
	if( Setting->ModifiedFlag == FALSE && Setting->CurrentSettingIndex > 0 )
	{
		EnableControl( IDC_SETTING_DELETE, TRUE );
	} else {
		EnableControl( IDC_SETTING_DELETE, FALSE );
	}
}

//----------------------------------------------------------------------------------------------
//	ChangeSetting
//----------------------------------------------------------------------------------------------
VOID CControllerPage::ChangeSetting()
{
	//	変数宣言
	LONG	MsgResult;

	//	変更済みかどうかで処理を振り分ける
	if( Setting->ModifiedFlag == TRUE )
	{
		//	同じ順番を選択したなら何もしない
		if( GetComboBoxIndex( IDC_SETTING ) == 0 )
		{
			return;
		}

		//	ダイアログを表示し、処理を振り分ける
		MsgResult	= ShowMessageBox(
						 IDS_SAVE_TITLE
						,IDS_SAVE_MESSAGE
						,NULL
						,MB_YESNOCANCEL | MB_ICONQUESTION );
		switch( MsgResult )
		{
			case IDYES:
				//	コントローラーの設定を保存する
				MsgResult	= (LONG)DialogBoxParam(
								 Instance
								,MAKEINTRESOURCE( IDD_SAVE_DIALOG )
								,Wnd
								,(DLGPROC)SaveDialogProc
								,(LPARAM)Setting );
				if( MsgResult == IDOK )
				{
					//	変更された設定を反映する
					ApplyChangedSetting();
				}
				if( MsgResult == IDCANCEL )
				{
					//	キャンセルならコントローラーの設定を 0 に戻す
					SetComboBoxIndex( IDC_SETTING, 0 );
				}
				break;

			case IDNO:
				//	コントローラーの設定を変更する
				Setting->ModifiedFlag	= FALSE;
				Setting->Change( GetComboBoxIndex( IDC_SETTING ) - 1, TRUE );
				//	変更された設定を反映する
				ApplyChangedSetting();
				break;

			case IDCANCEL:
				//	キャンセルならコントローラーの設定を 0 に戻す
				SetComboBoxIndex( IDC_SETTING, 0 );
				break;
		}

	} else {
		//	同じ順番を選択したなら何もしない
		if( GetComboBoxIndex( IDC_SETTING ) == Setting->CurrentSettingIndex )
		{
			return;
		}

		//	選択された設定に変更する
		Setting->Change( GetComboBoxIndex( IDC_SETTING ), TRUE );
		//	変更された設定を反映する
		ApplyChangedSetting();
	}
}

//----------------------------------------------------------------------------------------------
//	SaveSetting
//----------------------------------------------------------------------------------------------
VOID CControllerPage::SaveSetting()
{
	//	ダイアログ ボックスを表示し、設定を保存する
	LONG	MsgResult	= (LONG)DialogBoxParam(
							 Instance
							,MAKEINTRESOURCE( IDD_SAVE_DIALOG )
							,Wnd
							,(DLGPROC)SaveDialogProc
							,(LPARAM)Setting );
	if( MsgResult == IDOK )
	{
		//	変更された設定を反映する
		ApplyChangedSetting();
	}
}

//----------------------------------------------------------------------------------------------
//	DeleteSetting
//----------------------------------------------------------------------------------------------
VOID CControllerPage::DeleteSetting()
{
	//	メッセージを表示し、設定の削除を確認する
	LONG	Result	= ShowMessageBox(
						 IDS_DELETE_TITLE
						,IDS_DELETE_MESSAGE
						,Setting->SettingName[Setting->CurrentSettingIndex]
						,MB_YESNO | MB_ICONQUESTION );
	if( Result == IDYES )
	{
		//	選択されている設定を削除する
		Setting->Delete();
		//	変更された設定を反映する
		ApplyChangedSetting();
	}
}

//----------------------------------------------------------------------------------------------
//	ApplyChangedSetting
//----------------------------------------------------------------------------------------------
VOID CControllerPage::ApplyChangedSetting()
{
	//	変数宣言
	HWND	Dialog	= GetParent( Wnd );

	//	設定を反映する
	ApplySetting();

	//	各ページに設定を反映する
	for( LONG Index = 1; Index < 7; Index ++ )
	{
		HWND	Page	= PropSheet_IndexToHwnd( Dialog, Index );
		if( Page != NULL )
		{
			SendMessage(
				 Page
				,WM_SETTING_LIST_CHANGED
				,NULL
				,NULL );
		}
	}

	//	プロパティ シートに変更を通知する
	PropSheet_Changed( Dialog, Wnd );
}

//----------------------------------------------------------------------------------------------
//	ClosePage
//----------------------------------------------------------------------------------------------
VOID CControllerPage::ClosePage()
{
	//	ポーリング用タイマーを停止する
	KillTimer( Wnd, POLLING_TIMER_ID );

	//	振動を停止する
	TestForceFeedback( 0 );

	//	各インスタンスを開放する
	delete	Device;
	delete	Report;

	//	ミューテックスを開放する
	CloseHandle( Mutex );
}

//----------------------------------------------------------------------------------------------
//	ShowControllerState
//----------------------------------------------------------------------------------------------
VOID CControllerPage::ShowControllerState()
{
	//	コントローラーの現在の状態を取得する
	FEATURE_REPORT_ID	FeatureReport;
	ZeroMemory( &FeatureReport, sizeof( FEATURE_REPORT_ID ) );
	Device->GetState( &FeatureReport );

	//	入力レポートを作成する（アラインメントの関係かポインタ渡しが上手くいかない）
	INPUT_REPORT	InputReport;
	InputReport.DigitalButton	= FeatureReport.DigitalButton;
	InputReport.LeftTrigger		= FeatureReport.LeftTrigger;
	InputReport.RightTrigger	= FeatureReport.RightTrigger;
	InputReport.LeftStickX		= FeatureReport.LeftStickX;
	InputReport.LeftStickY		= FeatureReport.LeftStickY;
	InputReport.RightStickX		= FeatureReport.RightStickX;
	InputReport.RightStickY		= FeatureReport.RightStickY;

	//	HID 読込用レポートを生成する
	HID_READ_REPORT	ReadReport;
	ZeroMemory( &ReadReport, sizeof( HID_READ_REPORT ) );
	Report->GenerateReadReport(
		 (INPUT_REPORT *)&InputReport
		,CurrentSetting
		,&ReadReport
		,&LastState );

	//	生成されたコントローラーの状態を表示する
	SetIntegerText( IDC_X_AXIS, ReadReport.XAxis );
	SetIntegerText( IDC_Y_AXIS, ReadReport.YAxis );
	SetIntegerText( IDC_X_ROTATION, ReadReport.XRotation );
	SetIntegerText( IDC_Y_ROTATION, ReadReport.YRotation );
	SetIntegerText( IDC_Z_AXIS, ReadReport.ZAxis );
	SetIntegerText( IDC_Z_ROTATION, ReadReport.ZRotation );
	SetIntegerText( IDC_SLIDER1, ReadReport.Slider1 );
	SetIntegerText( IDC_SLIDER2, ReadReport.Slider2 );
	SetIntegerText(
		 IDC_HAT_SWITCH1
		,ReadReport.HatSwitch1 != -1 ? ReadReport.HatSwitch1 * 45 : -1 );
	SetIntegerText(
		 IDC_HAT_SWITCH2
		,ReadReport.HatSwitch2 != -1 ? ReadReport.HatSwitch2 * 45 : -1 );
	SetIntegerText(
		 IDC_HAT_SWITCH3
		,ReadReport.HatSwitch3 != -1 ? ReadReport.HatSwitch3 * 45 : -1 );
	SetIntegerText(
		 IDC_HAT_SWITCH4
		,ReadReport.HatSwitch4 != -1 ? ReadReport.HatSwitch4 * 45 : -1 );
	SetButtonNumberText( IDC_BUTTON, ReadReport.Buttons );
}

//----------------------------------------------------------------------------------------------
//	TestForceFeedback
//----------------------------------------------------------------------------------------------
VOID CControllerPage::TestForceFeedback( LONG TestLevel )
{
	//	レポートを作成する
	HID_WRITE_REPORT_ID	WriteReport;
	ZeroMemory( &WriteReport, sizeof( HID_WRITE_REPORT_ID ) );
	WriteReport.Id		= 0;
	WriteReport.Type	= REPORT_TYPE_DIRECT;
	Report->SetActuator(
		 TestLevel
		,CurrentSetting->ActuatorLevel[INDEX_LEFT_ACTUATOR]
		,CurrentSetting->ActuatorMinThreshold
		,CurrentSetting->ActuatorMaxThreshold
		,&WriteReport.Left );
	Report->SetActuator(
		 TestLevel
		,CurrentSetting->ActuatorLevel[INDEX_RIGHT_ACTUATOR]
		,CurrentSetting->ActuatorMinThreshold
		,CurrentSetting->ActuatorMaxThreshold
		,&WriteReport.Right );

	//	デバイスに設定する
	Device->SetState( &WriteReport );
}
