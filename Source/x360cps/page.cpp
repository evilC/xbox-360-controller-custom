
//	Property Sheet for Microsoft Xbox 360 Controller

#include	"page.h"
#include	"extension.h"

//----------------------------------------------------------------------------------------------
//	CPage
//----------------------------------------------------------------------------------------------
CPage::CPage()
{
	//	ウィンドウ ハンドル
	Wnd				= NULL;

	//	設定
	Setting			= NULL;

	//	現在のコントローラーの設定
	CurrentSetting	= NULL;
}

//----------------------------------------------------------------------------------------------
//	wmInitDialog
//----------------------------------------------------------------------------------------------
VOID CPage::wmInitDialog( HWND )
{
	;
}

//----------------------------------------------------------------------------------------------
//	wmCommand
//----------------------------------------------------------------------------------------------
VOID CPage::wmCommand( HWND, WORD, WORD )
{
	;
}

//----------------------------------------------------------------------------------------------
//	wmNotify
//----------------------------------------------------------------------------------------------
VOID CPage::wmNotify( NMHDR * )
{
	;
}

//----------------------------------------------------------------------------------------------
//	wmTimer
//----------------------------------------------------------------------------------------------
VOID CPage::wmTimer( WPARAM )
{
	;
}

//----------------------------------------------------------------------------------------------
//	wmHScroll
//----------------------------------------------------------------------------------------------
VOID CPage::wmHScroll( HWND, WORD )
{
	;
}

//----------------------------------------------------------------------------------------------
//	wmPageChanged
//----------------------------------------------------------------------------------------------
VOID CPage::wmPageChanged()
{
	;
}

//----------------------------------------------------------------------------------------------
//	wmSettingListChanged
//----------------------------------------------------------------------------------------------
VOID CPage::wmSettingListChanged()
{
	//	設定を反映する
	ApplySetting();
}

//----------------------------------------------------------------------------------------------
//	ApplySetting
//----------------------------------------------------------------------------------------------
VOID CPage::ApplySetting()
{
	;
}

//----------------------------------------------------------------------------------------------
//	SettingChanged
//----------------------------------------------------------------------------------------------
VOID CPage::SettingChanged()
{
	//	変数宣言
	HWND	Dialog	= GetParent( Wnd );
	HWND	Page	= PropSheet_IndexToHwnd( Dialog, 0 );

	//	設定リストの内容を更新し、変更済みの設定を選択する
	Setting->ModifiedFlag	= TRUE;
	SendMessage(
		 Page
		,WM_PAGE_CHANGED
		,NULL
		,NULL );
}

//----------------------------------------------------------------------------------------------
//	InitComboBox
//----------------------------------------------------------------------------------------------
VOID CPage::InitComboBox(
	 WORD	ComboBoxId
	,WORD *	ResourceId
	,WORD	ResourceCount )
{
	//	コンボ ボックスを初期化する
	HWND	ComboBox	= GetDlgItem( Wnd, ComboBoxId );
	for( WORD Index = 0; Index < ResourceCount; Index ++ )
	{
		WCHAR	FunctionName[MAX_PATH];
		LoadString( Instance, ResourceId[Index], FunctionName, MAX_PATH );
		SendMessage( ComboBox, CB_INSERTSTRING, Index, (LPARAM)FunctionName );
	}
}

//----------------------------------------------------------------------------------------------
//	SetComboBoxIndex
//----------------------------------------------------------------------------------------------
VOID CPage::SetComboBoxIndex(
	 WORD	ComboBoxId
	,UCHAR	Index )
{
	//	コンボ ボックスを順番を設定する
	HWND	ComboBox	= GetDlgItem( Wnd, ComboBoxId );
	if( SendMessage( ComboBox, CB_GETCURSEL, 0, 0 ) != Index )
	{
		SendMessage( ComboBox, CB_SETCURSEL, Index, 0 );
	}
}

