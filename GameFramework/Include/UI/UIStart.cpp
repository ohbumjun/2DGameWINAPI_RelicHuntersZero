
#include "UIStart.h"
#include "Button.h"
#include "UIImage.h"
#include "../GameManager.h"
#include "../Scene/MainScene.h"
#include "../Scene/SceneManager.h"

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

	CButton* Button = CreateWidget<CButton>("StartButton");

	Button->SetTexture("StartButton", TEXT("StartButton.bmp"));
	Button->SetPos(RS.Width / 2.f - 100.f, RS.Height / 2.f - 75.f);
	Button->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(200.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Click, Vector2(400.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Disable, Vector2(600.f, 0.f), Vector2(200.f, 100.f));
	Button->SetMouseOnSound("ButtonMouseOn");
	Button->SetClickSound("ButtonClick");
	Button->SetZOrder(1);

	Button->SetClickCallback<CUIStart>(this, &CUIStart::StartClick);

	Button = CreateWidget<CButton>("ExitButton");

	Button->SetTexture("StartButton", TEXT("StartButton.bmp"));
	Button->SetPos(RS.Width / 2.f - 100.f, RS.Height / 2.f + 75.f);
	Button->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::MouseOn, Vector2(200.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Click, Vector2(400.f, 0.f), Vector2(200.f, 100.f));
	Button->SetFrameData(EButton_State::Disable, Vector2(600.f, 0.f), Vector2(200.f, 100.f));
	Button->SetMouseOnSound("ButtonMouseOn");
	Button->SetClickSound("ButtonClick");
	Button->SetZOrder(1);

	Button->SetClickCallback<CUIStart>(this, &CUIStart::ExitClick);

	return true;
}

void CUIStart::StartClick()
{
	CSceneManager::GetInst()->CreateScene<CMainScene>();
}

void CUIStart::ExitClick()
{
	CGameManager::GetInst()->Exit();
}
