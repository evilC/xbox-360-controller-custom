
//	Property Sheet for Microsoft Xbox 360 Controller

#pragma	once

#include	<windows.h>

#include	"setting.h"

//----------------------------------------------------------------------------------------------
//	定数
//----------------------------------------------------------------------------------------------

#define	DIRECTIONAL_PAD_UP			0x0001
#define	DIRECTIONAL_PAD_DOWN		0x0002
#define	DIRECTIONAL_PAD_LEFT		0x0004
#define	DIRECTIONAL_PAD_RIGHT		0x0008
#define	START_BUTTON				0x0010
#define	BACK_BUTTON					0x0020
#define	LEFT_STICK_BUTTON			0x0040
#define	RIGHT_STICK_BUTTON			0x0080
#define	LB_BUTTON					0x0100
#define	RB_BUTTON					0x0200
#define	XBOX_GUIDE_BUTTON			0x0400
#define	A_BUTTON					0x1000
#define	B_BUTTON					0x2000
#define	X_BUTTON					0x4000
#define	Y_BUTTON					0x8000

#define	X_AXIS						0
#define	Y_AXIS						1
#define	X_ROTATION					2
#define	Y_ROTATION					3

#define	HAT_SWITCH1					0
#define	HAT_SWITCH2					1
#define	HAT_SWITCH3					2
#define	HAT_SWITCH4					3

#define	REPORT_TYPE_NORMAL			0
#define	REPORT_TYPE_DIRECT			1

#define	ADD_SHORT( D0, D1 )( (SHORT)max( -32768, min( 32767, (LONG)( D0 ) + (LONG)( D1 ) ) ) )

//----------------------------------------------------------------------------------------------
//	構造体
//----------------------------------------------------------------------------------------------

//	入力用レポート
typedef struct _INPUT_REPORT
{
	UCHAR	Zero1;
	UCHAR	Length;
	USHORT	DigitalButton;
	UCHAR	LeftTrigger;
	UCHAR	RightTrigger;
	SHORT	LeftStickX;
	SHORT	LeftStickY;
	SHORT	RightStickX;
	SHORT	RightStickY;
	UCHAR	Zero2;
	UCHAR	Zero3;
	UCHAR	Zero4;
	UCHAR	Zero5;
	UCHAR	Zero6;
	UCHAR	Zero7;
} INPUT_REPORT, *PINPUT_REPORT;

//	出力用レポート
typedef struct _OUTPUT_REPORT
{
	UCHAR	Type;
	UCHAR	Length;
	UCHAR	Pattern;
	UCHAR	Left;
	UCHAR	Right;
	UCHAR	Zero1;
	UCHAR	Zero2;
	UCHAR	Zero3;
} OUTPUT_REPORT, *POUTPUT_REPORT;

//	HID 読込用レポート
typedef struct _HID_READ_REPORT
{
	SHORT	XAxis;
	SHORT	YAxis;
	SHORT	XRotation;
	SHORT	YRotation;
	UCHAR	ZAxis;
	UCHAR	ZRotation;
	UCHAR	Slider2;
	UCHAR	Slider1;
	CHAR	HatSwitch1;
	CHAR	HatSwitch2;
	CHAR	HatSwitch3;
	CHAR	HatSwitch4;
	ULONG	Buttons;
} HID_READ_REPORT, *PHID_READ_REPORT;

//	HID 書込用レポート
typedef struct _HID_WRITE_REPORT
{
	UCHAR	Type;
	UCHAR	Left;
	UCHAR	Right;
} HID_WRITE_REPORT, *PHID_WRITE_REPORT;

//	最後の状態
typedef struct _LAST_STATE
{
	BOOLEAN	StickWayLastState[2][4];
	UCHAR	StickWayRapidFireCount[2][4];
	BOOLEAN	DirectionalPadWayLastState[4];
	UCHAR	DirectionalPadWayRapidFireCount[4];
	BOOLEAN	TriggerLastState[2];
	UCHAR	TriggerRapidFireCount[2];
	BOOLEAN	ButtonLastState[11];
	UCHAR	ButtonRapidFireCount[11];
} LAST_STATE, *PLAST_STATE;

//----------------------------------------------------------------------------------------------
//	クラス
//----------------------------------------------------------------------------------------------

class CReport
{

public:

	//	HID 読込用レポートを作成する
	VOID GenerateReadReport( INPUT_REPORT *, SETTING *, HID_READ_REPORT *, LAST_STATE * );
	VOID AdjustStickValue( LONG, LONG, BOOLEAN, BOOLEAN, LONG, LONG, SHORT, SHORT *, SHORT * );
	VOID AdjustStickToAxis( LONG, LONG, LONG, LONG, LONG, SHORT *, SHORT * );
	VOID AdjustStickXToSlider( LONG, UCHAR, BOOLEAN *, SHORT * );
	VOID AdjustStickYToSlider( LONG, UCHAR, BOOLEAN *, SHORT * );
	VOID AdjustStickToHatSwitch( LONG, LONG, LONG, UCHAR * );
	VOID AdjustTriggerToSlider( LONG, LONG, LONG, UCHAR * );
	VOID AdjustTriggerToButton( UCHAR, LONG, BOOLEAN * );
	VOID SetAxis( SHORT, UCHAR, BOOLEAN, UCHAR, SHORT *, BOOLEAN *, UCHAR * );
	VOID SetSlider( SHORT, UCHAR, BOOLEAN, UCHAR, SHORT *, BOOLEAN *, UCHAR * );
	VOID SetButton( BOOLEAN, UCHAR, BOOLEAN, UCHAR, ULONG *, BOOLEAN *, UCHAR * );

	//	出力用レポートを作成する
	VOID GenerateOutputReport( HID_WRITE_REPORT *, SETTING *, POUTPUT_REPORT );
	VOID SetActuator( LONG, LONG, LONG, LONG, UCHAR * );

private:

protected:

};
