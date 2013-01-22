
//	Property Sheet for Microsoft Xbox 360 Controller

#include	"stick_page.h"
#include	"resource.h"

//----------------------------------------------------------------------------------------------
//	CStickPage
//----------------------------------------------------------------------------------------------
CStickPage::CStickPage( CSetting * Setting )
{
	//	設定
	this->Setting		= Setting;

	//	現在のコントローラーの設定
	CurrentSetting		= &Setting->CurrentSetting;

	//	トラック バーの選択状態
	TrackBarSelecting	= FALSE;
}

//----------------------------------------------------------------------------------------------
//	wmInitDialog
//----------------------------------------------------------------------------------------------
VOID CStickPage::wmInitDialog( HWND Wnd )
{
	//	ウィンドウ ハンドル
	this->Wnd	= Wnd;

	//	スティックのファンクション
	WORD	StickFunction[]	= {	 IDS_NONE
								,IDS_X_AXIS_Y_AXIS
								,IDS_X_AXIS_NONE
								,IDS_NONE_X_AXIS
								,IDS_Y_AXIS_X_AXIS
								,IDS_Y_AXIS_NONE
								,IDS_NONE_Y_AXIS
								,IDS_X_ROTATION_Y_ROTATION
								,IDS_X_ROTATION_NONE
								,IDS_NONE_X_ROTATION
								,IDS_Y_ROTATION_X_ROTATION
								,IDS_Y_ROTATION_NONE
								,IDS_NONE_Y_ROTATION
								,IDS_Z_AXIS_ROTATION
								,IDS_Z_AXIS_SLIDER1
								,IDS_Z_AXIS_SLIDER2
								,IDS_Z_AXIS_NONE
								,IDS_NONE_Z_AXIS
								,IDS_Z_ROTATION_AXIS
								,IDS_Z_ROTATION_SLIDER1
								,IDS_Z_ROTATION_SLIDER2
								,IDS_Z_ROTATION_NONE
								,IDS_NONE_Z_ROTATION
								,IDS_SLIDER1_Z_AXIS
								,IDS_SLIDER1_Z_ROTATION
								,IDS_SLIDER1_SLIDER2
								,IDS_SLIDER1_NONE
								,IDS_NONE_SLIDER1
								,IDS_SLIDER2_Z_AXIS
								,IDS_SLIDER2_Z_ROTATION
								,IDS_SLIDER2_SLIDER1
								,IDS_SLIDER2_NONE
								,IDS_NONE_SLIDER2
								,IDS_HAT_SWITCH1
								,IDS_HAT_SWITCH2
								,IDS_HAT_SWITCH3
								,IDS_HAT_SWITCH4 };
	WORD	StickFunctionCount	= sizeof( StickFunction ) / sizeof( WORD ) ;

	//	「スティックの機能（左）」
	InitComboBox( IDC_LEFT_STICK_USAGE, StickFunction, StickFunctionCount );
	//	「スティックの機能（右）」
	InitComboBox( IDC_RIGHT_STICK_USAGE, StickFunction, StickFunctionCount );
	//	「有効範囲」
	InitTrackBar( IDC_STICK_THRESHOLD, 0, 100, 10, 10 );
	//	「スティックの入力閾値」
	InitTrackBar( IDC_STICK_HAT_SWITCH_THRESHOLD, 0, 100, 10, 10 );
}

