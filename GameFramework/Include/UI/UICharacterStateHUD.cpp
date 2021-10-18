
#include "UICharacterStateHUD.h"
#include "../GameManager.h"
#include "../Scene/MainScene.h"
#include "../Scene/SceneManager.h"

CUICharacterStateHUD::CUICharacterStateHUD() :
										m_HPBar(nullptr),
										m_MPBar(nullptr),
										m_SteminaBar(nullptr),
										m_GoldImage(nullptr),
										m_GoldHunderedWidget(nullptr),
										m_GoldTenWidget(nullptr),
										m_GoldOneWidget(nullptr)
{
}

CUICharacterStateHUD::~CUICharacterStateHUD()
{
}

bool CUICharacterStateHUD::Init()
{
	SetPos(30.f, 20.f);

	Resolution	RS = CGameManager::GetInst()->GetResolution();

	// Ability 
	CUIImage* Back = CreateWidget<CUIImage>("HPBarBack");
	Back->SetTexture("BarBack", TEXT("BarBack.bmp"));
	Back->SetTextureColorKey(255, 0, 255);
	m_HPBar = CreateWidget<CProgressBar>("HPBar");
	m_HPBar->SetTexture("HPBar", TEXT("BarDefault.bmp"));
	m_HPBar->SetPos(10.f, 10.f);

	Back = CreateWidget<CUIImage>("MPBarBack");
	Back->SetTexture("BarBack", TEXT("BarBack.bmp"));
	Back->SetTextureColorKey(255, 0, 255);
	Back->SetPos(0, 40.f);
	m_MPBar = CreateWidget<CProgressBar>("MPBar");
	m_MPBar->SetTexture("MPBar", TEXT("BarMP.bmp"));
	m_MPBar->SetPos(10.f, 50.f);

	Back = CreateWidget<CUIImage>("SteminaBarBack");
	Back->SetTexture("BarBack", TEXT("BarBack.bmp"));
	Back->SetTextureColorKey(255, 0, 255);
	Back->SetPos(0, 80.f);
	m_SteminaBar = CreateWidget<CProgressBar>("SteminaBar");
	m_SteminaBar->SetTexture("SteminaBar", TEXT("BarStemina.bmp"));
	m_SteminaBar->SetPos(10.f, 90.f);

	// Gold 
	m_GoldImage = CreateWidget<CUIImage>("GoldImage");
	m_GoldImage->SetTexture("GoldImage", TEXT("images/Items/coin_single.bmp"));
	m_GoldImage->SetTextureColorKey(255, 255, 255);
	m_GoldImage->SetPos(250.f, 10.f);

	std::vector<std::wstring>	vecNumberFileName;
	for (int i = 0; i < 10; ++i)
	{
		TCHAR	FileName[256] = {};
		wsprintf(FileName, TEXT("Number/%d.bmp"), i);
		vecNumberFileName.push_back(FileName);
	}

	m_GoldHunderedWidget = CreateWidget<CNumberWidget>("Number");
	m_GoldHunderedWidget->SetTexture("Number", vecNumberFileName);
	m_GoldHunderedWidget->SetPos(280.f, 10.f);
	m_GoldHunderedWidget->SetSize(29.f, 48.f);
	for (int i = 0; i < 10; i++)
	{
		m_GoldHunderedWidget->SetTextureColorKey(255, 255, 255, i);
	}

	m_GoldTenWidget = CreateWidget<CNumberWidget>("Number");
	m_GoldTenWidget->SetTexture("Number", vecNumberFileName);
	m_GoldTenWidget->SetPos(310.f, 10.f);
	m_GoldTenWidget->SetSize(29.f, 48.f);
	for (int i = 0; i < 10; i++)
	{
		m_GoldTenWidget->SetTextureColorKey(255, 255, 255, i);
	}

	m_GoldOneWidget = CreateWidget<CNumberWidget>("Number");
	m_GoldOneWidget->SetTexture("Number", vecNumberFileName);
	m_GoldOneWidget->SetPos(340.f, 10.f);
	m_GoldOneWidget->SetSize(29.f, 48.f);
	for (int i = 0; i < 10; i++)
	{
		m_GoldOneWidget->SetTextureColorKey(255, 255, 255, i);
	}

	return true;
}
