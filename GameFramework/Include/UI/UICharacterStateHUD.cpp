
#include "UICharacterStateHUD.h"
#include "../GameManager.h"
#include "../Scene/MainScene.h"
#include "../Scene/SceneManager.h"

CUICharacterStateHUD::CUICharacterStateHUD() :
										m_HPBar(nullptr),
										m_MPBar(nullptr),
										m_SteminaBar(nullptr)
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

	return true;
}
