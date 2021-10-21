
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
	Back->SetPos(100.f, 200.f);

	// Btn 1
	CButton* Button = CreateWidget<CButton>("StartButton");
	Button->SetTexture("StartButton", TEXT("images/MenuScene/small_MenuBtn.bmp"));
	Button->SetTextureColorKey(255, 255, 255);
	Button->SetPos(100.f, RS.Height / 2.f - 175.f);
	Button->SetFrameData(EButton_State::Normal, Vector2(40.f, 0.f), Vector2(332.f, 41.f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(371.f, 0.f), Vector2(372.f, 41.f));
	Button->SetFrameData(EButton_State::Click, Vector2(371.f, 0.f), Vector2(372.f, 41.f));
	Button->SetFrameData(EButton_State::Disable, Vector2(371.f, 0.f), Vector2(372.f, 41.f));
	Button->SetMouseOnSound("ButtonMouseOn");
	Button->SetClickSound("ButtonClick");
	Button->SetZOrder(1);
	// Button->SetClickCallback<CUIStart>(this, &CUIStart::StartClick);

	// Btn 2
	Button->SetTexture("StartButton", TEXT("images/MenuScene/small_MenuBtn.bmp"));
	Button->SetTextureColorKey(255, 255, 255);
	Button->SetPos(100.f, RS.Height / 2.f - 175.f);
	Button->SetFrameData(EButton_State::Normal, Vector2(40.f, 0.f), Vector2(332.f, 41.f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(371.f, 0.f), Vector2(372.f, 41.f));
	Button->SetFrameData(EButton_State::Click, Vector2(371.f, 0.f), Vector2(372.f, 41.f));
	Button->SetFrameData(EButton_State::Disable, Vector2(371.f, 0.f), Vector2(372.f, 41.f));
	Button->SetMouseOnSound("ButtonMouseOn");
	Button->SetClickSound("ButtonClick");
	Button->SetZOrder(1);

	// Btn 3

	Button->SetTexture("StartButton", TEXT("images/MenuScene/small_MenuBtn.bmp"));
	Button->SetTextureColorKey(255, 255, 255);
	Button->SetPos(100.f, RS.Height / 2.f - 175.f);
	Button->SetFrameData(EButton_State::Normal, Vector2(40.f, 0.f), Vector2(332.f, 41.f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(371.f, 0.f), Vector2(372.f, 41.f));
	Button->SetFrameData(EButton_State::Click, Vector2(371.f, 0.f), Vector2(372.f, 41.f));
	Button->SetFrameData(EButton_State::Disable, Vector2(371.f, 0.f), Vector2(372.f, 41.f));
	Button->SetMouseOnSound("ButtonMouseOn");
	Button->SetClickSound("ButtonClick");
	Button->SetZOrder(1);

	// Btn 4
	Button->SetTexture("StartButton", TEXT("images/MenuScene/small_MenuBtn.bmp"));
	Button->SetTextureColorKey(255, 255, 255);
	Button->SetPos(100.f, RS.Height / 2.f - 175.f);
	Button->SetFrameData(EButton_State::Normal, Vector2(40.f, 0.f), Vector2(332.f, 41.f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(371.f, 0.f), Vector2(372.f, 41.f));
	Button->SetFrameData(EButton_State::Click, Vector2(371.f, 0.f), Vector2(372.f, 41.f));
	Button->SetFrameData(EButton_State::Disable, Vector2(371.f, 0.f), Vector2(372.f, 41.f));
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
