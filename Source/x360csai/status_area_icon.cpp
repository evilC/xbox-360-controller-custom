
//	Status Area Icon for Microsoft Xbox 360 Controller

#include	"status_area_icon.h"
#include	"resource.h"

//----------------------------------------------------------------------------------------------
//	CStatusAreaIcon
//----------------------------------------------------------------------------------------------
CStatusAreaIcon::CStatusAreaIcon(
	 HINSTANCE	Instance
	,HWND		Wnd )
{
	//	インスタンス
	this->Instance	= Instance;
	//	ウィンドウ ハンドル
	this->Wnd		= Wnd;

	//	プロセス ID
	ProcessID		= NULL;

	//	各インスタンスを作成する
	Setting			= new CSetting( Instance, Wnd );
	PropertySheet	= new CPropertySheet( Instance, Wnd );
	Device			= new CDevice();

	//	デバイスが存在すればステータス エリアにアイコンを追加する
	if( Device->IsConnected() == TRUE )
	{
		ShowIcon();
	}

	//	設定の自動切換え用タイマーを開始する
	if( Setting->AutoSettingChange == TRUE )
	{
		SetTimer( Wnd, AUTO_SETTING_CHANGE_TIMER, 0, NULL );
	}
}

//----------------------------------------------------------------------------------------------
//	~CStatusAreaIcon
//----------------------------------------------------------------------------------------------
CStatusAreaIcon::~CStatusAreaIcon()
{
	//	ステータス エリアからアイコンを削除する
	HideIcon();

	//	各インスタンスを開放する
	delete Device;
	delete PropertySheet;
	delete Setting;
}

//----------------------------------------------------------------------------------------------
//	wmNotify
//----------------------------------------------------------------------------------------------
VOID CStatusAreaIcon::wmNotify(
	 WPARAM	wParam
	,LPARAM	lParam )
{
	switch( wParam )
	{
		case STATUS_AREA_ICON_ID:
			switch( lParam )
			{
				case WM_LBUTTONDOWN:
					//	タイマーでポップアップ メニュー（左）を表示する
					SetTimer( Wnd, LEFT_CLICK_TIMER, GetDoubleClickTime(), NULL );
					break;

				case WM_LBUTTONDBLCLK:
					//	左クリック用タイマーを停止する
					KillTimer( Wnd, LEFT_CLICK_TIMER );
					//	プロパティ シートを開く
					if( PropertySheet->IsOpened() == FALSE )
					{
						PropertySheet->Open();
					}
					break;

				case WM_RBUTTONUP:
					//	ポップアップ メニュー（右）を表示する
					ShowRightPopupMenu();
					break;
			}
			break;

		case UPDATE_SETTING:
			//	レジストリから設定を読み込む
			Setting->LoadRegistry();
			break;
	}
}

//----------------------------------------------------------------------------------------------
//	wmCommand
//----------------------------------------------------------------------------------------------
VOID CStatusAreaIcon::wmCommand( WORD CommandId )
{
	switch( CommandId )
	{
		case IDM_PROPERTY:
			//	プロパティ(&P)
			PropertySheet->Open();
			break;

		case IDM_AUTO_SETTING_CHANGE:
			//	設定の自動切換え(A)
			if( Setting->AutoSettingChange == TRUE )
			{
				//	設定の自動切換えを停止する
				KillTimer( Wnd, AUTO_SETTING_CHANGE_TIMER );
				Setting->AutoSettingChange	= FALSE;
			} else {
				//	設定の自動切換えを開始する
				SetTimer( Wnd, AUTO_SETTING_CHANGE_TIMER, 0, NULL );
				Setting->AutoSettingChange	= TRUE;
			}
			//	レジストリに設定を書き込む
			Setting->SaveRegistry();
			break;

		case IDM_EXIT:
			//	終了(X)
			if( PropertySheet->IsOpened() == FALSE )
			{
				PostMessage( Wnd, WM_CLOSE, 0, 0 );
			}
			break;

		default:
			//	コントローラーの設定を変更する
			LONG	SettingIndex	= CommandId - IDM_SETTING;
			if(	PropertySheet->IsOpened() == FALSE
			&&	0 <= SettingIndex && SettingIndex < Setting->SettingCount
			&&	SettingIndex != Setting->CurrentSettingIndex )
			{
				//	コントローラーの設定を変更する
				Setting->Change( SettingIndex, FALSE );
				Setting->SaveRegistry();
				Device->Apply();
			}
			break;
	}
}

