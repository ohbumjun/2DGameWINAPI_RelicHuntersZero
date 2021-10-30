
#include "UIGameOver.h"
#include "../GameManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
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
}

void CUIGameOver::SetGameOverWidgets()
{
	Resolution	RS = CGameManager::GetInst()->GetResolution();

	CUIImage* GameOverImg = CreateWidget<CUIImage>("GameOver");
	GameOverImg->SetTexture("GameOver", TEXT("images/MenuScene/gameover.bmp"));
	GameOverImg->SetPos(340.f, 270.f);
	GameOverImg->SetTextureColorKey(255, 255, 255);

	m_UpperImg = CreateWidget<CUIImage>("GameOver1");
	m_UpperImg->SetTexture("MenuBack", TEXT("images/MenuScene/black_background.bmp"));
	m_UpperImg->SetPos(0.f, -720.f);
	m_DownImg = CreateWidget<CUIImage>("GameOver2");
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
	m_MenuBtn->SetPos(RS.Width / 2.f - 250.f, RS.Height / 2.f + 55.f);
	m_MenuBtn->SetMouseOnSound("ButtonMouseOn");
	m_MenuBtn->SetClickSound("ButtonClick");
	m_MenuBtn->SetZOrder(1);
	m_MenuBtn->SetClickCallback<CUIGameOver>(this, &CUIGameOver::MenuClick);
	// Texts
	m_MenuTxt = CreateWidget<CUIText>("MenuTxt");
	m_MenuTxt->SetText(TEXT("Back To Menu"));
	m_MenuTxt->SetTextColor(255, 255, 255);
	m_MenuTxt->SetPos(RS.Width / 2.f - 190.f, RS.Height / 2.f + 65.f);
	m_MenuTxt->SetZOrder(2);

	// Buttons
	m_AgainBtn = CreateWidget<CButton>("AgainBtn");
	m_AgainBtn->SetTexture("GameStartBtn", TEXT("images/Character/Stats/charSelectBtn.bmp"));
	m_AgainBtn->SetTextureColorKey(255, 255, 255);
	m_AgainBtn->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(240.f, 63.f));
	m_AgainBtn->SetFrameData(EButton_State::MouseOn, Vector2(250.f, 0.f), Vector2(240.f, 63.f));
	m_AgainBtn->SetFrameData(EButton_State::Click, Vector2(0.f, 0.f), Vector2(240.f, 63.f));
	m_AgainBtn->SetFrameData(EButton_State::Disable, Vector2(250.f, 0.f), Vector2(240.f, 63.f));
	m_AgainBtn->SetPos(RS.Width / 2.f - 50.f, RS.Height / 2.f + 55.f);
	m_AgainBtn->SetMouseOnSound("ButtonMouseOn");
	m_AgainBtn->SetClickSound("ButtonClick");
	m_AgainBtn->SetZOrder(1);
	m_AgainBtn->SetClickCallback<CUIGameOver>(this, &CUIGameOver::AgainClick);
	// Texts
	m_AgainTxt = CreateWidget<CUIText>("AginTxt");
	m_AgainTxt->SetText(TEXT("Try Again"));
	m_AgainTxt->SetTextColor(255, 255, 255);
	m_AgainTxt->SetPos(RS.Width / 2.f + 10.f , RS.Height / 2.f + 65.f);
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

