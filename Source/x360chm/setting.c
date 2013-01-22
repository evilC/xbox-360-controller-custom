
//	HID Minidriver for Microsoft Xbox 360 Controller

#include	"driver.h"

//----------------------------------------------------------------------------------------------
//	LoadSetting
//----------------------------------------------------------------------------------------------
VOID LoadSetting( IN SETTING * Setting )
{
	//	変数宣言
	UCHAR	Index;
	UCHAR	WayIndex;

	//	レジストリが読めない場合の初期設定を行う
	RtlZeroMemory( Setting, sizeof( SETTING ) );

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

	//	レジストリより設定を読み込む
	GetRegistryBinary(
		 REGISTRY_PATH
		,CURRENT_SETTING
		,(char *)Setting
		,sizeof( SETTING ) );

	//	ポーリングを行わない場合は連射機能を無効にする
	if( DevicesArePolled == FALSE )
	{
		for( Index = INDEX_A_BUTTON; Index <= INDEX_XBOX_GUIDE_BUTTON; Index ++ )
		{
			Setting->ButtonRapidFire[Index]	= FALSE;
		}
	}

	//	制限モードの値を補正する
	Setting->LimitationMode	= min( Setting->LimitationMode, MAX_LIMITATION_MODE );
}
