
#include "UIGunStateHUD.h"
#include "../GameManager.h"
#include "../Scene/MainScene.h"
#include "../Scene/SceneManager.h"
#include "../UI/WidgetComponent.h"

CUIGunStateHUD::CUIGunStateHUD() :
	m_GunLightBar(nullptr),
	m_GunMediumBar(nullptr),
	m_GunHeavyBar(nullptr),
	m_LightGunText(nullptr),
	m_MediumGunText(nullptr),
	m_HeavyGunText(nullptr),
	m_CurrentGunImage(nullptr),
	m_SecondGunImage(nullptr),
	m_ThirdGunImage(nullptr)
{
}

CUIGunStateHUD::~CUIGunStateHUD()
{
}

bool CUIGunStateHUD::Init()
{
	SetPos(30.f, 20.f);

	Resolution	RS = CGameManager::GetInst()->GetResolution();

	// Gun State 
	// Light ---
	CUIImage* Back = CreateWidget<CUIImage>("GunLightBarBack");
	Back->SetTexture("GunLightBarBack", TEXT("BarBack.bmp"));
	Back->SetTextureColorKey(255, 0, 255);
	m_GunLightBar = CreateWidget<CProgressBar>("GunLightBar");
	m_GunLightBar->SetTexture("GunLightBar", TEXT("BarGunLight.bmp"));
	m_GunLightBar->SetPos(10.f, 180.f);
	m_GunLightBar->SetPercent(100.f);

	m_LightGunText = CreateWidget<CUIText>("LightGunText");
	m_LightGunText->SetText(TEXT("Light Gun"));
	m_LightGunText->SetTextColor(0, 0, 0);
	m_LightGunText->SetPos(50.f, 200.f);
	m_LightGunText->SetZOrder(1);

	// Medium ---
	Back = CreateWidget<CUIImage>("GunMediumBarBack");
	Back->SetTexture("GunMediumBarBack", TEXT("BarBack.bmp"));
	Back->SetTextureColorKey(255, 0, 255);
	Back->SetPos(0, 40.f);
	m_GunMediumBar = CreateWidget<CProgressBar>("GunMediumBar");
	m_GunMediumBar->SetTexture("GunMediumBar", TEXT("BarDefault.bmp"));
	m_GunMediumBar->SetPos(10.f, 220.f);
	m_GunMediumBar->SetPercent(100.f);

	m_MediumGunText = CreateWidget<CUIText>("MediumGunText");
	m_MediumGunText->SetText(TEXT("Medium Gun"));
	m_MediumGunText->SetTextColor(0, 0, 0);
	m_MediumGunText->SetPos(50.f, 240.f);
	m_MediumGunText->SetZOrder(1);

	// Heavy ---
	Back = CreateWidget<CUIImage>("GunLargeBarBack");
	Back->SetTexture("GunLargeBarBack", TEXT("BarBack.bmp"));
	Back->SetTextureColorKey(255, 0, 255);
	Back->SetPos(0, 80.f);
	m_GunHeavyBar = CreateWidget<CProgressBar>("GunHeavyBar");
	m_GunHeavyBar->SetTexture("GunHeavyBar", TEXT("BarMP.bmp"));
	m_GunHeavyBar->SetPos(10.f, 260.f);
	m_GunHeavyBar->SetPercent(100.f);

	m_HeavyGunText = CreateWidget<CUIText>("HeavyGunText");
	m_HeavyGunText->SetText(TEXT("Heavy Gun"));
	m_HeavyGunText->SetTextColor(0, 0, 0);
	m_HeavyGunText->SetPos(50.f, 280.f);
	m_HeavyGunText->SetZOrder(1);


	// Gun Images
	m_CurrentGunImage = CreateWidget<CUIImage>("GunLightImage");
	m_CurrentGunImage->SetTexture("GunLightImage", TEXT("BarGunLight.bmp"));
	m_CurrentGunImage->SetTextureColorKey(255, 0, 255);
	m_CurrentGunImage->SetPos(10.f, 550.f);

	m_SecondGunImage = CreateWidget<CUIImage>("GunMediumImage");
	m_SecondGunImage->SetTexture("GunMediumImage", TEXT("BarGunLight.bmp"));
	m_SecondGunImage->SetTextureColorKey(255, 0, 255);
	m_SecondGunImage->SetPos(10.f, 600.f);

	m_ThirdGunImage = CreateWidget<CUIImage>("GunHeavyImage");
	m_ThirdGunImage->SetTexture("GunHeavyImage", TEXT("BarGunLight.bmp"));
	m_ThirdGunImage->SetTextureColorKey(255, 0, 255);
	m_ThirdGunImage->SetPos(90.f, 600.f);

	// Bullet Text 

	return true;
}
