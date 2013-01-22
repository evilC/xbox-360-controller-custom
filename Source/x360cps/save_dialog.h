
//	Property Sheet for Microsoft Xbox 360 Controller

#pragma	once

#include	<windows.h>

#include	"setting.h"

//----------------------------------------------------------------------------------------------
//	クラス
//----------------------------------------------------------------------------------------------

class CSaveDialog
{

public:

	//	コンストラクタ
	CSaveDialog( HWND, CSetting * );

	//	コマンドを処理する
	VOID wmCommand( HWND, HWND, WORD, WORD );

private:

	//	設定を保存する
	BOOL Save();

	//	メッセージ ボックスを表示する
	LONG ShowMessageBox( HWND, ULONG, ULONG, WCHAR *, ULONG );

	//	ウィンドウ ハンドル
	HWND		Wnd;
	//	設定
	CSetting *	Setting;

protected:

};

//----------------------------------------------------------------------------------------------
//	関数
//----------------------------------------------------------------------------------------------

INT_PTR CALLBACK SaveDialogProc( HWND, UINT, WPARAM, LPARAM );

