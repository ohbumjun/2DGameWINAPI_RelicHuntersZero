
#include "UIIntro.h"
#include "UIIntroBack.h"
#include "../UI/UIIntroLogo.h"
#include "../GameManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/StartScene.h"
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
	
	/*
	CUIImage* Back = CreateWidget<CUIImage>("Back");
	Back->SetTexture("IntroBack", TEXT("GameBack.bmp"));

	// Intro ==
	CButton* Button = CreateWidget<CButton>("IntroButton");
	Button->SetTexture("IntroButton", TEXT("ButtonBack.bmp"));
	Button->SetPos(RS.Width / 2.f - 100.f, RS.Height / 2.f - 175.f);
	Button->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(200.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Click, Vector2(400.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Disable, Vector2(600.f, 0.f), Vector2(200.f, 100.f));
	Button->SetMouseOnSound("ButtonMouseOn");
	Button->SetClickSound("ButtonClick");
	Button->SetZOrder(1);

	Button->SetClickCallback<CUIIntro>(this, &CUIIntro::IntroClick);

	CUIText* Text = CreateWidget<CUIText>("IntroButtonText");

	Text->SetText(TEXT("Intro"));
	Text->SetTextColor(100, 150, 200);
	Text->SetPos(RS.Width / 2.f - 50.f, RS.Height / 2.f - 155.f);
	Text->SetZOrder(2);
	*/

	return true;
}

void CUIIntro::IntroClick()
{
	CSceneManager::GetInst()->CreateScene<CStartScene>();
	// CSceneManager::GetInst()->CreateScene<CMainScene>();
	// CSceneManager::GetInst()->CreateScene<CMain2Scene>();
	// CSceneManager::GetInst()->CreateScene<CMain3Scene>();
}

