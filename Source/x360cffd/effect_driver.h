
//	Force Feedback Driver for Microsoft Xbox 360 Controller

#pragma	once

#ifndef	DIRECTINPUT_VERSION
#define	DIRECTINPUT_VERSION	0x0800
#endif

#include	<windows.h>
#include	<objbase.h>
#include	<initguid.h>
#include	<dinput.h>
#include	<dinputd.h>

#include	"extension.h"
#include	"effect.h"

//----------------------------------------------------------------------------------------------
//	定数
//----------------------------------------------------------------------------------------------

#define	REPORT_TYPE_NORMAL	0
#define	REPORT_TYPE_DIRECT	1

//----------------------------------------------------------------------------------------------
//	構造体
//----------------------------------------------------------------------------------------------

typedef struct _HID_WRITE_REPORT_ID
{
	UCHAR	Id;
	UCHAR	Type;
	UCHAR	Left;
	UCHAR	Right;
} HID_WRITE_REPORT_ID, *PHID_WRITE_REPORT_ID;

//----------------------------------------------------------------------------------------------
//	クラス
//----------------------------------------------------------------------------------------------

class CEffectDriver : public IDirectInputEffectDriver
{

public:

	//	コンストラクタ
	CEffectDriver( VOID );

	//	IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface( REFIID, PVOID * );
	ULONG STDMETHODCALLTYPE AddRef( VOID );
	ULONG STDMETHODCALLTYPE Release( VOID );

	//	IDirectInputEffectDriver
	HRESULT STDMETHODCALLTYPE DeviceID( DWORD, DWORD, DWORD, DWORD, LPVOID );
	HRESULT STDMETHODCALLTYPE GetVersions( LPDIDRIVERVERSIONS );
	HRESULT STDMETHODCALLTYPE Escape( DWORD, DWORD, LPDIEFFESCAPE );
	HRESULT STDMETHODCALLTYPE SetGain( DWORD, DWORD );
	HRESULT STDMETHODCALLTYPE SendForceFeedbackCommand( DWORD, DWORD );
	HRESULT STDMETHODCALLTYPE GetForceFeedbackState( DWORD, LPDIDEVICESTATE );
	HRESULT STDMETHODCALLTYPE DownloadEffect( DWORD, DWORD, LPDWORD, LPCDIEFFECT, DWORD );
	HRESULT STDMETHODCALLTYPE DestroyEffect( DWORD, DWORD );
	HRESULT STDMETHODCALLTYPE StartEffect( DWORD, DWORD, DWORD, DWORD );
	HRESULT STDMETHODCALLTYPE StopEffect( DWORD, DWORD );
	HRESULT STDMETHODCALLTYPE GetEffectStatus( DWORD, DWORD, LPDWORD );

	//	デバイス インターフェイス
	HANDLE		Device;

	//	エフェクト数
	LONG		EffectCount;
	//	エフェクト リスト
	CEffect * *	EffectList;

	//	ゲイン
	DWORD		Gain;
	//	アクチュエータ
	BOOL		Actuator;

	//	終了シグナル
	BOOL		Quit;

private:

	//	参照カウンタ
	LONG	ReferenceCount;

	//	ワーカ スレッド
	HANDLE	WorkerThread;

	//	エフェクト番号
	LONG	EffectIndex;

	//	停止
	BOOL	Stopped;
	//	一時停止
	BOOL	Paused;
	//	一時停止した時刻
	LONG	PausedTime;

protected:

};

//----------------------------------------------------------------------------------------------
//	関数
//----------------------------------------------------------------------------------------------

STDAPI_(DWORD) WINAPI EffectProc( LPVOID );

