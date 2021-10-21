
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

	// Btn 1
	CButton* Button = CreateWidget<CButton>("StartButton");
	Button->SetTexture("StartButton", TEXT("images/MenuScene/small_MenuBtn.bmp"));
	Button->SetTextureColorKey(255, 255, 255);
	Button->SetPos(0.f, RS.Height / 2.f);
	Button->SetFrameData(EButton_State::Normal, Vector2(100.f, 15.f), Vector2(470.f, 36.f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(680.f, 15.f), Vector2(470.f, 36.f));
	Button->SetFrameData(EButton_State::Click, Vector2(680.f, 15.f), Vector2(470.f, 36.f));
	Button->SetFrameData(EButton_State::Disable, Vector2(680.f, 15.f), Vector2(470.f, 36.f));
	Button->SetMouseOnSound("ButtonMouseOn");
	Button->SetClickSound("ButtonClick");
	Button->SetZOrder(1);
	// Button->SetClickCallback<CUIStart>(this, &CUIStart::StartClick);

	// Btn 2
	Button = CreateWidget<CButton>("EditorButton");
	Button->SetTexture("EditorButton", TEXT("images/MenuScene/small_MenuBtn.bmp"));
	Button->SetTextureColorKey(255, 255, 255);
	Button->SetPos(-50.f, RS.Height / 2.f + 75.f);
	Button->SetFrameData(EButton_State::Normal, Vector2(100.f, 15.f), Vector2(470.f, 36.f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(680.f, 15.f), Vector2(470.f, 36.f));
	Button->SetFrameData(EButton_State::Click, Vector2(680.f, 15.f), Vector2(470.f, 36.f));
	Button->SetFrameData(EButton_State::Disable, Vector2(680.f, 15.f), Vector2(470.f, 36.f));
	Button->SetMouseOnSound("ButtonMouseOn");
	Button->SetClickSound("ButtonClick");
	Button->SetZOrder(1);

	// Btn 3
	Button = CreateWidget<CButton>("SettingButton");
	Button->SetTexture("SettingButton", TEXT("images/MenuScene/small_MenuBtn.bmp"));
	Button->SetTextureColorKey(255, 255, 255);
	Button->SetPos(-100.f, RS.Height / 2.f + 150.f);
	Button->SetFrameData(EButton_State::Normal, Vector2(100.f, 15.f), Vector2(470.f, 36.f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(680.f, 15.f), Vector2(470.f, 36.f));
	Button->SetFrameData(EButton_State::Click, Vector2(680.f, 15.f), Vector2(470.f, 36.f));
	Button->SetFrameData(EButton_State::Disable, Vector2(680.f, 15.f), Vector2(470.f, 36.f));
	Button->SetMouseOnSound("ButtonMouseOn");
	Button->SetClickSound("ButtonClick");
	Button->SetZOrder(1);

	// Btn 4
	Button = CreateWidget<CButton>("ExitButton");
	Button->SetTexture("ExitButton", TEXT("images/MenuScene/small_MenuBtn.bmp"));
	Button->SetTextureColorKey(255, 255, 255);
	Button->SetPos(-150.f, RS.Height / 2.f + 225.f);
	Button->SetFrameData(EButton_State::Normal, Vector2(100.f, 15.f), Vector2(470.f, 36.f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(680.f, 15.f), Vector2(470.f, 36.f));
	Button->SetFrameData(EButton_State::Click, Vector2(680.f, 15.f), Vector2(470.f, 36.f));
	Button->SetFrameData(EButton_State::Disable, Vector2(680.f, 15.f), Vector2(470.f, 36.f));
	Button->SetMouseOnSound("ButtonMouseOn");
	Button->SetClickSound("ButtonClick");
	Button->SetZOrder(1);

	return true;
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
