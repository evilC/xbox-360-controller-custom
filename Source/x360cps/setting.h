
//	Property Sheet for Microsoft Xbox 360 Controller

#pragma	once

#include	<windows.h>
#include	<stdio.h>

//----------------------------------------------------------------------------------------------
//	定数
//----------------------------------------------------------------------------------------------

#define	REGISTRY_KEY				L"SYSTEM\\CurrentControlSet\\Services\\Xbox360Controller\\Parameters"

#define	CURRENT_SETTING				L"Current Setting"
#define	CURRENT_SETTING_INDEX		L"Current Setting Index"

#define	SETTING_COUNT				L"Setting Count"
#define	SETTING_DATA				L"Setting %d"
#define	SETTING_NAME				L"Setting Name %d"

#define	MODIFIED_FLAG				L"Modified Flag"

#define	AUTO_SETTING_CHANGE			L"Auto Setting Change"

#define	LIMITATION_NONE				0
#define	LIMITATION_H1B16			1
#define	LIMITATION_NO_RX_RY			2
#define	LIMITATION_NO_SLIDER		3

#define	Z_AXIS						0
#define	Z_ROTATION					1
#define	SLIDER1						2
#define	SLIDER2						3

#define	INDEX_LEFT_STICK			0
#define	INDEX_RIGHT_STICK			1

#define	INDEX_WAY_UP				0
#define	INDEX_WAY_DOWN				1
#define	INDEX_WAY_LEFT				2
#define	INDEX_WAY_RIGHT				3

#define	INDEX_LEFT_TRIGGER			0
#define	INDEX_RIGHT_TRIGGER			1

#define	INDEX_A_BUTTON				0
#define	INDEX_B_BUTTON				1
#define	INDEX_X_BUTTON				2
#define	INDEX_Y_BUTTON				3
#define	INDEX_LB_BUTTON				4
#define	INDEX_RB_BUTTON				5
#define	INDEX_START_BUTTON			6
#define	INDEX_BACK_BUTTON			7
#define	INDEX_LEFT_STICK_BUTTON		8
#define	INDEX_RIGHT_STICK_BUTTON	9
#define	INDEX_XBOX_GUIDE_BUTTON		10

#define	INDEX_LEFT_ACTUATOR			0
#define	INDEX_RIGHT_ACTUATOR		1

#define	USAGE_NONE					0
#define	USAGE_X_AXIS_Y_AXIS			1
#define	USAGE_X_AXIS_NONE			2
#define	USAGE_NONE_X_AXIS			3
#define	USAGE_Y_AXIS_X_AXIS			4
#define	USAGE_Y_AXIS_NONE			5
#define	USAGE_NONE_Y_AXIS			6
#define	USAGE_X_ROTATION_Y_ROTATION	7
#define	USAGE_X_ROTATION_NONE		8
#define	USAGE_NONE_X_ROTATION		9
#define	USAGE_Y_ROTATION_X_ROTATION	10
#define	USAGE_Y_ROTATION_NONE		11
#define	USAGE_NONE_Y_ROTATION		12
#define	USAGE_Z_AXIS_Z_ROTAION		13
#define	USAGE_Z_AXIS_SLIDER1		14
#define	USAGE_Z_AXIS_SLIDER2		15
#define	USAGE_Z_AXIS_NONE			16
#define	USAGE_NONE_Z_AXIS			17
#define	USAGE_Z_ROTATION_Z_AXIS		18
#define	USAGE_Z_ROTATION_SLIDER1	19
#define	USAGE_Z_ROTATION_SLIDER2	20
#define	USAGE_Z_ROTATION_NONE		21
#define	USAGE_NONE_Z_ROTATION		22
#define	USAGE_SLIDER1_Z_AXIS		23
#define	USAGE_SLIDER1_Z_ROTATION	24
#define	USAGE_SLIDER1_SLIDER2		25
#define	USAGE_SLIDER1_NONE			26
#define	USAGE_NONE_SLIDER1			27
#define	USAGE_SLIDER2_Z_AXIS		28
#define	USAGE_SLIDER2_Z_ROTATION	29
#define	USAGE_SLIDER2_SLIDER1		30
#define	USAGE_SLIDER2_NONE			31
#define	USAGE_NONE_SLIDER2			32
#define	USAGE_HAT_SWITCH1			33
#define	USAGE_HAT_SWITCH2			34
#define	USAGE_HAT_SWITCH3			35
#define	USAGE_HAT_SWITCH4			36
#define	USAGE_4WAY_BUTTON			37

