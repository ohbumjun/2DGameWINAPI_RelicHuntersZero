#include "UISetting.h"
#include "../GameManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/HomeScene.h"
#include "../Scene/StartScene.h"
#include "../Scene/EditorScene.h"
#include "UIText.h"
#include "UIImage.h"

CUISetting::CUISetting() : 
	m_LTText(nullptr),
	m_RTText(nullptr)
{
}

CUISetting::~CUISetting()
{
}

bool CUISetting::Init()
{
	Resolution	RS = CGameManager::GetInst()->GetResolution();

	CUIImage* Back = CreateWidget<CUIImage>("Back");
	Back->SetTexture("MenuBack", TEXT("images/MenuScene/bg_menu_settings.bmp"));

	// Left Top
	m_LTBtn = CreateWidget<CUIImage>("LTTitle");
	m_LTBtn->SetTexture("LTTitle", TEXT("images/MenuScene/spr_menu_button_1.bmp"));
	m_LTBtn->SetTextureColorKey(255, 255, 255);
	m_LTBtn->SetPos(-50.f, 50.f);
	m_LTBtn->SetZOrder(1);

	m_LTText = CreateWidget<CUIText>("LTText");
	m_LTText->SetTextColor(255, 255, 255);
	m_LTText->SetPos(50.f, 50.f);
	m_LTText->SetText(TEXT("SETTINGS"));
	m_LTText->SetZOrder(2);

	// Right Top
	m_RTBtn = CreateWidget<CUIImage>("RTTitle");
	m_RTBtn->SetTexture("RTTitle", TEXT("images/MenuScene/spr_menu_button_1.bmp"));
	m_RTBtn->SetTextureColorKey(255, 255, 255);
	m_RTBtn->SetPos(590.f, RS.Height / 2.f - 75.f );
	m_RTBtn->SetZOrder(1);

	m_RTText = CreateWidget<CUIText>("RTText");
	m_RTText->SetTextColor(255, 255, 255);
	m_RTText->SetPos(630.f, RS.Height / 2.f - 75.f);
	m_RTText->SetText(TEXT("MUSIC OPTIONS"));
	m_RTText->SetZOrder(2);

	return true;
}

void CUISetting::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
}


void CUISetting::StartClick()
{
	CSceneManager::GetInst()->CreateScene<CHomeScene>();
	// CSceneManager::GetInst()->CreateScene<CMainScene>();
	// CSceneManager::GetInst()->CreateScene<CMain2Scene>();
	// CSceneManager::GetInst()->CreateScene<CMain3Scene>();
}

void CUISetting::ExitClick()
{
	CGameManager::GetInst()->Exit();
}

void CUISetting::EditorClick()
{
	CSceneManager::GetInst()->CreateScene<CEditorScene>();
}

void CUISetting::SettingClick()
{
}