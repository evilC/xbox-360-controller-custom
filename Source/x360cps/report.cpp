
//	Property Sheet for Microsoft Xbox 360 Controller

#include	"report.h"
#include	"math.h"

//----------------------------------------------------------------------------------------------
//	GenerateReadReport
//----------------------------------------------------------------------------------------------
VOID CReport::GenerateReadReport(
	 INPUT_REPORT *		InputReport
	,SETTING *			Setting
	,HID_READ_REPORT *	Report
	,LAST_STATE *		LastState )
{
	//	変数宣言
	LONG	Index;
	LONG	WayIndex;
	LONG	AxisIndex;

	//	スティック
	SHORT	StickX[2]	= { InputReport->LeftStickX, InputReport->RightStickX };
	SHORT	StickY[2]	= { InputReport->LeftStickY, InputReport->RightStickY };
	SHORT	StickX_Axis[2];
	SHORT	StickY_Axis[2];
	SHORT	StickX_Slider[2];
	SHORT	StickY_Slider[2];
	UCHAR	Stick_HatSwitch[2];
	UCHAR	StickWay_Slider[2][4];
	BOOLEAN	StickWay_Button[2][4];
	SHORT	StickWay_AxisPlus[2][4];
	SHORT	StickWay_AxisMinus[2][4];

	//	方向パッド
	SHORT	DirectionalPadTable[]	= {			//	右,左,    軸
										 0		//	 0, 0,     0
										,-32768	//	 0, 1,-32768
										,32767	//	 1, 0, 32767
										,0 };	//	 1, 1,     0
	SHORT	DirectionalPadX
				= DirectionalPadTable[ ( InputReport->DigitalButton & 0x0f ) >> 2 ];
	SHORT	DirectionalPadY	= DirectionalPadTable[ InputReport->DigitalButton & 0x03 ];
	SHORT	DirectionalPadX_Slider;
	SHORT	DirectionalPadY_Slider;
	UCHAR	DirectionalPad_HatSwitch;
	UCHAR	DirectionalPadWay_Slider[4]	= {
				 InputReport->DigitalButton & DIRECTIONAL_PAD_UP ? 255 : 0
				,InputReport->DigitalButton & DIRECTIONAL_PAD_DOWN ? 255 : 0
				,InputReport->DigitalButton & DIRECTIONAL_PAD_LEFT ? 255 : 0
				,InputReport->DigitalButton & DIRECTIONAL_PAD_RIGHT ? 255 : 0 };
	BOOLEAN	DirectionalPadWay_Button[4]	= {
				 InputReport->DigitalButton & DIRECTIONAL_PAD_UP ? TRUE : FALSE
				,InputReport->DigitalButton & DIRECTIONAL_PAD_DOWN ? TRUE : FALSE
				,InputReport->DigitalButton & DIRECTIONAL_PAD_LEFT ? TRUE : FALSE
				,InputReport->DigitalButton & DIRECTIONAL_PAD_RIGHT ? TRUE : FALSE };
	SHORT	DirectionalPadWay_AxisPlus[4]	= {
				 InputReport->DigitalButton & DIRECTIONAL_PAD_UP ? 32767 : 0
				,InputReport->DigitalButton & DIRECTIONAL_PAD_DOWN ? 32767 : 0
				,InputReport->DigitalButton & DIRECTIONAL_PAD_LEFT ? 32767 : 0
				,InputReport->DigitalButton & DIRECTIONAL_PAD_RIGHT ? 32767 : 0 };
	SHORT	DirectionalPadWay_AxisMinus[4]	= {
				 InputReport->DigitalButton & DIRECTIONAL_PAD_UP ? -32768 : 0
				,InputReport->DigitalButton & DIRECTIONAL_PAD_DOWN ? -32768 : 0
				,InputReport->DigitalButton & DIRECTIONAL_PAD_LEFT ? -32768 : 0
				,InputReport->DigitalButton & DIRECTIONAL_PAD_RIGHT ? -32768 : 0 };

	//	トリガー
	UCHAR	Trigger[2]	= { InputReport->LeftTrigger, InputReport->RightTrigger };
	UCHAR	Trigger_Slider[2];
	BOOLEAN	Trigger_Button[2];
	SHORT	Trigger_AxisPlus[2];
	SHORT	Trigger_AxisMinus[2];

	//	ボタン
	UCHAR	Button_Slider[11]	= {
				 InputReport->DigitalButton & A_BUTTON ? 255 : 0
				,InputReport->DigitalButton & B_BUTTON ? 255 : 0
				,InputReport->DigitalButton & X_BUTTON ? 255 : 0
				,InputReport->DigitalButton & Y_BUTTON ? 255 : 0
				,InputReport->DigitalButton & LB_BUTTON ? 255 : 0
				,InputReport->DigitalButton & RB_BUTTON ? 255 : 0
				,InputReport->DigitalButton & START_BUTTON ? 255 : 0
				,InputReport->DigitalButton & BACK_BUTTON ? 255 : 0
				,InputReport->DigitalButton & LEFT_STICK_BUTTON ? 255 : 0
				,InputReport->DigitalButton & RIGHT_STICK_BUTTON ? 255 : 0
				,InputReport->DigitalButton & XBOX_GUIDE_BUTTON ? 255 : 0 };
	UCHAR	Button_Button[11]	= {
				 InputReport->DigitalButton & A_BUTTON ? TRUE : FALSE
				,InputReport->DigitalButton & B_BUTTON ? TRUE : FALSE
				,InputReport->DigitalButton & X_BUTTON ? TRUE : FALSE
				,InputReport->DigitalButton & Y_BUTTON ? TRUE : FALSE
				,InputReport->DigitalButton & LB_BUTTON ? TRUE : FALSE
				,InputReport->DigitalButton & RB_BUTTON ? TRUE : FALSE
				,InputReport->DigitalButton & START_BUTTON ? TRUE : FALSE
				,InputReport->DigitalButton & BACK_BUTTON ? TRUE : FALSE
				,InputReport->DigitalButton & LEFT_STICK_BUTTON ? TRUE : FALSE
				,InputReport->DigitalButton & RIGHT_STICK_BUTTON ? TRUE : FALSE
				,InputReport->DigitalButton & XBOX_GUIDE_BUTTON ? TRUE : FALSE };
	SHORT	Button_AxisPlus[11]	= {
				 InputReport->DigitalButton & A_BUTTON ? 32767 : 0
				,InputReport->DigitalButton & B_BUTTON ? 32767 : 0
				,InputReport->DigitalButton & X_BUTTON ? 32767 : 0
				,InputReport->DigitalButton & Y_BUTTON ? 32767 : 0
				,InputReport->DigitalButton & LB_BUTTON ? 32767 : 0
				,InputReport->DigitalButton & RB_BUTTON ? 32767 : 0
				,InputReport->DigitalButton & START_BUTTON ? 32767 : 0
				,InputReport->DigitalButton & BACK_BUTTON ? 32767 : 0
				,InputReport->DigitalButton & LEFT_STICK_BUTTON ? 32767 : 0
				,InputReport->DigitalButton & RIGHT_STICK_BUTTON ? 32767 : 0
				,InputReport->DigitalButton & XBOX_GUIDE_BUTTON ? 32767 : 0 };
	SHORT	Button_AxisMinus[11]	= {
				 InputReport->DigitalButton & A_BUTTON ? -32768 : 0
				,InputReport->DigitalButton & B_BUTTON ? -32768 : 0
				,InputReport->DigitalButton & X_BUTTON ? -32768 : 0
				,InputReport->DigitalButton & Y_BUTTON ? -32768 : 0
				,InputReport->DigitalButton & LB_BUTTON ? -32768 : 0
				,InputReport->DigitalButton & RB_BUTTON ? -32768 : 0
				,InputReport->DigitalButton & START_BUTTON ? -32768 : 0
				,InputReport->DigitalButton & BACK_BUTTON ? -32768 : 0
				,InputReport->DigitalButton & LEFT_STICK_BUTTON ? -32768 : 0
				,InputReport->DigitalButton & RIGHT_STICK_BUTTON ? -32768 : 0
				,InputReport->DigitalButton & XBOX_GUIDE_BUTTON ? -32768 : 0 };

	//	レポート
	SHORT	Axis[4]			= { 0, 0, 0, 0 };
	UCHAR	HatSwitch[4]	= { 0, 0, 0, 0 };
	SHORT	Slider[4]		= { 0, 0, 0, 0 };
	ULONG	ButtonState		= 0;

	//	ハット スイッチ
	CHAR	HatSwitchTable[]	= {			//	右,左,下,上,角度
									 -1		//	 0, 0, 0, 0,  -1
									,0		//	 0, 0, 0, 1,   0
									,4		//	 0, 0, 1, 0, 180
									,-1		//	 0, 0, 1, 1,  -1
									,6		//	 0, 1, 0, 0, 270
									,7		//	 0, 1, 0, 1, 315
									,5		//	 0, 1, 1, 0, 225
									,-1		//	 0, 1, 1, 1,  -1
									,2		//	 1, 0, 0, 0,  90
									,1		//	 1, 0, 0, 1,  45
									,3		//	 1, 0, 1, 0, 135
									,-1		//	 1, 0, 1, 1,  -1
									,-1		//	 1, 1, 0, 0,  -1
									,-1		//	 1, 1, 0, 1,  -1
									,-1		//	 1, 1, 1, 0,  -1
									,-1 };	//	 1, 1, 1, 1,  -1

	//	スティックの値を調整する
	for( Index = INDEX_LEFT_STICK; Index <= INDEX_RIGHT_STICK; Index ++ )
	{
		//	上下反転、左右反転、回転を調整する
		AdjustStickValue(
			 StickX[Index]
			,StickY[Index]
			,Setting->StickReverseX[Index]
			,Setting->StickReverseY[Index]
			,Setting->StickDeadZoneType[Index]
			,Setting->StickTransformType[Index]
			,Setting->StickAngle[Index]
			,&StickX[Index]
			,&StickY[Index] );
		//	軸として使用する場合のデッド ゾーンを調整する
		AdjustStickToAxis(
			 StickX[Index]
			,StickY[Index]
			,Setting->StickDeadZoneType[Index]
			,Setting->StickMinThreshold[Index]
			,Setting->StickMaxThreshold[Index]
			,&StickX_Axis[Index]
			,&StickY_Axis[Index] );
		//	スライダーとして使用する場合の値を調整する
		AdjustStickXToSlider(
			 StickX_Axis[Index]
			,Setting->StickUsage[Index]
			,Setting->SlideSliderValue
			,&StickX_Slider[Index] );
		AdjustStickYToSlider(
			 StickY_Axis[Index]
			,Setting->StickUsage[Index]
			,Setting->SlideSliderValue
			,&StickY_Slider[Index] );
		//	ハット スイッチとして使用する場合のデッド ゾーンを調整する
		AdjustStickToHatSwitch(
			 StickX[Index]
			,StickY[Index]
			,Setting->StickHatSwitchThreshold[Index]
			,&Stick_HatSwitch[Index] );
		//	４方向スライダーとして使用する場合の値を調整する
		StickWay_Slider[Index][0]
			= (UCHAR)( StickY_Axis[Index] <= 0 ? min( -StickY_Axis[Index] >> 7, 255 ) : 0 );
		StickWay_Slider[Index][1]
			= (UCHAR)( StickY_Axis[Index] > 0 ? min( StickY_Axis[Index] >> 7, 255 ) : 0 );
		StickWay_Slider[Index][2]
			= (UCHAR)( StickX_Axis[Index] <= 0 ? min( -StickX_Axis[Index] >> 7, 255 ) : 0 );
		StickWay_Slider[Index][3]
			= (UCHAR)( StickX_Axis[Index] > 0 ? min( StickX_Axis[Index] >> 7, 255 ) : 0 );
		//	４方向ボタンとして使用する場合の値を調整する
		StickWay_Button[Index][0]
			= Stick_HatSwitch[Index] & DIRECTIONAL_PAD_UP ? TRUE : FALSE;
		StickWay_Button[Index][1]
			= Stick_HatSwitch[Index] & DIRECTIONAL_PAD_DOWN ? TRUE : FALSE;
		StickWay_Button[Index][2]
			= Stick_HatSwitch[Index] & DIRECTIONAL_PAD_LEFT ? TRUE : FALSE;
		StickWay_Button[Index][3]
			= Stick_HatSwitch[Index] & DIRECTIONAL_PAD_RIGHT ? TRUE : FALSE;
		//	４方向軸として使用する場合の値を調整する
		StickWay_AxisPlus[Index][0]
			= min( StickY_Axis[Index] < 0 ? -StickY_Axis[Index] : 0, 32767 );
		StickWay_AxisPlus[Index][1]		= StickY_Axis[Index] > 0 ? StickY_Axis[Index] : 0;
		StickWay_AxisPlus[Index][2]
			= min( StickX_Axis[Index] < 0 ? -StickX_Axis[Index] : 0, 32767 );
		StickWay_AxisPlus[Index][3]		= StickX_Axis[Index] > 0 ? StickX_Axis[Index] : 0;
		StickWay_AxisMinus[Index][0]	= StickY_Axis[Index] < 0 ? StickY_Axis[Index] : 0;
		StickWay_AxisMinus[Index][1]
			= min( StickY_Axis[Index] > 0 ? -StickY_Axis[Index] : 0, 32767 );
		StickWay_AxisMinus[Index][2]	= StickX_Axis[Index] < 0 ? StickX_Axis[Index] : 0;
		StickWay_AxisMinus[Index][3]
			= min( StickX_Axis[Index] > 0 ? -StickX_Axis[Index] : 0, 32767 );
	}

	//	方向パッドの上下反転、左右反転、回転を調整する
	AdjustStickValue(
		 DirectionalPadX
		,DirectionalPadY
		,Setting->DirectionalPadReverseX
		,Setting->DirectionalPadReverseY
		,DEAD_ZONE_TYPE_SQUARE
		,Setting->DirectionalPadTransformType
		,Setting->DirectionalPadAngle
		,&DirectionalPadX
		,&DirectionalPadY );
	//	方向パッドを４方向スライダーとして使用する場合の値を調整する
	AdjustStickXToSlider(
		 DirectionalPadX
		,Setting->DirectionalPadUsage
		,Setting->SlideSliderValue
		,&DirectionalPadX_Slider );
	AdjustStickYToSlider(
		 DirectionalPadY
		,Setting->DirectionalPadUsage
		,Setting->SlideSliderValue
		,&DirectionalPadY_Slider );
	//	方向パッドをハット スイッチとして使用する場合のデッド ゾーンを調整する
	AdjustStickToHatSwitch(
		 DirectionalPadX
		,-DirectionalPadY
		,50
		,&DirectionalPad_HatSwitch );

	//	トリガーの値を調整する
	for( Index = INDEX_LEFT_TRIGGER; Index <= INDEX_RIGHT_TRIGGER; Index ++ )
	{
		//	トリガーをスライダーとして使用する場合の値を調整する
		AdjustTriggerToSlider(
			 Trigger[Index]
			,Setting->TriggerMinThreshold[Index]
			,Setting->TriggerMaxThreshold[Index]
			,&Trigger_Slider[Index] );
		//	トリガーをボタンとして使用する場合の値を調整する
		AdjustTriggerToButton(
			 Trigger[Index]
			,Setting->TriggerButtonThreshold[Index]
			,&Trigger_Button[Index] );
		//	トリガーを軸として使用する場合の値を調整する
		Trigger_AxisPlus[Index]
			= Trigger_Slider[Index] == 255 ? 32767 : (SHORT)Trigger_Slider[Index] << 7;
		Trigger_AxisMinus[Index]
			= Trigger_Slider[Index] == 255 ? -32768 : -(SHORT)Trigger_Slider[Index] << 7;
	}

	//	スティック
	for( Index = INDEX_LEFT_STICK; Index <= INDEX_RIGHT_STICK; Index ++ )
	{
		switch( Setting->StickUsage[Index] )
		{
			case USAGE_X_AXIS_Y_AXIS:
				Axis[X_AXIS]		= ADD_SHORT( Axis[X_AXIS], StickX_Axis[Index] );
				Axis[Y_AXIS]		= ADD_SHORT( Axis[Y_AXIS], StickY_Axis[Index] );
				break;

			case USAGE_X_AXIS_NONE:
				Axis[X_AXIS]		= ADD_SHORT( Axis[X_AXIS], StickX_Axis[Index] );
				break;

			case USAGE_NONE_X_AXIS:
				Axis[X_AXIS]		= ADD_SHORT( Axis[X_AXIS], StickY_Axis[Index] );
				break;

			case USAGE_Y_AXIS_X_AXIS:
				Axis[Y_AXIS]		= ADD_SHORT( Axis[Y_AXIS], StickX_Axis[Index] );
				Axis[X_AXIS]		= ADD_SHORT( Axis[X_AXIS], StickY_Axis[Index] );
				break;

			case USAGE_Y_AXIS_NONE:
				Axis[Y_AXIS]		= ADD_SHORT( Axis[Y_AXIS], StickX_Axis[Index] );
				break;

			case USAGE_NONE_Y_AXIS:
				Axis[Y_AXIS]		= ADD_SHORT( Axis[Y_AXIS], StickY_Axis[Index] );
				break;

			case USAGE_X_ROTATION_Y_ROTATION:
				Axis[X_ROTATION]	= ADD_SHORT( Axis[X_ROTATION], StickX_Axis[Index] );
				Axis[Y_ROTATION]	= ADD_SHORT( Axis[Y_ROTATION], StickY_Axis[Index] );
				break;

			case USAGE_X_ROTATION_NONE:
				Axis[X_ROTATION]	= ADD_SHORT( Axis[X_ROTATION], StickX_Axis[Index] );
				break;

			case USAGE_NONE_X_ROTATION:
				Axis[X_ROTATION]	= ADD_SHORT( Axis[X_ROTATION], StickY_Axis[Index] );
				break;

			case USAGE_Y_ROTATION_X_ROTATION:
				Axis[Y_ROTATION]	= ADD_SHORT( Axis[Y_ROTATION], StickX_Axis[Index] );
				Axis[X_ROTATION]	= ADD_SHORT( Axis[X_ROTATION], StickY_Axis[Index] );
				break;

			case USAGE_Y_ROTATION_NONE:
				Axis[Y_ROTATION]	= ADD_SHORT( Axis[Y_ROTATION], StickX_Axis[Index] );
				break;

			case USAGE_NONE_Y_ROTATION:
				Axis[Y_ROTATION]	= ADD_SHORT( Axis[Y_ROTATION], StickY_Axis[Index] );
				break;

			case USAGE_Z_AXIS_Z_ROTAION:
				Slider[Z_AXIS]		= ADD_SHORT( Slider[Z_AXIS], StickX_Slider[Index] );
				Slider[Z_ROTATION]	= ADD_SHORT( Slider[Z_ROTATION], StickY_Slider[Index] );
				break;

			case USAGE_Z_AXIS_SLIDER1:
				Slider[Z_AXIS]		= ADD_SHORT( Slider[Z_AXIS], StickX_Slider[Index] );
				Slider[SLIDER1]		= ADD_SHORT( Slider[SLIDER1], StickY_Slider[Index] );
				break;

			case USAGE_Z_AXIS_SLIDER2:
				Slider[Z_AXIS]		= ADD_SHORT( Slider[Z_AXIS], StickX_Slider[Index] );
				Slider[SLIDER2]		= ADD_SHORT( Slider[SLIDER2], StickY_Slider[Index] );
				break;

			case USAGE_Z_AXIS_NONE:
				Slider[Z_AXIS]		= ADD_SHORT( Slider[Z_AXIS], StickX_Slider[Index] );
				break;

			case USAGE_NONE_Z_AXIS:
				Slider[Z_AXIS]		= ADD_SHORT( Slider[Z_AXIS], StickY_Slider[Index] );
				break;

			case USAGE_Z_ROTATION_Z_AXIS:
				Slider[Z_ROTATION]	= ADD_SHORT( Slider[Z_ROTATION], StickX_Slider[Index] );
				Slider[Z_AXIS]		= ADD_SHORT( Slider[Z_AXIS], StickY_Slider[Index] );
				break;

			case USAGE_Z_ROTATION_SLIDER1:
				Slider[Z_ROTATION]	= ADD_SHORT( Slider[Z_ROTATION], StickX_Slider[Index] );
				Slider[SLIDER1]		= ADD_SHORT( Slider[SLIDER1], StickY_Slider[Index] );
				break;

			case USAGE_Z_ROTATION_SLIDER2:
				Slider[Z_ROTATION]	= ADD_SHORT( Slider[Z_ROTATION], StickX_Slider[Index] );
				Slider[SLIDER2]		= ADD_SHORT( Slider[SLIDER2], StickY_Slider[Index] );
				break;

			case USAGE_Z_ROTATION_NONE:
				Slider[Z_ROTATION]	= ADD_SHORT( Slider[Z_ROTATION], StickX_Slider[Index] );
				break;

			case USAGE_NONE_Z_ROTATION:
				Slider[Z_ROTATION]	= ADD_SHORT( Slider[Z_ROTATION], StickY_Slider[Index] );
				break;

			case USAGE_SLIDER1_Z_AXIS:
				Slider[SLIDER1]		= ADD_SHORT( Slider[SLIDER1], StickX_Slider[Index] );
				Slider[Z_AXIS]		= ADD_SHORT( Slider[Z_AXIS], StickY_Slider[Index] );
				break;

			case USAGE_SLIDER1_Z_ROTATION:
				Slider[SLIDER1]		= ADD_SHORT( Slider[SLIDER1], StickX_Slider[Index] );
				Slider[Z_ROTATION]	= ADD_SHORT( Slider[Z_ROTATION], StickY_Slider[Index] );
				break;

			case USAGE_SLIDER1_SLIDER2:
				Slider[SLIDER1]		= ADD_SHORT( Slider[SLIDER1], StickX_Slider[Index] );
				Slider[SLIDER2]		= ADD_SHORT( Slider[SLIDER2], StickY_Slider[Index] );
				break;

			case USAGE_SLIDER1_NONE:
				Slider[SLIDER1]		= ADD_SHORT( Slider[SLIDER1], StickX_Slider[Index] );
				break;

			case USAGE_NONE_SLIDER1:
				Slider[SLIDER1]		= ADD_SHORT( Slider[SLIDER1], StickY_Slider[Index] );
				break;

			case USAGE_SLIDER2_Z_AXIS:
				Slider[SLIDER2]		= ADD_SHORT( Slider[SLIDER2], StickX_Slider[Index] );
				Slider[Z_AXIS]		= ADD_SHORT( Slider[Z_AXIS], StickY_Slider[Index] );
				break;

			case USAGE_SLIDER2_Z_ROTATION:
				Slider[SLIDER2]		= ADD_SHORT( Slider[SLIDER2], StickX_Slider[Index] );
				Slider[Z_ROTATION]	= ADD_SHORT( Slider[Z_ROTATION], StickY_Slider[Index] );
				break;

			case USAGE_SLIDER2_SLIDER1:
				Slider[SLIDER2]		= ADD_SHORT( Slider[SLIDER2], StickX_Slider[Index] );
				Slider[SLIDER1]		= ADD_SHORT( Slider[SLIDER1], StickY_Slider[Index] );
				break;

			case USAGE_SLIDER2_NONE:
				Slider[SLIDER2]		= ADD_SHORT( Slider[SLIDER2], StickX_Slider[Index] );
				break;

			case USAGE_NONE_SLIDER2:
				Slider[SLIDER2]		= ADD_SHORT( Slider[SLIDER2], StickY_Slider[Index] );
				break;

			case USAGE_HAT_SWITCH1:
				HatSwitch[HAT_SWITCH1]	|= Stick_HatSwitch[Index];
				break;

			case USAGE_HAT_SWITCH2:
				HatSwitch[HAT_SWITCH2]	|= Stick_HatSwitch[Index];
				break;

			case USAGE_HAT_SWITCH3:
				HatSwitch[HAT_SWITCH3]	|= Stick_HatSwitch[Index];
				break;

			case USAGE_HAT_SWITCH4:
				HatSwitch[HAT_SWITCH4]	|= Stick_HatSwitch[Index];
				break;

			case USAGE_4WAY_BUTTON:
				for( WayIndex = INDEX_WAY_UP; WayIndex <= INDEX_WAY_RIGHT; WayIndex ++ )
				{
					//	軸 (プラス)
					if( USAGE_X_AXIS_PLUS <= Setting->StickWayUsage[Index][WayIndex]
					&&	Setting->StickWayUsage[Index][WayIndex] <= USAGE_Y_ROTATION_PLUS )
					{
						SetAxis(
							 (SHORT)StickWay_AxisPlus[Index][WayIndex]
							,Setting->StickWayUsage[Index][WayIndex] - USAGE_X_AXIS_PLUS
							,Setting->StickWayRapidFire[Index][WayIndex]
							,Setting->StickWayRapidFireSpeed[Index][WayIndex]
							,Axis
							,&LastState->StickWayLastState[Index][WayIndex]
							,&LastState->StickWayRapidFireCount[Index][WayIndex] );
					}
					//	軸 (マイナス)
					if( USAGE_X_AXIS_MINUS <= Setting->StickWayUsage[Index][WayIndex]
					&&	Setting->StickWayUsage[Index][WayIndex] <= USAGE_Y_ROTATION_MINUS )
					{
						SetAxis(
							 (SHORT)StickWay_AxisMinus[Index][WayIndex]
							,Setting->StickWayUsage[Index][WayIndex] - USAGE_X_AXIS_PLUS
							,Setting->StickWayRapidFire[Index][WayIndex]
							,Setting->StickWayRapidFireSpeed[Index][WayIndex]
							,Axis
							,&LastState->StickWayLastState[Index][WayIndex]
							,&LastState->StickWayRapidFireCount[Index][WayIndex] );
					}
					//	スライダー (プラス)
					if( USAGE_Z_AXIS_PLUS <= Setting->StickWayUsage[Index][WayIndex]
					&&	Setting->StickWayUsage[Index][WayIndex] <= USAGE_SLIDER2_PLUS )
					{
						SetSlider(
							 (SHORT)StickWay_Slider[Index][WayIndex]
							,Setting->StickWayUsage[Index][WayIndex] - USAGE_Z_AXIS_PLUS
							,Setting->StickWayRapidFire[Index][WayIndex]
							,Setting->StickWayRapidFireSpeed[Index][WayIndex]
							,Slider
							,&LastState->StickWayLastState[Index][WayIndex]
							,&LastState->StickWayRapidFireCount[Index][WayIndex] );
					}
					//	スライダー (マイナス)
					if( USAGE_Z_AXIS_MINUS <= Setting->StickWayUsage[Index][WayIndex]
					&&	Setting->StickWayUsage[Index][WayIndex] <= USAGE_SLIDER2_MINUS )
					{
						SetSlider(
							 (SHORT)StickWay_Slider[Index][WayIndex] * -1
							,Setting->StickWayUsage[Index][WayIndex] - USAGE_Z_AXIS_MINUS
							,Setting->StickWayRapidFire[Index][WayIndex]
							,Setting->StickWayRapidFireSpeed[Index][WayIndex]
							,Slider
							,&LastState->StickWayLastState[Index][WayIndex]
							,&LastState->StickWayRapidFireCount[Index][WayIndex] );
					}
					//	ボタン
					if( USAGE_BUTTON_1 <= Setting->StickWayUsage[Index][WayIndex]
					&&	Setting->StickWayUsage[Index][WayIndex] <= USAGE_BUTTON_32 )
					{
						SetButton(
							 StickWay_Button[Index][WayIndex]
							,Setting->StickWayUsage[Index][WayIndex] - USAGE_BUTTON_1
							,Setting->StickWayRapidFire[Index][WayIndex]
							,Setting->StickWayRapidFireSpeed[Index][WayIndex]
							,&ButtonState
							,&LastState->StickWayLastState[Index][WayIndex]
							,&LastState->StickWayRapidFireCount[Index][WayIndex] );
					}
				}
				break;
		}
	}

	//	方向パッド
	switch( Setting->DirectionalPadUsage )
	{
		case USAGE_X_AXIS_Y_AXIS:
			Axis[X_AXIS]		= ADD_SHORT( Axis[X_AXIS], DirectionalPadX );
			Axis[Y_AXIS]		= ADD_SHORT( Axis[Y_AXIS], DirectionalPadY );
			break;

		case USAGE_X_AXIS_NONE:
			Axis[X_AXIS]		= ADD_SHORT( Axis[X_AXIS], DirectionalPadX );
			break;

		case USAGE_NONE_Y_AXIS:
			Axis[X_AXIS]		= ADD_SHORT( Axis[X_AXIS], DirectionalPadY );
			break;

		case USAGE_Y_AXIS_X_AXIS:
			Axis[Y_AXIS]		= ADD_SHORT( Axis[Y_AXIS], DirectionalPadX );
			Axis[X_AXIS]		= ADD_SHORT( Axis[X_AXIS], DirectionalPadY );
			break;

		case USAGE_Y_AXIS_NONE:
			Axis[Y_AXIS]		= ADD_SHORT( Axis[Y_AXIS], DirectionalPadX );
			break;

		case USAGE_NONE_X_AXIS:
			Axis[Y_AXIS]		= ADD_SHORT( Axis[Y_AXIS], DirectionalPadY );
			break;

		case USAGE_X_ROTATION_Y_ROTATION:
			Axis[X_ROTATION]	= ADD_SHORT( Axis[X_ROTATION], DirectionalPadX );
			Axis[Y_ROTATION]	= ADD_SHORT( Axis[Y_ROTATION], DirectionalPadY );
			break;

		case USAGE_X_ROTATION_NONE:
			Axis[X_ROTATION]	= ADD_SHORT( Axis[X_ROTATION], DirectionalPadX );
			break;

		case USAGE_NONE_X_ROTATION:
			Axis[X_ROTATION]	= ADD_SHORT( Axis[X_ROTATION], DirectionalPadY );
			break;

		case USAGE_Y_ROTATION_X_ROTATION:
			Axis[Y_ROTATION]	= ADD_SHORT( Axis[Y_ROTATION], DirectionalPadX );
			Axis[X_ROTATION]	= ADD_SHORT( Axis[X_ROTATION], DirectionalPadY );
			break;

		case USAGE_Y_ROTATION_NONE:
			Axis[Y_ROTATION]	= ADD_SHORT( Axis[Y_ROTATION], DirectionalPadX );
			break;

		case USAGE_NONE_Y_ROTATION:
			Axis[Y_ROTATION]	= ADD_SHORT( Axis[Y_ROTATION], DirectionalPadY );
			break;

		case USAGE_Z_AXIS_Z_ROTAION:
			Slider[Z_AXIS]		= ADD_SHORT( Slider[Z_AXIS], DirectionalPadX_Slider );
			Slider[Z_ROTATION]	= ADD_SHORT( Slider[Z_ROTATION], DirectionalPadY_Slider );
			break;

		case USAGE_Z_AXIS_SLIDER1:
			Slider[Z_AXIS]		= ADD_SHORT( Slider[Z_AXIS], DirectionalPadX_Slider );
			Slider[SLIDER1]		= ADD_SHORT( Slider[SLIDER1], DirectionalPadY_Slider );
			break;

		case USAGE_Z_AXIS_SLIDER2:
			Slider[Z_AXIS]		= ADD_SHORT( Slider[Z_AXIS], DirectionalPadX_Slider );
			Slider[SLIDER2]		= ADD_SHORT( Slider[SLIDER2], DirectionalPadY_Slider );
			break;

		case USAGE_Z_AXIS_NONE:
			Slider[Z_AXIS]		= ADD_SHORT( Slider[Z_AXIS], DirectionalPadX_Slider );
			break;

		case USAGE_NONE_Z_AXIS:
			Slider[Z_AXIS]		= ADD_SHORT( Slider[Z_AXIS], DirectionalPadY_Slider );
			break;

		case USAGE_Z_ROTATION_Z_AXIS:
			Slider[Z_ROTATION]	= ADD_SHORT( Slider[Z_ROTATION], DirectionalPadX_Slider );
			Slider[Z_AXIS]		= ADD_SHORT( Slider[Z_AXIS], DirectionalPadY_Slider );
			break;

		case USAGE_Z_ROTATION_SLIDER1:
			Slider[Z_ROTATION]	= ADD_SHORT( Slider[Z_ROTATION], DirectionalPadX_Slider );
			Slider[SLIDER1]		= ADD_SHORT( Slider[SLIDER1], DirectionalPadY_Slider );
			break;

		case USAGE_Z_ROTATION_SLIDER2:
			Slider[Z_ROTATION]	= ADD_SHORT( Slider[Z_ROTATION], DirectionalPadX_Slider );
			Slider[SLIDER2]		= ADD_SHORT( Slider[SLIDER2], DirectionalPadY_Slider );
			break;

		case USAGE_Z_ROTATION_NONE:
			Slider[Z_ROTATION]	= ADD_SHORT( Slider[Z_ROTATION], DirectionalPadX_Slider );
			break;

		case USAGE_NONE_Z_ROTATION:
			Slider[Z_ROTATION]	= ADD_SHORT( Slider[Z_ROTATION], DirectionalPadY_Slider );
			break;

		case USAGE_SLIDER1_Z_AXIS:
			Slider[SLIDER1]		= ADD_SHORT( Slider[SLIDER1], DirectionalPadX_Slider );
			Slider[Z_AXIS]		= ADD_SHORT( Slider[Z_AXIS], DirectionalPadY_Slider );
			break;

		case USAGE_SLIDER1_Z_ROTATION:
			Slider[SLIDER1]		= ADD_SHORT( Slider[SLIDER1], DirectionalPadX_Slider );
			Slider[Z_ROTATION]	= ADD_SHORT( Slider[Z_ROTATION], DirectionalPadY_Slider );
			break;

		case USAGE_SLIDER1_SLIDER2:
			Slider[SLIDER1]		= ADD_SHORT( Slider[SLIDER1], DirectionalPadX_Slider );
			Slider[SLIDER2]		= ADD_SHORT( Slider[SLIDER2], DirectionalPadY_Slider );
			break;

		case USAGE_SLIDER1_NONE:
			Slider[SLIDER1]		= ADD_SHORT( Slider[SLIDER1], DirectionalPadX_Slider );
			break;

		case USAGE_NONE_SLIDER1:
			Slider[SLIDER1]		= ADD_SHORT( Slider[SLIDER1], DirectionalPadY_Slider );
			break;

		case USAGE_SLIDER2_Z_AXIS:
			Slider[SLIDER2]		= ADD_SHORT( Slider[SLIDER2], DirectionalPadX_Slider );
			Slider[Z_AXIS]		= ADD_SHORT( Slider[Z_AXIS], DirectionalPadY_Slider );
			break;

		case USAGE_SLIDER2_Z_ROTATION:
			Slider[SLIDER2]		= ADD_SHORT( Slider[SLIDER2], DirectionalPadX_Slider );
			Slider[Z_ROTATION]	= ADD_SHORT( Slider[Z_ROTATION], DirectionalPadY_Slider );
			break;

		case USAGE_SLIDER2_SLIDER1:
			Slider[SLIDER2]		= ADD_SHORT( Slider[SLIDER2], DirectionalPadX_Slider );
			Slider[SLIDER1]		= ADD_SHORT( Slider[SLIDER1], DirectionalPadY_Slider );
			break;

		case USAGE_SLIDER2_NONE:
			Slider[SLIDER2]		= ADD_SHORT( Slider[SLIDER2], DirectionalPadX_Slider );
			break;

		case USAGE_NONE_SLIDER2:
			Slider[SLIDER2]		= ADD_SHORT( Slider[SLIDER2], DirectionalPadY_Slider );
			break;

		case USAGE_HAT_SWITCH1:
			HatSwitch[HAT_SWITCH1]	|= DirectionalPad_HatSwitch;
			break;

		case USAGE_HAT_SWITCH2:
			HatSwitch[HAT_SWITCH2]	|= DirectionalPad_HatSwitch;
			break;

		case USAGE_HAT_SWITCH3:
			HatSwitch[HAT_SWITCH3]	|= DirectionalPad_HatSwitch;
			break;

		case USAGE_HAT_SWITCH4:
			HatSwitch[HAT_SWITCH4]	|= DirectionalPad_HatSwitch;
			break;

		case USAGE_4WAY_BUTTON:
			for( WayIndex = INDEX_WAY_UP; WayIndex <= INDEX_WAY_RIGHT; WayIndex ++ )
			{
				//	軸 (プラス)
				if( USAGE_X_AXIS_PLUS <= Setting->DirectionalPadWayUsage[WayIndex]
				&&	Setting->DirectionalPadWayUsage[WayIndex] <= USAGE_Y_ROTATION_PLUS )
				{
					SetAxis(
						 (SHORT)DirectionalPadWay_AxisPlus[WayIndex]
						,Setting->DirectionalPadWayUsage[WayIndex] - USAGE_X_AXIS_PLUS
						,Setting->DirectionalPadWayRapidFire[WayIndex]
						,Setting->DirectionalPadWayRapidFireSpeed[WayIndex]
						,Axis
						,&LastState->DirectionalPadWayLastState[WayIndex]
						,&LastState->DirectionalPadWayRapidFireCount[WayIndex] );
				}
				//	軸 (マイナス)
				if( USAGE_X_AXIS_MINUS <= Setting->DirectionalPadWayUsage[WayIndex]
				&&	Setting->DirectionalPadWayUsage[WayIndex] <= USAGE_Y_ROTATION_MINUS )
				{
					SetAxis(
						 (SHORT)DirectionalPadWay_AxisMinus[WayIndex]
						,Setting->DirectionalPadWayUsage[WayIndex] - USAGE_X_AXIS_MINUS
						,Setting->DirectionalPadWayRapidFire[WayIndex]
						,Setting->DirectionalPadWayRapidFireSpeed[WayIndex]
						,Axis
						,&LastState->DirectionalPadWayLastState[WayIndex]
						,&LastState->DirectionalPadWayRapidFireCount[WayIndex] );
				}
				//	スライダー (プラス)
				if( USAGE_Z_AXIS_PLUS <= Setting->DirectionalPadWayUsage[WayIndex]
				&&	Setting->DirectionalPadWayUsage[WayIndex] <= USAGE_SLIDER2_PLUS )
				{
					SetSlider(
						 (SHORT)DirectionalPadWay_Slider[WayIndex]
						,Setting->DirectionalPadWayUsage[WayIndex] - USAGE_Z_AXIS_PLUS
						,Setting->DirectionalPadWayRapidFire[WayIndex]
						,Setting->DirectionalPadWayRapidFireSpeed[WayIndex]
						,Slider
						,&LastState->DirectionalPadWayLastState[WayIndex]
						,&LastState->DirectionalPadWayRapidFireCount[WayIndex] );
				}
				//	スライダー (マイナス)
				if( USAGE_Z_AXIS_MINUS <= Setting->DirectionalPadWayUsage[WayIndex]
				&&	Setting->DirectionalPadWayUsage[WayIndex] <= USAGE_SLIDER2_MINUS )
				{
					SetSlider(
						 (SHORT)DirectionalPadWay_Slider[WayIndex] * -1
						,Setting->DirectionalPadWayUsage[WayIndex] - USAGE_Z_AXIS_MINUS
						,Setting->DirectionalPadWayRapidFire[WayIndex]
						,Setting->DirectionalPadWayRapidFireSpeed[WayIndex]
						,Slider
						,&LastState->DirectionalPadWayLastState[WayIndex]
						,&LastState->DirectionalPadWayRapidFireCount[WayIndex] );
				}
				//	ボタン
				if( USAGE_BUTTON_1 <= Setting->DirectionalPadWayUsage[WayIndex]
				&&	Setting->DirectionalPadWayUsage[WayIndex] <= USAGE_BUTTON_32 )
				{
					SetButton(
						 DirectionalPadWay_Button[WayIndex]
						,Setting->DirectionalPadWayUsage[WayIndex] - USAGE_BUTTON_1
						,Setting->DirectionalPadWayRapidFire[WayIndex]
						,Setting->DirectionalPadWayRapidFireSpeed[WayIndex]
						,&ButtonState
						,&LastState->DirectionalPadWayLastState[WayIndex]
						,&LastState->DirectionalPadWayRapidFireCount[WayIndex] );
				}
			}
			break;
	}

	//	トリガー
	for( Index = INDEX_LEFT_TRIGGER; Index <= INDEX_RIGHT_TRIGGER; Index ++ )
	{
		//	軸 (プラス)
		if( USAGE_X_AXIS_PLUS <= Setting->TriggerUsage[Index]
		&&	Setting->TriggerUsage[Index] <= USAGE_Y_ROTATION_PLUS )
		{
			SetAxis(
				 (SHORT)Trigger_AxisPlus[Index]
				,Setting->TriggerUsage[Index] - USAGE_X_AXIS_PLUS
				,Setting->TriggerRapidFire[Index]
				,Setting->TriggerRapidFireSpeed[Index]
				,Axis
				,&LastState->TriggerLastState[Index]
				,&LastState->TriggerRapidFireCount[Index] );
		}
		//	軸 (マイナス)
		if( USAGE_X_AXIS_MINUS <= Setting->TriggerUsage[Index]
		&&	Setting->TriggerUsage[Index] <= USAGE_Y_ROTATION_MINUS )
		{
			SetAxis(
				 (SHORT)Trigger_AxisMinus[Index]
				,Setting->TriggerUsage[Index] - USAGE_X_AXIS_MINUS
				,Setting->TriggerRapidFire[Index]
				,Setting->TriggerRapidFireSpeed[Index]
				,Axis
				,&LastState->TriggerLastState[Index]
				,&LastState->TriggerRapidFireCount[Index] );
		}
		//	スライダー (プラス)
		if( USAGE_Z_AXIS_PLUS <= Setting->TriggerUsage[Index]
		&&	Setting->TriggerUsage[Index] <= USAGE_SLIDER2_PLUS )
		{
			SetSlider(
				 (SHORT)Trigger_Slider[Index]
				,Setting->TriggerUsage[Index] - USAGE_Z_AXIS_PLUS
				,Setting->TriggerRapidFire[Index]
				,Setting->TriggerRapidFireSpeed[Index]
				,Slider
				,&LastState->TriggerLastState[Index]
				,&LastState->TriggerRapidFireCount[Index] );
		}
		//	スライダー (マイナス)
		if( USAGE_Z_AXIS_MINUS <= Setting->TriggerUsage[Index]
		&&	Setting->TriggerUsage[Index] <= USAGE_SLIDER2_MINUS )
		{
			SetSlider(
				 (SHORT)Trigger_Slider[Index] * -1
				,Setting->TriggerUsage[Index] - USAGE_Z_AXIS_MINUS
				,Setting->TriggerRapidFire[Index]
				,Setting->TriggerRapidFireSpeed[Index]
				,Slider
				,&LastState->TriggerLastState[Index]
				,&LastState->TriggerRapidFireCount[Index] );
		}
		//	ボタン
		if( USAGE_BUTTON_1 <= Setting->TriggerUsage[Index]
		&&	Setting->TriggerUsage[Index] <= USAGE_BUTTON_32 )
		{
			SetButton(
				 Trigger_Button[Index]
				,Setting->TriggerUsage[Index] - USAGE_BUTTON_1
				,Setting->TriggerRapidFire[Index]
				,Setting->TriggerRapidFireSpeed[Index]
				,&ButtonState
				,&LastState->TriggerLastState[Index]
				,&LastState->TriggerRapidFireCount[Index] );
		}
	}

	//	ボタン
	for( Index = INDEX_A_BUTTON; Index <= INDEX_XBOX_GUIDE_BUTTON; Index ++ )
	{
		//	軸 (プラス)
		if( USAGE_X_AXIS_PLUS <= Setting->ButtonUsage[Index]
		&&	Setting->ButtonUsage[Index] <= USAGE_Y_ROTATION_PLUS )
		{
			SetAxis(
				 (SHORT)Button_AxisPlus[Index]
				,Setting->ButtonUsage[Index] - USAGE_X_AXIS_PLUS
				,Setting->ButtonRapidFire[Index]
				,Setting->ButtonRapidFireSpeed[Index]
				,Axis
				,&LastState->ButtonLastState[Index]
				,&LastState->ButtonRapidFireCount[Index] );
		}
		//	軸 (マイナス)
		if( USAGE_X_AXIS_MINUS <= Setting->ButtonUsage[Index]
		&&	Setting->ButtonUsage[Index] <= USAGE_Y_ROTATION_MINUS )
		{
			SetAxis(
				 (SHORT)Button_AxisMinus[Index]
				,Setting->ButtonUsage[Index] - USAGE_X_AXIS_MINUS
				,Setting->ButtonRapidFire[Index]
				,Setting->ButtonRapidFireSpeed[Index]
				,Axis
				,&LastState->ButtonLastState[Index]
				,&LastState->ButtonRapidFireCount[Index] );
		}
		//	スライダー (プラス)
		if( USAGE_Z_AXIS_PLUS <= Setting->ButtonUsage[Index]
		&&	Setting->ButtonUsage[Index] <= USAGE_SLIDER2_PLUS )
		{
			SetSlider(
				 (SHORT)Button_Slider[Index]
				,Setting->ButtonUsage[Index] - USAGE_Z_AXIS_PLUS
				,Setting->ButtonRapidFire[Index]
				,Setting->ButtonRapidFireSpeed[Index]
				,Slider
				,&LastState->ButtonLastState[Index]
				,&LastState->ButtonRapidFireCount[Index] );
		}
		//	スライダー (マイナス)
		if( USAGE_Z_AXIS_MINUS <= Setting->ButtonUsage[Index]
		&&	Setting->ButtonUsage[Index] <= USAGE_SLIDER2_MINUS )
		{
			SetSlider(
				 (SHORT)Button_Slider[Index] * -1
				,Setting->ButtonUsage[Index] - USAGE_Z_AXIS_MINUS
				,Setting->ButtonRapidFire[Index]
				,Setting->ButtonRapidFireSpeed[Index]
				,Slider
				,&LastState->ButtonLastState[Index]
				,&LastState->ButtonRapidFireCount[Index] );
		}
		//	ボタン
		if( USAGE_BUTTON_1 <= Setting->ButtonUsage[Index]
		&&	Setting->ButtonUsage[Index] <= USAGE_BUTTON_32 )
		{
			SetButton(
				 Button_Button[Index]
				,Setting->ButtonUsage[Index] - USAGE_BUTTON_1
				,Setting->ButtonRapidFire[Index]
				,Setting->ButtonRapidFireSpeed[Index]
				,&ButtonState
				,&LastState->ButtonLastState[Index]
				,&LastState->ButtonRapidFireCount[Index] );
		}
	}

	//	スライダーの値を調整する
	for( Index = Z_AXIS; Index <= SLIDER2; Index ++ )
	{
		//	スライダーの値を中心にスライドする？
		if( Setting->SlideSliderValue[Index] == TRUE )
		{
			if( Setting->ReverseSliderValue[Index] == TRUE )
			{
				if( Setting->SlideSliderCenterValue[Index] == TRUE )
				{
					if( Slider[Index] >= 0 )
					{
						Slider[Index]	= 128 + Slider[Index] / 2;
					} else {
						Slider[Index]	= 128 + Slider[Index] / 2 - 1;
					}
				} else {
					if( Slider[Index] >= 0 )
					{
						Slider[Index]	= 127 + ( Slider[Index] ? Slider[Index] / 2 + 1 : 0 );
					} else {
						Slider[Index]	= 127 + Slider[Index] / 2;
					}
				}
			} else {
				if( Setting->SlideSliderCenterValue[Index] == TRUE )
				{
					if( Slider[Index] >= 0 )
					{
						Slider[Index]	= 127 + ( Slider[Index] ? Slider[Index] / 2 + 1 : 0 );
					} else {
						Slider[Index]	= 127 + Slider[Index] / 2;
					}
				} else {
					if( Slider[Index] >= 0 )
					{
						Slider[Index]	= 128 + Slider[Index] / 2;
					} else {
						Slider[Index]	= 128 + Slider[Index] / 2 - 1;
					}
				}
			}
		} else {
			if( Setting->ReverseSliderValue[Index] == TRUE )
			{
				if( Setting->SlideSliderCenterValue[Index] == FALSE )
				{
					if( Slider[Index] == 128 )
					{
						Slider[Index]	= 127;
					}
				}
			} else {
				if( Setting->SlideSliderCenterValue[Index] == TRUE )
				{
					if( Slider[Index] == 128 )
					{
						Slider[Index]	= 127;
					}
				}
			}
		}

		//	スライダーの値を反転する？
		if( Setting->ReverseSliderValue[Index] == TRUE )
		{
			Slider[Index]	= 255 - Slider[Index];
		}
	}

	//	X 軸 / Y 軸
	Report->XAxis		= Axis[X_AXIS];
	Report->YAxis		= Axis[Y_AXIS];

	//	X 回転 / Y 回転
	Report->XRotation	= Axis[X_ROTATION];
	Report->YRotation	= Axis[Y_ROTATION];

	//	Z 軸 / Z 回転
	Report->ZAxis		= (UCHAR)min( max( Slider[Z_AXIS], 0 ), 255 );
	Report->ZRotation	= (UCHAR)min( max( Slider[Z_ROTATION], 0 ), 255 );

	//	スライダー 1 ～ 2
	Report->Slider1		= (UCHAR)min( max( Slider[SLIDER1], 0 ), 255 );
	Report->Slider2		= (UCHAR)min( max( Slider[SLIDER2], 0 ), 255 );

	//	ハット スイッチ 1 ～ 4
	Report->HatSwitch1	= HatSwitchTable[ HatSwitch[HAT_SWITCH1] ];
	Report->HatSwitch2	= HatSwitchTable[ HatSwitch[HAT_SWITCH2] ];
	Report->HatSwitch3	= HatSwitchTable[ HatSwitch[HAT_SWITCH3] ];
	Report->HatSwitch4	= HatSwitchTable[ HatSwitch[HAT_SWITCH4] ];

	//	ボタン 1 ～ 32
	Report->Buttons		= ButtonState;
}

