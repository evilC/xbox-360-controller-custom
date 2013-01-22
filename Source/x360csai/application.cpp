
//	Status Area Icon for Microsoft Xbox 360 Controller

#include	"application.h"
#include	"status_area_icon.h"

//----------------------------------------------------------------------------------------------
//	グローバル変数
//----------------------------------------------------------------------------------------------

HINSTANCE	Instance;

//----------------------------------------------------------------------------------------------
//	関数
//----------------------------------------------------------------------------------------------

ATOM RegisterMyClass();
HWND CreateMyWindow();
LRESULT CALLBACK WindowProc( HWND, UINT, WPARAM, LPARAM );

//----------------------------------------------------------------------------------------------
//	WinMain
//----------------------------------------------------------------------------------------------
int WINAPI WinMain(
	 HINSTANCE	InstanceHandle
	,HINSTANCE	PrevInstanceHandle
	,LPSTR		CmdLine
	,int		CmdShow )
{
	//	変数宣言
	MSG	Msg;

	//	アプリケーションのインスタンスを保存する
	Instance	= InstanceHandle;

	//	アプリケーションの複数起動を防止する
	HANDLE	Mutex	= CreateMutex( NULL, TRUE, X360CSAI_MUTEX );
	if( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		return( ERROR_ALREADY_EXISTS );
	}

	//	ウィンドウ クラスを登録する
	ATOM	Atom	= RegisterMyClass();
	if( Atom == 0 )
	{
		return( 0 );
	}

	//	登録したウィンドウ クラスからウィンドウを作成する
	HWND	Wnd	= CreateMyWindow();
	if( Wnd == NULL )
	{
		return( 0 );
	}

	//	メッセージ ループ
	for(;;)
	{
		//	メッセージ キューからメッセージを取得する
		BOOL	Result	= GetMessage( &Msg, NULL, 0, 0 );
		//	取得に失敗した場合は終了する
		if( Result == -1 )
		{
			return( 0 );
		}
		//	WM_QUIT メッセージを取得した場合はループを終了する
		if( Result == 0 )
		{
			break;
		}

		//	仮想キー メッセージを文字メッセージへ変換する
		TranslateMessage( &Msg );
		//	メッセージをウィンドウ プロシージャへ送出する
		DispatchMessage( &Msg );
	}

	//	ミューテックスを開放する
	CloseHandle( Mutex );

	//	WM_QUIT メッセージの終了コードを返す
	return( (int)Msg.wParam );
}

//----------------------------------------------------------------------------------------------
//	RegisterMyClass
//----------------------------------------------------------------------------------------------
ATOM RegisterMyClass()
{
	//	ウィンドウ クラスの属性を設定する
	WNDCLASSEX	wcex;
	ZeroMemory( &wcex, sizeof( wcex ) );
	wcex.cbSize			= sizeof( wcex );
	wcex.style			= 0;
	wcex.lpfnWndProc	= WindowProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= Instance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= X360CSAI_CLASS_NAME;
	wcex.hIconSm		= NULL;

	//	ウィンドウ クラスを登録する
	return( RegisterClassEx( &wcex ) );
}

//----------------------------------------------------------------------------------------------
//	CreateMyWindow
//----------------------------------------------------------------------------------------------
HWND CreateMyWindow()
{
	//	登録したウィンドウ クラスからウィンドウを作成する
	HWND	Wnd	= CreateWindowEx(
					 0
					,X360CSAI_CLASS_NAME
					,X360CSAI_WINDOW_NAME
					,WS_OVERLAPPEDWINDOW
					,CW_USEDEFAULT
					,CW_USEDEFAULT
					,CW_USEDEFAULT
					,CW_USEDEFAULT
					,NULL
					,NULL
					,Instance
					,NULL );

	//	作成結果を返す
	return( Wnd );
}

//----------------------------------------------------------------------------------------------
//	WindowProc
//----------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(
	 HWND	Wnd
	,UINT	Msg
	,WPARAM	wParam
	,LPARAM lParam )
{
	//	変数宣言
			LRESULT				Result	= 0;
	static	CStatusAreaIcon	*	StatusAreaIcon;
	static	UINT				TaskbarCreated;

	//	メッセージによって処理を振り分ける
	switch( Msg )
	{
		case WM_CREATE:
			//	コモン コントロールを初期化する
			InitCommonControls();
			//	ステータス エリア アイコンを作成する
			StatusAreaIcon	= new CStatusAreaIcon( Instance, Wnd );
			//	タスク バーの再作成に備える
			TaskbarCreated	= RegisterWindowMessage( L"TaskbarCreated" );
			break;

		case WM_STATUS_AREA_ICON_NOTIFY:
			//	ステータス エリア アイコンのイベントを処理する
			StatusAreaIcon->wmNotify( wParam, lParam );
			break;

		case WM_COMMAND:
			//	コマンドを処理する
			StatusAreaIcon->wmCommand( LOWORD( wParam ) );
			break;

		case WM_TIMER:
			//	タイマーを処理する
			StatusAreaIcon->wmTimer( wParam );
			break;

		case WM_DEVICECHANGE:
			//	デバイスの変更を処理する
			StatusAreaIcon->wmDeviceChange();
			Result	= TRUE;
			break;

		case WM_DESTROY:
			//	ステータス エリア アイコンを削除する
			delete StatusAreaIcon;
			//	アプリケーションを終了する
			PostQuitMessage( 0 );
			break;

		default:
			//	タスク バーの再作成を処理する
			if( Msg == TaskbarCreated )
			{
				StatusAreaIcon->wmTaskbarCtreated();
			}
			//	既定のウィンドウ プロシージャを呼び出す
			Result	= DefWindowProc( Wnd, Msg, wParam, lParam );
			break;
	}

	//	メッセージ処理の結果を返す
	return( Result );
}
