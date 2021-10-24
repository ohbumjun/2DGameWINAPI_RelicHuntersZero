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

void CUISelect::CharacterBarInit()
{
	AssInit();
	BiuInit();
	JimmyInit();
	PingkyInit();
	PunnyInit();
	RaffInit();
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

void CUISelect::BiuInit()
{
	CUIBtnAnimation* BiuBtn = CreateWidget<CUIBtnAnimation>("BiuDefaultBar");
	BiuBtn->SetDefaultAnimation(CARD_BIU_DEFAULT);
	BiuBtn->SetClickAnimation(CARD_BIU_CLICK);
	BiuBtn->SetMouseOnAnimation(CARD_BIU_HOVER);
	BiuBtn->SetPos(950.f, 150.f);
	BiuBtn->SetMouseOnSound("ButtonMouseOn");
	BiuBtn->SetClickSound("ButtonClick");
	BiuBtn->SetZOrder(3);
}

void CUISelect::JimmyInit()
{
	CUIBtnAnimation* JimmyBtn = CreateWidget<CUIBtnAnimation>("JimmyDefaultBar");
	JimmyBtn->SetDefaultAnimation(CARD_JIMMY_DEFAULT);
	JimmyBtn->SetClickAnimation(CARD_JIMMY_CLICK);
	JimmyBtn->SetMouseOnAnimation(CARD_JIMMY_HOVER);
	JimmyBtn->SetPos(950.f, 250.f);
	JimmyBtn->SetMouseOnSound("ButtonMouseOn");
	JimmyBtn->SetClickSound("ButtonClick");
	JimmyBtn->SetZOrder(3);
}

void CUISelect::PingkyInit()
{
	CUIBtnAnimation* PinkyBtn = CreateWidget<CUIBtnAnimation>("PinkyDefaultBar");
	PinkyBtn->SetDefaultAnimation(CARD_PINKY_DEFAULT);
	PinkyBtn->SetClickAnimation(CARD_PINKY_CLICK);
	PinkyBtn->SetMouseOnAnimation(CARD_PINKY_HOVER);
	PinkyBtn->SetPos(950.f, 350.f);
	PinkyBtn->SetMouseOnSound("ButtonMouseOn");
	PinkyBtn->SetClickSound("ButtonClick");
	PinkyBtn->SetZOrder(3);
}

void CUISelect::PunnyInit()
{
	CUIBtnAnimation* PunnyBtn = CreateWidget<CUIBtnAnimation>("PunnyDefaultBar");
	PunnyBtn->SetDefaultAnimation(CARD_PUNNY_DEFAULT);
	PunnyBtn->SetClickAnimation(CARD_PUNNY_CLICK);
	PunnyBtn->SetMouseOnAnimation(CARD_PUNNY_HOVER);
	PunnyBtn->SetPos(950.f, 450.f);
	PunnyBtn->SetMouseOnSound("ButtonMouseOn");
	PunnyBtn->SetClickSound("ButtonClick");
	PunnyBtn->SetZOrder(3);
}

void CUISelect::RaffInit()
{
	CUIBtnAnimation* RaffBtn = CreateWidget<CUIBtnAnimation>("RaffDefaultBar");
	RaffBtn->SetDefaultAnimation(CARD_RAFF_DEFAULT);
	RaffBtn->SetClickAnimation(CARD_RAFF_CLICK);
	RaffBtn->SetMouseOnAnimation(CARD_RAFF_HOVER);
	RaffBtn->SetPos(950.f, 550.f);
	RaffBtn->SetMouseOnSound("ButtonMouseOn");
	RaffBtn->SetClickSound("ButtonClick");
	RaffBtn->SetZOrder(3);
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

	CharacterBarInit();

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
