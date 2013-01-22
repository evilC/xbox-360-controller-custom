
//	Property Sheet for Microsoft Xbox 360 Controller

#pragma	once

#include	"page.h"
#include	"device.h"
#include	"report.h"

//----------------------------------------------------------------------------------------------
//	定数
//----------------------------------------------------------------------------------------------

#define	X360CPS_MUTEX				L"x360cps mutex"

#define	X360CSAI_CLASS_NAME			L"x360csai class"
#define	X360CSAI_WINDOW_NAME		L"x360csai window"

#define	WM_STATUS_AREA_ICON_NOTIFY	WM_USER + 1

#define	STATUS_AREA_ICON_ID			0
#define	UPDATE_SETTING				1

#define	POLLING_TIMER_ID			1
#define POLLING_INTERVAL			16

//----------------------------------------------------------------------------------------------
//	クラス
//----------------------------------------------------------------------------------------------

class CControllerPage : public CPage
{

public:

	//	コンストラクタ
	CControllerPage( CSetting * );

	//	ダイアログを初期化する
	VOID wmInitDialog( HWND );
	//	コマンドを処理する
	VOID wmCommand( HWND, WORD, WORD );
	//	通知を処理する
	VOID wmNotify( NMHDR * );
	//	タイマーを処理する
	VOID wmTimer( WPARAM );
	//	ページ変更の通知を処理する
	VOID wmPageChanged();

	//	設定を反映する
	VOID ApplySetting();

	//	設定の順番を変更する
	VOID ChangeSetting();
	//	現在の設定を保存する
	VOID SaveSetting();
	//	現在の設定を削除する
	VOID DeleteSetting();

	//	設定の変更を反映する
	VOID ApplyChangedSetting();

	//	ページを閉じる
	VOID ClosePage();

	//	コントローラーの状態を表示する
	VOID ShowControllerState();
	//	振動をテストする
	VOID TestForceFeedback( LONG );

private:

	//	ミューテックス
	HANDLE		Mutex;

	//	デバイスに適用されているセッティング
	SETTING		DeviceSetting;

	//	デバイス
	CDevice *	Device;
	//	レポート
	CReport *	Report;

	//	コントローラーの最後の状態
	LAST_STATE	LastState;

protected:

};
