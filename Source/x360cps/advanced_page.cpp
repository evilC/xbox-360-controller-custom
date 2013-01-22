
//	Property Sheet for Microsoft Xbox 360 Controller

#include	"advanced_page.h"
#include	"resource.h"

//----------------------------------------------------------------------------------------------
//	CAdvancedPage
//----------------------------------------------------------------------------------------------
CAdvancedPage::CAdvancedPage( CSetting * Setting )
{
	//	設定
	this->Setting		= Setting;

	//	現在のコントローラーの設定
	CurrentSetting		= &Setting->CurrentSetting;
}

//----------------------------------------------------------------------------------------------
//	wmInitDialog
//----------------------------------------------------------------------------------------------
VOID CAdvancedPage::wmInitDialog( HWND Wnd )
{
	//	ウィンドウ ハンドル
	this->Wnd	= Wnd;

	//	機能の一覧
	WORD	SliderList[]	= {	 IDS_Z_AXIS
								,IDS_Z_ROTATION
								,IDS_SLIDER1
								,IDS_SLIDER2 };
	WORD	SliderListCount	= sizeof( SliderList ) / sizeof( WORD ) ;

	//	互換性モード
	WORD	LimitationMode[]	= {	 IDS_LIMITATION_MODE_0
									,IDS_LIMITATION_MODE_1
									,IDS_LIMITATION_MODE_2
									,IDS_LIMITATION_MODE_3 };
	WORD	LimitationModeCount	= sizeof( LimitationMode ) / sizeof( WORD ) ;

	//	「機能の一覧」
	InitListBox(
		 IDC_SLIDER_LIST
		,SliderList
		,SliderListCount );
	SetListBoxIndex( IDC_SLIDER_LIST, 0 );
	//	「互換性モード」
	InitComboBox(
		 IDC_LIMITATION_MODE
		,LimitationMode
		,LimitationModeCount );
}

