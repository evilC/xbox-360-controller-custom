
//	Property Sheet for Microsoft Xbox 360 Controller

#pragma	once

#include	<windows.h>
#include	<commctrl.h>

#include	"setting.h"

//----------------------------------------------------------------------------------------------
//	定数
//----------------------------------------------------------------------------------------------

#define	WM_PAGE_CHANGED				WM_USER + 1
#define	WM_SETTING_LIST_CHANGED		WM_USER + 2

//----------------------------------------------------------------------------------------------
//	クラス
//----------------------------------------------------------------------------------------------

class CPage
{

public:

	//	コンストラクタ
	CPage();

	//	ダイアログを初期化する
	virtual VOID wmInitDialog( HWND );
	//	コマンドを処理する
	virtual VOID wmCommand( HWND, WORD, WORD );
	//	通知を処理する
	virtual VOID wmNotify( NMHDR * );
	//	タイマーを処理する
	virtual VOID wmTimer( WPARAM );
	//	水平スクロールを処理する
	virtual VOID wmHScroll( HWND, WORD );
	//	ページ変更の通知を処理する
	virtual VOID wmPageChanged();
	//	設定変更の通知を処理する
	virtual VOID wmSettingListChanged();

	//	設定を反映する
	virtual VOID ApplySetting();

private:

protected:

	//	設定の変更を通知する
	VOID SettingChanged();

	//	コンボ ボックスを初期化する
	VOID InitComboBox( WORD, WORD *, WORD );
	//	コンボ ボックスを順番を設定する
	VOID SetComboBoxIndex( WORD, UCHAR );
	//	コンボ ボックスを順番を取得する
	UCHAR GetComboBoxIndex( WORD );

	//	リスト ボックスを初期化する
	VOID InitListBox( WORD, WORD *, WORD );
	//	リスト ボックスの順番を設定する
	VOID SetListBoxIndex( WORD, LONG );
	//	リスト ボックスの順番を取得する
	LONG GetListBoxIndex( WORD );

	//	チェック ボックスの状態を設定する
	VOID SetCheckBoxState( WORD, BOOLEAN );
	//	チェック ボックスの状態を取得する
	BOOLEAN GetCheckBoxState( WORD );

	//	トラック バーを初期化する
	VOID InitTrackBar( WORD, UCHAR, UCHAR, UCHAR, UCHAR );
	//	トラック バーの位置を設定する
	VOID SetTrackBarPos( WORD, UCHAR );
	//	トラック バーの位置を取得する
	UCHAR GetTrackBarPos( WORD );
	//	トラック バーの範囲を設定する
	VOID SetTrackBarRange( WORD, UCHAR, UCHAR );
	//	トラック バーの範囲を取得する
	VOID GetTrackBarRange( WORD, UCHAR *, UCHAR * );
	//	トラック バーの選択を行う
	VOID SelectTrackBar( HWND, WORD, BOOL * );

	//	メッセージ ボックスを表示する
	LONG ShowMessageBox( ULONG, ULONG, WCHAR *, ULONG );

	//	ウィンドウ文字列に数値を設定する
	VOID SetIntegerText( WORD, LONG );
	//	ウィンドウ文字列にボタン番号を設定する
	VOID SetButtonNumberText( WORD, ULONG );

	//	コントロールの順番から方向別の機能を取得する
	UCHAR GetWayUsageByControlIndex( WORD );
	//	方向別の機能からコントロールの順番を取得する
	UCHAR GetControlIndexByWayUsage( UCHAR );

	//	コントロールの有効、無効を切り替える
	VOID EnableControl( WORD, BOOL );
	//	範囲内のコントロールの有効、無効を切り替える
	VOID EnableControls( WORD, WORD, BOOL );

	//	ウィンドウ ハンドル
	HWND		Wnd;
	//	設定
	CSetting *	Setting;

	//	現在のコントローラーの設定
	SETTING *	CurrentSetting;

};

//----------------------------------------------------------------------------------------------
//	関数
//----------------------------------------------------------------------------------------------

INT_PTR CALLBACK PageProc( HWND, UINT, WPARAM, LPARAM );

