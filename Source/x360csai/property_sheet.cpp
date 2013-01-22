
//	Status Area Icon for Microsoft Xbox 360 Controller

#include	"property_sheet.h"

//----------------------------------------------------------------------------------------------
//	CPropertySheet
//----------------------------------------------------------------------------------------------
CPropertySheet::CPropertySheet(
	 HINSTANCE	Instance
	,HWND		Wnd )
{
	//	インスタンス
	this->Instance	= Instance;
	//	ウィンドウ ハンドル
	this->Wnd		= Wnd;
}

//----------------------------------------------------------------------------------------------
//	Open
//----------------------------------------------------------------------------------------------
VOID CPropertySheet::Open()
{
	//	COM を初期化する
	CoInitialize( NULL );

	//	IDIGameCntrlPropSheet を作成する
	IDIGameCntrlPropSheet *	PropSheet;
	HRESULT	 Result	= CoCreateInstance(
						 ClassID_X360CPS
						,NULL
						,CLSCTX_SERVER
						,IID_IDIGameCntrlPropSheet
						,(LPVOID *)&PropSheet );
	if( Result != S_OK )
	{
		return;
	}

	//	シートの情報を取得する
	LPDIGCSHEETINFO	DiGCSheetInfo;
	Result	= PropSheet->GetSheetInfo( &DiGCSheetInfo );
	if( Result != S_OK )
	{
		return;
	}

	//	ページの情報を取得する
	LPDIGCPAGEINFO	DiGCPageInfo;
	Result	= PropSheet->GetPageInfo( &DiGCPageInfo );
	if( Result != S_OK )
	{
		return;
	}

	//	ページの情報を作成する
	PROPSHEETPAGE *	PageInfo	= (PROPSHEETPAGE *)LocalAlloc(
									 LPTR
									,sizeof( PROPSHEETPAGE ) * DiGCSheetInfo->nNumPages );
	for( LONG Index = 0; Index < DiGCSheetInfo->nNumPages; Index ++ )
	{
		PageInfo[Index].dwSize		= sizeof( PROPSHEETPAGE );
		PageInfo[Index].dwFlags		= PSP_USEICONID;
		PageInfo[Index].hInstance	= DiGCPageInfo[Index].hInstance;
		PageInfo[Index].pszTemplate	= DiGCPageInfo[Index].lpwszTemplate;
		PageInfo[Index].pszIcon		= DiGCPageInfo[Index].lpwszPageIcon;
		PageInfo[Index].pszTitle	= DiGCPageInfo[Index].lpwszPageTitle;
		PageInfo[Index].pfnDlgProc	= DiGCPageInfo[Index].fpPageProc;
		PageInfo[Index].lParam		= DiGCPageInfo[Index].lParam;
		PageInfo[Index].pfnCallback	= NULL;
	}

	//	シートの情報を作成する
	PROPSHEETHEADER	SheetHeader;
	SheetHeader.dwSize		= sizeof( PROPSHEETHEADER );
	SheetHeader.dwFlags		= PSH_USEICONID | PSH_PROPSHEETPAGE;
	SheetHeader.hwndParent	= Wnd;
	SheetHeader.hInstance	= Instance;
	SheetHeader.pszIcon		= DiGCSheetInfo->lpwszSheetIcon;
	SheetHeader.pszCaption	= DiGCSheetInfo->lpwszSheetCaption;
	SheetHeader.nPages		= DiGCSheetInfo->nNumPages;
	SheetHeader.nStartPage	= 0;
	SheetHeader.ppsp		= (LPCPROPSHEETPAGE)PageInfo;
	SheetHeader.pfnCallback	= NULL;

	//	プロパティ シートを表示する
	PropertySheet( &SheetHeader );

	//	IDIGameCntrlPropSheet を開放する
	PropSheet->Release();

	//	COM を終了する
	CoUninitialize();
}

//----------------------------------------------------------------------------------------------
//	IsOpened
//----------------------------------------------------------------------------------------------
BOOL CPropertySheet::IsOpened()
{
	//	プロパティ シートが開いているか確認する
	HANDLE	Mutex	= CreateMutex( NULL, FALSE, X360CPS_MUTEX );
	if( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		//	ミューテックスを開放する
		CloseHandle( Mutex );
		//	プロパティ シートは開いている
		return( TRUE );
	} else {
		//	ミューテックスを開放する
		CloseHandle( Mutex );
		//	プロパティ シートは閉じている
		return( FALSE );
	}
}
