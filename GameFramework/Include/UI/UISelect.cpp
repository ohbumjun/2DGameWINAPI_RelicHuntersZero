#include "UISelect.h"
#include "../GameManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/HomeScene.h"
#include "../Scene/StartScene.h"
#include "Button.h"
#include "UIBtnAnimation.h"
#include "UIText.h"
#include "UIImage.h"
#include "../Object/CharacterManager.h"

CUISelect::CUISelect() : 
	m_CharImg(nullptr),
	m_CharImgBackGrounds{},
	m_BtnAnimations{},
	m_CharAbilityTexts{},
	m_StatsUIs{}
{
}

CUISelect::~CUISelect()
{
	m_BtnAnimations.clear();
	m_CharImgBackGrounds.clear();
}

void CUISelect::CharacterClick(EChar_Type& CharType)
{
	// Delete From Widget Array

	// Delete From Character List
	SetCharacterInfoBox();
	SetCharacterAbility(CharType);
	SetCharacterImg(CharType);

}

void CUISelect::SetCharacterInfoBox()
{
	auto iter = m_CharImgBackGrounds.begin();
	auto iterEnd = m_CharImgBackGrounds.end();
	for (; iter != iterEnd; ++iter)
		(*iter)->SetVisibility(true);
}
void CUISelect::SetCharacterAbility(EChar_Type& CharType)
{
	// Ability Texts
	auto iter    = m_CharAbilityTexts.begin();
	auto iterEnd = m_CharAbilityTexts.end();
	for (; iter != iterEnd; iter++)
		(*iter)->SetVisibility(true);

	// Ability Stats
	for (int i = 0; i < m_StatsUIs.size(); i++)
		m_StatsUIs[i]->SetVisibility(false);
	CharacterInfo CharInfo = CCharacterManager::GetInst()->FindCharInfo(CharType);
	for (int i = 0; i < 5; i++)
	{
		int StIdx = i * 5;
		if(i == 0) SetAttackAbility(CharInfo, StIdx);
		if(i == 1) SetArmorAbility(CharInfo, StIdx);
		if(i == 2) SetHPAbility(CharInfo, StIdx);
		if(i == 3) SetMPAbility(CharInfo, StIdx);
		if(i == 4) SetSpeedAbility(CharInfo, StIdx);
	}
}

void CUISelect::SetCharacterImg(EChar_Type& CharType)
{
	// Make New Widget 
	if(!m_CharImg)
		m_CharImg = CreateWidget<CUIImage>("CharImg");
	switch (CharType)
	{
	case EChar_Type::Ass:
		m_CharImg->SetTexture("AssImg", TEXT(CARD_IMG_PATH_ASS));
		break;
	case EChar_Type::Biu:
		m_CharImg->SetTexture("BiuImg", TEXT(CARD_IMG_PATH_BIU));
		break;
	case EChar_Type::Jimmy:
		m_CharImg->SetTexture("JimmyImg", TEXT(CARD_IMG_PATH_JIMMY));
		break;
	case EChar_Type::Pinky:
		m_CharImg->SetTexture("PinkyImg", TEXT(CARD_IMG_PATH_PINKY));
		break;
	case EChar_Type::Punny:
		m_CharImg->SetTexture("PunnyImg", TEXT(CARD_IMG_PATH_PUNNY));
		break;
	case EChar_Type::Raff:
		m_CharImg->SetTexture("RaffImg", TEXT(CARD_IMG_PATH_RAFF));
		break;
	}
	m_CharImg->SetTextureColorKey(255, 255, 255);
	m_CharImg->SetPos(50.f, 250.f);
	m_CharImg->SetZOrder(4);
}

void CUISelect::SetHPAbility(CharacterInfo &CharInfo, int StartIdx)
{
	int CharHPMax  = CCharacterManager::GetInst()->GetCharMaxHP();
	int CharHP     = CharInfo.HPMax;
	int HPRatio    = (int)((CharHP / (float)CharHPMax) * 10);
	int NumStats   = (int)(HPRatio / 2.f);
	for (int i = StartIdx; i < StartIdx + NumStats; i++)
		m_StatsUIs[i]->SetVisibility(true);
}

void CUISelect::SetMPAbility(CharacterInfo &CharInfo, int StartIdx)
{
	int CharMPMax = CCharacterManager::GetInst()->GetCharMaxMP();
	int CharMP = CharInfo.MPMax;
	int MPRatio = (int)((CharMP / (float)CharMPMax) * 10);
	int NumStats = (int)(MPRatio / 2.f);
	for (int i = StartIdx; i < StartIdx + NumStats; i++)
		m_StatsUIs[i]->SetVisibility(true);
}