//----------------------------------------------------------------------------------------------
//	GetComboBoxIndex
//----------------------------------------------------------------------------------------------
UCHAR CPage::GetComboBoxIndex( WORD ComboBoxId )
{
	//	コンボ ボックスを順番を取得する
	return( (UCHAR)SendMessage( GetDlgItem( Wnd, ComboBoxId ), CB_GETCURSEL, 0, 0 ) );
}

//----------------------------------------------------------------------------------------------
//	InitListBox
//----------------------------------------------------------------------------------------------
VOID CPage::InitListBox(
	 WORD	ListBoxId
	,WORD *	ResourceId
	,WORD	ResourceCount )
{
	//	リスト ボックスを初期化する
	HWND	ListBox	= GetDlgItem( Wnd, ListBoxId );
	for( WORD Index = 0; Index < ResourceCount; Index ++ )
	{
		WCHAR	FunctionName[MAX_PATH];
		LoadString( Instance, ResourceId[Index], FunctionName, MAX_PATH );
		SendMessage( ListBox, LB_ADDSTRING, Index, (LPARAM)FunctionName );
	}
}

//----------------------------------------------------------------------------------------------
//	SetListBoxIndex
//----------------------------------------------------------------------------------------------
VOID CPage::SetListBoxIndex(
	 WORD	ListBoxId
	,LONG	Index )
{
	//	リスト ボックスの順番を設定する
	HWND	ListBox	= GetDlgItem( Wnd, ListBoxId );
	if( SendMessage( ListBox, LB_GETCURSEL, 0, 0 ) != Index )
	{
		SendMessage( ListBox, LB_SETCURSEL, Index, 0 );
	}
}

//----------------------------------------------------------------------------------------------
//	GetListBoxIndex
//----------------------------------------------------------------------------------------------
LONG CPage::GetListBoxIndex( WORD ListBoxId )
{
	//	リスト ボックスの順番を取得する
	return( (LONG)SendMessage( GetDlgItem( Wnd, ListBoxId ), LB_GETCURSEL, 0, 0 ) );
}

//----------------------------------------------------------------------------------------------
//	SetCheckBoxState
//----------------------------------------------------------------------------------------------
VOID CPage::SetCheckBoxState(
	 WORD		CheckBoxId
	,BOOLEAN	State )
{
	//	チェック ボックスの状態を設定する
	HWND	CheckBox	= GetDlgItem( Wnd, CheckBoxId );
	if( SendMessage( CheckBox, BM_GETCHECK, 0, 0 ) != State )
	{
		SendMessage( CheckBox, BM_SETCHECK, State, 0 );
	}
}

//----------------------------------------------------------------------------------------------
//	GetCheckBoxState
//----------------------------------------------------------------------------------------------
BOOLEAN CPage::GetCheckBoxState( WORD CheckBoxId )
{
	//	チェック ボックスの状態を取得する
	return( (BOOLEAN)SendMessage( GetDlgItem( Wnd, CheckBoxId ), BM_GETCHECK, 0, 0 ) );
}

//----------------------------------------------------------------------------------------------
//	InitTrackBar
//----------------------------------------------------------------------------------------------
VOID CPage::InitTrackBar(
	 WORD	TrackBarId
	,UCHAR	Min
	,UCHAR	Max
	,UCHAR	TicFreq
	,UCHAR	PageSize )
{
	//	トラック バーを初期化する
	HWND	TrackBar	= GetDlgItem( Wnd, TrackBarId );
	SendMessage( TrackBar, TBM_SETRANGE, FALSE, MAKELONG( Min, Max ) );
	SendMessage( TrackBar, TBM_SETTICFREQ, TicFreq, 0 );
	SendMessage( TrackBar, TBM_SETPAGESIZE, 0, PageSize );
}

//----------------------------------------------------------------------------------------------
//	SetTrackBarPos
//----------------------------------------------------------------------------------------------
VOID CPage::SetTrackBarPos(
	 WORD	TrackBarId
	,UCHAR	Pos )
{
	//	トラック バーの位置を設定する
	HWND	TrackBar	= GetDlgItem( Wnd, TrackBarId );
	if( SendMessage( TrackBar, TBM_GETPOS, 0, 0 ) != Pos )
	{
		SendMessage( TrackBar, TBM_SETPOS, TRUE, Pos );
	}
}