//----------------------------------------------------------------------------------------------
//	wmTimer
//----------------------------------------------------------------------------------------------
VOID CStatusAreaIcon::wmTimer( WPARAM TimerId )
{
	switch( TimerId )
	{
		case LEFT_CLICK_TIMER:
			//	左クリック用タイマーを停止する
			KillTimer( Wnd, TimerId );
			//	ポップアップ メニュー（左）を表示する
			ShowLeftPopupMenu();
			break;

		case AUTO_SETTING_CHANGE_TIMER:
			//	設定の自動切換え用タイマーを停止する
			KillTimer( Wnd, TimerId );

			//	フォア グラウンドのアプリケーションのプロセス ID を取得する
			DWORD	CurrentProcessID;
			GetWindowThreadProcessId( GetForegroundWindow(), &CurrentProcessID );
			if( ProcessID != CurrentProcessID )
			{
				//	設定の自動切換えを行う
				if(	PropertySheet->IsOpened() == FALSE
				&&	Setting->ModifiedFlag == FALSE )
				{
					AutomaticChange( CurrentProcessID );
					//	プロセス ID を退避する
					ProcessID	= CurrentProcessID;
				}
			}

			//	設定の自動切換え用タイマーを開始する
			SetTimer( Wnd, AUTO_SETTING_CHANGE_TIMER, CHECK_INTERVAL, NULL );
			break;
	}
}

//----------------------------------------------------------------------------------------------
//	wmDeviceChange
//----------------------------------------------------------------------------------------------
VOID CStatusAreaIcon::wmDeviceChange()
{
	//	デバイスの有無によってアイコンの表示、非表示を切換える
	if( Device->IsConnected() == TRUE )
	{
		//	ステータス エリアにアイコンを追加する
		ShowIcon();
	} else {
		//	ステータス エリアからアイコンを削除する
		HideIcon();
	}
}

//----------------------------------------------------------------------------------------------
//	wmTaskbarCtreated
//----------------------------------------------------------------------------------------------
VOID CStatusAreaIcon::wmTaskbarCtreated()
{
	//	デバイスが有ればステータス エリアにアイコンを追加する
	if( Device->IsConnected() == TRUE )
	{
		ShowIcon();
	}
}

//----------------------------------------------------------------------------------------------
//	ShowIcon
//----------------------------------------------------------------------------------------------
VOID CStatusAreaIcon::ShowIcon()
{
	//	ステータス エリアにアイコンを追加する
	NOTIFYICONDATA	IconData;
	ZeroMemory( &IconData, sizeof( NOTIFYICONDATA ) );
	IconData.cbSize				= sizeof( NOTIFYICONDATA );
	IconData.hWnd				= Wnd;
	IconData.uID				= STATUS_AREA_ICON_ID;
	IconData.uFlags				= NIF_ICON | NIF_MESSAGE | NIF_TIP;
	IconData.uCallbackMessage	= WM_STATUS_AREA_ICON_NOTIFY;
	IconData.hIcon				= (HICON)LoadImage(
									 Instance
									,MAKEINTRESOURCE( IDI_ICON )
									,IMAGE_ICON
									,0
									,0
									,LR_DEFAULTSIZE );
	LoadString(
		 Instance
		,IDS_ICON_TIP
		,IconData.szTip
		,sizeof( IconData.szTip ) / sizeof( TCHAR ) );
	Shell_NotifyIcon( NIM_ADD, &IconData );
	//	アイコンを削除する
	if( IconData.hIcon != NULL )
	{
		DestroyIcon( IconData.hIcon );
	}
	SetForegroundWindow( Wnd );
}

