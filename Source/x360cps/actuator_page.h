
//	Property Sheet for Microsoft Xbox 360 Controller

#pragma	once

#include	"page.h"

//----------------------------------------------------------------------------------------------
//	クラス
//----------------------------------------------------------------------------------------------

class CActuatorPage : public CPage
{

public:

	//	コンストラクタ
	CActuatorPage( CSetting * );

	//	ダイアログを初期化する
	VOID wmInitDialog( HWND );
	//	コマンドを処理する
	VOID wmCommand( HWND, WORD, WORD );
	//	水平スクロールを処理する
	VOID wmHScroll( HWND, WORD );

	//	設定を反映する
	VOID ApplySetting();

private:

	//	トラック バーの選択状態
	BOOL	TrackBarSelecting;

protected:

};