//----------------------------------------------------------------------------------------------
//	GetTrackBarPos
//----------------------------------------------------------------------------------------------
UCHAR CPage::GetTrackBarPos( WORD TrackBarId )
{
	//	トラック バーの位置を取得する
	return( (UCHAR)SendMessage( GetDlgItem( Wnd, TrackBarId ), TBM_GETPOS, 0, 0 ) );
}

//----------------------------------------------------------------------------------------------
//	SetTrackBarRange
//----------------------------------------------------------------------------------------------
VOID CPage::SetTrackBarRange(
	 WORD	TrackBarId
	,UCHAR	StartPos
	,UCHAR	EndPos )
{
	//	トラック バーの範囲を設定する
	HWND	TrackBar	= GetDlgItem( Wnd, TrackBarId );
	if( SendMessage( TrackBar, TBM_GETSELSTART, 0, 0 ) != StartPos
	||	SendMessage( TrackBar, TBM_GETSELEND, 0, 0 ) != EndPos )
	{
		SendMessage( TrackBar, TBM_SETSELSTART, TRUE, StartPos );
		SendMessage( TrackBar, TBM_SETSELEND, TRUE, EndPos );
	}
}

//----------------------------------------------------------------------------------------------
//	GetTrackBarRange
//----------------------------------------------------------------------------------------------
VOID CPage::GetTrackBarRange(
	 WORD	TrackBarId
	,UCHAR *	StartPos
	,UCHAR *	EndPos )
{
	//	トラック バーの範囲を取得する
	HWND	TrackBar	= GetDlgItem( Wnd, TrackBarId );
	*StartPos	= (UCHAR)SendMessage( TrackBar, TBM_GETSELSTART, 0, 0 );
	*EndPos		= (UCHAR)SendMessage( TrackBar, TBM_GETSELEND, 0, 0 );
}

//----------------------------------------------------------------------------------------------
//	SelectTrackBar
//----------------------------------------------------------------------------------------------
VOID CPage::SelectTrackBar(
	 HWND	TrackBar
	,WORD	Request
	,BOOL *	Selecting )
{
	//	変数宣言
	LONG	CurrentPos;

	//	トラック バーの現在位置を取得する
	CurrentPos	= (LONG)SendMessage( TrackBar, TBM_GETPOS, 0, 0 );

	//	リクエストによって処理を振り分ける
	switch( Request )
	{
		case TB_ENDTRACK:
			if( *Selecting == TRUE )
			{
				//	範囲の選択を終了した
				SendMessage( TrackBar, TBM_SETSELEND, TRUE, (LPARAM)CurrentPos );
				*Selecting	= FALSE;
			}
			break;

		default:
			if( GetAsyncKeyState(VK_SHIFT) & 0x80000000 )
			{
				if( *Selecting == FALSE )
				{
					//	範囲の選択を開始する
					SendMessage( TrackBar, TBM_CLEARSEL, TRUE, 0 );
					SendMessage( TrackBar, TBM_SETSELSTART, TRUE, (LPARAM)CurrentPos );
					*Selecting	= TRUE;
				} else {
					//	範囲を選択中
					SendMessage( TrackBar, TBM_SETSELEND, TRUE, (LPARAM)CurrentPos );
				}
			} else {
				if( *Selecting == TRUE )
				{
					//	範囲を選択中に Shift キーを離した
					SendMessage( TrackBar, TBM_SETSELEND, TRUE, (LPARAM)CurrentPos );
					*Selecting	= FALSE;
				}
			}
			break;
	}
}

