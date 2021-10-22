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
	m_RTText(nullptr),
	m_MVVol(10),
	m_BGVol(10),
	m_EffectVol(10)
{
}

CUISetting::~CUISetting()
{
	m_MVVol = 10;
	m_BGVol = 10;
	m_EffectVol = 10;
}

void CUISetting::SetTextImages()
{
	Resolution	RS = CGameManager::GetInst()->GetResolution();
	// Left Top
	m_LTBackImg = CreateWidget<CUIImage>("LTTitle");
	m_LTBackImg->SetTexture("LTTitle", TEXT("images/MenuScene/spr_menu_button_1.bmp"));
	m_LTBackImg->SetTextureColorKey(255, 255, 255);
	m_LTBackImg->SetPos(-50.f, 50.f);
	m_LTBackImg->SetZOrder(1);

	m_LTText = CreateWidget<CUIText>("LTText");
	m_LTText->SetTextColor(255, 255, 255);
	m_LTText->SetPos(50.f, 50.f);
	m_LTText->SetText(TEXT("SETTINGS"));
	m_LTText->SetZOrder(2);

	// Right Top
	m_RTBackImg = CreateWidget<CUIImage>("RTTitle");
	m_RTBackImg->SetTexture("RTTitle", TEXT("images/MenuScene/spr_menu_button_1.bmp"));
	m_RTBackImg->SetTextureColorKey(255, 255, 255);
	m_RTBackImg->SetPos(590.f, RS.Height / 2.f - 75.f);
	m_RTBackImg->SetZOrder(1);

	m_RTText = CreateWidget<CUIText>("RTText");
	m_RTText->SetTextColor(255, 255, 255);
	m_RTText->SetPos(630.f, RS.Height / 2.f - 75.f);
	m_RTText->SetText(TEXT("MUSIC OPTIONS"));
	m_RTText->SetZOrder(2);
}

void CUISetting::UpMainVol()
{
	if(m_MVVol < 10)
		m_MVVol += 1;
}

void CUISetting::DownMainVol()
{
	if (m_MVVol > 0)
		m_MVVol -= 1;
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
	m_MVImage->SetZOrder(1);

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
	m_MVLeftBtn->SetClickCallback<CUISetting>(this, &CUISetting::DownMainVol);

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
	m_MVRightBtn->SetClickCallback<CUISetting>(this, &CUISetting::UpMainVol);


	std::vector<std::wstring>	vecNumberFileName;
	for (int i = 0; i < 10; ++i)
	{
		TCHAR	FileName[256] = {};
		wsprintf(FileName, TEXT("Number/Small/%d.bmp"), i);
		vecNumberFileName.push_back(FileName);
	}
	m_MVTenWidget = CreateWidget<CNumberWidget>("MVTenNumber");
	m_MVTenWidget->SetTexture("MVTenNumber", vecNumberFileName);
	m_MVTenWidget->SetSize(10.f,18.f);
	m_MVTenWidget->SetPos(310.f, 180.f);
	m_MVTenWidget->SetZOrder(2);
	for (int i = 0; i < 10; i++)
		m_MVTenWidget->SetTextureColorKey(255, 255, 255, i);

	m_MVOneWidget = CreateWidget<CNumberWidget>("MVOneNumber");
	m_MVOneWidget->SetTexture("MVOneNumber", vecNumberFileName);
	m_MVOneWidget->SetSize(10.f,18.f);
	m_MVOneWidget->SetPos(330.f, 180.f);
	m_MVOneWidget->SetZOrder(2);
	for (int i = 0; i < 10; i++)
		m_MVOneWidget->SetTextureColorKey(255, 255, 255, i);
}

void CUISetting::MainVolumeUpdate()
{
	int FullT = (m_MVVol % 100) / 10, FullO = m_MVVol % 10;
	if (FullT != 0)
		m_MVTenWidget->SetNumber(FullT);
	else
		m_MVTenWidget->SetRenderEnable(false);
	if (FullT != 0 || FullO != 0)
		m_MVOneWidget->SetNumber(FullO);
	else
		m_MVOneWidget->SetNumber(0);
}

void CUISetting::UpBackGroundVol()
{
	if (m_BGVol < 10) m_BGVol += 1;
	CGameManager::GetInst()->SetBGVol(m_BGVol);
}

