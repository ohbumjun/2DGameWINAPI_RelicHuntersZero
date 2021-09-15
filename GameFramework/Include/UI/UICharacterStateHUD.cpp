
#include "UICharacterStateHUD.h"
#include "../GameManager.h"
#include "../Scene/MainScene.h"
#include "../Scene/SceneManager.h"

CUICharacterStateHUD::CUICharacterStateHUD()
{
}

CUICharacterStateHUD::~CUICharacterStateHUD()
{
}

bool CUICharacterStateHUD::Init()
{
	// ��ü window
	SetPos(30.f, 20.f);

	Resolution	RS = CGameManager::GetInst()->GetResolution();
	CUIImage* Back = CreateWidget<CUIImage>("HPBarBack");

	Back->SetTexture("BarBack", TEXT("BarBack.bmp"));
	Back->SetTextureColorKey(255, 0, 255);

	// ��ü hp bar
	m_HPBar = CreateWidget<CProgressBar>("HPBar");
	m_HPBar->SetTexture("HPBar", TEXT("BarDefault.bmp"));
	m_HPBar->SetPos(10.f, 10.f);

	// stemina bar
	m_SteminaBar = CreateWidget<CProgressBar>("SteminaBar");
	m_SteminaBar->SetTexture("SteminaBar", TEXT("BarStemina.bmp"));
	m_SteminaBar->SetPos(10.f, 30.f);
	m_SteminaBar->SetZOrder(1);

	return true;
}
