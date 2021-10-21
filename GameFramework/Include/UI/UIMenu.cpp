
#include "UIMenu.h"
#include "../GameManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/StartScene.h"
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

	return true;
}

