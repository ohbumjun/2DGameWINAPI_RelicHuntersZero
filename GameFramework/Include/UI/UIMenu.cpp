#include "UIMenu.h"
#include "../GameManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/HomeScene.h"
#include "../Scene/StartScene.h"
#include "../Scene/EditorScene.h"
#include "../UI/Button.h"
#include "UIText.h"
#include "UIImage.h"

CUIMenu::CUIMenu()
{
}

CUIMenu::~CUIMenu()
{
}

bool CUIMenu::Init()
{
	Resolution	RS = CGameManager::GetInst()->GetResolution();

	CUIImage* Back = CreateWidget<CUIImage>("Back");
	Back->SetTexture("MenuBack", TEXT("images/MenuScene/bg_menu_main.bmp"));

	Back = CreateWidget<CUIImage>("Back");
	Back->SetTexture("Logo", TEXT("images/MenuScene/small_logoRHS.bmp"));
	Back->SetTextureColorKey(255, 255, 255);
	Back->SetPos(0.f, 75.f);
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
		m_Buttons[i]->SetPos(0.f, RS.Height / 2.f + 75.f * i);
		m_Buttons[i]->SetFrameData(EButton_State::Normal, Vector2(100.f, 15.f), Vector2(470.f, 36.f));
		m_Buttons[i]->SetFrameData(EButton_State::MouseOn, Vector2(680.f, 15.f), Vector2(470.f, 36.f));
		m_Buttons[i]->SetFrameData(EButton_State::Click, Vector2(680.f, 15.f), Vector2(470.f, 36.f));
		m_Buttons[i]->SetFrameData(EButton_State::Disable, Vector2(680.f, 15.f), Vector2(470.f, 36.f));
		m_Buttons[i]->SetMouseOnSound("ButtonMouseOn");
		m_Buttons[i]->SetClickSound("ButtonClick");
		m_Buttons[i]->SetZOrder(1);

		// Texts
		std::string TxtName    = "Txt" + std::to_string(i);
		m_Texts.push_back(CreateWidget<CUIText>(TxtName));
		m_Texts[i]->SetTextColor(255, 255, 255);
		m_Texts[i]->SetPos(50.f, RS.Height / 2.f + 75.f * i );
		m_Texts[i]->SetZOrder(2);
	}
	
	// Set Btn Callback
	m_Buttons[0]->SetClickCallback<CUIMenu>(this, &CUIMenu::StartClick);
	m_Buttons[1]->SetClickCallback<CUIMenu>(this, &CUIMenu::EditorClick);
	m_Buttons[2]->SetClickCallback<CUIMenu>(this, &CUIMenu::SettingClick);
	m_Buttons[3]->SetClickCallback<CUIMenu>(this, &CUIMenu::ExitClick);

	// Set Texts
	m_Texts[0]->SetText(TEXT("GAME"));
	m_Texts[1]->SetText(TEXT("MAP EDIT"));
	m_Texts[2]->SetText(TEXT("SETTINGS"));
	m_Texts[3]->SetText(TEXT("EXIT"));

	return true;
}

void CUIMenu::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
}


void CUIMenu::StartClick()
{
	CSceneManager::GetInst()->CreateScene<CHomeScene>();
	// CSceneManager::GetInst()->CreateScene<CMainScene>();
	// CSceneManager::GetInst()->CreateScene<CMain2Scene>();
	// CSceneManager::GetInst()->CreateScene<CMain3Scene>();
}

void CUIMenu::ExitClick()
{
	CGameManager::GetInst()->Exit();
}

void CUIMenu::EditorClick()
{
	CSceneManager::GetInst()->CreateScene<CEditorScene>();
}

void CUIMenu::SettingClick()
{
}