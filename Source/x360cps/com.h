
//	Property Sheet for Microsoft Xbox 360 Controller

#pragma	once

#include	<windows.h>
#include	<objbase.h>

//----------------------------------------------------------------------------------------------
//	クラス
//----------------------------------------------------------------------------------------------

class CClassFactory : public IClassFactory
{

public:

	//	コンストラクタ
	CClassFactory( VOID );

	//	IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface( REFIID, PVOID * );
	ULONG STDMETHODCALLTYPE AddRef( VOID );
	ULONG STDMETHODCALLTYPE Release( VOID );

	//	IClassFactory
	HRESULT STDMETHODCALLTYPE CreateInstance( IUnknown *, REFIID, PVOID * );
	HRESULT STDMETHODCALLTYPE LockServer( BOOL );

private:

	//	参照カウンタ
	LONG	ReferenceCount;

};
