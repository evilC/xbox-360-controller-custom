
//	Force Feedback Driver for Microsoft Xbox 360 Controller

#pragma	once

#ifndef	DIRECTINPUT_VERSION
#define	DIRECTINPUT_VERSION	0x0800
#endif

#include	<windows.h>
#include	<dinput.h>
#include	<math.h>

//----------------------------------------------------------------------------------------------
//	定数
//----------------------------------------------------------------------------------------------

#define	CONSTANT_FORCE	0x00
#define	RAMP_FORCE		0x01
#define	SQUARE			0x02
#define	SINE			0x03
#define	TRIANGLE		0x04
#define	SAWTOOTH_UP		0x05
#define	SAWTOOTH_DOWN	0x06
#define	SPRING			0x07
#define	DAMPER			0x08
#define	INERTIA			0x09
#define	FRICTION		0x0A
#define	CUSTOM_FORCE	0x0B

//----------------------------------------------------------------------------------------------
//	クラス
//----------------------------------------------------------------------------------------------

class CEffect
{

public:

	//	コンストラクタ
	CEffect();

	//	エフェクトを計算する
	VOID Calc( LONG *, LONG * );

	//	種類
	DWORD			Type;
	//	DIEFFECT 構造体
	DIEFFECT		DiEffect;
	//	DIENVELOPE 構造体
	DIENVELOPE		DiEnvelope;
	//	DICONSTANTFORCE 構造体
	DICONSTANTFORCE	DiConstantForce;
	//	DIPERIODIC 構造体
	DIPERIODIC		DiPeriodic;
	//	DIRAMPFORCE 構造体
	DIRAMPFORCE		DiRampforce;

	//	ハンドル
	DWORD			Handle;

	//	状態
	DWORD			Status;
	//	再生回数
	DWORD			PlayCount;
	//	再生時刻
	DWORD			StartTime;

private:

	//	エンベロープを計算する
	VOID CalcEnvelope( ULONG, ULONG, LONG *, LONG *, LONG * );
	//	フォースを計算する
	VOID CalcForce( ULONG, ULONG, LONG, LONG, LONG, LONG * );

protected:

};
