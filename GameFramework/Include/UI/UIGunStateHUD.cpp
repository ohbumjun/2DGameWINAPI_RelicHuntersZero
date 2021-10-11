
#include "UIGunStateHUD.h"
#include "../GameManager.h"
#include "../Scene/MainScene.h"
#include "../Scene/SceneManager.h"

CUIGunStateHUD::CUIGunStateHUD() :
	m_GunLightBar(nullptr),
	m_GunMediumBar(nullptr),
	m_GunHeavyBar(nullptr)
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
	CUIImage* Back = CreateWidget<CUIImage>("GunLightBarBack");
	Back->SetTexture("GunLightBarBack", TEXT("BarBack.bmp"));
	Back->SetTextureColorKey(255, 0, 255);
	m_GunLightBar = CreateWidget<CProgressBar>("GunLightBar");
	m_GunLightBar->SetTexture("GunLightBar", TEXT("BarGunLight.bmp"));
	m_GunLightBar->SetPos(10.f, 180.f);

	Back = CreateWidget<CUIImage>("GunMediumBarBack");
	Back->SetTexture("GunMediumBarBack", TEXT("BarBack.bmp"));
	Back->SetTextureColorKey(255, 0, 255);
	Back->SetPos(0, 40.f);
	m_GunMediumBar = CreateWidget<CProgressBar>("GunMediumBar");
	m_GunMediumBar->SetTexture("GunMediumBar", TEXT("BarDefault.bmp"));
	m_GunMediumBar->SetPos(10.f, 220.f);

	Back = CreateWidget<CUIImage>("GunLargeBarBack");
	Back->SetTexture("GunLargeBarBack", TEXT("BarBack.bmp"));
	Back->SetTextureColorKey(255, 0, 255);
	Back->SetPos(0, 80.f);
	m_GunHeavyBar = CreateWidget<CProgressBar>("GunHeavyBar");
	m_GunHeavyBar->SetTexture("GunHeavyBar", TEXT("BarMP.bmp"));
	m_GunHeavyBar->SetPos(10.f, 260.f);

	// Gun Images
	m_CurrentGunImage = CreateWidget<CUIImage>("GunLightImage");
	m_CurrentGunImage->SetTexture("GunLightImage", TEXT("BarGunLight.bmp"));
	m_CurrentGunImage->SetTextureColorKey(255, 0, 255);
	m_CurrentGunImage->SetPos(10.f, 400.f);

	m_SecondGunImage = CreateWidget<CUIImage>("GunMediumImage");
	m_SecondGunImage->SetTexture("GunMediumImage", TEXT("BarGunLight.bmp"));
	m_SecondGunImage->SetTextureColorKey(255, 0, 255);
	m_SecondGunImage->SetPos(10.f, 450.f);

	m_ThirdGunImage = CreateWidget<CUIImage>("GunHeavyImage");
	m_ThirdGunImage->SetTexture("GunHeavyImage", TEXT("BarGunLight.bmp"));
	m_ThirdGunImage->SetTextureColorKey(255, 0, 255);
	m_ThirdGunImage->SetPos(10.f, 500.f);

	return true;
}