//----------------------------------------------------------------------------------------------
//	wmCommand
//----------------------------------------------------------------------------------------------
VOID CAdvancedPage::wmCommand(
	 HWND	Control
	,WORD	ControlID
	,WORD	NotifyMessage )
{
	//	コントロールによって処理を振り分ける
	switch( ControlID )
	{
		case IDC_SLIDER_LIST:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case LBN_SELCHANGE:
					ApplySetting();
					break;
			}
			break;

		case IDC_SLIDE_SLIDER_VALUE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case BN_CLICKED:
					//	現在の設定に反映する
					CurrentSetting->SlideSliderValue[GetListBoxIndex( IDC_SLIDER_LIST )]
						= GetCheckBoxState( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_SLIDE_SLIDER_CENTER_VALUE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case BN_CLICKED:
					//	現在の設定に反映する
					CurrentSetting->SlideSliderCenterValue[GetListBoxIndex( IDC_SLIDER_LIST )]
						= GetCheckBoxState( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_REVERSE_SLIDER_VALUE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case BN_CLICKED:
					//	現在の設定に反映する
					CurrentSetting->ReverseSliderValue[GetListBoxIndex( IDC_SLIDER_LIST )]
						= GetCheckBoxState( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_LIMITATION_MODE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case CBN_SELCHANGE:
					//	現在の設定に反映する
					CurrentSetting->LimitationMode	= GetComboBoxIndex( ControlID );
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_STICK_DEAD_ZONE_TYPE_SQUARE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case BN_CLICKED:
					//	現在の設定に反映する
					CurrentSetting->StickDeadZoneType[INDEX_LEFT_STICK]	= DEAD_ZONE_TYPE_SQUARE;
					CurrentSetting->StickDeadZoneType[INDEX_RIGHT_STICK]
						= CurrentSetting->StickDeadZoneType[INDEX_LEFT_STICK];
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_STICK_DEAD_ZONE_TYPE_CIRCLE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case BN_CLICKED:
					//	現在の設定に反映する
					CurrentSetting->StickDeadZoneType[INDEX_LEFT_STICK]	= DEAD_ZONE_TYPE_CIRCLE;
					CurrentSetting->StickDeadZoneType[INDEX_RIGHT_STICK]
						= CurrentSetting->StickDeadZoneType[INDEX_LEFT_STICK];
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_TRANSFORM_TYPE_SQUARE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case BN_CLICKED:
					//	現在の設定に反映する
					if( GetCheckBoxState( ControlID ) == BST_CHECKED )
					{
						CurrentSetting->StickTransformType[INDEX_LEFT_STICK]
							= TRANSFORM_TYPE_SQUARE;
						CurrentSetting->StickTransformType[INDEX_RIGHT_STICK]
							= TRANSFORM_TYPE_SQUARE;
						SetCheckBoxState( IDC_TRANSFORM_TYPE_CIRCLE, BST_UNCHECKED );
					} else {
						CurrentSetting->StickTransformType[INDEX_LEFT_STICK]
							= TRANSFORM_TYPE_NONE;
						CurrentSetting->StickTransformType[INDEX_RIGHT_STICK]
							= TRANSFORM_TYPE_NONE;
					}
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_TRANSFORM_TYPE_CIRCLE:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case BN_CLICKED:
					//	現在の設定に反映する
					if( GetCheckBoxState( ControlID ) == BST_CHECKED )
					{
						CurrentSetting->StickTransformType[INDEX_LEFT_STICK]
							= TRANSFORM_TYPE_CIRCLE;
						CurrentSetting->StickTransformType[INDEX_RIGHT_STICK]
							= TRANSFORM_TYPE_CIRCLE;
						SetCheckBoxState( IDC_TRANSFORM_TYPE_SQUARE, BST_UNCHECKED );
					} else {
						CurrentSetting->StickTransformType[INDEX_LEFT_STICK]
							= TRANSFORM_TYPE_NONE;
						CurrentSetting->StickTransformType[INDEX_RIGHT_STICK]
							= TRANSFORM_TYPE_NONE;
					}
					//	設定の変更を通知する
					SettingChanged();
					break;
			}
			break;

		case IDC_DEVICES_ARE_POLLED:

			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case BN_CLICKED:
					//	現在の設定に反映する
					CurrentSetting->DevicesArePolled
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
VOID CAdvancedPage::ApplySetting()
{
	//	「スティックの有効範囲」
	if( CurrentSetting->StickDeadZoneType[INDEX_LEFT_STICK] == DEAD_ZONE_TYPE_SQUARE )
	{
		SetCheckBoxState( IDC_STICK_DEAD_ZONE_TYPE_SQUARE, BST_CHECKED );
		SetCheckBoxState( IDC_STICK_DEAD_ZONE_TYPE_CIRCLE, BST_UNCHECKED );
	} else {
		SetCheckBoxState( IDC_STICK_DEAD_ZONE_TYPE_SQUARE, BST_UNCHECKED );
		SetCheckBoxState( IDC_STICK_DEAD_ZONE_TYPE_CIRCLE, BST_CHECKED );
	}

	//	「スティックの可動範囲」
	if( CurrentSetting->StickTransformType[INDEX_LEFT_STICK] == TRANSFORM_TYPE_NONE )
	{
		SetCheckBoxState( IDC_TRANSFORM_TYPE_SQUARE, BST_UNCHECKED );
		SetCheckBoxState( IDC_TRANSFORM_TYPE_CIRCLE, BST_UNCHECKED );
	}
	if( CurrentSetting->StickTransformType[INDEX_LEFT_STICK] == TRANSFORM_TYPE_SQUARE )
	{
		SetCheckBoxState( IDC_TRANSFORM_TYPE_SQUARE, BST_CHECKED );
		SetCheckBoxState( IDC_TRANSFORM_TYPE_CIRCLE, BST_UNCHECKED );
	}
	if( CurrentSetting->StickTransformType[INDEX_LEFT_STICK] == TRANSFORM_TYPE_CIRCLE )
	{
		SetCheckBoxState( IDC_TRANSFORM_TYPE_SQUARE, BST_UNCHECKED );
		SetCheckBoxState( IDC_TRANSFORM_TYPE_CIRCLE, BST_CHECKED );
	}

	//	「機能の一覧」
	LONG	ListBoxIndex	= GetListBoxIndex( IDC_SLIDER_LIST );
	//	「中心を基準にする」
	SetCheckBoxState( IDC_SLIDE_SLIDER_VALUE, CurrentSetting->SlideSliderValue[ListBoxIndex] );
	//	「中心の基準値をずらす」
	SetCheckBoxState(
		 IDC_SLIDE_SLIDER_CENTER_VALUE
		,CurrentSetting->SlideSliderCenterValue[ListBoxIndex] );
	//	「値を反転する」
	SetCheckBoxState(
		 IDC_REVERSE_SLIDER_VALUE
		,CurrentSetting->ReverseSliderValue[ListBoxIndex] );

	//	「互換性モード」
	SetComboBoxIndex( IDC_LIMITATION_MODE, CurrentSetting->LimitationMode );

	//	「ポーリングを行う」
	SetCheckBoxState( IDC_DEVICES_ARE_POLLED, CurrentSetting->DevicesArePolled );
}