void CUISetting::DownBackGroundVol()
{
	if (m_BGVol > 0) m_BGVol -= 1;
	CGameManager::GetInst()->SetBGVol(m_BGVol);
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
	m_BVImage->SetZOrder(1);

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
	m_BVLeftBtn->SetClickCallback<CUISetting>(this, &CUISetting::DownBackGroundVol);

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
	m_BVRightBtn->SetClickCallback<CUISetting>(this, &CUISetting::UpBackGroundVol);

	std::vector<std::wstring>	vecNumberFileName;
	for (int i = 0; i < 10; ++i)
	{
		TCHAR	FileName[256] = {};
		wsprintf(FileName, TEXT("Number/Small/%d.bmp"), i);
		vecNumberFileName.push_back(FileName);
	}
	m_BVTenWidget = CreateWidget<CNumberWidget>("BVTenNumber");
	m_BVTenWidget->SetTexture("BVTenNumber", vecNumberFileName);
	m_BVTenWidget->SetSize(10.f,18.f);
	m_BVTenWidget->SetPos(310.5f, 230.f);
	m_BVTenWidget->SetZOrder(2);
	for (int i = 0; i < 10; i++)
		m_BVTenWidget->SetTextureColorKey(255, 255, 255, i);

	m_BVOneWidget = CreateWidget<CNumberWidget>("BVOneNumber");
	m_BVOneWidget->SetTexture("BVOneNumber", vecNumberFileName);
	m_BVOneWidget->SetSize(10.f,18.f);
	m_BVOneWidget->SetPos(330.5f, 230.f);
	m_BVOneWidget->SetZOrder(2);
	for (int i = 0; i < 10; i++)
		m_BVOneWidget->SetTextureColorKey(255, 255, 255, i);
}

void CUISetting::BGVolumeUpdate()
{
	int FullT = (m_BGVol % 100) / 10, FullO = m_BGVol % 10;
	if (FullT != 0)
		m_BVTenWidget->SetNumber(FullT);
	else
		m_BVTenWidget->SetRenderEnable(false);
	if (FullT != 0 || FullO != 0)
		m_BVOneWidget->SetNumber(FullO);
	else
		m_BVOneWidget->SetNumber(0);
}

void CUISetting::UpEffectVol()
{
	if (m_EffectVol < 10) m_EffectVol += 1;
}

void CUISetting::DownEffectVol()
{
	if (m_EffectVol > 0) m_EffectVol -= 1;
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
	m_EVImage->SetZOrder(1);

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
	m_EVLeftBtn->SetClickCallback<CUISetting>(this, &CUISetting::DownEffectVol);

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
	m_EVRightBtn->SetClickCallback<CUISetting>(this, &CUISetting::UpEffectVol);

	std::vector<std::wstring>	vecNumberFileName;
	for (int i = 0; i < 10; ++i)
	{
		TCHAR	FileName[256] = {};
		wsprintf(FileName, TEXT("Number/Small/%d.bmp"), i);
		vecNumberFileName.push_back(FileName);
	}
	m_EVTenWidget = CreateWidget<CNumberWidget>("EVOneNumber");
	m_EVTenWidget->SetTexture("EVOneNumber", vecNumberFileName);
	m_EVTenWidget->SetTexture("Number", vecNumberFileName);
	m_EVTenWidget->SetSize(10.f,18.f);
	m_EVTenWidget->SetPos(310.5f, 280.f);
	m_EVTenWidget->SetZOrder(2);
	for (int i = 0; i < 10; i++)
		m_EVTenWidget->SetTextureColorKey(255, 255, 255, i);

	m_EVOneWidget = CreateWidget<CNumberWidget>("EVTenNumber");
	m_EVOneWidget->SetTexture("EVTenNumber", vecNumberFileName);
	m_EVOneWidget->SetTexture("Number", vecNumberFileName);
	m_EVOneWidget->SetSize(10.f,18.f);
	m_EVOneWidget->SetPos(330.5f, 280.f);
	m_EVOneWidget->SetZOrder(2);
	for (int i = 0; i < 10; i++)
		m_EVOneWidget->SetTextureColorKey(255, 255, 255, i);
}

void CUISetting::EffectVolumeUpdate()
{
	int FullT = (m_EffectVol % 100) / 10, FullO = m_EffectVol % 10;
	if (FullT != 0)
		m_EVTenWidget->SetNumber(FullT);
	else
		m_EVTenWidget->SetRenderEnable(false);
	if (FullT != 0 || FullO != 0)
		m_EVOneWidget->SetNumber(FullO);
	else
		m_EVOneWidget->SetNumber(0);
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

	EffectVolumeUpdate();
	BGVolumeUpdate();
	MainVolumeUpdate();
	
	return true;
}

void CUISetting::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);

	EffectVolumeUpdate();
	BGVolumeUpdate();
	MainVolumeUpdate();
}


void CUISetting::StartClick()
{
	CSceneManager::GetInst()->CreateScene<CHomeScene>();
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