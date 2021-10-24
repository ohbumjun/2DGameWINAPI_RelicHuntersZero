#include "UISelect.h"
#include "../GameManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/HomeScene.h"
#include "../Scene/StartScene.h"
#include "../UI/Button.h"
#include "../UI/UIBtnAnimation.h"
#include "UIText.h"
#include "UIImage.h"

CUISelect::CUISelect()
{
}

CUISelect::~CUISelect()
{
}

void CUISelect::CharacterClick()
{
	CUIImage* Back = CreateWidget<CUIImage>("CharacterBack");
	Back->SetTexture("CharacterBack", TEXT("images/MenuScene/background3.bmp"));
	Back->SetTextureColorKey(255, 255, 255);
	Back->SetPos(150.f, 200.f);
	Back->SetZOrder(2);

	Back = CreateWidget<CUIImage>("CharacterInfoBlock");
	Back->SetTexture("CharacterInfoBlock", TEXT("images/MenuScene/background2.bmp"));
	Back->SetTextureColorKey(255, 255, 255);
	Back->SetPos(300.f, 260.f);
	Back->SetZOrder(3);
}

void CUISelect::AssInit()
{
	CUIBtnAnimation* AssBtn = CreateWidget<CUIBtnAnimation>("AssDefaultBar");
	AssBtn->SetDefaultAnimation(CARD_ASS_DEFAULT);
	AssBtn->SetClickAnimation(CARD_ASS_CLICK);
	AssBtn->SetMouseOnAnimation(CARD_ASS_HOVER);
	AssBtn->SetPos(950.f, 50.f);
	AssBtn->SetMouseOnSound("ButtonMouseOn");
	AssBtn->SetClickSound("ButtonClick");
	AssBtn->SetZOrder(3);
}

bool CUISelect::Init()
{
	Resolution	RS = CGameManager::GetInst()->GetResolution();

	CUIImage* Back = CreateWidget<CUIImage>("Back");
	Back->SetTexture("MenuBack", TEXT("images/MenuScene/background0.bmp"));

	Back = CreateWidget<CUIImage>("CharacterSelectionBar");
	Back->SetTexture("Bar", TEXT("images/MenuScene/spr_menu_button_1.bmp"));
	Back->SetTextureColorKey(255, 255, 255);
	Back->SetPos(-50.f, 25.f);
	Back->SetZOrder(1);

	Back = CreateWidget<CUIImage>("BGBar2");
	Back->SetTexture("BGBar2", TEXT("images/MenuScene/background1.bmp"));
	Back->SetTextureColorKey(255, 255, 255);
	Back->SetPos(900.f, 0.f);
	Back->SetZOrder(1);

	CUIText* IntroBar = CreateWidget<CUIText>("BarTxt");
	IntroBar->SetTextColor(255, 255, 255);
	IntroBar->SetPos(50.f, 35.f);
	IntroBar->SetZOrder(2);
	IntroBar->SetText(TEXT("Character Selection"));

	AssInit();

	CharacterClick();

	return true;
}

void CUISelect::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
}


void CUISelect::StartClick()
{
	CSceneManager::GetInst()->CreateScene<CHomeScene>();
}

void CUISelect::ExitClick()
{
	CGameManager::GetInst()->Exit();
}