void CUISelect::SetAttackAbility(CharacterInfo &CharInfo, int StartIdx)
{
	int CharAttackMax = CCharacterManager::GetInst()->GetCharMaxAttack();
	int CharAttack = CharInfo.Attack;
	int AttackRatio = (int)((CharAttack / (float)CharAttackMax) * 10);
	int NumStats = (int)(AttackRatio / 2.f);
	for (int i = StartIdx; i < StartIdx + NumStats; i++)
		m_StatsUIs[i]->SetVisibility(true);
}

void CUISelect::SetArmorAbility(CharacterInfo &CharInfo, int StartIdx)
{
	int CharArmorMax = CCharacterManager::GetInst()->GetCharMaxArmor();
	int CharArmor = CharInfo.Armor;
	int ArmorRatio = (int)((CharArmor / (float)CharArmorMax) * 10);
	int NumStats = (int)(ArmorRatio / 2.f);
	for (int i = StartIdx; i < StartIdx + NumStats; i++)
		m_StatsUIs[i]->SetVisibility(true);
}

void CUISelect::SetSpeedAbility(CharacterInfo &CharInfo, int StartIdx)
{
	int CharSpeedMax = CCharacterManager::GetInst()->GetCharMaxSpeed();
	int CharSpeed = CharInfo.MoveSpeed;
	int SpeedRatio = (int)((CharSpeed / (float)CharSpeedMax) * 10);
	int NumStats = (int)(SpeedRatio / 2.f);
	for (int i = StartIdx; i < StartIdx + NumStats; i++)
		m_StatsUIs[i]->SetVisibility(true);
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

void CUISelect::CharacterStatsInit()
{
	// Texts
	m_CharAbilityTexts.reserve(5);
	for (int i = 0; i < 5; i++)
	{
		m_CharAbilityTexts.push_back(CreateWidget<CUIText>("AbilityText"));
		m_CharAbilityTexts[i]->SetTextColor(255, 255, 255);
		m_CharAbilityTexts[i]->SetPos(330.f, 280.f + 40.f * i);
		m_CharAbilityTexts[i]->SetZOrder(4);
		m_CharAbilityTexts[i]->SetVisibility(false);
	}
	m_CharAbilityTexts[0]->SetText(TEXT("ATTACK"));
	m_CharAbilityTexts[1]->SetText(TEXT("ARMOR"));
	m_CharAbilityTexts[2]->SetText(TEXT("HP"));
	m_CharAbilityTexts[3]->SetText(TEXT("MP"));
	m_CharAbilityTexts[4]->SetText(TEXT("SPEED"));

	// Bar 
	m_StatsUIs.reserve(25);
	CUIImage* Stat = nullptr;
	for (int i = 0; i < 5; i++)
	{
		TCHAR	FileName[256] = {};
		wsprintf(FileName, TEXT("images/Character/Stats/spr_char_statBar_%d.bmp"), i);
		for (int j = 0; j < 5; j++)
		{
			const std::string StatWidName = std::to_string(i * 5 + j);
			Stat = CreateWidget<CUIImage>(StatWidName);
			Stat->SetTexture(StatWidName, FileName);
			Stat->SetTextureColorKey(255, 255, 255);
			Stat->SetPos(430.f + j * 20.f, 280.f + i * 40.f);
			Stat->SetZOrder(5);
			Stat->SetVisibility(false);
			m_StatsUIs.push_back(Stat);
		}
	}
	
	// BackGround Boxes
	CUIImage* Box = CreateWidget<CUIImage>("CharacterBackBox");
	Box->SetTexture("CharacterBackBox", TEXT("images/MenuScene/background3.bmp"));
	Box->SetTextureColorKey(255, 255, 255);
	Box->SetPos(150.f, 200.f);
	Box->SetZOrder(2);
	Box->SetVisibility(false);
	m_CharImgBackGrounds.push_back(Box);

	Box = CreateWidget<CUIImage>("CharacterInfoBlock");
	Box->SetTexture("CharacterInfoBlock", TEXT("images/MenuScene/background2.bmp"));
	Box->SetTextureColorKey(255, 255, 255);
	Box->SetPos(300.f, 260.f);
	Box->SetZOrder(3);
	Box->SetVisibility(false);
	m_CharImgBackGrounds.push_back(Box);
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
	CharacterStatsInit();

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