//----------------------------------------------------------------------------------------------
//	ShowMessageBox
//----------------------------------------------------------------------------------------------
LONG CPage::ShowMessageBox(
	 ULONG		TitleId
	,ULONG		MessageId
	,WCHAR *	Message
	,ULONG		Type )
{
	//	メッセージを読み込む
	WCHAR	Title[MAX_PATH];
	LoadString( Instance, TitleId, Title, MAX_PATH );
	WCHAR	ResourceMessage[MAX_PATH];
	LoadString( Instance, MessageId, ResourceMessage, MAX_PATH );
	WCHAR	FormattedMessage[MAX_PATH];
	swprintf_s( FormattedMessage, ResourceMessage, Message );

	//	メッセージ ボックスを表示する
	return( MessageBox( Wnd, FormattedMessage, Title, Type ) );
}

//----------------------------------------------------------------------------------------------
//	SetIntegerText
//----------------------------------------------------------------------------------------------
VOID CPage::SetIntegerText(
	 WORD	TextId
	,LONG	Value )
{
	//	変数宣言
	HWND	Text	= GetDlgItem( Wnd, TextId );
	WCHAR	OldText[MAX_PATH];
	WCHAR	NewText[MAX_PATH];

	//	ウィンドウの文字列を生成する
	swprintf_s( NewText, L"%d", Value );

	//	ウィンドウの文字列が異なれば置き換える
	GetWindowText( Text, OldText, MAX_PATH );
	if( wcscmp( OldText, NewText ) != 0 )
	{
		SetWindowText( Text, NewText );
	}
}

//----------------------------------------------------------------------------------------------
//	SetButtonNumberText
//----------------------------------------------------------------------------------------------
VOID CPage::SetButtonNumberText(
	 WORD	TextId
	,ULONG	Value )
{
	//	変数宣言
	HWND	Text	= GetDlgItem( Wnd, TextId );
	WCHAR	OldText[MAX_PATH];
	WCHAR	NewText[MAX_PATH];
	LONG	Index;

	//	ウィンドウの文字列を生成する
	ZeroMemory( NewText, sizeof( NewText ) );
	for( Index = 0; Index < 32; Index ++ )
	{
		if( Value & ( 1 << Index ) )
		{
			WCHAR	WorkText[8];
			swprintf_s( WorkText, L" %02d", Index + 1 );
			wcscat_s( NewText, WorkText );
		}
	}

	//	ウィンドウの文字列が異なれば置き換える
	GetWindowText( Text, OldText, MAX_PATH );
	if( wcscmp( OldText, NewText ) != 0 )
	{
		SetWindowText( Text, NewText );
	}
}

//----------------------------------------------------------------------------------------------
//	GetWayUsageByControlIndex
//----------------------------------------------------------------------------------------------
UCHAR CPage::GetWayUsageByControlIndex( WORD ComboBoxId )
{
	//	方向別の用途
	UCHAR	WayUsage[]	= {	 USAGE_NONE
							,USAGE_X_AXIS_PLUS
							,USAGE_X_AXIS_MINUS
							,USAGE_Y_AXIS_PLUS
							,USAGE_Y_AXIS_MINUS
							,USAGE_X_ROTATION_PLUS
							,USAGE_X_ROTATION_MINUS
							,USAGE_Y_ROTATION_PLUS
							,USAGE_Y_ROTATION_MINUS
							,USAGE_Z_AXIS_PLUS
							,USAGE_Z_AXIS_MINUS
							,USAGE_Z_ROTATION_PLUS
							,USAGE_Z_ROTATION_MINUS
							,USAGE_SLIDER1_PLUS
							,USAGE_SLIDER1_MINUS
							,USAGE_SLIDER2_PLUS
							,USAGE_SLIDER2_MINUS
							,USAGE_BUTTON_1
							,USAGE_BUTTON_2
							,USAGE_BUTTON_3
							,USAGE_BUTTON_4
							,USAGE_BUTTON_5
							,USAGE_BUTTON_6
							,USAGE_BUTTON_7
							,USAGE_BUTTON_8
							,USAGE_BUTTON_9
							,USAGE_BUTTON_10
							,USAGE_BUTTON_11
							,USAGE_BUTTON_12
							,USAGE_BUTTON_13
							,USAGE_BUTTON_14
							,USAGE_BUTTON_15
							,USAGE_BUTTON_16
							,USAGE_BUTTON_17
							,USAGE_BUTTON_18
							,USAGE_BUTTON_19
							,USAGE_BUTTON_20
							,USAGE_BUTTON_21
							,USAGE_BUTTON_22
							,USAGE_BUTTON_23
							,USAGE_BUTTON_24
							,USAGE_BUTTON_25
							,USAGE_BUTTON_26
							,USAGE_BUTTON_27
							,USAGE_BUTTON_28
							,USAGE_BUTTON_29
							,USAGE_BUTTON_30
							,USAGE_BUTTON_31
							,USAGE_BUTTON_32 };

	//	方向別の機能を返す
	return WayUsage[GetComboBoxIndex( ComboBoxId )];
}

