
//	Property Sheet for Microsoft Xbox 360 Controller

#include	"trigger_page.h"
#include	"resource.h"

//----------------------------------------------------------------------------------------------
//	CTriggerPage
//----------------------------------------------------------------------------------------------
CTriggerPage::CTriggerPage( CSetting * Setting )
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
VOID CTriggerPage::wmInitDialog( HWND Wnd )
{
	//	ウィンドウ ハンドル
	this->Wnd	= Wnd;

	//	トリガーのファンクション
	WORD	TriggerFunction[]	= {	 IDS_NONE
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
	WORD	TriggerFunctionCount	= sizeof( TriggerFunction ) / sizeof( WORD ) ;

	//	「トリガーの機能（左）」
	InitComboBox( IDC_LEFT_TRIGGER_USAGE, TriggerFunction, TriggerFunctionCount );
	//	「トリガーの機能（右）」
	InitComboBox( IDC_RIGHT_TRIGGER_USAGE, TriggerFunction, TriggerFunctionCount );
	//	「詳細設定」
	InitTrackBar( IDC_TRIGGER_THRESHOLD, 0, 100, 10, 10 );
	//	「トリガーの入力閾値」
	InitTrackBar( IDC_TRIGGER_BUTTON_THRESHOLD, 0, 100, 10, 10 );
}

//----------------------------------------------------------------------------------------------
//	wmCommand
//----------------------------------------------------------------------------------------------
VOID CTriggerPage::wmCommand(
	 HWND	Control
	,WORD	ControlID
	,WORD	NotifyMessage )
{
	//	コントロールによって処理を振り分ける
	switch( ControlID )
	{
		case IDC_LEFT_TRIGGER_USAGE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case CBN_SELCHANGE:
					//	現在の設定に反映する
					CurrentSetting->TriggerUsage[INDEX_LEFT_TRIGGER]
						= GetWayUsageByControlIndex( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_RIGHT_TRIGGER_USAGE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case CBN_SELCHANGE:
					//	現在の設定に反映する
					CurrentSetting->TriggerUsage[INDEX_RIGHT_TRIGGER]
						= GetWayUsageByControlIndex( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_LEFT_TRIGGER_RAPID_FIRE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case BN_CLICKED:
					//	現在の設定に反映する
					CurrentSetting->TriggerRapidFire[INDEX_LEFT_TRIGGER]
						= GetCheckBoxState( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_RIGHT_TRIGGER_RAPID_FIRE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case BN_CLICKED:
					//	現在の設定に反映する
					CurrentSetting->TriggerRapidFire[INDEX_RIGHT_TRIGGER]
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
VOID CTriggerPage::wmHScroll(
	 HWND	Control
	,WORD	Request )
{
	//	「有効範囲」からのメッセージ
	if( Control == GetDlgItem( Wnd, IDC_TRIGGER_THRESHOLD ) )
	{
		//	現在の選択状態を退避する
		BOOL	PrevSelecting	= TrackBarSelecting;

		//	トラック バーの範囲選択を行う
		SelectTrackBar( Control, Request, &TrackBarSelecting );
		if( PrevSelecting != TrackBarSelecting )
		{
			//	現在の設定に反映する
			GetTrackBarRange(
				 IDC_TRIGGER_THRESHOLD
				,&CurrentSetting->TriggerMinThreshold[INDEX_LEFT_TRIGGER]
				,&CurrentSetting->TriggerMaxThreshold[INDEX_LEFT_TRIGGER] );
			CurrentSetting->TriggerMinThreshold[INDEX_RIGHT_TRIGGER]
				= CurrentSetting->TriggerMinThreshold[INDEX_LEFT_TRIGGER];
			CurrentSetting->TriggerMaxThreshold[INDEX_RIGHT_TRIGGER]
				= CurrentSetting->TriggerMaxThreshold[INDEX_LEFT_TRIGGER];
			//	設定の変更を通知する
			SettingChanged();
		}
	}

	//	「トリガーの入力閾値」からのメッセージ
	if( Control == GetDlgItem( Wnd, IDC_TRIGGER_BUTTON_THRESHOLD ) )
	{
		//	現在の設定に反映する
		CurrentSetting->TriggerButtonThreshold[INDEX_LEFT_TRIGGER]
			= GetTrackBarPos( IDC_TRIGGER_BUTTON_THRESHOLD );
		CurrentSetting->TriggerButtonThreshold[INDEX_RIGHT_TRIGGER]
			= CurrentSetting->TriggerButtonThreshold[INDEX_LEFT_TRIGGER];
		//	設定の変更を通知する
		SettingChanged();
	}
}

//----------------------------------------------------------------------------------------------
//	ApplySetting
//----------------------------------------------------------------------------------------------
VOID CTriggerPage::ApplySetting()
{
	//	「トリガーの機能（左）」
	SetComboBoxIndex(
		 IDC_LEFT_TRIGGER_USAGE
		,GetControlIndexByWayUsage( CurrentSetting->TriggerUsage[INDEX_LEFT_TRIGGER] ) );
	//	「トリガーの連射を行う（左）」
	SetCheckBoxState(
		 IDC_LEFT_TRIGGER_RAPID_FIRE
		,CurrentSetting->TriggerRapidFire[INDEX_LEFT_TRIGGER] );
	//	「トリガーの機能（左）」が「なし」の場合は、「トリガーの連射を行う（左）」を無効にする
	if( CurrentSetting->TriggerUsage[INDEX_LEFT_TRIGGER] == USAGE_NONE )
	{
		EnableControl( IDC_LEFT_TRIGGER_RAPID_FIRE, FALSE );
	} else {
		EnableControl( IDC_LEFT_TRIGGER_RAPID_FIRE, TRUE );
	}

	//	「トリガーの機能（右）」
	SetComboBoxIndex(
		 IDC_RIGHT_TRIGGER_USAGE
		,GetControlIndexByWayUsage( CurrentSetting->TriggerUsage[INDEX_RIGHT_TRIGGER] ) );
	//	「トリガーの連射を行う（右）」
	SetCheckBoxState(
		 IDC_RIGHT_TRIGGER_RAPID_FIRE
		,CurrentSetting->TriggerRapidFire[INDEX_RIGHT_TRIGGER] );
	//	「トリガーの機能（右）」が「なし」の場合は、「トリガーの連射を行う（右）」を無効にする
	if( CurrentSetting->TriggerUsage[INDEX_RIGHT_TRIGGER] == USAGE_NONE )
	{
		EnableControl( IDC_RIGHT_TRIGGER_RAPID_FIRE, FALSE );
	} else {
		EnableControl( IDC_RIGHT_TRIGGER_RAPID_FIRE, TRUE );
	}

	//	「有効範囲」
	SetTrackBarRange(
		 IDC_TRIGGER_THRESHOLD
		,CurrentSetting->TriggerMinThreshold[INDEX_LEFT_TRIGGER]
		,CurrentSetting->TriggerMaxThreshold[INDEX_LEFT_TRIGGER] );
	SetTrackBarPos(
		 IDC_TRIGGER_THRESHOLD
		,CurrentSetting->TriggerMinThreshold[INDEX_LEFT_TRIGGER] );
	//	「トリガーの入力閾値」
	SetTrackBarPos(
		 IDC_TRIGGER_BUTTON_THRESHOLD
		,CurrentSetting->TriggerButtonThreshold[INDEX_LEFT_TRIGGER] );
}