//----------------------------------------------------------------------------------------------
//	HideIcon
//----------------------------------------------------------------------------------------------
VOID CStatusAreaIcon::HideIcon()
{
	//	ステータス エリアからアイコンを削除する
	NOTIFYICONDATA	IconData;
	ZeroMemory( &IconData, sizeof( NOTIFYICONDATA ) );
	IconData.cbSize	= sizeof( NOTIFYICONDATA );
	IconData.hWnd	= Wnd;
	IconData.uID	= STATUS_AREA_ICON_ID;
	Shell_NotifyIcon( NIM_DELETE, &IconData );
}

//----------------------------------------------------------------------------------------------
//	ShowLeftPopupMenu
//----------------------------------------------------------------------------------------------
VOID CStatusAreaIcon::ShowLeftPopupMenu()
{
	//	プロパティ シートの状態を取得する
	BOOL	PropertySheetOpened	= PropertySheet->IsOpened();

	//	ポップアップ メニューを作成する
	HMENU	PopupMenu	= CreatePopupMenu();

	//	コントローラーの設定を表示する
	for( LONG Index = 0; Index < Setting->SettingCount; Index ++ )
	{
		MENUITEMINFO	ItemInfo;
		ZeroMemory( &ItemInfo, sizeof( ItemInfo ) );
		ItemInfo.cbSize	= sizeof( ItemInfo );
		ItemInfo.fMask	= MIIM_TYPE | MIIM_STATE | MIIM_ID;
		ItemInfo.fType	= MFT_STRING;
		ItemInfo.fState	= MFS_ENABLED;
		if( PropertySheetOpened == TRUE )
		{
			ItemInfo.fState	|= MFS_DISABLED;
		}
		ItemInfo.wID	= IDM_SETTING + Index;
		if( Setting->CurrentSettingIndex == Index )
		{
			ItemInfo.fState		|= MFS_CHECKED;
		}
		WCHAR	SettingName[MAX_PATH * 2];
		wcscpy_s( SettingName, Setting->SettingName[Index] );
		//	変更済みフラグが上がっていれば「（変更済み）」を設定する
		if( Setting->ModifiedFlag == TRUE && Setting->CurrentSettingIndex == Index )
		{
			WCHAR	Modified[MAX_PATH];
			LoadString( Instance, IDS_MODIFIED, Modified, MAX_PATH );
			wcscat_s( SettingName, Modified );
		}
		ItemInfo.dwTypeData	= SettingName;
		InsertMenuItem( PopupMenu, Index, TRUE, &ItemInfo );
	}

	//	ポップアップ メニューを表示する
	SetForegroundWindow( Wnd );
	POINT	CursorPos;
	GetCursorPos( &CursorPos );
	TrackPopupMenuEx(
		 PopupMenu
		,TPM_LEFTALIGN | TPM_RIGHTBUTTON
		,CursorPos.x
		,CursorPos.y
		,Wnd
		,NULL );
	DestroyMenu( PopupMenu );
	PostMessage( Wnd, WM_NULL, 0, 0 );
}