//----------------------------------------------------------------------------------------------
//	GetControlIndexByWayUsage
//----------------------------------------------------------------------------------------------
UCHAR CPage::GetControlIndexByWayUsage( UCHAR WayUsage )
{
	//	コントロールの順番
	UCHAR	ControlIndex[]	= {	 USAGE_NONE
								,USAGE_X_AXIS_PLUS
								,USAGE_X_AXIS_MINUS
								,USAGE_Y_AXIS_PLUS
								,USAGE_Y_AXIS_MINUS
								,USAGE_X_ROTATION_PLUS
								,USAGE_X_ROTATION_MINUS
								,USAGE_Y_ROTATION_PLUS
								,USAGE_Y_ROTATION_MINUS
								,USAGE_Z_AXIS_PLUS
								,USAGE_Z_AXIS_MINUS
								,USAGE_Z_ROTATION_PLUS
								,USAGE_Z_ROTATION_MINUS
								,USAGE_SLIDER1_PLUS
								,USAGE_SLIDER1_MINUS
								,USAGE_SLIDER2_PLUS
								,USAGE_SLIDER2_MINUS
								,USAGE_BUTTON_1
								,USAGE_BUTTON_2
								,USAGE_BUTTON_3
								,USAGE_BUTTON_4
								,USAGE_BUTTON_5
								,USAGE_BUTTON_6
								,USAGE_BUTTON_7
								,USAGE_BUTTON_8
								,USAGE_BUTTON_9
								,USAGE_BUTTON_10
								,USAGE_BUTTON_11
								,USAGE_BUTTON_12
								,USAGE_BUTTON_13
								,USAGE_BUTTON_14
								,USAGE_BUTTON_15
								,USAGE_BUTTON_16
								,USAGE_BUTTON_17
								,USAGE_BUTTON_18
								,USAGE_BUTTON_19
								,USAGE_BUTTON_20
								,USAGE_BUTTON_21
								,USAGE_BUTTON_22
								,USAGE_BUTTON_23
								,USAGE_BUTTON_24
								,USAGE_BUTTON_25
								,USAGE_BUTTON_26
								,USAGE_BUTTON_27
								,USAGE_BUTTON_28
								,USAGE_BUTTON_29
								,USAGE_BUTTON_30
								,USAGE_BUTTON_31
								,USAGE_BUTTON_32 };

	//	コントロールの順番を返す
	for( UCHAR Index = 0; Index < sizeof( ControlIndex ); Index ++ )
	{
		if( ControlIndex[Index] == WayUsage )
		{
			return Index;
		}
	}

	return USAGE_NONE;
}

//----------------------------------------------------------------------------------------------
//	EnableControl
//----------------------------------------------------------------------------------------------
VOID CPage::EnableControl(
	 WORD	ControlId
	,BOOL	State )
{
	//	コントロールの有効、無効を切り替える
	HWND	Control	= GetDlgItem( Wnd, ControlId );
	if( IsWindowEnabled( Control ) == 0 )
	{
		if( State == TRUE )
		{
			EnableWindow( Control, TRUE );
		}
	} else {
		if( State == FALSE )
		{
			EnableWindow( Control, FALSE );
		}
	}
}

