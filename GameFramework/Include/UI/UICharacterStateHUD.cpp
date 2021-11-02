
#include "UICharacterStateHUD.h"
#include "../GameManager.h"
#include "../Scene/MainScene.h"
#include "../Scene/SceneManager.h"

CUICharacterStateHUD::CUICharacterStateHUD() :
										m_HPBar(nullptr),
										m_MPBar(nullptr),
										m_SteminaBar(nullptr),
										m_GoldImage(nullptr),
										m_HpInvTenWidget(nullptr),
										m_HpInvOneWidget(nullptr),
										m_MpInvOneWidget(nullptr),
										m_MpInvTenWidget(nullptr),
										m_ShieldInvOneWidget(nullptr),
										m_ShieldInvTenWidget(nullptr),
										m_GoldHunderedWidget(nullptr),
										m_GoldTenWidget(nullptr),
										m_GoldOneWidget(nullptr),
										m_BulletHunderedWidget(nullptr),
										m_BulletTenWidget(nullptr),
										m_BulletOneWidget(nullptr),
										m_FullBulletHunderedWidget(nullptr),
										m_FullBulletTenWidget(nullptr),
										m_FullBulletOneWidget(nullptr)
{
}

CUICharacterStateHUD::~CUICharacterStateHUD()
{
}

void CUICharacterStateHUD::SetAbilityUI()
{
	// Hp, Mp, Stemina Bar ---
	m_HPBar = CreateWidget<CProgressBar>("HPBar");
	m_HPBar->SetTexture("HPBar", TEXT("BarDefault.bmp"));
	m_HPBar->SetPos(0.f, 10.f);

	m_MPBar = CreateWidget<CProgressBar>("MPBar");
	m_MPBar->SetTexture("MPBar", TEXT("BarMP.bmp"));
	m_MPBar->SetPos(0.f, 40.f);

	m_SteminaBar = CreateWidget<CProgressBar>("SteminaBar");
	m_SteminaBar->SetTexture("SteminaBar", TEXT("BarStemina.bmp"));
	m_SteminaBar->SetPos(0.f, 70.f);

	m_ShieldBar = CreateWidget<CProgressBar>("ShieldBar");
	m_ShieldBar->SetTexture("ShieldBar", TEXT("BarGunLight.bmp"));
	m_ShieldBar->SetPos(0.f, 100.f);
	SetShieldPercent(0.f);
}

void CUICharacterStateHUD::SetGoldUI(std::vector<std::wstring>& vecNumberFileName)
{
	m_GoldImage = CreateWidget<CUIImage>("GoldImage");
	m_GoldImage->SetTexture("GoldImage", TEXT("images/Items/coin_single.bmp"));
	m_GoldImage->SetTextureColorKey(255, 255, 255);
	m_GoldImage->SetPos(250.f, 10.f);

	m_GoldHunderedWidget = CreateWidget<CNumberWidget>("GoldHDNumber");
	m_GoldHunderedWidget->SetTexture("GoldHDNumber", vecNumberFileName);
	m_GoldHunderedWidget->SetPos(280.f, 10.f);
	m_GoldHunderedWidget->SetSize(16.f, 30.f);
	for (int i = 0; i < 10; i++)
	{
		m_GoldHunderedWidget->SetTextureColorKey(255, 255, 255, i);
	}

	m_GoldTenWidget = CreateWidget<CNumberWidget>("GoldTenNumber");
	m_GoldTenWidget->SetTexture("GoldTenNumber", vecNumberFileName);
	m_GoldTenWidget->SetPos(300.f, 10.f);
	m_GoldTenWidget->SetSize(16.f, 30.f);
	for (int i = 0; i < 10; i++)
	{
		m_GoldTenWidget->SetTextureColorKey(255, 255, 255, i);
	}

	m_GoldOneWidget = CreateWidget<CNumberWidget>("GoldOneNumber");
	m_GoldOneWidget->SetTexture("GoldOneNumber", vecNumberFileName);
	m_GoldOneWidget->SetPos(320.f, 10.f);
	m_GoldOneWidget->SetSize(16.f, 30.f);
	for (int i = 0; i < 10; i++)
	{
		m_GoldOneWidget->SetTextureColorKey(255, 255, 255, i);
	}
}

