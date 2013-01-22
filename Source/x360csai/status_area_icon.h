
//	Status Area Icon for Microsoft Xbox 360 Controller

#pragma	once

#include	<windows.h>
#include	<shellapi.h>

#include	"setting.h"
#include	"property_sheet.h"
#include	"device.h"

//----------------------------------------------------------------------------------------------
//	定数
//----------------------------------------------------------------------------------------------

#define	WM_STATUS_AREA_ICON_NOTIFY	WM_USER + 1

#define	STATUS_AREA_ICON_ID			0
#define	UPDATE_SETTING				1

#define	LEFT_CLICK_TIMER			1
#define	AUTO_SETTING_CHANGE_TIMER	2

#define CHECK_INTERVAL				1000

#define	TH32CS_SNAPPROCESS			0x00000002

//----------------------------------------------------------------------------------------------
//	構造体
//----------------------------------------------------------------------------------------------

typedef struct tagPROCESSENTRY32
{
	DWORD		dwSize;
	DWORD		cntUsage;
	DWORD		th32ProcessID;
	ULONG_PTR	th32DefaultHeapID;
	DWORD		th32ModuleID;
	DWORD		cntThreads;
	DWORD		th32ParentProcessID;
	LONG		pcPriClassBase;
	DWORD		dwFlags;
	WCHAR		szExeFile[MAX_PATH];
} PROCESSENTRY32, *PPROCESSENTRY32;

//----------------------------------------------------------------------------------------------
//	クラス
//----------------------------------------------------------------------------------------------

class CStatusAreaIcon
{

public:

	//	コンストラクタ
	CStatusAreaIcon( HINSTANCE, HWND );
	//	デストラクタ
	~CStatusAreaIcon();

	//	ステータス エリア アイコンのイベントを処理する
	VOID wmNotify( WPARAM, LPARAM );
	//	コマンドを処理する
	VOID wmCommand( WORD );
	//	タイマーを処理する
	VOID wmTimer( WPARAM );
	//	デバイスの変更を処理する
	VOID wmDeviceChange();
	//	タスク バーの再作成を処理する
	VOID wmTaskbarCtreated();

	//	インスタンス
	HINSTANCE	Instance;
	//	ウィンドウ ハンドル
	HWND		Wnd;

	//	プロセス ID
	DWORD		ProcessID;

private:

	//	ステータス エリアにアイコンを表示する
	VOID ShowIcon();
	//	ステータス エリアからアイコンを削除する
	VOID HideIcon();

	//	ポップアップ メニュー（左）を表示する
	VOID ShowLeftPopupMenu();
	//	ポップアップ メニュー（右）を表示する
	VOID ShowRightPopupMenu();

	//	設定の自動切換えを行う
	VOID AutomaticChange( DWORD );

	//	設定
	CSetting *			Setting;
	//	プロパティ シート
	CPropertySheet *	PropertySheet;
	//	デバイス
	CDevice *			Device;

protected:

};

//----------------------------------------------------------------------------------------------
//	関数
//----------------------------------------------------------------------------------------------

extern "C"
{
HANDLE WINAPI CreateToolhelp32Snapshot( DWORD, DWORD );
BOOL WINAPI Process32FirstW( HANDLE, PPROCESSENTRY32 );
BOOL WINAPI Process32NextW( HANDLE, PPROCESSENTRY32 );
}