//----------------------------------------------------------------------------------------------
//	EnableControls
//----------------------------------------------------------------------------------------------
VOID CPage::EnableControls(
	 WORD	StartId
	,WORD	EndId
	,BOOL	State )
{
	//	範囲内のコントロールの有効、無効を切り替える
	for( WORD Index = StartId; Index <= EndId; Index ++ )
	{
		EnableControl( Index, State );
	}
}

//----------------------------------------------------------------------------------------------
//	PageProc
//----------------------------------------------------------------------------------------------
INT_PTR CALLBACK PageProc(
	 HWND	Wnd
	,UINT	Message
	,WPARAM	wParam
	,LPARAM	lParam )
{
	//	変数宣言
			INT_PTR		Result	= FALSE;
	static	LONG		PageCount;
	static	CPage * *	Page;
	static	HWND *		PageWnd;

	//	メッセージによって処理を振り分ける
	switch( Message )
	{
		case WM_INITDIALOG:
			//	ページ クラスを取得する
			PageCount	++;
			if( Page == NULL )
			{
				Page	= (CPage * *)LocalAlloc( LPTR, PageCount * sizeof( CPage * ) );
				PageWnd	= (HWND *)LocalAlloc( LPTR, PageCount * sizeof( HWND ) );
			} else {
				Page	= (CPage * *)LocalReAlloc(
							 Page
							,PageCount * sizeof( CPage * )
							,LMEM_MOVEABLE );
				PageWnd	= (HWND *)LocalReAlloc(
							 PageWnd
							,PageCount * sizeof( HWND )
							,LMEM_MOVEABLE );
			}
			Page[PageCount - 1]		= (CPage *)((PROPSHEETPAGE *)lParam)->lParam;
			PageWnd[PageCount - 1]	= Wnd;
			//	ダイアログを初期化する
			Page[PageCount - 1]->wmInitDialog( Wnd );
			//	設定を反映する
			Page[PageCount - 1]->ApplySetting();
			break;

		case WM_COMMAND:
			//	コマンドを処理する
			for( LONG Index = 0; Index < PageCount; Index ++ )
			{
				if( Wnd == PageWnd[Index] )
				{
					Page[Index]->wmCommand( (HWND)lParam, LOWORD( wParam ), HIWORD( wParam ) );
				}
			}
			break;

		case WM_HSCROLL:
			//	水平スクロールを処理する
			for( LONG Index = 0; Index < PageCount; Index ++ )
			{
				if( Wnd == PageWnd[Index] )
				{
					Page[Index]->wmHScroll( (HWND)lParam, LOWORD( wParam ) );
				}
			}
			break;

		case WM_NOTIFY:
			//	通知を処理する
			for( LONG Index = 0; Index < PageCount; Index ++ )
			{
				if( Wnd == PageWnd[Index] )
				{
					Page[Index]->wmNotify( (NMHDR *)lParam );
				}
			}
			break;

		case WM_TIMER:
			//	タイマーを処理する
			for( LONG Index = 0; Index < PageCount; Index ++ )
			{
				if( Wnd == PageWnd[Index] )
				{
					Page[Index]->wmTimer( wParam );
				}
			}
			break;

		case WM_PAGE_CHANGED:
			//	ページ変更の通知を処理する
			for( LONG Index = 0; Index < PageCount; Index ++ )
			{
				if( Wnd == PageWnd[Index] )
				{
					Page[Index]->wmPageChanged();
				}
			}
			break;

		case WM_SETTING_LIST_CHANGED:
			//	設定変更の通知を処理する
			for( LONG Index = 0; Index < PageCount; Index ++ )
			{
				if( Wnd == PageWnd[Index] )
				{
					Page[Index]->wmSettingListChanged();
				}
			}
			break;
	}

	return( Result );
}
