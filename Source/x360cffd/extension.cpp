
//	Force Feedback Driver for Microsoft Xbox 360 Controller

#include	"extension.h"
#include	"com.h"

//----------------------------------------------------------------------------------------------
//	グローバル変数
//----------------------------------------------------------------------------------------------

CRITICAL_SECTION	CriticalSection;

//----------------------------------------------------------------------------------------------
//	DllMain
//----------------------------------------------------------------------------------------------
BOOL WINAPI DllMain(
	 HINSTANCE	Instance
	,DWORD		Reason
	,LPVOID		Reserved )
{
	switch( Reason )
	{
		case DLL_PROCESS_ATTACH:
			//	クリティカル セクションを初期化する
			InitializeCriticalSection( &CriticalSection );
			break;

		case DLL_PROCESS_DETACH:
			//	クリティカル セクションを削除する
			DeleteCriticalSection( &CriticalSection );
			break;
	}

	return( TRUE );
}

//----------------------------------------------------------------------------------------------
//	DllGetClassObject
//----------------------------------------------------------------------------------------------
STDAPI DllGetClassObject(
	 REFCLSID	ClassID
	,REFIID		InterfaceID
	,LPVOID *	Interface )
{
	//	変数宣言
	HRESULT	Result	= S_OK;

	//	IClassFactory を作成する
	if( IsEqualGUID( ClassID, ClassID_X360CFFD ) )
	{
		CClassFactory *	Factory	= new CClassFactory();
		if( Factory != NULL )
		{
			Result	= Factory->QueryInterface( InterfaceID, Interface );
			Factory->Release();
		} else {
			Result	= E_OUTOFMEMORY;
		}
	} else {
		Result	= CLASS_E_CLASSNOTAVAILABLE;
	}

	return( Result );
}

//----------------------------------------------------------------------------------------------
//	DllCanUnloadNow
//----------------------------------------------------------------------------------------------
STDAPI DllCanUnloadNow( VOID )
{
	return( S_OK );
}