//----------------------------------------------------------------------------------------------
//	AdjustStickValue
//----------------------------------------------------------------------------------------------
VOID CReport::AdjustStickValue(
	 LONG		ValueX
	,LONG		ValueY
	,BOOLEAN	ReverseX
	,BOOLEAN	ReverseY
	,LONG		DeadZoneType
	,LONG		TransformType
	,SHORT		Angle
	,SHORT *	StickX
	,SHORT *	StickY )
{
	//	変数宣言
	LONGLONG	Length;
	LONG		CurrentAngle;

	//	スティックの左右を反転する
	if( ReverseX == TRUE )
	{
		ValueX	= min( ( ValueX == 32767 ? -32768 : -ValueX ), 32767 );
	}

	//	スティックの上下を反転する
	if( ReverseY == TRUE )
	{
		ValueY	= min( ( ValueY == 32767 ? -32768 : -ValueY ), 32767 );
	}

	//	スティックの可動範囲を調整する
	if( TransformType == TRANSFORM_TYPE_SQUARE )
	{
		//	中心からの距離と角度を求める
		Length			= Sqrt(
							(LONGLONG)ValueX * (LONGLONG)ValueX
							+ (LONGLONG)ValueY * (LONGLONG)ValueY );
		CurrentAngle	= ATan2( ValueY, ValueX );

		//	四角形に合わせた長さに拡張する(９時方向から逆時計回り)
		if( -512 <= CurrentAngle && CurrentAngle <= -385 )
		{
			Length	= Length * 32767 / max( Cos( CurrentAngle - 512 ), 1 );
		}
		if( -384 <= CurrentAngle && CurrentAngle <= -257 )
		{
			Length	= Length * 32767 / max( Sin( CurrentAngle - 512 ), 1 );
		}
		if( -256 <= CurrentAngle && CurrentAngle <= -129 )
		{
			Length	= Length * 32767 / max( Cos( CurrentAngle - 768 ), 1 );
		}
		if( -128 <= CurrentAngle && CurrentAngle <= -1 )
		{
			Length	= Length * 32767 / max( Sin( CurrentAngle - 768 ), 1 );
		}
		if( 0 <= CurrentAngle && CurrentAngle <= 127 )
		{
			Length	= Length * 32767 / max( Cos( CurrentAngle ), 1 );
		}
		if( 128 <= CurrentAngle && CurrentAngle <= 255 )
		{
			Length	= Length * 32767 / max( Sin( CurrentAngle ), 1 );
		}
		if( 256 <= CurrentAngle && CurrentAngle <= 383 )
		{
			Length	= Length * 32767 / max( Cos( CurrentAngle - 256 ), 1 );
		}
		if( 384 <= CurrentAngle && CurrentAngle <= 511 )
		{
			Length	= Length * 32767 / max( Sin( CurrentAngle - 256 ), 1 );
		}

		//	距離と角度から座標を求める
		ValueX	= (LONG)max( -32768, min( 32767, Length * Cos( CurrentAngle ) / 32767 ) );
		if( ValueX == -32767 )
		{
			ValueX	= -32768;
		}
		ValueY	= (LONG)max( -32768, min( 32767, Length * Sin( CurrentAngle ) / 32767 ) );
		if( ValueY == -32767 )
		{
			ValueY	= -32768;
		}
	}
	if( TransformType == TRANSFORM_TYPE_CIRCLE )
	{
		//	中心からの距離と角度を求める
		Length			= Sqrt(
							(LONGLONG)ValueX * (LONGLONG)ValueX
							+ (LONGLONG)ValueY * (LONGLONG)ValueY );
		CurrentAngle	= ATan2( ValueY, ValueX );

		//	円形に合わせた長さに縮小する(９時方向から逆時計回り)
		if( -512 <= CurrentAngle && CurrentAngle <= -385 )
		{
			Length	= Length * max( Cos( CurrentAngle - 512 ), 1 ) / 32767;
		}
		if( -384 <= CurrentAngle && CurrentAngle <= -257 )
		{
			Length	= Length * max( Sin( CurrentAngle - 512 ), 1 ) / 32767;
		}
		if( -256 <= CurrentAngle && CurrentAngle <= -129 )
		{
			Length	= Length * max( Cos( CurrentAngle - 768 ), 1 ) / 32767;
		}
		if( -128 <= CurrentAngle && CurrentAngle <= -1 )
		{
			Length	= Length * max( Sin( CurrentAngle - 768 ), 1 ) / 32767;
		}
		if( 0 <= CurrentAngle && CurrentAngle <= 127 )
		{
			Length	= Length * max( Cos( CurrentAngle ), 1 ) / 32767;
		}
		if( 128 <= CurrentAngle && CurrentAngle <= 255 )
		{
			Length	= Length * max( Sin( CurrentAngle ), 1 ) / 32767;
		}
		if( 256 <= CurrentAngle && CurrentAngle <= 383 )
		{
			Length	= Length * max( Cos( CurrentAngle - 256 ), 1 ) / 32767;
		}
		if( 384 <= CurrentAngle && CurrentAngle <= 511 )
		{
			Length	= Length * max( Sin( CurrentAngle - 256 ), 1 ) / 32767;
		}

		//	距離と角度から座標を求める
		ValueX	= (LONG)max( -32768, min( 32767, Length * Cos( CurrentAngle ) / 32767 ) );
		if( ValueX == -32767 )
		{
			ValueX	= -32768;
		}
		ValueY	= (LONG)max( -32768, min( 32767, Length * Sin( CurrentAngle ) / 32767 ) );
		if( ValueY == -32767 )
		{
			ValueY	= -32768;
		}
	}

	//	スティックを回転する
	if( Angle != 0 )
	{
		//	中心からの距離と角度を求める
		Length			= Sqrt(
							(LONGLONG)ValueX * (LONGLONG)ValueX
							+ (LONGLONG)ValueY * (LONGLONG)ValueY );
		CurrentAngle	= ATan2( ValueY, ValueX ) + Angle;

		//	距離と角度から座標を求める
		ValueX	= (LONG)max( -32768, min( 32767, Length * Cos( CurrentAngle ) / 32767 ) );
		if( ValueX == -32767 )
		{
			ValueX	= -32768;
		}
		ValueY	= (LONG)max( -32768, min( 32767, Length * Sin( CurrentAngle ) / 32767 ) );
		if( ValueY == -32767 )
		{
			ValueY	= -32768;
		}
	}

	*StickX	= (SHORT)ValueX;
	*StickY	= (SHORT)ValueY;
}

