
#include "UICharacterStateHUD.h"
#include "../GameManager.h"
#include "../Scene/MainScene.h"
#include "../Scene/SceneManager.h"

CUICharacterStateHUD::CUICharacterStateHUD() :
										m_HPBar(nullptr),
										m_MPBar(nullptr),
										m_SteminaBar(nullptr),
										m_GunLightBar(nullptr),
										m_GunMediumBar(nullptr),
										m_GunHeavyBar(nullptr)
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

	// Gun 
	Back = CreateWidget<CUIImage>("GunLightBarBack");
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

	return true;
}
