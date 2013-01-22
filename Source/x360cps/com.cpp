
//	Property Sheet for Microsoft Xbox 360 Controller

#include	"com.h"
#include	"property_sheet.h"

//----------------------------------------------------------------------------------------------
//	CClassFactory
//----------------------------------------------------------------------------------------------
CClassFactory::CClassFactory( VOID )
{
	//	参照カウンタを初期化する
	ReferenceCount	= 1;
}

//----------------------------------------------------------------------------------------------
//	QueryInterface
//----------------------------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CClassFactory::QueryInterface(
	 REFIID		InterfaceID
	,PVOID *	Interface )
{
	//	変数宣言
	HRESULT	Result	= S_OK;

	//	自身のインターフェイスを返す
	if(	IsEqualIID( InterfaceID, IID_IUnknown )
	||	IsEqualIID( InterfaceID, IID_IClassFactory ) )
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
ULONG STDMETHODCALLTYPE CClassFactory::AddRef( VOID )
{
	//	参照カウンタを加算する
	return( InterlockedIncrement( &ReferenceCount ) );
}

//----------------------------------------------------------------------------------------------
//	Release
//----------------------------------------------------------------------------------------------
ULONG STDMETHODCALLTYPE CClassFactory::Release( VOID )
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
//	CreateInstance
//----------------------------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CClassFactory::CreateInstance(
	 IUnknown *	UnknownInterface
	,REFIID		InterfaceID
	,PVOID *	Interface )
{
	//	変数宣言
	HRESULT	Result	= S_OK;

	//	IDIGameCntrlPropSheet を作成する
	CPropertySheet *	PropertySheet	= new CPropertySheet();
	if( PropertySheet != NULL )
	{
		Result	= PropertySheet->QueryInterface( InterfaceID, Interface );
		PropertySheet->Release();
	} else {
		Result	= E_OUTOFMEMORY;
	}

	return( Result );
}

//----------------------------------------------------------------------------------------------
//	LockServer
//----------------------------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE CClassFactory::LockServer( BOOL Lock )
{
	return( S_OK );
}