//----------------------------------------------------------------------------------------------
//	AdjustStickToAxis
//----------------------------------------------------------------------------------------------
VOID CReport::AdjustStickToAxis(
	 LONG		ValueX
	,LONG		ValueY
	,LONG		DeadZoneType
	,LONG		MinThreshold
	,LONG		MaxThreshold
	,SHORT *	XAxis
	,SHORT *	YAxis )
{
	//	変数宣言
	LONG		Denominator	= ( MaxThreshold - MinThreshold ) * 32767 / 100;
	LONGLONG	Length;
	LONG		Angle;

	switch( DeadZoneType )
	{
		case DEAD_ZONE_TYPE_SQUARE:
			//	上下を反転する
			ValueY	= min( ( ValueY == 32767 ? -32768 : -ValueY ), 32767 );

			//	小さな値は無視する
			if( ValueX > 0 )
			{
				ValueX	= max( 0, ValueX - ( MinThreshold * 32767 / 100 ) );
			}
			if( ValueX < 0 )
			{
				ValueX	= min( 0, ValueX + ( MinThreshold * 32767 / 100 ) );
			}
			if( ValueY > 0 )
			{
				ValueY	= max( 0, ValueY - ( MinThreshold * 32767 / 100 ) );
			}
			if( ValueY < 0 )
			{
				ValueY	= min( 0, ValueY + ( MinThreshold * 32767 / 100 ) );
			}

			//	値を閾値まで大きくする
			if( Denominator != 0 )
			{
				ValueX	= ValueX * 32767 / Denominator;
				ValueY	= ValueY * 32767 / Denominator;
			} else {
				ValueX	= 0;
				ValueY	= 0;
			}

			//	値の範囲を調整する
			ValueX	= max( -32768, min( 32767, ValueX ) );
			ValueY	= max( -32768, min( 32767, ValueY ) );
			break;

		case DEAD_ZONE_TYPE_CIRCLE:
			//	中心からの距離と角度を求める
			Length	= Sqrt(
						(LONGLONG)ValueX * (LONGLONG)ValueX
						+ (LONGLONG)ValueY * (LONGLONG)ValueY );
			Angle	= ATan2( ValueY, ValueX );

			//	小さな値は無視する
			if( Length > 0 )
			{
				Length	= max( 0, Length - ( MinThreshold * 32767 / 100 ) );
			}
			if( Length < 0 )
			{
				Length	= min( 0, Length + ( MinThreshold * 32767 / 100 ) );
			}

			//	値を閾値まで大きくする
			if( Denominator != 0 )
			{
				Length	= Length * 32767 / Denominator;
			} else {
				Length	= 0;
			}

			//	値の範囲を調整する
			Length	= max( -32768, min( 32767, Length ) );

			//	距離と角度から座標を求める
			ValueX	= (LONG)max( -32768, min( 32767, Length * Cos( Angle ) / 32767 ) );
			if( ValueX == -32767 )
			{
				ValueX	= -32768;
			}
			ValueY	= (LONG)max( -32768, min( 32767, Length * -Sin( Angle ) / 32767 ) );
			if( ValueY == -32767 )
			{
				ValueY	= -32768;
			}
			break;
	}

	//	値を返す
	*XAxis	= (SHORT)ValueX;
	*YAxis	= (SHORT)ValueY;
}

