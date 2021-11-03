#include "UIBossStateHUD.h"
#include"../GameManager.h"

CUIBossStateHUD::CUIBossStateHUD()
{
}

CUIBossStateHUD::~CUIBossStateHUD()
{
}

void CUIBossStateHUD::SetHpBarUI()
{
	// Hp, Mp, Stemina Bar ---
	m_HPBar = CreateWidget<CProgressBar>("BossHPBar");
	m_HPBar->SetTexture("BossHPBar", TEXT("images/Monster/Boss/BossMonsterBar.bmp"));
	m_HPBar->SetPos(10.f, 10.f);

}

void CUIBossStateHUD::SetGeneratorUI()
{
	// m_GeneratorImage = CreateWidget<CUIImage>("GoldImage");
	// m_GeneratorImage->SetTexture("GoldImage", TEXT("images/Items/coin_single.bmp"));
	// m_GeneratorImage->SetTextureColorKey(255, 255, 255);
	// m_GeneratorImage->SetPos(250.f, 10.f);
}

bool CUIBossStateHUD::Init()
{
	SetPos(400.f, 20.f);

	SetHpBarUI();
	SetGeneratorUI();

	return true;
}