#define	USAGE_X_AXIS_PLUS			1
#define	USAGE_Y_AXIS_PLUS			2
#define	USAGE_X_ROTATION_PLUS		3
#define	USAGE_Y_ROTATION_PLUS		4
#define	USAGE_X_AXIS_MINUS			5
#define	USAGE_Y_AXIS_MINUS			6
#define	USAGE_X_ROTATION_MINUS		7
#define	USAGE_Y_ROTATION_MINUS		8
#define	USAGE_Z_AXIS_PLUS			9
#define	USAGE_Z_ROTATION_PLUS		10
#define	USAGE_SLIDER1_PLUS			11
#define	USAGE_SLIDER2_PLUS			12
#define	USAGE_Z_AXIS_MINUS			13
#define	USAGE_Z_ROTATION_MINUS		14
#define	USAGE_SLIDER1_MINUS			15
#define	USAGE_SLIDER2_MINUS			16
#define	USAGE_BUTTON_1				17
#define	USAGE_BUTTON_2				18
#define	USAGE_BUTTON_3				19
#define	USAGE_BUTTON_4				20
#define	USAGE_BUTTON_5				21
#define	USAGE_BUTTON_6				22
#define	USAGE_BUTTON_7				23
#define	USAGE_BUTTON_8				24
#define	USAGE_BUTTON_9				25
#define	USAGE_BUTTON_10				26
#define	USAGE_BUTTON_11				27
#define	USAGE_BUTTON_12				28
#define	USAGE_BUTTON_13				29
#define	USAGE_BUTTON_14				30
#define	USAGE_BUTTON_15				31
#define	USAGE_BUTTON_16				32
#define	USAGE_BUTTON_17				33
#define	USAGE_BUTTON_18				34
#define	USAGE_BUTTON_19				35
#define	USAGE_BUTTON_20				36
#define	USAGE_BUTTON_21				37
#define	USAGE_BUTTON_22				38
#define	USAGE_BUTTON_23				39
#define	USAGE_BUTTON_24				40
#define	USAGE_BUTTON_25				41
#define	USAGE_BUTTON_26				42
#define	USAGE_BUTTON_27				43
#define	USAGE_BUTTON_28				44
#define	USAGE_BUTTON_29				45
#define	USAGE_BUTTON_30				46
#define	USAGE_BUTTON_31				47
#define	USAGE_BUTTON_32				48

#define	DEAD_ZONE_TYPE_SQUARE		0
#define	DEAD_ZONE_TYPE_CIRCLE		1

#define	TRANSFORM_TYPE_NONE			0
#define	TRANSFORM_TYPE_SQUARE		1
#define	TRANSFORM_TYPE_CIRCLE		2

//----------------------------------------------------------------------------------------------
//	構造体
//----------------------------------------------------------------------------------------------

