
#include "UIGameOver.h"
#include "../GameManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/MenuScene.h"
#include "../Scene/HomeScene.h"
#include "UIText.h"
#include "UIImage.h"
#include "Button.h"

CUIGameOver::CUIGameOver()
{
}

CUIGameOver::~CUIGameOver()
{
}

bool CUIGameOver::Init()
{
	return true;
}

void CUIGameOver::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
	if (m_UpperImg)
	{
		Vector2 UpperImgPos = m_UpperImg->GetPos();
		if (UpperImgPos.y <= -520.f)
		{
			float UpdateAmount = DeltaTime * 50;
			Vector2 Dir = Vector2(0.f, 1.f);
			m_UpperImg->Move(Dir, UpdateAmount);
		}	
	}
	if (m_DownImg)
	{
		Vector2 DownImgPos = m_DownImg->GetPos();
		if (DownImgPos.y >= 520.f)
		{
			float UpdateAmount = DeltaTime * 50;
			Vector2 Dir = Vector2(0.f, -1.f);
			m_DownImg->Move(Dir, UpdateAmount);
		}
	}
}

void CUIGameOver::Render(HDC hdc)
{
	CUIWindow::Render(hdc);
	if (m_UpperImg) m_UpperImg->Render(m_UpperImg->GetPos(),hdc);
	if (m_DownImg)  m_DownImg->Render(m_DownImg->GetPos(),hdc);
}

void CUIGameOver::SetGameOverWidgets()
{
	Resolution	RS = CGameManager::GetInst()->GetResolution();

	m_UpperImg = CreateWidget<CUIImage>("Back");
	m_UpperImg->SetTexture("MenuBack", TEXT("images/MenuScene/black_background.bmp"));
	m_UpperImg->SetPos(0.f, -720.f);
	m_DownImg = CreateWidget<CUIImage>("Back");
	m_DownImg->SetTexture("MenuBack", TEXT("images/MenuScene/black_background.bmp"));
	m_DownImg->SetPos(0.f, 720.f);

	// Buttons
	m_MenuBtn = CreateWidget<CButton>("MenuBtn");
	m_MenuBtn->SetTexture("GameStartBtn", TEXT("images/Character/Stats/charSelectBtn.bmp"));
	m_MenuBtn->SetTextureColorKey(255, 255, 255);
	m_MenuBtn->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(240.f, 63.f));
	m_MenuBtn->SetFrameData(EButton_State::MouseOn, Vector2(250.f, 0.f), Vector2(240.f, 63.f));
	m_MenuBtn->SetFrameData(EButton_State::Click, Vector2(0.f, 0.f), Vector2(240.f, 63.f));
	m_MenuBtn->SetFrameData(EButton_State::Disable, Vector2(250.f, 0.f), Vector2(240.f, 63.f));
	m_MenuBtn->SetPos(100.f, RS.Height / 2.f + 75.f);
	m_MenuBtn->SetMouseOnSound("ButtonMouseOn");
	m_MenuBtn->SetClickSound("ButtonClick");
	m_MenuBtn->SetZOrder(1);
	// Texts
	m_MenuTxt = CreateWidget<CUIText>("MenuTxt");
	m_MenuTxt->SetText(TEXT("Back To Menu"));
	m_MenuTxt->SetTextColor(255, 255, 255);
	m_MenuTxt->SetPos(150.f, RS.Height / 2.f + 75.f);
	m_MenuTxt->SetZOrder(2);

	// Buttons
	m_AgainBtn = CreateWidget<CButton>("AgainBtn");
	m_AgainBtn->SetTexture("GameStartBtn", TEXT("images/Character/Stats/charSelectBtn.bmp"));
	m_AgainBtn->SetTextureColorKey(255, 255, 255);
	m_AgainBtn->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(240.f, 63.f));
	m_AgainBtn->SetFrameData(EButton_State::MouseOn, Vector2(250.f, 0.f), Vector2(240.f, 63.f));
	m_AgainBtn->SetFrameData(EButton_State::Click, Vector2(0.f, 0.f), Vector2(240.f, 63.f));
	m_AgainBtn->SetFrameData(EButton_State::Disable, Vector2(250.f, 0.f), Vector2(240.f, 63.f));
	m_AgainBtn->SetPos(100.f, RS.Height / 2.f + 175.f);
	m_AgainBtn->SetMouseOnSound("ButtonMouseOn");
	m_AgainBtn->SetClickSound("ButtonClick");
	m_AgainBtn->SetZOrder(1);
	// Texts
	m_AgainTxt = CreateWidget<CUIText>("AginTxt");
	m_AgainTxt->SetText(TEXT("Try Again"));
	m_AgainTxt->SetTextColor(255, 255, 255);
	m_AgainTxt->SetPos(150.f, RS.Height / 2.f + 175.f);
	m_AgainTxt->SetZOrder(2);
}

void CUIGameOver::MenuClick()
{
	CSceneManager::GetInst()->CreateScene<CMenuScene>();
}

void CUIGameOver::AgainClick()
{
	CSceneManager::GetInst()->CreateScene<CHomeScene>();
}

