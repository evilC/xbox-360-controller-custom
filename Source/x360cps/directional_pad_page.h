
//	Property Sheet for Microsoft Xbox 360 Controller

#pragma	once

#include	"page.h"

//----------------------------------------------------------------------------------------------
//	�N���X
//----------------------------------------------------------------------------------------------

class CDirectionalPadPage : public CPage
{

public:

	//	�R���X�g���N�^
	CDirectionalPadPage( CSetting * );

	//	�_�C�A���O������������
	VOID wmInitDialog( HWND );
	//	�R�}���h����������
	VOID wmCommand( HWND, WORD, WORD );

	//	�ݒ�𔽉f����
	VOID ApplySetting();

private:

protected:

};