#include "UISelect.h"
#include "../GameManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/HomeScene.h"
#include "../Scene/StartScene.h"
#include "../UI/Button.h"
#include "../UI/UIBtnAnimation.h"
#include "UIText.h"
#include "UIImage.h"

CUISelect::CUISelect() : 
	m_CharImg(nullptr),
	m_CharInfo{},
	m_CommonElemInfo{},
	m_BtnAnimations{}
{
}

CUISelect::~CUISelect()
{
	m_BtnAnimations.clear();
	m_CommonElemInfo.clear();
	m_CharInfo.clear();
}

void CUISelect::CharacterClick(EChar_Type& CharType)
{
	// Delete From Widget Array

	// Delete From Character List
	SetCharacterInfoBox();
	SetCharacterInfo(CharType);
}

void CUISelect::SetCharacterInfoBox()
{
	if (m_CommonElemInfo.empty())
	{
		CUIImage* Box = CreateWidget<CUIImage>("CharacterBackBox");
		Box->SetTexture("CharacterBackBox", TEXT("images/MenuScene/background3.bmp"));
		Box->SetTextureColorKey(255, 255, 255);
		Box->SetPos(150.f, 200.f);
		Box->SetZOrder(2);
		m_CommonElemInfo.push_back(Box);

		Box = CreateWidget<CUIImage>("CharacterInfoBlock");
		Box->SetTexture("CharacterInfoBlock", TEXT("images/MenuScene/background2.bmp"));
		Box->SetTextureColorKey(255, 255, 255);
		Box->SetPos(300.f, 260.f);
		Box->SetZOrder(3);
		m_CommonElemInfo.push_back(Box);
	}
}

void CUISelect::SetCharacterInfo(EChar_Type& CharType)
{
	SetCharacterAbility();
	SetCharacterImg(CharType);
}

void CUISelect::SetCharacterAbility()
{
	m_CharInfo.clear();
	// Delete From Widget List

	// Make New Widget 
}

void CUISelect::SetCharacterImg(EChar_Type& CharType)
{
	// Make New Widget 
	if(!m_CharImg)
		m_CharImg = CreateWidget<CUIImage>("CharImg");
	switch (CharType)
	{
	case EChar_Type::Ass:
		m_CharImg->SetTexture("CharImg", TEXT(CARD_IMG_PATH_ASS));
		break;
	case EChar_Type::Biu:
		m_CharImg->SetTexture("CharImg", TEXT(CARD_IMG_PATH_BIU));
		break;
	case EChar_Type::Jimmy:
		m_CharImg->SetTexture("CharImg", TEXT(CARD_IMG_PATH_JIMMY));
		break;
	case EChar_Type::Pinky:
		m_CharImg->SetTexture("CharImg", TEXT(CARD_IMG_PATH_PINKY));
		break;
	case EChar_Type::Punny:
		m_CharImg->SetTexture("CharImg", TEXT(CARD_IMG_PATH_PUNNY));
		break;
	case EChar_Type::Raff:
		m_CharImg->SetTexture("CharImg", TEXT(CARD_IMG_PATH_RAFF));
		break;
	}
	m_CharImg->SetTextureColorKey(255, 255, 255);
	m_CharImg->SetPos(50.f, 250.f);
	m_CharImg->SetZOrder(4);
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
	CSharedPtr<CUIBtnAnimation> AssBtn = CreateWidget<CUIBtnAnimation>("AssDefaultBar");
	AssBtn->SetDefaultAnimation(CARD_ASS_DEFAULT);
	AssBtn->SetClickAnimation(CARD_ASS_CLICK);
	AssBtn->SetMouseOnAnimation(CARD_ASS_HOVER);
	AssBtn->SetPos(950.f, 50.f);
	AssBtn->SetMouseOnSound("ButtonMouseOn");
	AssBtn->SetClickSound("ButtonClick");
	AssBtn->SetZOrder(3);
	AssBtn->SetCharType(EChar_Type::Ass);

	m_BtnAnimations.insert(std::make_pair("Ass", AssBtn));
	AssBtn->SetOwner(this);
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
	BiuBtn->SetCharType(EChar_Type::Biu);

	m_BtnAnimations.insert(std::make_pair("Biu", BiuBtn));
	BiuBtn->SetOwner(this);
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
	JimmyBtn->SetCharType(EChar_Type::Jimmy);

	m_BtnAnimations.insert(std::make_pair("Jimmy", JimmyBtn));
	JimmyBtn->SetOwner(this);
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
	PinkyBtn->SetCharType(EChar_Type::Pinky);

	m_BtnAnimations.insert(std::make_pair("Pinky", PinkyBtn));
	PinkyBtn->SetOwner(this);
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
	PunnyBtn->SetCharType(EChar_Type::Punny);

	m_BtnAnimations.insert(std::make_pair("Punny", PunnyBtn));
	PunnyBtn->SetOwner(this);
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
	RaffBtn->SetCharType(EChar_Type::Raff);

	m_BtnAnimations.insert(std::make_pair("Raff", RaffBtn));
	RaffBtn->SetOwner(this);
}

void CUISelect::ResetClick(const CUIBtnAnimation* BtnAnim)
{
	auto iter    = m_BtnAnimations.begin();
	auto iterEnd = m_BtnAnimations.end();
	for (; iter != iterEnd; ++iter)
	{
		if (iter->second->GetName() == BtnAnim->GetName()) continue;
		if (iter->second->GetCardSelected())
		{
			iter->second->SetCardSelected(false);
		}
	}
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

	return true;
}

void CUISelect::StartClick()
{
	CSceneManager::GetInst()->CreateScene<CHomeScene>();
}

void CUISelect::ExitClick()
{
	CGameManager::GetInst()->Exit();
}
