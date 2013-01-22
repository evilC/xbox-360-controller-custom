
//	Property Sheet for Microsoft Xbox 360 Controller

#include	"button_page.h"
#include	"resource.h"

//----------------------------------------------------------------------------------------------
//	CButtonPage
//----------------------------------------------------------------------------------------------
CButtonPage::CButtonPage( CSetting * Setting )
{
	//	設定
	this->Setting		= Setting;

	//	現在のコントローラーの設定
	CurrentSetting		= &Setting->CurrentSetting;
}

//----------------------------------------------------------------------------------------------
//	wmInitDialog
//----------------------------------------------------------------------------------------------
VOID CButtonPage::wmInitDialog( HWND Wnd )
{
	//	ウィンドウ ハンドル
	this->Wnd	= Wnd;

	//	ボタンの一覧
	WORD	ButtonList[]	= {	 IDS_A_BUTTON
								,IDS_B_BUTTON
								,IDS_X_BUTTON
								,IDS_Y_BUTTON
								,IDS_LB_BUTTON
								,IDS_RB_BUTTON
								,IDS_START_BUTTON
								,IDS_BACK_BUTTON
								,IDS_LEFT_STICK_BUTTON
								,IDS_RIGHT_STICK_BUTTON
								,IDS_XBOX_GUIDE_BUTTON };
	WORD	ButtonListCount	= sizeof( ButtonList ) / sizeof( WORD ) ;

	//	ボタンのファンクション
	WORD	ButtonFunction[]	= {	 IDS_NONE
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
	WORD	ButtonFunctionCount	= sizeof( ButtonFunction ) / sizeof( WORD ) ;

	//	リング ライトの点滅パターン
	WORD	RingLightPattern[]	= {	 IDS_RING_LIGHT_0
									,IDS_RING_LIGHT_1
									,IDS_RING_LIGHT_2
									,IDS_RING_LIGHT_3
									,IDS_RING_LIGHT_4
									,IDS_RING_LIGHT_5
									,IDS_RING_LIGHT_6
									,IDS_RING_LIGHT_7
									,IDS_RING_LIGHT_8
									,IDS_RING_LIGHT_9
									,IDS_RING_LIGHT_10
									,IDS_RING_LIGHT_11
									,IDS_RING_LIGHT_12
									,IDS_RING_LIGHT_13
									,IDS_RING_LIGHT_14
									,IDS_RING_LIGHT_15 };
	WORD	RingLightPatternCount	= sizeof( RingLightPattern ) / sizeof( WORD ) ;

	//	「ボタンの一覧」
	InitListBox( IDC_BUTTON_LIST, ButtonList, ButtonListCount );
	SetListBoxIndex( IDC_BUTTON_LIST, 0 );
	//	「ボタンの機能」
	InitComboBox( IDC_BUTTON_USAGE, ButtonFunction, ButtonFunctionCount );
	//	「ボタンの連射速度」
	InitTrackBar( IDC_BUTTON_RAPID_FIRE_SPEED, 0, 100, 20, 10 );

	//	「点滅パターンの一覧」
	InitComboBox( IDC_RING_LIGHT_PATTERN, RingLightPattern, RingLightPatternCount );
}

//----------------------------------------------------------------------------------------------
//	wmCommand
//----------------------------------------------------------------------------------------------
VOID CButtonPage::wmCommand(
	 HWND	Control
	,WORD	ControlID
	,WORD	NotifyMessage )
{
	//	コントロールによって処理を振り分ける
	switch( ControlID )
	{
		case IDC_BUTTON_LIST:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case LBN_SELCHANGE:
					ApplySetting();
					break;
			}
			break;

		case IDC_BUTTON_USAGE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case CBN_SELCHANGE:
					//	現在の設定に反映する
					CurrentSetting->ButtonUsage[GetListBoxIndex( IDC_BUTTON_LIST )]
						= GetComboBoxIndex( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_BUTTON_RAPID_FIRE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case BN_CLICKED:
					//	現在の設定に反映する
					CurrentSetting->ButtonRapidFire[GetListBoxIndex( IDC_BUTTON_LIST )]
						= GetCheckBoxState( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_RING_LIGHT_PATTERN:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case CBN_SELCHANGE:
					//	現在の設定に反映する
					CurrentSetting->RingLightPattern	= GetComboBoxIndex( ControlID );
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
VOID CButtonPage::wmHScroll(
	 HWND	Control
	,WORD	Request )
{
	//	「ボタンの連射速度」からのメッセージ
	if( Control == GetDlgItem( Wnd, IDC_BUTTON_RAPID_FIRE_SPEED ) )
	{
		//	現在の設定に反映する
		CurrentSetting->ButtonRapidFireSpeed[GetListBoxIndex( IDC_BUTTON_LIST )]
			= GetTrackBarPos( IDC_BUTTON_RAPID_FIRE_SPEED );
		//	設定の変更を通知する
		SettingChanged();
	}
}

//----------------------------------------------------------------------------------------------
//	ApplySetting
//----------------------------------------------------------------------------------------------
VOID CButtonPage::ApplySetting()
{
	//	「ボタンの機能」
	LONG	ListBoxIndex	= GetListBoxIndex( IDC_BUTTON_LIST );
	//	「ボタンの機能」
	SetComboBoxIndex( IDC_BUTTON_USAGE, CurrentSetting->ButtonUsage[ListBoxIndex] );
	//	「ボタンの連射を行う」
	SetCheckBoxState( IDC_BUTTON_RAPID_FIRE, CurrentSetting->ButtonRapidFire[ListBoxIndex] );
	//	「ボタンの連射速度」
	SetTrackBarPos(
		 IDC_BUTTON_RAPID_FIRE_SPEED
		,CurrentSetting->ButtonRapidFireSpeed[ListBoxIndex] );
	if( CurrentSetting->ButtonRapidFire[ListBoxIndex] == FALSE )
	{
		EnableControl( IDC_BUTTON_RAPID_FIRE_SPEED, FALSE );
	} else {
		EnableControl( IDC_BUTTON_RAPID_FIRE_SPEED, TRUE );
	}

	//	「点滅パターンの一覧」
	SetComboBoxIndex( IDC_RING_LIGHT_PATTERN, CurrentSetting->RingLightPattern );
}
