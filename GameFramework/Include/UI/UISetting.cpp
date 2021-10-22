#include "UISetting.h"
#include "../GameManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/HomeScene.h"
#include "../Scene/StartScene.h"
#include "../Scene/EditorScene.h"
#include "UIText.h"
#include "Button.h"
#include "UIImage.h"

CUISetting::CUISetting() : 
	m_LTText(nullptr),
	m_RTText(nullptr)
{
}

CUISetting::~CUISetting()
{
}

void CUISetting::SetTextImages()
{
	Resolution	RS = CGameManager::GetInst()->GetResolution();
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
	m_RTBtn->SetPos(590.f, RS.Height / 2.f - 75.f);
	m_RTBtn->SetZOrder(1);

	m_RTText = CreateWidget<CUIText>("RTText");
	m_RTText->SetTextColor(255, 255, 255);
	m_RTText->SetPos(630.f, RS.Height / 2.f - 75.f);
	m_RTText->SetText(TEXT("MUSIC OPTIONS"));
	m_RTText->SetZOrder(2);
}

void CUISetting::SetMainVolumeElements()
{
	// Text
	m_MVText = CreateWidget<CUIText>("MVText");
	m_MVText->SetTextColor(255, 0, 0);
	m_MVText->SetPos(30.f, 175.f);
	m_MVText->SetText(TEXT("Master Volume"));
	m_MVText->SetZOrder(2);

	m_MVImage = CreateWidget<CUIImage>("MVImg");
	m_MVImage->SetTexture("MVImg", TEXT("images/MenuScene/spr_menu_img.bmp"));
	m_MVImage->SetPos(300.f, 175.f);
	m_MVImage->SetZOrder(2);

	m_MVLeftBtn= CreateWidget<CButton>("MVLeftBtn");
	m_MVLeftBtn->SetTexture("MVLeftBtn", TEXT("images/MenuScene/spr_menu_minus_0.bmp"));
	m_MVLeftBtn->SetTextureColorKey(255, 255, 255);
	m_MVLeftBtn->SetPos(270.f, 175.f);
	m_MVLeftBtn->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_MVLeftBtn->SetFrameData(EButton_State::MouseOn, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_MVLeftBtn->SetFrameData(EButton_State::Click, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_MVLeftBtn->SetFrameData(EButton_State::Disable, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_MVLeftBtn->SetMouseOnSound("ButtonMouseOn");
	m_MVLeftBtn->SetClickSound("ButtonClick");
	m_MVLeftBtn->SetZOrder(1);

	// class CButton* m_MVRightBtn;
	m_MVRightBtn = CreateWidget<CButton>("MVRightBtn");
	m_MVRightBtn->SetTexture("MVRightBtn", TEXT("images/MenuScene/spr_menu_plus_0.bmp"));
	m_MVRightBtn->SetTextureColorKey(255, 255, 255);
	m_MVRightBtn->SetPos(367.5f, 175.f);
	m_MVRightBtn->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_MVRightBtn->SetFrameData(EButton_State::MouseOn, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_MVRightBtn->SetFrameData(EButton_State::Click, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_MVRightBtn->SetFrameData(EButton_State::Disable, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_MVRightBtn->SetMouseOnSound("ButtonMouseOn");
	m_MVRightBtn->SetClickSound("ButtonClick");
	m_MVRightBtn->SetZOrder(1);
}

void CUISetting::SetBackgroundVolumeElements()
{
	m_BVText = CreateWidget<CUIText>("BVText");
	m_BVText->SetTextColor(255, 0, 0);
	m_BVText->SetPos(30.f, 225.f);
	m_BVText->SetText(TEXT("Background Volume"));
	m_BVText->SetZOrder(2);

	m_BVImage = CreateWidget<CUIImage>("BVImg");
	m_BVImage->SetTexture("BVImg", TEXT("images/MenuScene/spr_menu_img.bmp"));
	m_BVImage->SetPos(300.f, 225.f);
	m_BVImage->SetZOrder(2);

	m_BVLeftBtn = CreateWidget<CButton>("BVLeftBtn");
	m_BVLeftBtn->SetTexture("BVLeftBtn", TEXT("images/MenuScene/spr_menu_minus_0.bmp"));
	m_BVLeftBtn->SetTextureColorKey(255, 255, 255);
	m_BVLeftBtn->SetPos(270.f, 225.f);
	m_BVLeftBtn->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_BVLeftBtn->SetFrameData(EButton_State::MouseOn, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_BVLeftBtn->SetFrameData(EButton_State::Click, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_BVLeftBtn->SetFrameData(EButton_State::Disable, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_BVLeftBtn->SetMouseOnSound("ButtonMouseOn");
	m_BVLeftBtn->SetClickSound("ButtonClick");
	m_BVLeftBtn->SetZOrder(1);

	// class CButton* m_MVRightBtn;
	m_BVRightBtn = CreateWidget<CButton>("BVRightBtn");
	m_BVRightBtn->SetTexture("BVRightBtn", TEXT("images/MenuScene/spr_menu_plus_0.bmp"));
	m_BVRightBtn->SetTextureColorKey(255, 255, 255);
	m_BVRightBtn->SetPos(367.5f, 225.f);
	m_BVRightBtn->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_BVRightBtn->SetFrameData(EButton_State::MouseOn, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_BVRightBtn->SetFrameData(EButton_State::Click, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_BVRightBtn->SetFrameData(EButton_State::Disable, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_BVRightBtn->SetMouseOnSound("ButtonMouseOn");
	m_BVRightBtn->SetClickSound("ButtonClick");
	m_BVRightBtn->SetZOrder(1);
}

void CUISetting::SetEffectVolumeElements()
{
	m_EVText = CreateWidget<CUIText>("EVText");
	m_EVText->SetTextColor(255, 0, 0);
	m_EVText->SetPos(30.f, 275.f);
	m_EVText->SetText(TEXT("Effect Volume"));
	m_EVText->SetZOrder(2);

	m_EVImage = CreateWidget<CUIImage>("EVImg");
	m_EVImage->SetTexture("EVImg", TEXT("images/MenuScene/spr_menu_img.bmp"));
	m_EVImage->SetPos(300.f, 275.f);
	m_EVImage->SetZOrder(2);

	m_EVLeftBtn = CreateWidget<CButton>("EVLeftBtn");
	m_EVLeftBtn->SetTexture("EVLeftBtn", TEXT("images/MenuScene/spr_menu_minus_0.bmp"));
	m_EVLeftBtn->SetTextureColorKey(255, 255, 255);
	m_EVLeftBtn->SetPos(270.f, 275.f);
	m_EVLeftBtn->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_EVLeftBtn->SetFrameData(EButton_State::MouseOn, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_EVLeftBtn->SetFrameData(EButton_State::Click, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_EVLeftBtn->SetFrameData(EButton_State::Disable, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_EVLeftBtn->SetMouseOnSound("ButtonMouseOn");
	m_EVLeftBtn->SetClickSound("ButtonClick");
	m_EVLeftBtn->SetZOrder(1);

	m_EVRightBtn = CreateWidget<CButton>("EVRightBtn");
	m_EVRightBtn->SetTexture("EVRightBtn", TEXT("images/MenuScene/spr_menu_plus_0.bmp"));
	m_EVRightBtn->SetTextureColorKey(255, 255, 255);
	m_EVRightBtn->SetPos(367.5f, 275.f);
	m_EVRightBtn->SetFrameData(EButton_State::Normal, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_EVRightBtn->SetFrameData(EButton_State::MouseOn, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_EVRightBtn->SetFrameData(EButton_State::Click, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_EVRightBtn->SetFrameData(EButton_State::Disable, Vector2(0.f, 0.f), Vector2(13.f, 26.f));
	m_EVRightBtn->SetMouseOnSound("ButtonMouseOn");
	m_EVRightBtn->SetClickSound("ButtonClick");
	m_EVRightBtn->SetZOrder(1);
}

bool CUISetting::Init()
{
	Resolution	RS = CGameManager::GetInst()->GetResolution();

	CUIImage* Back = CreateWidget<CUIImage>("Back");
	Back->SetTexture("MenuBack", TEXT("images/MenuScene/bg_menu_settings.bmp"));

	SetTextImages();
	SetMainVolumeElements();
	SetBackgroundVolumeElements();
	SetEffectVolumeElements();
	
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