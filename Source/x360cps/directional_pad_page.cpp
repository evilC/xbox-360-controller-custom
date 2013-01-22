
//	Property Sheet for Microsoft Xbox 360 Controller

#include	"directional_pad_page.h"
#include	"resource.h"

//----------------------------------------------------------------------------------------------
//	CDirectionalPadPage
//----------------------------------------------------------------------------------------------
CDirectionalPadPage::CDirectionalPadPage( CSetting * Setting )
{
	//	設定
	this->Setting		= Setting;

	//	現在のコントローラーの設定
	CurrentSetting		= &Setting->CurrentSetting;
}

//----------------------------------------------------------------------------------------------
//	wmInitDialog
//----------------------------------------------------------------------------------------------
VOID CDirectionalPadPage::wmInitDialog( HWND Wnd )
{
	//	ウィンドウ ハンドル
	this->Wnd	= Wnd;

	//	方向パッドのファンクション
	WORD	DirectionalPadFunction[]	= {	 IDS_NONE
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
											,IDS_HAT_SWITCH4
											,IDS_4WAY_BUTTON };
	WORD	DirectionalPadFunctionCount	= sizeof( DirectionalPadFunction ) / sizeof( WORD ) ;

	//	方向別のファンクション
	WORD	WayButtonFunction[]			= {	 IDS_NONE
											,IDS_X_AXIS_PLUS
											,IDS_X_AXIS_MINUS
											,IDS_Y_AXIS_PLUS
											,IDS_Y_AXIS_MINUS
											,IDS_X_ROTATION_PLUS
											,IDS_X_ROTATION_MINUS
											,IDS_Y_ROTATION_PLUS
											,IDS_Y_ROTATION_MINUS
											,IDS_Z_AXIS_PLUS
											,IDS_Z_AXIS_MINUS
											,IDS_Z_ROTATION_PLUS
											,IDS_Z_ROTATION_MINUS
											,IDS_SLIDER1_PLUS
											,IDS_SLIDER1_MINUS
											,IDS_SLIDER2_PLUS
											,IDS_SLIDER2_MINUS
											,IDS_BUTTON1
											,IDS_BUTTON2
											,IDS_BUTTON3
											,IDS_BUTTON4
											,IDS_BUTTON5
											,IDS_BUTTON6
											,IDS_BUTTON7
											,IDS_BUTTON8
											,IDS_BUTTON9
											,IDS_BUTTON10
											,IDS_BUTTON11
											,IDS_BUTTON12
											,IDS_BUTTON13
											,IDS_BUTTON14
											,IDS_BUTTON15
											,IDS_BUTTON16
											,IDS_BUTTON17
											,IDS_BUTTON18
											,IDS_BUTTON19
											,IDS_BUTTON20
											,IDS_BUTTON21
											,IDS_BUTTON22
											,IDS_BUTTON23
											,IDS_BUTTON24
											,IDS_BUTTON25
											,IDS_BUTTON26
											,IDS_BUTTON27
											,IDS_BUTTON28
											,IDS_BUTTON29
											,IDS_BUTTON30
											,IDS_BUTTON31
											,IDS_BUTTON32 };
	WORD	WayButtonFunctionCount	= sizeof( WayButtonFunction ) / sizeof( WORD ) ;

	//	「方向パッドの機能」
	InitComboBox(
		 IDC_DIRECTIONAL_PAD_USAGE
		,DirectionalPadFunction
		,DirectionalPadFunctionCount );
	//	「方向パッド上の機能」
	InitComboBox( IDC_DIRECTIONAL_PAD_UP_USAGE, WayButtonFunction, WayButtonFunctionCount );
	//	「方向パッド下の機能」
	InitComboBox( IDC_DIRECTIONAL_PAD_DOWN_USAGE, WayButtonFunction, WayButtonFunctionCount );
	//	「方向パッド左の機能」
	InitComboBox( IDC_DIRECTIONAL_PAD_LEFT_USAGE, WayButtonFunction, WayButtonFunctionCount );
	//	「方向パッド右の機能」
	InitComboBox( IDC_DIRECTIONAL_PAD_RIGHT_USAGE, WayButtonFunction, WayButtonFunctionCount );
}

