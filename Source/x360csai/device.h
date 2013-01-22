
//	Status Area Icon for Microsoft Xbox 360 Controller

#pragma	once

#include	<windows.h>
#include	<stdio.h>
#include	<setupapi.h>

extern "C"
{
#include	<hidsdi.h>
}

//----------------------------------------------------------------------------------------------
//	マクロ定義
//----------------------------------------------------------------------------------------------

#define	ADD_UCHAR( D0, D1 )( (CHAR)max( 0, min( 255, (LONG)( D0 ) + (LONG)( D1 ) ) ) )
#define	ADD_SHORT( D0, D1 )( (SHORT)max( -32768, min( 32767, (LONG)( D0 ) + (LONG)( D1 ) ) ) )

//----------------------------------------------------------------------------------------------
//	定数
//----------------------------------------------------------------------------------------------

#define	REGISTRY_KEY				L"SYSTEM\\CurrentControlSet\\Services\\Xbox360Controller\\Parameters"

#define	SUPPORTED_DEVICE_COUNT		L"Supported Device Count"
#define	SUPPORTED_DEVICE_ATTRIBUTE	L"Supported Device Attribute %d"

#define	REPORT_TYPE_NORMAL			0
#define	REPORT_TYPE_DIRECT			1

//----------------------------------------------------------------------------------------------
//	構造体
//----------------------------------------------------------------------------------------------

//	HID 書込用レポート
typedef struct _HID_WRITE_REPORT_ID
{
	UCHAR	Id;
	UCHAR	Type;
	UCHAR	Left;
	UCHAR	Right;
} HID_WRITE_REPORT_ID, *PHID_WRITE_REPORT_ID;

//	フューチャー用レポート
typedef struct _FEATURE_REPORT_ID
{
	UCHAR	Id;
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
} FEATURE_REPORT_ID, *PFEATURE_REPORT_ID;

//----------------------------------------------------------------------------------------------
//	クラス
//----------------------------------------------------------------------------------------------

class CDevice
{

public:

	//	コンストラクタ
	CDevice();
	//	デストラクタ
	~CDevice();

	//	デバイスが接続されているかどうかを返す
	BOOL IsConnected();
	//	デバイスの状態を取得する
	BOOL GetState( FEATURE_REPORT_ID * );
	//	デバイスの状態を設定する
	BOOL SetState( HID_WRITE_REPORT_ID * );
	//	デバイスに変更を通知する
	BOOL Apply();

private:

	//	レジストリから値を取得する
	VOID GetRegistry( VOID *, ULONG, ULONG, WCHAR * );
	//	デバイスにアクセスする
	BOOL IoDevice( LONG, HID_WRITE_REPORT_ID *, FEATURE_REPORT_ID * );

	//	デバイスへのアクセス方法
	static const LONG IO_MODE_IS_CONNECTED	= 1;
	static const LONG IO_MODE_GET_STATE		= 2;
	static const LONG IO_MODE_SET_STATE		= 3;
	static const LONG IO_MODE_APPLY			= 4;

	//	サポートしているデバイスの数
	LONG	SupportedDeviceCount;
	//	サポートしているデバイスの属性
	ULONG *	SupportedDeviceAttribute;

protected:

};
