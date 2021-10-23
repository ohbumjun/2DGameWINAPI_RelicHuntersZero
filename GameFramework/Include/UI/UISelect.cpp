#include "UISelect.h"
#include "../GameManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/HomeScene.h"
#include "../Scene/StartScene.h"
#include "../UI/Button.h"
#include "UIText.h"
#include "UIImage.h"

CUISelect::CUISelect()
{
}

CUISelect::~CUISelect()
{
}

bool CUISelect::Init()
{
	Resolution	RS = CGameManager::GetInst()->GetResolution();

	CUIImage* Back = CreateWidget<CUIImage>("Back");
	Back->SetTexture("MenuBack", TEXT("images/MenuScene/background0.bmp"));

	Back = CreateWidget<CUIImage>("CharacterSelectionBar");
	Back->SetTexture("Bar", TEXT("images/MenuScene/spr_menu_button_1.bmp"));
	Back->SetTextureColorKey(255, 255, 255);
	Back->SetPos(-50.f, 75.f);
	Back->SetZOrder(1);

	CUIText* IntroBar = CreateWidget<CUIText>("BarTxt");
	IntroBar->SetTextColor(255, 255, 255);
	IntroBar->SetPos(50.f, 85.f);
	IntroBar->SetZOrder(2);
	IntroBar->SetText(TEXT("Character Selection"));

	return true;
}

void CUISelect::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
}


void CUISelect::StartClick()
{
	CSceneManager::GetInst()->CreateScene<CHomeScene>();
}

void CUISelect::ExitClick()
{
	CGameManager::GetInst()->Exit();
}
