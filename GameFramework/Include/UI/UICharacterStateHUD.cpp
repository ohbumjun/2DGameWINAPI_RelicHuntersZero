
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
}

void CUICharacterStateHUD::SetGoldUI(std::vector<std::wstring>& vecNumberFileName)
{
	m_GoldImage = CreateWidget<CUIImage>("GoldImage");
	m_GoldImage->SetTexture("GoldImage", TEXT("images/Items/coin_single.bmp"));
	m_GoldImage->SetTextureColorKey(255, 255, 255);
	m_GoldImage->SetPos(250.f, 10.f);



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
}

void CUICharacterStateHUD::SetHpInvUI(std::vector<std::wstring>& vecNumberFileName)
{
	m_HpInvImage = CreateWidget<CUIImage>("HpInvImage");
	m_HpInvImage->SetTexture("HpInvImage", TEXT("images/Items/hp.bmp"));
	m_HpInvImage->SetTextureColorKey(255, 255, 255);
	m_HpInvImage->SetPos(350.f, 500.f);

	m_HpInvTenWidget = CreateWidget<CNumberWidget>("Number");
	m_HpInvTenWidget->SetTexture("Number", vecNumberFileName);
	m_HpInvTenWidget->SetPos(380.f, 530.f);
	m_HpInvTenWidget->SetSize(29.f, 48.f);
	for (int i = 0; i < 10; i++)
	{
		m_HpInvTenWidget->SetTextureColorKey(255, 255, 255, i);
	}

	m_HpInvOneWidget = CreateWidget<CNumberWidget>("Number");
	m_HpInvOneWidget->SetTexture("Number", vecNumberFileName);
	m_HpInvOneWidget->SetPos(410.f, 560.f);
	m_HpInvOneWidget->SetSize(29.f, 48.f);
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
	m_MpInvImage->SetPos(450.f, 530.f);

	m_MpInvTenWidget = CreateWidget<CNumberWidget>("Number");
	m_MpInvTenWidget->SetTexture("Number", vecNumberFileName);
	m_MpInvTenWidget->SetPos(480.f, 530.f);
	m_MpInvTenWidget->SetSize(29.f, 48.f);
	for (int i = 0; i < 10; i++)
	{
		m_MpInvTenWidget->SetTextureColorKey(255, 255, 255, i);
	}

	m_MpInvOneWidget = CreateWidget<CNumberWidget>("Number");
	m_MpInvOneWidget->SetTexture("Number", vecNumberFileName);
	m_MpInvOneWidget->SetPos(510.f, 530.f);
	m_MpInvOneWidget->SetSize(29.f, 48.f);
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
	m_ShieldInvImage->SetPos(550.f, 530.f);

	m_ShieldInvTenWidget = CreateWidget<CNumberWidget>("Number");
	m_ShieldInvTenWidget->SetTexture("Number", vecNumberFileName);
	m_ShieldInvTenWidget->SetPos(580.f, 530.f);
	m_ShieldInvTenWidget->SetSize(29.f, 48.f);
	for (int i = 0; i < 10; i++)
	{
		m_ShieldInvTenWidget->SetTextureColorKey(255, 255, 255, i);
	}

	m_ShieldInvOneWidget = CreateWidget<CNumberWidget>("Number");
	m_ShieldInvOneWidget->SetTexture("Number", vecNumberFileName);
	m_ShieldInvOneWidget->SetPos(610.f, 530.f);
	m_ShieldInvOneWidget->SetSize(29.f, 48.f);
	for (int i = 0; i < 10; i++)
	{
		m_ShieldInvOneWidget->SetTextureColorKey(255, 255, 255, i);
	}
}

void CUICharacterStateHUD::SetGunBulletUI(std::vector<std::wstring>& vecNumberFileName)
{
	m_BulletHunderedWidget = CreateWidget<CNumberWidget>("Number");
	m_BulletHunderedWidget->SetTexture("Number", vecNumberFileName);
	m_BulletHunderedWidget->SetPos(CURRENT_GUN_X_POS + 120.f, CURRENT_GUN_Y_POS);
	m_BulletHunderedWidget->SetSize(29.f, 48.f);
	for (int i = 0; i < 10; i++)
	{
		m_BulletHunderedWidget->SetTextureColorKey(255, 0, 255, i);
	}

	m_BulletTenWidget = CreateWidget<CNumberWidget>("Number");
	m_BulletTenWidget->SetTexture("Number", vecNumberFileName);
	m_BulletTenWidget->SetPos(CURRENT_GUN_X_POS + 150.f, CURRENT_GUN_Y_POS);
	m_BulletTenWidget->SetSize(29.f, 48.f);
	for (int i = 0; i < 10; i++)
	{
		m_BulletTenWidget->SetTextureColorKey(255, 0, 255, i);
	}

	m_BulletOneWidget = CreateWidget<CNumberWidget>("Number");
	m_BulletOneWidget->SetTexture("Number", vecNumberFileName);
	m_BulletOneWidget->SetPos(CURRENT_GUN_X_POS + 180.f, CURRENT_GUN_Y_POS);
	m_BulletOneWidget->SetSize(29.f, 48.f);
	for (int i = 0; i < 10; i++)
	{
		m_BulletOneWidget->SetTextureColorKey(255, 0, 255, i);
	}

	// Bullet Full 
	m_FullBulletHunderedWidget = CreateWidget<CNumberWidget>("Number");
	m_FullBulletHunderedWidget->SetTexture("Number", vecNumberFileName);
	m_FullBulletHunderedWidget->SetPos(CURRENT_GUN_X_POS + 230.f, CURRENT_GUN_Y_POS);
	m_FullBulletHunderedWidget->SetSize(29.f, 48.f);
	for (int i = 0; i < 10; i++)
	{
		m_FullBulletHunderedWidget->SetTextureColorKey(255, 0, 255, i);
	}

	m_FullBulletTenWidget = CreateWidget<CNumberWidget>("Number");
	m_FullBulletTenWidget->SetTexture("Number", vecNumberFileName);
	m_FullBulletTenWidget->SetPos(CURRENT_GUN_X_POS + 260.f, CURRENT_GUN_Y_POS);
	m_FullBulletTenWidget->SetSize(29.f, 48.f);
	for (int i = 0; i < 10; i++)
	{
		m_FullBulletTenWidget->SetTextureColorKey(255, 0, 255, i);
	}

	m_FullBulletOneWidget = CreateWidget<CNumberWidget>("Number");
	m_FullBulletOneWidget->SetTexture("Number", vecNumberFileName);
	m_FullBulletOneWidget->SetPos(CURRENT_GUN_X_POS + 290.f, CURRENT_GUN_Y_POS);
	m_FullBulletOneWidget->SetSize(29.f, 48.f);
	for (int i = 0; i < 10; i++)
	{
		m_FullBulletOneWidget->SetTextureColorKey(255, 0, 255, i);
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
