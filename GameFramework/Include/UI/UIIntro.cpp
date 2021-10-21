
#include "UIIntro.h"
#include "UIIntroBack.h"
#include "../UI/UIIntroLogo.h"
#include "../GameManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/MenuScene.h"
#include "UIText.h"

CUIIntro::CUIIntro()
{
}

CUIIntro::~CUIIntro()
{
}

bool CUIIntro::Init()
{
	Resolution	RS = CGameManager::GetInst()->GetResolution();

	CUIIntroBack* IntroBack = CreateWidget<CUIIntroBack>("UIIntroBack");
	IntroBack->SetClickCallback<CUIIntro>(this, &CUIIntro::IntroClick);

	CUIIntroLogo* IntroLogo = CreateWidget<CUIIntroLogo>("UIIntroLogo");
	IntroLogo->SetZOrder(1);
	
	CUIText* Text = CreateWidget<CUIText>("IntroText");
	Text->SetText(TEXT("CLICK TO START"));
	Text->SetTextColor(255, 255, 255);
	Text->SetPos(RS.Width / 2.f - 50.f, RS.Height / 2.f + 155.f);
	Text->SetZOrder(2);

	return true;
}

void CUIIntro::IntroClick()
{
	CSceneManager::GetInst()->CreateScene<CMenuScene>();
}