//----------------------------------------------------------------------------------------------
//	AdjustStickXToSlider
//----------------------------------------------------------------------------------------------
VOID CReport::AdjustStickXToSlider(
	 LONG		Value
	,UCHAR		Usage
	,BOOLEAN *	SlideSliderValue
	,SHORT *	Slider )
{
	//	変数宣言
	BOOLEAN	Slide	= FALSE;

	//	対象のスライダーが中心を基準にしているか判断する
	switch( Usage )
	{
		case USAGE_Z_AXIS_Z_ROTAION:
		case USAGE_Z_AXIS_SLIDER1:
		case USAGE_Z_AXIS_SLIDER2:
			if( SlideSliderValue[Z_AXIS] == TRUE )
			{
				Slide	= TRUE;
			}
			break;

		case USAGE_Z_ROTATION_Z_AXIS:
		case USAGE_Z_ROTATION_SLIDER1:
		case USAGE_Z_ROTATION_SLIDER2:
			if( SlideSliderValue[Z_ROTATION] == TRUE )
			{
				Slide	= TRUE;
			}
			break;

		case USAGE_SLIDER1_Z_AXIS:
		case USAGE_SLIDER1_Z_ROTATION:
		case USAGE_SLIDER1_SLIDER2:
			if( SlideSliderValue[SLIDER1] == TRUE )
			{
				Slide	= TRUE;
			}
			break;

		case USAGE_SLIDER2_Z_AXIS:
		case USAGE_SLIDER2_Z_ROTATION:
		case USAGE_SLIDER2_SLIDER1:
			if( SlideSliderValue[SLIDER2] == TRUE )
			{
				Slide	= TRUE;
			}
			break;
	}

	//	値を返す
	if( Slide == TRUE )
	{
		if( Value < 0 )
		{
			*Slider	= (SHORT)( ( max( Value, -32767 ) * -1 ) >> 7 );
		} else {
			*Slider	= (SHORT)( Value >> 7 );
		}
	} else {
		*Slider	= (SHORT)( ( Value + 32768 ) >> 8 );
	}
}

