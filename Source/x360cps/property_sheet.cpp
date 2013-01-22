
//	Property Sheet for Microsoft Xbox 360 Controller

#include	"property_sheet.h"
#include	"extension.h"
#include	"controller_page.h"
#include	"stick_page.h"
#include	"directional_pad_page.h"
#include	"trigger_page.h"
#include	"button_page.h"
#include	"actuator_page.h"
#include	"advanced_page.h"
#include	"resource.h"

//----------------------------------------------------------------------------------------------
//	CPropertySheet
//----------------------------------------------------------------------------------------------
CPropertySheet::CPropertySheet( VOID )
{
	//	参照カウンタを初期化する
	ReferenceCount	= 1;

	//	各インスタンスを初期化する
	Setting	= new CSetting( Instance, NULL );

	//	プロパティ シートの情報を設定する
	LoadString( Instance, IDS_SHEET_CAPTION, SheetCaption, MAX_PATH );
	SheetInfo.dwSize			= sizeof( DIGCSHEETINFO );
	SheetInfo.nNumPages			= sizeof( PageInfo ) / sizeof( DIGCPAGEINFO );
	SheetInfo.lpwszSheetCaption	= SheetCaption;
	SheetInfo.fSheetIconFlag	= TRUE;
	SheetInfo.lpwszSheetIcon	= MAKEINTRESOURCE( IDI_ICON );

	//	コントローラー ページの情報を設定する
	PageInfo[0].dwSize			= sizeof( DIGCPAGEINFO );
	PageInfo[0].lpwszPageTitle	= NULL;
	PageInfo[0].fpPageProc		= PageProc;
	PageInfo[0].fProcFlag		= FALSE;
	PageInfo[0].fpPrePostProc	= NULL;
	PageInfo[0].fIconFlag		= FALSE;
	PageInfo[0].lpwszPageIcon	= NULL;
	PageInfo[0].lpwszTemplate	= MAKEINTRESOURCE( IDD_CONTROLLER );
	PageInfo[0].lParam			= (LPARAM)(new CControllerPage( Setting ));
	PageInfo[0].hInstance		= Instance;

	//	スティック ページの情報を設定する
	PageInfo[1].dwSize			= sizeof( DIGCPAGEINFO );
	PageInfo[1].lpwszPageTitle	= NULL;
	PageInfo[1].fpPageProc		= PageProc;
	PageInfo[1].fProcFlag		= FALSE;
	PageInfo[1].fpPrePostProc	= NULL;
	PageInfo[1].fIconFlag		= FALSE;
	PageInfo[1].lpwszPageIcon	= NULL;
	PageInfo[1].lpwszTemplate	= MAKEINTRESOURCE( IDD_STICK );
	PageInfo[1].lParam			= (LPARAM)(new CStickPage( Setting ));
	PageInfo[1].hInstance		= Instance;

	//	方向パッド ページの情報を設定する
	PageInfo[2].dwSize			= sizeof( DIGCPAGEINFO );
	PageInfo[2].lpwszPageTitle	= NULL;
	PageInfo[2].fpPageProc		= PageProc;
	PageInfo[2].fProcFlag		= FALSE;
	PageInfo[2].fpPrePostProc	= NULL;
	PageInfo[2].fIconFlag		= FALSE;
	PageInfo[2].lpwszPageIcon	= NULL;
	PageInfo[2].lpwszTemplate	= MAKEINTRESOURCE( IDD_DIRECTIONAL_PAD );
	PageInfo[2].lParam			= (LPARAM)(new CDirectionalPadPage( Setting ));
	PageInfo[2].hInstance		= Instance;

	//	トリガー ページの情報を設定する
	PageInfo[3].dwSize			= sizeof( DIGCPAGEINFO );
	PageInfo[3].lpwszPageTitle	= NULL;
	PageInfo[3].fpPageProc		= PageProc;
	PageInfo[3].fProcFlag		= FALSE;
	PageInfo[3].fpPrePostProc	= NULL;
	PageInfo[3].fIconFlag		= FALSE;
	PageInfo[3].lpwszPageIcon	= NULL;
	PageInfo[3].lpwszTemplate	= MAKEINTRESOURCE( IDD_TRIGGER );
	PageInfo[3].lParam			= (LPARAM)(new CTriggerPage( Setting ));
	PageInfo[3].hInstance		= Instance;

	//	ボタン ページの情報を設定する
	PageInfo[4].dwSize			= sizeof( DIGCPAGEINFO );
	PageInfo[4].lpwszPageTitle	= NULL;
	PageInfo[4].fpPageProc		= PageProc;
	PageInfo[4].fProcFlag		= FALSE;
	PageInfo[4].fpPrePostProc	= NULL;
	PageInfo[4].fIconFlag		= FALSE;
	PageInfo[4].lpwszPageIcon	= NULL;
	PageInfo[4].lpwszTemplate	= MAKEINTRESOURCE( IDD_BUTTON );
	PageInfo[4].lParam			= (LPARAM)(new CButtonPage( Setting ));
	PageInfo[4].hInstance		= Instance;

	//	振動ページの情報を設定する
	PageInfo[5].dwSize			= sizeof( DIGCPAGEINFO );
	PageInfo[5].lpwszPageTitle	= NULL;
	PageInfo[5].fpPageProc		= PageProc;
	PageInfo[5].fProcFlag		= FALSE;
	PageInfo[5].fpPrePostProc	= NULL;
	PageInfo[5].fIconFlag		= FALSE;
	PageInfo[5].lpwszPageIcon	= NULL;
	PageInfo[5].lpwszTemplate	= MAKEINTRESOURCE( IDD_ACTUATOR );
	PageInfo[5].lParam			= (LPARAM)(new CActuatorPage( Setting ));
	PageInfo[5].hInstance		= Instance;

	//	詳細設定ページの情報を設定する
	PageInfo[6].dwSize			= sizeof( DIGCPAGEINFO );
	PageInfo[6].lpwszPageTitle	= NULL;
	PageInfo[6].fpPageProc		= PageProc;
	PageInfo[6].fProcFlag		= FALSE;
	PageInfo[6].fpPrePostProc	= NULL;
	PageInfo[6].fIconFlag		= FALSE;
	PageInfo[6].lpwszPageIcon	= NULL;
	PageInfo[6].lpwszTemplate	= MAKEINTRESOURCE( IDD_ADVANCED );
	PageInfo[6].lParam			= (LPARAM)(new CAdvancedPage( Setting ));
	PageInfo[6].hInstance		= Instance;
}

