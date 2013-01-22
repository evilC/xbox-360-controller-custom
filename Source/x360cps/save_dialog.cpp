
//	Property Sheet for Microsoft Xbox 360 Controller

#include	"save_dialog.h"
#include	"extension.h"
#include	"resource.h"

//----------------------------------------------------------------------------------------------
//	CSaveDialog
//----------------------------------------------------------------------------------------------
CSaveDialog::CSaveDialog(
	  HWND			Wnd
	 ,CSetting *	Setting )
{
	//	ウィンドウ ハンドル
	this->Wnd		= Wnd;
	//	設定
	this->Setting	= Setting;

	//	初期値として現在の設定名称を設定する
	SetWindowText(
		 GetDlgItem( Wnd, IDC_SETTING_NAME )
		,Setting->SettingName[Setting->CurrentSettingIndex] );
}

//----------------------------------------------------------------------------------------------
//	wmCommand
//----------------------------------------------------------------------------------------------
VOID CSaveDialog::wmCommand(
	 HWND	Wnd
	,HWND	Control
	,WORD	ControlID
	,WORD	NotifyMessage )
{
	//	コントロールによって処理を振り分ける
	switch( ControlID )
	{
		case IDC_SETTING_NAME:
			//	通知メッセージによって処理を振り分ける
			switch( NotifyMessage )
			{
				case EN_CHANGE:
					//	設定の名称が空の場合は OK ボタンを無効にする
					WCHAR	SettingName[MAX_PATH];
					GetWindowText( Control, SettingName, MAX_PATH );
					if( wcslen( SettingName ) > 0 )
					{
						EnableWindow( GetDlgItem( Wnd, IDOK ), TRUE );
					} else {
						EnableWindow( GetDlgItem( Wnd, IDOK ), FALSE );
					}
					break;
			}
			break;

		case IDOK:
			//	設定を保存する
			if( Save() == TRUE )
			{
				EndDialog( Wnd, IDOK );
				delete this;
			}
			break;

		case IDCANCEL:
			//	ダイアログを閉じる
			EndDialog( Wnd, IDCANCEL );
			delete this;
			break;
	}
}

//----------------------------------------------------------------------------------------------
//	Save
//----------------------------------------------------------------------------------------------
BOOL CSaveDialog::Save()
{
	//	入力された設定名称を取得する
	WCHAR	SettingName[MAX_PATH];
	GetWindowText( GetDlgItem( Wnd, IDC_SETTING_NAME ), SettingName, MAX_PATH );

	//	「標準の設定」と重複するかチェックする
	if( wcscmp( SettingName, Setting->SettingName[0] ) == 0 )
	{
		ShowMessageBox(
			 Wnd
			,IDS_WARNING_TITLE
			,IDS_WARNING_MESSAGE
			,SettingName
			,MB_OK | MB_ICONINFORMATION );
		return( FALSE );
	}

	//	他の設定名称と重複するかチェックする
	LONG	DuplicateSettingIndex	= 0;
	for( LONG Index = 1; Index < Setting->SettingCount; Index ++ )
	{
		if( wcscmp( SettingName, Setting->SettingName[Index] ) == 0 )
		{
			DuplicateSettingIndex	= Index;
			break;
		}
	}
	if( DuplicateSettingIndex == 0 )
	{
		//	重複しなければ、入力された設定名称で保存する
		Setting->SaveAs( SettingName );
	} else {
		//	重複していれば、ダイアログを表示して上書きを確認する
		LONG	Result	= ShowMessageBox(
							 Wnd
							,IDS_REPLACE_TITLE
							,IDS_REPLACE_MESSAGE
							,SettingName
							,MB_YESNO | MB_ICONQUESTION );
		if( Result == IDYES )
		{
			Setting->Save( DuplicateSettingIndex );
		} else {
			return( FALSE );
		}
	}

	return( TRUE );
}

//----------------------------------------------------------------------------------------------
//	ShowMessageBox
//----------------------------------------------------------------------------------------------
LONG CSaveDialog::ShowMessageBox(
	 HWND		Wnd
	,ULONG		TitleID
	,ULONG		MessageID
	,WCHAR *	Message
	,ULONG		Type )
{
	//	メッセージを読み込む
	WCHAR	Title[MAX_PATH];
	LoadString( Instance, TitleID, Title, MAX_PATH );
	WCHAR	ResourceMessage[MAX_PATH];
	LoadString( Instance, MessageID, ResourceMessage, MAX_PATH );
	WCHAR	FormattedMessage[MAX_PATH];
	swprintf_s( FormattedMessage, ResourceMessage, Message );

	//	メッセージ ボックスを表示する
	return( MessageBox( Wnd, FormattedMessage, Title, Type ) );
}

//----------------------------------------------------------------------------------------------
//	SaveDialogProc
//----------------------------------------------------------------------------------------------
INT_PTR CALLBACK SaveDialogProc(
	 HWND	Wnd
	,UINT	Message
	,WPARAM	wParam
	,LPARAM	lParam )
{
	//	変数宣言
			INT_PTR			Result	= FALSE;
	static	CSaveDialog *	SaveDialog;

	//	メッセージによって処理を振り分ける
	switch( Message )
	{
		case WM_INITDIALOG:
			//	ダイアログを作成する
			SaveDialog	= new CSaveDialog( Wnd, (CSetting *)lParam );
			break;

		case WM_COMMAND:
			//	コマンドを処理する
			SaveDialog->wmCommand( Wnd, (HWND)lParam, LOWORD( wParam ), HIWORD( wParam ) );
			break;
	}

	return( Result );
}
