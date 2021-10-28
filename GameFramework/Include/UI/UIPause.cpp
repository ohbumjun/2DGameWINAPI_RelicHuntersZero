#include "UIPause.h"
#include "../GameManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SettingScene.h"
#include "../Scene/MenuScene.h"
#include "../Scene/HomeScene.h"
#include "../Scene/SelectScene.h"
#include "../Scene/StartScene.h"
#include "../Scene/EditorScene.h"
#include "../UI/Button.h"
#include "UIText.h"
#include "UIImage.h"
#include "../Object/Player.h"

CUIPause::CUIPause()
{
	for (int i = 0; i < 4; i++) m_Toggles[i] = false;
}

CUIPause::~CUIPause()
{
	for (int i = 0; i < 4; i++) m_Toggles[i] = false;
}

bool CUIPause::Init()
{
	Resolution	RS = CGameManager::GetInst()->GetResolution();

	CUIImage* Back = CreateWidget<CUIImage>("Back");
	Back->SetTexture("PauseBack", TEXT("images/MenuScene/bg_menu_nested.bmp"));

	Back = CreateWidget<CUIImage>("Back");
	Back->SetTexture("Logo", TEXT("images/MenuScene/small_logoRHS.bmp"));
	Back->SetTextureColorKey(255, 255, 255);
	Back->SetPos(0.f, 475.f);
	Back->SetZOrder(1);

	m_Buttons.reserve(4);
	m_Texts.reserve(4);

	for (size_t i = 0; i < 4; i++)
	{
		// Buttons
		std::string BtnName = "Btn" + std::to_string(i);
		m_Buttons.push_back(CreateWidget<CButton>(BtnName));
		m_Buttons[i]->SetTexture(BtnName, TEXT("images/MenuScene/small_MenuBtn.bmp"));
		m_Buttons[i]->SetTextureColorKey(255, 255, 255);
		m_Buttons[i]->SetPos(0.f, RS.Height / 2.f - 75.f * i);
		m_Buttons[i]->SetFrameData(EButton_State::Normal, Vector2(100.f, 15.f), Vector2(470.f, 36.f));
		m_Buttons[i]->SetFrameData(EButton_State::MouseOn, Vector2(680.f, 15.f), Vector2(470.f, 36.f));
		m_Buttons[i]->SetFrameData(EButton_State::Click, Vector2(680.f, 15.f), Vector2(470.f, 36.f));
		m_Buttons[i]->SetFrameData(EButton_State::Disable, Vector2(680.f, 15.f), Vector2(470.f, 36.f));
		m_Buttons[i]->SetMouseOnSound("ButtonMouseOn");
		m_Buttons[i]->SetClickSound("ButtonClick");
		m_Buttons[i]->SetZOrder(1);

		// Texts
		std::string TxtName = "Txt" + std::to_string(i);
		m_Texts.push_back(CreateWidget<CUIText>(TxtName));
		m_Texts[i]->SetTextColor(255, 255, 255);
		m_Texts[i]->SetPos(50.f, RS.Height / 2.f - 75.f * i);
		m_Texts[i]->SetZOrder(2);
	}

	// Set Btn Callback
	m_Buttons[0]->SetClickCallback<CUIPause>(this, &CUIPause::ExitClick);
	m_Buttons[1]->SetClickCallback<CUIPause>(this, &CUIPause::SelectClick);
	m_Buttons[2]->SetClickCallback<CUIPause>(this, &CUIPause::MainMenuClick);
	m_Buttons[3]->SetClickCallback<CUIPause>(this, &CUIPause::ContinueClick);

	// Set Texts
	m_Texts[0]->SetText(TEXT("EXIT"));
	m_Texts[1]->SetText(TEXT("BACK TO SELECT"));
	m_Texts[2]->SetText(TEXT("MAIN MENU"));
	m_Texts[3]->SetText(TEXT("CONTINUE"));

	return true;
}

void CUIPause::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
	for (size_t i = 0; i < 4; i++)
	{
		EButton_State BtnState = m_Buttons[i]->GetButtonState();
		Vector2 TxtPos = m_Texts[i]->GetPos();
		if (BtnState == EButton_State::MouseOn && !m_Toggles[i])
		{
			m_Texts[i]->SetPos(TxtPos.x + 50.f, TxtPos.y);
			m_Texts[i]->SetTextColor(255, 0, 0);
			m_Toggles[i] = true;
		}
		else if (BtnState == EButton_State::Normal && m_Toggles[i])
		{
			m_Texts[i]->SetPos(TxtPos.x - 50.f, TxtPos.y);
			m_Texts[i]->SetTextColor(255, 255, 255);
			m_Toggles[i] = false;
		}
	}
}


void CUIPause::ContinueClick()
{
	m_Player->Resume(0.f);
	m_Scene->SetPauseUIVisbility(false);
}
void CUIPause::SelectClick()
{
	CSceneManager::GetInst()->CreateScene<CSelectScene>();
}
void CUIPause::MainMenuClick()
{
	CSceneManager::GetInst()->CreateScene<CMenuScene>();
}
void CUIPause::ExitClick()
{
	CGameManager::GetInst()->Exit();
}