//----------------------------------------------------------------------------------------------
//	ShowRightPopupMenu
//----------------------------------------------------------------------------------------------
VOID CStatusAreaIcon::ShowRightPopupMenu()
{
	//	プロパティ シートの状態を取得する
	BOOL	PropertySheetOpened	= PropertySheet->IsOpened();

	//	ポップアップ メニューを作成する
	HMENU	Menu		= LoadMenu( Instance, MAKEINTRESOURCE( IDR_MENU ) );
	HMENU	PopupMenu	= GetSubMenu( Menu, 0 );

	//	「プロパティ」をデフォルトにする
	MENUITEMINFO	ItemInfo;
	ZeroMemory( &ItemInfo, sizeof( ItemInfo ) );
	ItemInfo.cbSize	= sizeof( ItemInfo );
	GetMenuItemInfo( PopupMenu, IDM_PROPERTY, FALSE, &ItemInfo );
	ItemInfo.fMask	= MIIM_STATE;
	ItemInfo.fState	= MFS_ENABLED | MFS_DEFAULT;
	if( PropertySheetOpened == TRUE )
	{
		ItemInfo.fState	|= MFS_DISABLED;
	}
	SetMenuItemInfo( PopupMenu, IDM_PROPERTY, FALSE, &ItemInfo );

	//	「設定の自動切換え」のチェックを設定する
	GetMenuItemInfo( PopupMenu, IDM_AUTO_SETTING_CHANGE, FALSE, &ItemInfo );
	ItemInfo.fMask	= MIIM_STATE;
	ItemInfo.fState	= MFS_ENABLED;
	if( Setting->AutoSettingChange == TRUE )
	{
		ItemInfo.fState	|= MFS_CHECKED;
	}
	if( PropertySheetOpened == TRUE )
	{
		ItemInfo.fState	|= MFS_DISABLED;
	}
	SetMenuItemInfo( PopupMenu, IDM_AUTO_SETTING_CHANGE, FALSE, &ItemInfo );

	//	「終了」の有効・無効を設定する
	GetMenuItemInfo( PopupMenu, IDM_EXIT, FALSE, &ItemInfo );
	ItemInfo.fMask	= MIIM_STATE;
	ItemInfo.fState	= MFS_ENABLED;
	if( PropertySheetOpened == TRUE )
	{
		ItemInfo.fState	|= MFS_DISABLED;
	}
	SetMenuItemInfo( PopupMenu, IDM_EXIT, FALSE, &ItemInfo );

	//	ポップアップ メニューを表示する
	SetForegroundWindow( Wnd );
	POINT	CursorPos;
	GetCursorPos( &CursorPos );
	TrackPopupMenuEx(
		 PopupMenu
		,TPM_LEFTALIGN | TPM_RIGHTBUTTON
		,CursorPos.x
		,CursorPos.y
		,Wnd
		,NULL );
	DestroyMenu( Menu );
	PostMessage( Wnd, WM_NULL, 0, 0 );
}

//----------------------------------------------------------------------------------------------
//	AutomaticChange
//----------------------------------------------------------------------------------------------
VOID CStatusAreaIcon::AutomaticChange( DWORD ProcessID )
{
	//	プロセスのスナップショットを作成する
	HANDLE	ProcessSnapshot	= CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

	//	プロセスのエントリを取得する
	PROCESSENTRY32	ProcessEntry;
	ZeroMemory( &ProcessEntry, sizeof( PROCESSENTRY32 ) );
	ProcessEntry.dwSize = sizeof( PROCESSENTRY32 );
	Process32FirstW( ProcessSnapshot, &ProcessEntry );
	//	全てのプロセスを処理する
	do
	{
		//	フォアグラウンドのアプリケーションを探す
		if( ProcessEntry.th32ProcessID == ProcessID )
		{
			//	モジュール名を切り出す
			WCHAR	ModuleName[MAX_PATH];
			if( wcschr( ProcessEntry.szExeFile, '\\' ) == NULL )
			{
				wcscpy_s( ModuleName, ProcessEntry.szExeFile );
			} else {
				wcscpy_s( ModuleName, &(wcsrchr( ProcessEntry.szExeFile, '\\' )[1]) );
			}

			//	モジュール名と一致するコントローラーの設定を検索する
			for( LONG Index = 1; Index < Setting->SettingCount; Index ++ )
			{
				if(	( _wcsicmp( ModuleName, Setting->SettingName[Index] ) == 0 )
				&&	( Setting->CurrentSettingIndex != Index ) )
				{
					//	コントローラーの設定を変更する
					Setting->Change( Index, TRUE );
					Setting->SaveRegistry();
					Device->Apply();
					break;
				}
			}
			break;
		}
	} while( Process32NextW(ProcessSnapshot, &ProcessEntry ) );

	//	スナップショットを閉じる
	CloseHandle( ProcessSnapshot );
}