//----------------------------------------------------------------------------------------------
//	AdjustStickYToSlider
//----------------------------------------------------------------------------------------------
VOID CReport::AdjustStickYToSlider(
	 LONG		Value
	,UCHAR		Usage
	,BOOLEAN *	SlideSliderValue
	,SHORT *	Slider )
{
	//	変数宣言
	BOOLEAN	Slide	= FALSE;

	//	対象のスライダーが中心を基準にしているか判断する
	switch( Usage )
	{
		case USAGE_Z_ROTATION_Z_AXIS:
		case USAGE_SLIDER1_Z_AXIS:
		case USAGE_SLIDER2_Z_AXIS:
			if( SlideSliderValue[Z_AXIS] == TRUE )
			{
				Slide	= TRUE;
			}
			break;

		case USAGE_Z_AXIS_Z_ROTAION:
		case USAGE_SLIDER1_Z_ROTATION:
		case USAGE_SLIDER2_Z_ROTATION:
			if( SlideSliderValue[Z_ROTATION] == TRUE )
			{
				Slide	= TRUE;
			}
			break;

		case USAGE_Z_AXIS_SLIDER1:
		case USAGE_Z_ROTATION_SLIDER1:
		case USAGE_SLIDER2_SLIDER1:
			if( SlideSliderValue[SLIDER1] == TRUE )
			{
				Slide	= TRUE;
			}
			break;

		case USAGE_Z_AXIS_SLIDER2:
		case USAGE_Z_ROTATION_SLIDER2:
		case USAGE_SLIDER1_SLIDER2:
			if( SlideSliderValue[SLIDER2] == TRUE )
			{
				Slide	= TRUE;
			}
			break;
	}

	//	値を返す
	if( Slide == TRUE )
	{
		if( Value < 0 )
		{
			*Slider	= (SHORT)( ( max( Value, -32767 ) * -1 ) >> 7 );
		} else {
			*Slider	= (SHORT)( Value >> 7 );
		}
	} else {
		*Slider	= (SHORT)( ( Value + 32768 ) >> 8 );
	}
}

