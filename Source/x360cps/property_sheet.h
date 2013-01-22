
//	Property Sheet for Microsoft Xbox 360 Controller

#pragma	once

#include	<windows.h>
#include	<commctrl.h>
#include	<objbase.h>
#include	<initguid.h>

#include	"setting.h"

//----------------------------------------------------------------------------------------------
//	GUID
//----------------------------------------------------------------------------------------------

DEFINE_GUID( IID_IDIGameCntrlPropSheet,
	0x7854FB22, 0x8EE3, 0x11D0, 0xA1, 0xAC, 0x00, 0x00, 0xF8, 0x02, 0x69, 0x77 );

//----------------------------------------------------------------------------------------------
//	構造体
//----------------------------------------------------------------------------------------------

typedef struct
{
	DWORD		dwSize;
	USHORT		nNumPages;
	LPWSTR		lpwszSheetCaption;
	BOOL		fSheetIconFlag;
	LPWSTR		lpwszSheetIcon;
} DIGCSHEETINFO, *LPDIGCSHEETINFO;

typedef struct
{
	DWORD		dwSize;
	LPWSTR		lpwszPageTitle;
	DLGPROC		fpPageProc;
	BOOL		fProcFlag;
	DLGPROC		fpPrePostProc;
	BOOL		fIconFlag;
	LPWSTR		lpwszPageIcon;
	LPWSTR		lpwszTemplate;
	LPARAM		lParam;
	HINSTANCE	hInstance;
} DIGCPAGEINFO, *LPDIGCPAGEINFO;

//----------------------------------------------------------------------------------------------
//	インターフェイス
//----------------------------------------------------------------------------------------------

#undef INTERFACE
#define INTERFACE IDIGameCntrlPropSheet

DECLARE_INTERFACE_( IDIGameCntrlPropSheet, IUnknown )
{
	//	IUnknown
	STDMETHOD( QueryInterface )( THIS_ REFIID, LPVOID * ) PURE;
	STDMETHOD_( ULONG, AddRef )( THIS ) PURE;
	STDMETHOD_( ULONG, Release )( THIS ) PURE;

	//	IDIGameCntrlPropSheet
	STDMETHOD( GetSheetInfo )( THIS_ LPDIGCSHEETINFO * ) PURE;
	STDMETHOD( GetPageInfo )( THIS_ LPDIGCPAGEINFO * ) PURE;
	STDMETHOD( SetID )( THIS_ USHORT ) PURE;
	STDMETHOD_( USHORT, GetID )( THIS ) PURE;
};

//----------------------------------------------------------------------------------------------
//	クラス
//----------------------------------------------------------------------------------------------

class CPropertySheet : public IDIGameCntrlPropSheet
{

public:

	//	コンストラクタ
	CPropertySheet( VOID );
	//	デストラクタ
	~CPropertySheet( VOID );

	//	IUnknown
	HRESULT STDMETHODCALLTYPE QueryInterface( REFIID, PVOID * );
	ULONG STDMETHODCALLTYPE AddRef( VOID );
	ULONG STDMETHODCALLTYPE Release( VOID );

	//	IDIGameCntrlPropSheet
	HRESULT STDMETHODCALLTYPE GetSheetInfo( LPDIGCSHEETINFO * );
	HRESULT STDMETHODCALLTYPE GetPageInfo( LPDIGCPAGEINFO * );
	HRESULT STDMETHODCALLTYPE SetID( USHORT );
	USHORT STDMETHODCALLTYPE GetID( VOID );

	//	プロパティ シートの情報
	DIGCSHEETINFO	SheetInfo;
	WCHAR			SheetCaption[MAX_PATH];

	//	プロパティ シートのページの情報
	DIGCPAGEINFO	PageInfo[7];

	//	設定
	CSetting *		Setting;

private:

	//	参照カウンタ
	LONG	ReferenceCount;

};