void CUICharacterStateHUD::SetHpInvUI(std::vector<std::wstring>& vecNumberFileName)
{
	m_HpInvImage = CreateWidget<CUIImage>("HpInvImage");
	m_HpInvImage->SetTexture("HpInvImage", TEXT("images/Items/hp.bmp"));
	m_HpInvImage->SetTextureColorKey(255, 255, 255);
	m_HpInvImage->SetPos(200.f, 600.f);

	m_HpInvTenWidget = CreateWidget<CNumberWidget>("HpInvTenNumber");
	m_HpInvTenWidget->SetTexture("HpInvTenNumber", vecNumberFileName);
	m_HpInvTenWidget->SetPos(250.f, 610.f);
	m_HpInvTenWidget->SetSize(16.f, 30.f);
	for (int i = 0; i < 10; i++)
	{
		m_HpInvTenWidget->SetTextureColorKey(255, 255, 255, i);
	}

	m_HpInvOneWidget = CreateWidget<CNumberWidget>("HpInvOneNumber");
	m_HpInvOneWidget->SetTexture("HpInvOneNumber", vecNumberFileName);
	m_HpInvOneWidget->SetPos(280.f, 610.f);
	m_HpInvOneWidget->SetSize(16.f, 30.f);
	for (int i = 0; i < 10; i++)
	{
		m_HpInvOneWidget->SetTextureColorKey(255, 255, 255, i);
	}
}

void CUICharacterStateHUD::SetMpInvUI(std::vector<std::wstring>& vecNumberFileName)
{
	m_MpInvImage = CreateWidget<CUIImage>("MpInvImage");
	m_MpInvImage->SetTexture("MpInvImage", TEXT("images/Items/mp.bmp"));
	m_MpInvImage->SetTextureColorKey(255, 255, 255);
	m_MpInvImage->SetPos(300.f, 600.f);

	m_MpInvTenWidget = CreateWidget<CNumberWidget>("MpInvTenNumber");
	m_MpInvTenWidget->SetTexture("MpInvTenNumber", vecNumberFileName);
	m_MpInvTenWidget->SetPos(350.f, 610.f);
	m_MpInvTenWidget->SetSize(16.f, 30.f);
	for (int i = 0; i < 10; i++)
	{
		m_MpInvTenWidget->SetTextureColorKey(255, 255, 255, i);
	}

	m_MpInvOneWidget = CreateWidget<CNumberWidget>("MpInvOneNumber");
	m_MpInvOneWidget->SetTexture("MpInvOneNumber", vecNumberFileName);
	m_MpInvOneWidget->SetPos(380.f, 610.f);
	m_MpInvOneWidget->SetSize(16.f, 30.f);
	for (int i = 0; i < 10; i++)
	{
		m_MpInvOneWidget->SetTextureColorKey(255, 255, 255, i);
	}
}

void CUICharacterStateHUD::SetShieldInvUI(std::vector<std::wstring>& vecNumberFileName)
{
	m_ShieldInvImage = CreateWidget<CUIImage>("ShieldInvImage");
	m_ShieldInvImage->SetTexture("ShieldInvImage", TEXT("images/Items/shield.bmp"));
	m_ShieldInvImage->SetTextureColorKey(255, 255, 255);
	m_ShieldInvImage->SetPos(400.f, 560.f);

	m_ShieldInvTenWidget = CreateWidget<CNumberWidget>("ShieldInvTenNumber");
	m_ShieldInvTenWidget->SetTexture("ShieldInvTenNumber", vecNumberFileName);
	m_ShieldInvTenWidget->SetPos(510.f, 600.f);
	m_ShieldInvTenWidget->SetSize(16.f, 30.f);
	for (int i = 0; i < 10; i++)
	{
		m_ShieldInvTenWidget->SetTextureColorKey(255, 255, 255, i);
	}

	m_ShieldInvOneWidget = CreateWidget<CNumberWidget>("ShieldInvOneNumber");
	m_ShieldInvOneWidget->SetTexture("ShieldInvOneNumber", vecNumberFileName);
	m_ShieldInvOneWidget->SetPos(530.f, 600.f);
	m_ShieldInvOneWidget->SetSize(16.f, 30.f);
	for (int i = 0; i < 10; i++)
	{
		m_ShieldInvOneWidget->SetTextureColorKey(255, 255, 255, i);
	}
}