//----------------------------------------------------------------------------------------------
//	AdjustStickToHatSwitch
//----------------------------------------------------------------------------------------------
VOID CReport::AdjustStickToHatSwitch(
	 LONG		ValueX
	,LONG		ValueY
	,LONG		Threshold
	,UCHAR *	HatSwitch )
{
	//	変数宣言
	LONG	Denominator	= Threshold * 32767 / 100;

	//	ニュートラル
	*HatSwitch	= 0;

	//	上
	if( Denominator < ValueY  )
	{
		*HatSwitch	|= DIRECTIONAL_PAD_UP;
	}
	//	下
	if( ValueY < ( -Denominator - 1 ) )
	{
		*HatSwitch	|= DIRECTIONAL_PAD_DOWN;
	}
	//	左
	if( ValueX < ( -Denominator - 1 ) )
	{
		*HatSwitch	|= DIRECTIONAL_PAD_LEFT;
	}
	//	右
	if( Denominator < ValueX  )
	{
		*HatSwitch	|= DIRECTIONAL_PAD_RIGHT;
	}
}

//----------------------------------------------------------------------------------------------
//	AdjustTriggerToSlider
//----------------------------------------------------------------------------------------------
VOID CReport::AdjustTriggerToSlider(
	 LONG		Value
	,LONG		MinThreshold
	,LONG		MaxThreshold
	,UCHAR *	Slider )
{
	//	変数宣言
	LONG	Denominator	= ( MaxThreshold - MinThreshold ) * 255 / 100;

	//	小さな値は無視する
	Value	= max( 0, Value - MinThreshold * 255 / 100 );

	//	値を閾値まで大きくする
	if( Denominator != 0 )
	{
		Value	= Value * 255 / Denominator;
	} else {
		Value	= 0;
	}

	//	値の範囲を調整する
	Value	= min( 255, max( 0, Value ) );

	//	値を返す
	*Slider	= (UCHAR)Value;
}