typedef struct _SETTING
{
	UCHAR	StickUsage[2];
	BOOLEAN	StickReverseX[2];
	BOOLEAN	StickReverseY[2];
	SHORT	StickAngle[2];
	UCHAR	StickDeadZoneType[2];
	UCHAR	StickMinThreshold[2];
	UCHAR	StickMaxThreshold[2];
	UCHAR	StickHatSwitchThreshold[2];
	UCHAR	StickWayUsage[2][4];
	BOOLEAN	StickWayRapidFire[2][4];
	UCHAR	StickWayRapidFireSpeed[2][4];

	UCHAR	DirectionalPadUsage;
	BOOLEAN	DirectionalPadReverseX;
	BOOLEAN	DirectionalPadReverseY;
	SHORT	DirectionalPadAngle;
	UCHAR	DirectionalPadWayUsage[4];
	BOOLEAN	DirectionalPadWayRapidFire[4];
	UCHAR	DirectionalPadWayRapidFireSpeed[4];

	UCHAR	TriggerUsage[2];
	BOOLEAN	TriggerRapidFire[2];
	UCHAR	TriggerRapidFireSpeed[2];
	UCHAR	TriggerMinThreshold[2];
	UCHAR	TriggerMaxThreshold[2];
	UCHAR	TriggerButtonThreshold[2];

	UCHAR	ButtonUsage[11];
	BOOLEAN	ButtonRapidFire[11];
	UCHAR	ButtonRapidFireSpeed[11];

	UCHAR	RingLightPattern;

	BOOLEAN	Actuator;
	UCHAR	ActuatorLevel[2];
	UCHAR	ActuatorMinThreshold;
	UCHAR	ActuatorMaxThreshold;

	BOOLEAN	SlideSliderValue[4];
	BOOLEAN	SlideSliderCenterValue[4];
	BOOLEAN	ReverseSliderValue[4];
	BOOLEAN	DevicesArePolled;
	UCHAR	LimitationMode;

	UCHAR	StickTransformType[2];
	UCHAR	DirectionalPadTransformType;

} SETTING, *PSETTING;

//----------------------------------------------------------------------------------------------
//	クラス
//----------------------------------------------------------------------------------------------

class CSetting
{

public:

	//	コンストラクタ
	CSetting( HINSTANCE, HWND );
	//	デストラクタ
	~CSetting();

	//	レジストリから設定を読み込む
	VOID LoadRegistry();
	//	レジストリへ設定を書き込む
	VOID SaveRegistry();

	//	現在の設定を変更する
	VOID Change( LONG, BOOL );
	//	現在の設定を名前を付けて保存する
	VOID SaveAs( WCHAR * );
	//	現在の設定を保存する
	VOID Save( LONG );
	//	現在の設定を削除する
	VOID Delete();

	//	現在のコントローラーの設定
	SETTING		CurrentSetting;
	//	現在のコントローラーの設定の順番
	LONG		CurrentSettingIndex;

	//	コントローラーの設定数
	LONG		SettingCount;
	//	コントローラーの設定
	SETTING *	Setting;
	//	コントローラーの設定の名称
	WCHAR * *	SettingName;

	//	変更済みフラグ
	BOOLEAN		ModifiedFlag;

	//	設定の自動切換え
	BOOLEAN		AutoSettingChange;

private:

	//	メッセージ ボックスを表示する
	LONG ShowMessageBox( HWND, ULONG, ULONG, ULONG );

	//	コントローラーの設定を取得する
	VOID GetSetting( LONG, SETTING * );
	//	コントローラーの設定を取得する
	VOID SetSetting( LONG, SETTING * );
	//	コントローラーの設定を削除する
	VOID DelSetting( LONG );

	//	コントローラーの設定の名称を取得する
	VOID GetSettingName( LONG, WCHAR *, LONG );
	//	コントローラーの設定の名称を取得する
	VOID SetSettingName( LONG, WCHAR * );
	//	コントローラーの設定の名称を削除する
	VOID DelSettingName( LONG );

	//	レジストリから値を取得する
	VOID GetRegistry( VOID *, ULONG, ULONG, WCHAR * );
	//	レジストリに値を設定する
	VOID SetRegistry( VOID *, ULONG, ULONG, WCHAR * );
	//	レジストリの値を削除する
	VOID DelRegistry( WCHAR * );

	//	標準の設定を取得する
	VOID GetDefaultSetting( SETTING * );

	//	インスタンス
	HINSTANCE	Instance;
	//	ウィンドウ ハンドル
	HWND		Wnd;

protected:

};