//----------------------------------------------------------------------------------------------
//	wmCommand
//----------------------------------------------------------------------------------------------
VOID CStickPage::wmCommand(
	 HWND	Control
	,WORD	ControlID
	,WORD	NotifyMessage )
{
	//	コントロールによって処理を振り分ける
	switch( ControlID )
	{
		case IDC_LEFT_STICK_USAGE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case CBN_SELCHANGE:
					//	現在の設定に反映する
					CurrentSetting->StickUsage[INDEX_LEFT_STICK]
						= GetComboBoxIndex( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_RIGHT_STICK_USAGE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case CBN_SELCHANGE:
					//	現在の設定に反映する
					CurrentSetting->StickUsage[INDEX_RIGHT_STICK]
						= GetComboBoxIndex( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_LEFT_STICK_REVERSE_Y:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case BN_CLICKED:
					//	現在の設定に反映する
					CurrentSetting->StickReverseY[INDEX_LEFT_STICK]
						= GetCheckBoxState( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_RIGHT_STICK_REVERSE_Y:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case BN_CLICKED:
					//	現在の設定に反映する
					CurrentSetting->StickReverseY[INDEX_RIGHT_STICK]
						= GetCheckBoxState( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;
	}
}

//----------------------------------------------------------------------------------------------
//	wmHScroll
//----------------------------------------------------------------------------------------------
VOID CStickPage::wmHScroll(
	 HWND	Control
	,WORD	Request )
{
	//	「有効範囲」からのメッセージ
	if( Control == GetDlgItem( Wnd, IDC_STICK_THRESHOLD ) )
	{
		//	現在の選択状態を退避する
		BOOL	PrevSelecting	= TrackBarSelecting;

		//	トラック バーの範囲選択を行う
		SelectTrackBar( Control, Request, &TrackBarSelecting );
		if( PrevSelecting != TrackBarSelecting )
		{
			//	現在の設定に反映する
			GetTrackBarRange(
				 IDC_STICK_THRESHOLD
				,&CurrentSetting->StickMinThreshold[INDEX_LEFT_STICK]
				,&CurrentSetting->StickMaxThreshold[INDEX_LEFT_STICK] );
			CurrentSetting->StickMinThreshold[INDEX_RIGHT_STICK]
				= CurrentSetting->StickMinThreshold[INDEX_LEFT_STICK];
			CurrentSetting->StickMaxThreshold[INDEX_RIGHT_STICK]
				= CurrentSetting->StickMaxThreshold[INDEX_LEFT_STICK];
			//	設定の変更を通知する
			SettingChanged();
		}
	}

	//	「スティックの入力閾値」からのメッセージ
	if( Control == GetDlgItem( Wnd, IDC_STICK_HAT_SWITCH_THRESHOLD ) )
	{
		//	現在の設定に反映する
		CurrentSetting->StickHatSwitchThreshold[INDEX_LEFT_STICK]
			= GetTrackBarPos( IDC_STICK_HAT_SWITCH_THRESHOLD );
		CurrentSetting->StickHatSwitchThreshold[INDEX_RIGHT_STICK]
			= CurrentSetting->StickHatSwitchThreshold[INDEX_LEFT_STICK];
		//	設定の変更を通知する
		SettingChanged();
	}
}

//----------------------------------------------------------------------------------------------
//	ApplySetting
//----------------------------------------------------------------------------------------------
VOID CStickPage::ApplySetting()
{
	//	「スティックの機能（左）」
	SetComboBoxIndex( IDC_LEFT_STICK_USAGE, CurrentSetting->StickUsage[INDEX_LEFT_STICK] );
	//	「上下を反転する（左）」
	SetCheckBoxState(
		 IDC_LEFT_STICK_REVERSE_Y
		,CurrentSetting->StickReverseY[INDEX_LEFT_STICK] );
	//	「スティックの機能（左）」が「なし」の場合は、「上下を反転する（左）」を無効にする
	if( CurrentSetting->StickUsage[INDEX_LEFT_STICK] == USAGE_NONE )
	{
		EnableControl( IDC_LEFT_STICK_REVERSE_Y, FALSE );
	} else {
		EnableControl( IDC_LEFT_STICK_REVERSE_Y, TRUE );
	}

	//	「スティックの機能（右）」
	SetComboBoxIndex( IDC_RIGHT_STICK_USAGE, CurrentSetting->StickUsage[INDEX_RIGHT_STICK] );
	//	「上下を反転する（右）」
	SetCheckBoxState(
		 IDC_RIGHT_STICK_REVERSE_Y
		,CurrentSetting->StickReverseY[INDEX_RIGHT_STICK] );
	//	「スティックの機能（右）」が「なし」の場合は、「上下を反転する（右）」を無効にする
	if( CurrentSetting->StickUsage[INDEX_RIGHT_STICK] == USAGE_NONE )
	{
		EnableControl( IDC_RIGHT_STICK_REVERSE_Y, FALSE );
	} else {
		EnableControl( IDC_RIGHT_STICK_REVERSE_Y, TRUE );
	}

	//	「有効範囲」
	SetTrackBarRange(
		 IDC_STICK_THRESHOLD
		,CurrentSetting->StickMinThreshold[INDEX_LEFT_STICK]
		,CurrentSetting->StickMaxThreshold[INDEX_LEFT_STICK] );
	SetTrackBarPos( IDC_STICK_THRESHOLD, CurrentSetting->StickMinThreshold[INDEX_LEFT_STICK] );
	//	「スティックの入力閾値」
	SetTrackBarPos(
		 IDC_STICK_HAT_SWITCH_THRESHOLD
		,CurrentSetting->StickHatSwitchThreshold[INDEX_LEFT_STICK] );
}
