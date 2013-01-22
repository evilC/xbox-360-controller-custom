
//	Property Sheet for Microsoft Xbox 360 Controller

#include	"actuator_page.h"
#include	"resource.h"

//----------------------------------------------------------------------------------------------
//	CActuatorPage
//----------------------------------------------------------------------------------------------
CActuatorPage::CActuatorPage( CSetting * Setting )
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
VOID CActuatorPage::wmInitDialog( HWND Wnd )
{
	//	ウィンドウ ハンドル
	this->Wnd	= Wnd;

	//	「左アクチュエータ」
	InitTrackBar( IDC_LEFT_ACTUATOR_LEVEL, 0, 100, 10, 10 );
	//	「右アクチュエータ」
	InitTrackBar( IDC_RIGHT_ACTUATOR_LEVEL, 0, 100, 10, 10 );
	//	「振動の有効範囲」
	InitTrackBar( IDC_ACTUATOR_THRESHOLD, 0, 100, 10, 10 );
}

//----------------------------------------------------------------------------------------------
//	wmCommand
//----------------------------------------------------------------------------------------------
VOID CActuatorPage::wmCommand(
	 HWND	Control
	,WORD	ControlID
	,WORD	NotifyMessage )
{
	//	コントロールによって処理を振り分ける
	switch( ControlID )
	{
		case IDC_ACTUATOR:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case BN_CLICKED:
					//	現在の設定に反映する
					CurrentSetting->Actuator	= GetCheckBoxState( ControlID );
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
VOID CActuatorPage::wmHScroll(
	 HWND	Control
	,WORD	Request )
{
	//	「左アクチュエータ」からのメッセージ
	if( Control == GetDlgItem( Wnd, IDC_LEFT_ACTUATOR_LEVEL ) )
	{
		//	現在の設定に反映する
		CurrentSetting->ActuatorLevel[INDEX_LEFT_ACTUATOR]
			= GetTrackBarPos( IDC_LEFT_ACTUATOR_LEVEL );
		//	設定の変更を通知する
		SettingChanged();
	}

	//	「右アクチュエータ」からのメッセージ
	if( Control == GetDlgItem( Wnd, IDC_RIGHT_ACTUATOR_LEVEL ) )
	{
		//	現在の設定に反映する
		CurrentSetting->ActuatorLevel[INDEX_RIGHT_ACTUATOR]
			= GetTrackBarPos( IDC_RIGHT_ACTUATOR_LEVEL );
		//	設定の変更を通知する
		SettingChanged();
	}

	//	「振動の有効範囲」からのメッセージ
	if( Control == GetDlgItem( Wnd, IDC_ACTUATOR_THRESHOLD ) )
	{
		//	現在の選択状態を退避する
		BOOL	PrevSelecting	= TrackBarSelecting;

		//	トラック バーの範囲選択を行う
		SelectTrackBar( Control, Request, &TrackBarSelecting );
		if( PrevSelecting != TrackBarSelecting )
		{
			//	現在の設定に反映する
			GetTrackBarRange(
				 IDC_ACTUATOR_THRESHOLD
				,&CurrentSetting->ActuatorMinThreshold
				,&CurrentSetting->ActuatorMaxThreshold );
			//	設定の変更を通知する
			SettingChanged();
		}
	}
}

//----------------------------------------------------------------------------------------------
//	ApplySetting
//----------------------------------------------------------------------------------------------
VOID CActuatorPage::ApplySetting()
{
	//	「コントローラの振動」
	SetCheckBoxState( IDC_ACTUATOR, CurrentSetting->Actuator );
	if( CurrentSetting->Actuator == TRUE )
	{
		EnableControls( IDC_STATIC_601, IDC_STATIC_613, TRUE );
	} else {
		EnableControls( IDC_STATIC_601, IDC_STATIC_613, FALSE );
	}

	//	「左アクチュエータ」
	SetTrackBarPos(
		 IDC_LEFT_ACTUATOR_LEVEL
		,CurrentSetting->ActuatorLevel[INDEX_LEFT_ACTUATOR] );
	//	「右アクチュエータ」
	SetTrackBarPos(
		 IDC_RIGHT_ACTUATOR_LEVEL
		,CurrentSetting->ActuatorLevel[INDEX_RIGHT_ACTUATOR] );

	//	「振動の有効範囲」
	SetTrackBarRange(
		 IDC_ACTUATOR_THRESHOLD
		,CurrentSetting->ActuatorMinThreshold
		,CurrentSetting->ActuatorMaxThreshold );
	SetTrackBarPos( IDC_ACTUATOR_THRESHOLD, CurrentSetting->ActuatorMinThreshold );
}