//----------------------------------------------------------------------------------------------
//	~CPropertySheet
//----------------------------------------------------------------------------------------------
CPropertySheet::~CPropertySheet( VOID )
{
	//	各インスタンスを開放する
	delete Setting;
}

//----------------------------------------------------------------------------------------------
//	QueryInterface
//----------------------------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CPropertySheet::QueryInterface(
	 REFIID		InterfaceID
	,PVOID *	Interface )
{
	//	変数宣言
	HRESULT	Result	= S_OK;

	//	自身のインターフェイスを返す
	if(	IsEqualIID( InterfaceID, IID_IUnknown )
	||	IsEqualIID( InterfaceID, IID_IDIGameCntrlPropSheet ) )
	{
		AddRef();
		*Interface	= this;
	} else {
		*Interface	= NULL;
		Result		= E_NOINTERFACE;
	}

	return( Result );
}

//----------------------------------------------------------------------------------------------
//	AddRef
//----------------------------------------------------------------------------------------------
ULONG STDMETHODCALLTYPE CPropertySheet::AddRef( VOID )
{
	//	参照カウンタを加算する
	return( InterlockedIncrement( &ReferenceCount ) );
}

//----------------------------------------------------------------------------------------------
//	Release
//----------------------------------------------------------------------------------------------
ULONG STDMETHODCALLTYPE CPropertySheet::Release( VOID )
{
	//	参照カウンタを減算する
	if( InterlockedDecrement( &ReferenceCount ) == 0 )
	{
		delete this;
		return( 0 );
	}

	return( ReferenceCount );
}

//----------------------------------------------------------------------------------------------
//	GetSheetInfo
//----------------------------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CPropertySheet::GetSheetInfo( LPDIGCSHEETINFO * SheetInfo )
{
	//	プロパティ シート情報を返す
	*SheetInfo	= &this->SheetInfo;

	return( S_OK );
}

//----------------------------------------------------------------------------------------------
//	GetPageInfo
//----------------------------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CPropertySheet::GetPageInfo( LPDIGCPAGEINFO * PageInfo )
{
	//	プロパティ シートのページ情報を返す
	*PageInfo	= this->PageInfo;

	return( S_OK );
}

//----------------------------------------------------------------------------------------------
//	SetID
//----------------------------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CPropertySheet::SetID( USHORT )
{
	return S_OK;
}

//----------------------------------------------------------------------------------------------
//	GetID
//----------------------------------------------------------------------------------------------
USHORT STDMETHODCALLTYPE CPropertySheet::GetID()
{
	return 0;
}
