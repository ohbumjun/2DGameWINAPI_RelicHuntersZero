
#include "UIEnd.h"
#include "../GameManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/MenuScene.h"
#include "../Scene/EndScene.h"
#include "UIText.h"
#include "UIImage.h"
#include "UIIntroLogo.h"

CUIEnd::CUIEnd()
{
}

CUIEnd::~CUIEnd()
{
}

bool CUIEnd::Init()
{
	Resolution	RS = CGameManager::GetInst()->GetResolution();

	CUIImage* Back = CreateWidget<CUIImage>("Back");
	Back->SetTexture("Back", TEXT("images/MenuScene/bg_end.bmp"));
	
	CUIIntroLogo* IntroLogo = CreateWidget<CUIIntroLogo>("UIIntroLogo");
	IntroLogo->SetZOrder(1);

	CUIText* Text = CreateWidget<CUIText>("EndText");
	Text->SetText(TEXT("THE END"));
	Text->SetTextColor(255, 255, 255);
	Text->SetPos(RS.Width / 2.f - 150.f, RS.Height / 2.f + 155.f);
	Text->SetZOrder(2);



	return true;
}