void CUICharacterStateHUD::SetGunBulletUI(std::vector<std::wstring>& vecNumberFileName)
{
	m_BulletHunderedWidget = CreateWidget<CNumberWidget>("BulletHundNumber");
	m_BulletHunderedWidget->SetTexture("BulletHundNumber", vecNumberFileName);
	m_BulletHunderedWidget->SetPos(CURRENT_GUN_X_POS + 50.f, CURRENT_GUN_Y_POS);
	m_BulletHunderedWidget->SetSize(16.f, 30.f);
	for (int i = 0; i < 10; i++)
	{
		m_BulletHunderedWidget->SetTextureColorKey(255, 255, 255, i);
	}

	m_BulletTenWidget = CreateWidget<CNumberWidget>("BulletTenNumber");
	m_BulletTenWidget->SetTexture("BulletTenNumber", vecNumberFileName);
	m_BulletTenWidget->SetPos(CURRENT_GUN_X_POS + 70.f, CURRENT_GUN_Y_POS);
	m_BulletTenWidget->SetSize(16.f, 30.f);
	for (int i = 0; i < 10; i++)
	{
		m_BulletTenWidget->SetTextureColorKey(255, 255, 255, i);
	}

	m_BulletOneWidget = CreateWidget<CNumberWidget>("BulletOneNumber");
	m_BulletOneWidget->SetTexture("BulletOneNumber", vecNumberFileName);
	m_BulletOneWidget->SetPos(CURRENT_GUN_X_POS + 90.f, CURRENT_GUN_Y_POS);
	m_BulletOneWidget->SetSize(16.f, 30.f);
	for (int i = 0; i < 10; i++)
	{
		m_BulletOneWidget->SetTextureColorKey(255, 255, 255, i);
	}

	// Bullet Full 
	m_FullBulletHunderedWidget = CreateWidget<CNumberWidget>("FullBulletHundNumber");
	m_FullBulletHunderedWidget->SetTexture("FullBulletHundNumber", vecNumberFileName);
	m_FullBulletHunderedWidget->SetPos(CURRENT_GUN_X_POS + 130.f, CURRENT_GUN_Y_POS);
	m_FullBulletHunderedWidget->SetSize(16.f, 30.f);
	for (int i = 0; i < 10; i++)
	{
		m_FullBulletHunderedWidget->SetTextureColorKey(255, 255, 255, i);
	}

	m_FullBulletTenWidget = CreateWidget<CNumberWidget>("FullBulletTenNumber");
	m_FullBulletTenWidget->SetTexture("FullBulletTenNumber", vecNumberFileName);
	m_FullBulletTenWidget->SetPos(CURRENT_GUN_X_POS + 150.f, CURRENT_GUN_Y_POS);
	m_FullBulletTenWidget->SetSize(16.f, 30.f);
	for (int i = 0; i < 10; i++)
	{
		m_FullBulletTenWidget->SetTextureColorKey(255, 255, 255, i);
	}

	m_FullBulletOneWidget = CreateWidget<CNumberWidget>("FullBulletOneNumber");
	m_FullBulletOneWidget->SetTexture("FullBulletOneNumber", vecNumberFileName);
	m_FullBulletOneWidget->SetPos(CURRENT_GUN_X_POS + 170.f, CURRENT_GUN_Y_POS);
	m_FullBulletOneWidget->SetSize(16.f, 30.f);
	for (int i = 0; i < 10; i++)
	{
		m_FullBulletOneWidget->SetTextureColorKey(255, 255, 255, i);
	}
}

bool CUICharacterStateHUD::Init()
{
	SetPos(30.f, 20.f);

	Resolution	RS = CGameManager::GetInst()->GetResolution();
	std::vector<std::wstring>	vecNumberFileName;
	for (int i = 0; i < 10; ++i)
	{
		TCHAR	FileName[256] = {};
		wsprintf(FileName, TEXT("Number/%d.bmp"), i);
		vecNumberFileName.push_back(FileName);
	}

	// Ability ---
	SetAbilityUI();
	// Gold ---
	SetGoldUI(vecNumberFileName);
	// HpInv --- 
	SetHpInvUI(vecNumberFileName);
	// MpInv --- 
	SetMpInvUI(vecNumberFileName);
	// ShieldInv --- 
	SetShieldInvUI(vecNumberFileName);
	// Bullet 
	SetGunBulletUI(vecNumberFileName);

	return true;
}