//----------------------------------------------------------------------------------------------
//	wmCommand
//----------------------------------------------------------------------------------------------
VOID CDirectionalPadPage::wmCommand(
	 HWND	Control
	,WORD	ControlID
	,WORD	NotifyMessage )
{
	//	コントロールによって処理を振り分ける
	switch( ControlID )
	{
		case IDC_DIRECTIONAL_PAD_USAGE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case CBN_SELCHANGE:
					//	現在の設定に反映する
					CurrentSetting->DirectionalPadUsage	= GetComboBoxIndex( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_DIRECTIONAL_PAD_UP_USAGE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case CBN_SELCHANGE:
					//	現在の設定に反映する
					CurrentSetting->DirectionalPadWayUsage[INDEX_WAY_UP]
						= GetWayUsageByControlIndex( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_DIRECTIONAL_PAD_DOWN_USAGE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case CBN_SELCHANGE:
					//	現在の設定に反映する
					CurrentSetting->DirectionalPadWayUsage[INDEX_WAY_DOWN]
						= GetWayUsageByControlIndex( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_DIRECTIONAL_PAD_LEFT_USAGE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case CBN_SELCHANGE:
					//	現在の設定に反映する
					CurrentSetting->DirectionalPadWayUsage[INDEX_WAY_LEFT]
						= GetWayUsageByControlIndex( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_DIRECTIONAL_PAD_RIGHT_USAGE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case CBN_SELCHANGE:
					//	現在の設定に反映する
					CurrentSetting->DirectionalPadWayUsage[INDEX_WAY_RIGHT]
						= GetWayUsageByControlIndex( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_DIRECTIONAL_PAD_REVERSE_Y:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case BN_CLICKED:
					//	現在の設定に反映する
					CurrentSetting->DirectionalPadReverseY	= GetCheckBoxState( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_DIRECTIONAL_PAD_UP_RAPID_FIRE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case BN_CLICKED:
					//	現在の設定に反映する
					CurrentSetting->DirectionalPadWayRapidFire[INDEX_WAY_UP]
						= GetCheckBoxState( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_DIRECTIONAL_PAD_DOWN_RAPID_FIRE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case BN_CLICKED:
					//	現在の設定に反映する
					CurrentSetting->DirectionalPadWayRapidFire[INDEX_WAY_DOWN]
						= GetCheckBoxState( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_DIRECTIONAL_PAD_LEFT_RAPID_FIRE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case BN_CLICKED:
					//	現在の設定に反映する
					CurrentSetting->DirectionalPadWayRapidFire[INDEX_WAY_LEFT]
						= GetCheckBoxState( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_DIRECTIONAL_PAD_RIGHT_RAPID_FIRE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case BN_CLICKED:
					//	現在の設定に反映する
					CurrentSetting->DirectionalPadWayRapidFire[INDEX_WAY_RIGHT]
						= GetCheckBoxState( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;
	}
}

//----------------------------------------------------------------------------------------------
//	ApplySetting
//----------------------------------------------------------------------------------------------
VOID CDirectionalPadPage::ApplySetting()
{
	//	「方向パッドの機能」
	SetComboBoxIndex( IDC_DIRECTIONAL_PAD_USAGE, CurrentSetting->DirectionalPadUsage );
	//	「上下を反転する」
	SetCheckBoxState( IDC_DIRECTIONAL_PAD_REVERSE_Y, CurrentSetting->DirectionalPadReverseY );
	//	「方向パッドの機能」が「なし」の場合は、「上下を反転する」を無効にする
	if( CurrentSetting->DirectionalPadUsage == USAGE_NONE )
	{
		EnableControl( IDC_DIRECTIONAL_PAD_REVERSE_Y, FALSE );
	} else {
		EnableControl( IDC_DIRECTIONAL_PAD_REVERSE_Y, TRUE );
	}

	//	「方向パッド上の機能」
	SetComboBoxIndex(
		 IDC_DIRECTIONAL_PAD_UP_USAGE
		,GetControlIndexByWayUsage( CurrentSetting->DirectionalPadWayUsage[INDEX_WAY_UP] ) );
	//	「方向パッド上の連射を行う」
	SetCheckBoxState(
		 IDC_DIRECTIONAL_PAD_UP_RAPID_FIRE
		,CurrentSetting->DirectionalPadWayRapidFire[INDEX_WAY_UP] );

	//	「方向パッド下の機能」
	SetComboBoxIndex(
		 IDC_DIRECTIONAL_PAD_DOWN_USAGE
		,GetControlIndexByWayUsage( CurrentSetting->DirectionalPadWayUsage[INDEX_WAY_DOWN] ) );
	//	「方向パッド下の連射を行う」
	SetCheckBoxState(
		 IDC_DIRECTIONAL_PAD_DOWN_RAPID_FIRE
		,CurrentSetting->DirectionalPadWayRapidFire[INDEX_WAY_DOWN] );

	//	「方向パッド左の機能」
	SetComboBoxIndex(
		 IDC_DIRECTIONAL_PAD_LEFT_USAGE
		,GetControlIndexByWayUsage( CurrentSetting->DirectionalPadWayUsage[INDEX_WAY_LEFT] ) );
	//	「方向パッド左の連射を行う」
	SetCheckBoxState(
		 IDC_DIRECTIONAL_PAD_LEFT_RAPID_FIRE
		,CurrentSetting->DirectionalPadWayRapidFire[INDEX_WAY_LEFT] );

	//	「方向パッド右の機能」
	SetComboBoxIndex(
		 IDC_DIRECTIONAL_PAD_RIGHT_USAGE
		,GetControlIndexByWayUsage( CurrentSetting->DirectionalPadWayUsage[INDEX_WAY_RIGHT] ) );
	//	「方向パッド右の連射を行う」
	SetCheckBoxState(
		 IDC_DIRECTIONAL_PAD_RIGHT_RAPID_FIRE
		,CurrentSetting->DirectionalPadWayRapidFire[INDEX_WAY_RIGHT] );

	//	「方向パッドの機能」が「方向別に割り当て」の以外の場合は、下部を無効にする
	if( CurrentSetting->DirectionalPadUsage != USAGE_4WAY_BUTTON )
	{
		EnableControls( IDC_STATIC_301, IDC_DIRECTIONAL_PAD_RIGHT_RAPID_FIRE, FALSE );
	} else {
		EnableControls( IDC_STATIC_301, IDC_DIRECTIONAL_PAD_RIGHT_RAPID_FIRE, TRUE );
		//	「方向パッド上の機能」が「なし」の場合は、「方向パッド上の連射を行う」を無効にする
		if( CurrentSetting->DirectionalPadWayUsage[INDEX_WAY_UP] == USAGE_NONE )
		{
			EnableControl( IDC_DIRECTIONAL_PAD_UP_RAPID_FIRE, FALSE );
		} else {
			EnableControl( IDC_DIRECTIONAL_PAD_UP_RAPID_FIRE, TRUE );
		}
		//	「方向パッド下の機能」が「なし」の場合は、「方向パッド下の連射を行う」を無効にする
		if( CurrentSetting->DirectionalPadWayUsage[INDEX_WAY_DOWN] == USAGE_NONE )
		{
			EnableControl( IDC_DIRECTIONAL_PAD_DOWN_RAPID_FIRE, FALSE );
		} else {
			EnableControl( IDC_DIRECTIONAL_PAD_DOWN_RAPID_FIRE, TRUE );
		}
		//	「方向パッド左の機能」が「なし」の場合は、「方向パッド左の連射を行う」を無効にする
		if( CurrentSetting->DirectionalPadWayUsage[INDEX_WAY_LEFT] == USAGE_NONE )
		{
			EnableControl( IDC_DIRECTIONAL_PAD_LEFT_RAPID_FIRE, FALSE );
		} else {
			EnableControl( IDC_DIRECTIONAL_PAD_LEFT_RAPID_FIRE, TRUE );
		}
		//	「方向パッド右の機能」が「なし」の場合は、「方向パッド右の連射を行う」を無効にする
		if( CurrentSetting->DirectionalPadWayUsage[INDEX_WAY_RIGHT] == USAGE_NONE )
		{
			EnableControl( IDC_DIRECTIONAL_PAD_RIGHT_RAPID_FIRE, FALSE );
		} else {
			EnableControl( IDC_DIRECTIONAL_PAD_RIGHT_RAPID_FIRE, TRUE );
		}
	}
}