//----------------------------------------------------------------------------------------------
//	AdjustTriggerToButton
//----------------------------------------------------------------------------------------------
VOID CReport::AdjustTriggerToButton(
	 UCHAR		Value
	,LONG		Threshold
	,BOOLEAN *	Button )
{
	//	変数宣言
	LONG	Denominator	= Threshold * 255 / 100;

	//	閾値を超えている場合、ボタン押下にする
	if( Denominator < Value  )
	{
		*Button	= TRUE;
	} else {
		*Button	= FALSE;
	}
}

//----------------------------------------------------------------------------------------------
//	SetAxis
//----------------------------------------------------------------------------------------------
VOID CReport::SetAxis(
	 SHORT		Value
	,UCHAR		Usage
	,BOOLEAN	RapidFire
	,UCHAR		RapidFireRate
	,SHORT *	Axis
	,BOOLEAN *	LastState
	,UCHAR *	RapidFireCount )
{
	//	軸の値を調整する
	if( RapidFire == FALSE || *LastState == FALSE )
	{
		if( Value != 0 )
		{
			if( RapidFire == FALSE )
			{
				Axis[Usage]	= ADD_SHORT( Axis[Usage], Value );
				*LastState	= TRUE;
			} else {
				if( *RapidFireCount >= 100 )
				{
					Axis[Usage]		= ADD_SHORT( Axis[Usage], Value );
					*LastState		= TRUE;
					*RapidFireCount	= *RapidFireCount - 100;
				}
				*RapidFireCount	= *RapidFireCount + RapidFireRate;
			}
		} else {
			*RapidFireCount	= 100;
			*LastState		= FALSE;
		}
	} else {
		*LastState	= FALSE;
	}
}

