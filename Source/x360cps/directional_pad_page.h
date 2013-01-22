
//	Property Sheet for Microsoft Xbox 360 Controller

#pragma	once

#include	"page.h"

//----------------------------------------------------------------------------------------------
//	クラス
//----------------------------------------------------------------------------------------------

class CDirectionalPadPage : public CPage
{

public:

	//	コンストラクタ
	CDirectionalPadPage( CSetting * );

	//	ダイアログを初期化する
	VOID wmInitDialog( HWND );
	//	コマンドを処理する
	VOID wmCommand( HWND, WORD, WORD );

	//	設定を反映する
	VOID ApplySetting();

private:

protected:

};
