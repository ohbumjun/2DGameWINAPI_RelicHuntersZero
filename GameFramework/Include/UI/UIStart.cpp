
#include "UIStart.h"
#include "Button.h"
#include "UIImage.h"
#include "../GameManager.h"
#include "../Scene/HomeScene.h"
#include "../Scene/MainScene.h"
#include "../Scene/EditorScene.h"
#include "../Scene/SceneManager.h"
#include "UIText.h"

CUIStart::CUIStart()
{
}

CUIStart::~CUIStart()
{
}

bool CUIStart::Init()
{
	Resolution	RS = CGameManager::GetInst()->GetResolution();

	CUIImage* Back = CreateWidget<CUIImage>("Back");
	Back->SetTexture("StartBack", TEXT("GameBack.bmp"));

	// Start ==
	CButton* Button = CreateWidget<CButton>("StartButton");
	Button->SetTexture("StartButton", TEXT("ButtonBack.bmp"));
	Button->SetPos(RS.Width / 2.f - 100.f, RS.Height / 2.f - 175.f);
	Button->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(200.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Click, Vector2(400.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Disable, Vector2(600.f, 0.f), Vector2(200.f, 100.f));
	Button->SetMouseOnSound("ButtonMouseOn");
	Button->SetClickSound("ButtonClick");
	Button->SetZOrder(1);

	Button->SetClickCallback<CUIStart>(this, &CUIStart::StartClick);

	CUIText* Text = CreateWidget<CUIText>("StartButtonText");

	Text->SetText(TEXT("START"));
	Text->SetTextColor(100, 150, 200);
	Text->SetPos(RS.Width / 2.f - 50.f, RS.Height / 2.f - 155.f);
	Text->SetZOrder(2);

	// Editor Btn ==
	Button = CreateWidget<CButton>("EditorButton");

	Button->SetTexture("StartButton", TEXT("ButtonBack.bmp"));
	Button->SetPos(RS.Width / 2.f - 100.f, RS.Height / 2.f - 50.f);
	Button->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(200.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Click, Vector2(400.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Disable, Vector2(600.f, 0.f), Vector2(200.f, 100.f));
	Button->SetMouseOnSound("ButtonMouseOn");
	Button->SetClickSound("ButtonClick");
	Button->SetZOrder(1);

	Button->SetClickCallback<CUIStart>(this, &CUIStart::EditorClick);

	Text = CreateWidget<CUIText>("EditorButtonText");

	Text->SetText(TEXT("EDITOR"));
	Text->SetTextColor(100, 150, 200);
	Text->SetPos(RS.Width / 2.f - 50.f, RS.Height / 2.f - 30.f);
	Text->SetZOrder(2);


	// Exit Btn ==
	Button = CreateWidget<CButton>("ExitButton");

	Button->SetTexture("StartButton", TEXT("ButtonBack.bmp"));
	Button->SetPos(RS.Width / 2.f - 100.f, RS.Height / 2.f + 75.f);
	Button->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(200.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Click, Vector2(400.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Disable, Vector2(600.f, 0.f), Vector2(200.f, 100.f));
	Button->SetMouseOnSound("ButtonMouseOn");
	Button->SetClickSound("ButtonClick");
	Button->SetZOrder(1);

	Button->SetClickCallback<CUIStart>(this, &CUIStart::ExitClick);

	Text = CreateWidget<CUIText>("ExitButtonText");

	Text->SetText(TEXT("EXIT"));
	Text->SetTextColor(100, 150, 200);
	Text->SetPos(RS.Width / 2.f - 50.f, RS.Height / 2.f + 75.f);
	Text->SetZOrder(2);

	return true;
}

void CUIStart::StartClick()
{
	//CSceneManager::GetInst()->CreateScene<CHomeScene>();
	CSceneManager::GetInst()->CreateScene<CMainScene>();
}

void CUIStart::ExitClick()
{
	CGameManager::GetInst()->Exit();
}

void CUIStart::EditorClick()
{
	CSceneManager::GetInst()->CreateScene<CEditorScene>();
}