//----------------------------------------------------------------------------------------------
//	SetSlider
//----------------------------------------------------------------------------------------------
VOID CReport::SetSlider(
	 SHORT		Value
	,UCHAR		Usage
	,BOOLEAN	RapidFire
	,UCHAR		RapidFireRate
	,SHORT *	Slider
	,BOOLEAN *	LastState
	,UCHAR *	RapidFireCount )
{
	//	スライダーの値を調整する
	if( RapidFire == FALSE || *LastState == FALSE )
	{
		if( Value != 0 )
		{
			if( RapidFire == FALSE )
			{
				Slider[Usage]	= Slider[Usage] + Value;
				*LastState		= TRUE;
			} else {
				if( *RapidFireCount >= 100 )
				{
					Slider[Usage]	= Slider[Usage] + Value;
					*LastState		= TRUE;
					*RapidFireCount	= *RapidFireCount - 100;
				}
				*RapidFireCount	= *RapidFireCount + RapidFireRate;
			}
		} else {
			*RapidFireCount	= 100;
			*LastState		= FALSE;
		}
	} else {
		*LastState	= FALSE;
	}
}

//----------------------------------------------------------------------------------------------
//	SetButton
//----------------------------------------------------------------------------------------------
VOID CReport::SetButton(
	 BOOLEAN	Value
	,UCHAR		Usage
	,BOOLEAN	RapidFire
	,UCHAR		RapidFireRate
	,ULONG *	Button
	,BOOLEAN *	LastState
	,UCHAR *	RapidFireCount )
{
	//	変数宣言
	ULONG	ButtonTable[]	= {	 0x00000001		//	ボタン 1
								,0x00000002		//	ボタン 2
								,0x00000004		//	ボタン 3
								,0x00000008		//	ボタン 4
								,0x00000010		//	ボタン 5
								,0x00000020		//	ボタン 6
								,0x00000040		//	ボタン 7
								,0x00000080		//	ボタン 8
								,0x00000100		//	ボタン 9
								,0x00000200		//	ボタン 10
								,0x00000400		//	ボタン 11
								,0x00000800		//	ボタン 12
								,0x00001000		//	ボタン 13
								,0x00002000		//	ボタン 14
								,0x00004000		//	ボタン 15
								,0x00008000		//	ボタン 16
								,0x00010000		//	ボタン 17
								,0x00020000		//	ボタン 18
								,0x00040000		//	ボタン 19
								,0x00080000		//	ボタン 20
								,0x00100000		//	ボタン 21
								,0x00200000		//	ボタン 22
								,0x00400000		//	ボタン 23
								,0x00800000		//	ボタン 24
								,0x01000000		//	ボタン 25
								,0x02000000		//	ボタン 26
								,0x04000000		//	ボタン 27
								,0x08000000		//	ボタン 28
								,0x10000000		//	ボタン 29
								,0x20000000		//	ボタン 30
								,0x40000000		//	ボタン 31
								,0x80000000 };	//	ボタン 32

	//	ボタンの値を調整する
	if( RapidFire == FALSE || *LastState == FALSE )
	{
		if( Value == TRUE )
		{
			if( RapidFire == FALSE )
			{
				*Button		|= ButtonTable[Usage];
				*LastState	= TRUE;
			} else {
				if( *RapidFireCount >= 100 )
				{
					*Button			|= ButtonTable[Usage];
					*LastState		= TRUE;
					*RapidFireCount	= *RapidFireCount - 100;
				}
				*RapidFireCount	= *RapidFireCount + RapidFireRate;
			}
		} else {
			*RapidFireCount	= 100;
			*LastState		= FALSE;
		}
	} else {
		*LastState	= FALSE;
	}
}

//----------------------------------------------------------------------------------------------
//	GenerateOutputReport
//----------------------------------------------------------------------------------------------
VOID CReport::GenerateOutputReport(
	 HID_WRITE_REPORT *	Report
	,SETTING *			Setting
	,OUTPUT_REPORT *	OutputReport )
{
	//	フォース フィードバック
	switch( Report->Type )
	{
		//	通常
		case REPORT_TYPE_NORMAL:
			if( Setting->Actuator == TRUE )
			{
				//	左アクチュエータ
				SetActuator(
					 Report->Left
					,Setting->ActuatorLevel[INDEX_LEFT_ACTUATOR]
					,Setting->ActuatorMinThreshold
					,Setting->ActuatorMaxThreshold
					,&OutputReport->Left );
				//	右アクチュエータ
				SetActuator(
					 Report->Right
					,Setting->ActuatorLevel[INDEX_RIGHT_ACTUATOR]
					,Setting->ActuatorMinThreshold
					,Setting->ActuatorMaxThreshold
					,&OutputReport->Right );
			} else {
				OutputReport->Left	= 0;
				OutputReport->Right	= 0;
			}
			break;

		//	調整なし
		case REPORT_TYPE_DIRECT:
			OutputReport->Left	= Report->Left;
			OutputReport->Right	= Report->Right;
			break;
	}
}

//----------------------------------------------------------------------------------------------
//	SetActuator
//----------------------------------------------------------------------------------------------
VOID CReport::SetActuator(
	 LONG		Value
	,LONG		Level
	,LONG		MinThreshold
	,LONG		MaxThreshold
	,UCHAR *	Actuator )
{
	//	変数宣言
	LONG	MaxLevel	= Level * 255 / 100;
	LONG	Denominator	= MaxThreshold * 255 / 100;

	//	値の強さを調整する
	Value	= Value * Level / 100;

	//	小さな値は無視する
	if( Value <= MinThreshold * MaxLevel / 100 )
	{
		Value	= 0;
	}

	//	値を閾値まで大きくする
	if( Denominator != 0 )
	{
		Value	= Value * 255 / Denominator;
	} else {
		Value	= 0;
	}

	//	値の範囲を調整する
	Value	= max( 0, min( MaxLevel, Value ) );

	//	値を返す
	*Actuator	= (UCHAR)Value;
}
