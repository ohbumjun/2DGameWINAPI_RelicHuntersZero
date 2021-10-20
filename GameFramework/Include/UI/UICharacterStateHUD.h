#pragma once

#include "UIWindow.h"
#include "UIText.h"
#include "ProgressBar.h"
#include "Button.h"
#include "UIImage.h"
#include "../UI/NumberWidget.h"

class CUICharacterStateHUD :
    public CUIWindow
{
	friend class CScene;

private:
	CUICharacterStateHUD();
	virtual ~CUICharacterStateHUD();
// HP,MP Bar
private:
	CSharedPtr<CProgressBar> m_HPBar;
	CSharedPtr<CProgressBar> m_MPBar;
	CSharedPtr<CProgressBar> m_SteminaBar;
	CSharedPtr<CProgressBar> m_ShieldBar;
private :
	void SetAbilityUI();
public:
	void SetHPPercent(float Percent)
	{
		m_HPBar->SetPercent(Percent);
	}
	void SetMPPercent(float Percent)
	{
		m_MPBar->SetPercent(Percent);
	}
	void SetSteminaPercent(float Percent)
	{
		m_SteminaBar->SetPercent(Percent);
	}
	void SetShieldPercent(float Percent)
	{
		m_ShieldBar->SetPercent(Percent);
	}
// Gold
private :
	CSharedPtr<CUIImage> m_GoldImage;
private :
	void SetGoldUI(std::vector<std::wstring>& vecNumberFileName);
public:
	void SetGoldImage(const std::string& TextureName)
	{
		m_GoldImage->SetTexture(TextureName);
	}
private :
	class   CNumberWidget* m_GoldHunderedWidget;
	class   CNumberWidget* m_GoldTenWidget;
	class   CNumberWidget* m_GoldOneWidget;
public:
	void SetGoldHundredWidget(int Number)
	{
		m_GoldHunderedWidget->SetNumber(Number);
	}
	void SetGoldHundredRenderEnable(bool State)
	{
		m_GoldHunderedWidget->SetRenderEnable(State);
	}
	void SetGoldTenWidget(int Number)
	{
		m_GoldTenWidget->SetNumber(Number);
	}
	void SetGoldTenRenderEnable(bool State)
	{
		m_GoldTenWidget->SetRenderEnable(State);
	}
	void SetGoldOneWidget(int Number)
	{
		m_GoldOneWidget->SetNumber(Number);
	}
	void SetGoldOneRenderEnable(bool State)
	{
		m_GoldOneWidget->SetRenderEnable(State);
	}
// Hp Inventory --- 
private:
	CSharedPtr<CUIImage> m_HpInvImage;
private :
	void SetHpInvUI(std::vector<std::wstring>& vecNumberFileName);
public:
	void SetHpInvImage(const std::string& TextureName)
	{
		m_HpInvImage->SetTexture(TextureName);
	}
private:
	class   CNumberWidget* m_HpInvTenWidget;
	class   CNumberWidget* m_HpInvOneWidget;
public:
	void SetHpInvTenWidget(int Number)
	{
		m_HpInvTenWidget->SetNumber(Number);
	}
	void SetHpInvTenRenderEnable(bool State)
	{
		m_HpInvTenWidget->SetRenderEnable(State);
	}
	void SetHpInvOneWidget(int Number)
	{
		m_HpInvOneWidget->SetNumber(Number);
	}
	void SetHpInvOneRenderEnable(bool State)
	{
		m_HpInvOneWidget->SetRenderEnable(State);
	}
// Mp Inventory --- 
private:
	CSharedPtr<CUIImage> m_MpInvImage;
private :
	void SetMpInvUI(std::vector<std::wstring>& vecNumberFileName);
public:
	void SetMpInvImage(const std::string& TextureName)
	{
		m_MpInvImage->SetTexture(TextureName);
	}
private:
	class   CNumberWidget* m_MpInvTenWidget;
	class   CNumberWidget* m_MpInvOneWidget;
public:
	void SetMpInvTenWidget(int Number)
	{
		m_MpInvTenWidget->SetNumber(Number);
	}
	void SetMpInvTenRenderEnable(bool State)
	{
		m_MpInvTenWidget->SetRenderEnable(State);
	}
	void SetMpInvOneWidget(int Number)
	{
		m_MpInvOneWidget->SetNumber(Number);
	}
	void SetMpInvOneRenderEnable(bool State)
	{
		m_MpInvOneWidget->SetRenderEnable(State);
	}
// Shield Inventory --- 
private:
	CSharedPtr<CUIImage> m_ShieldInvImage;
private :
	void SetShieldInvUI(std::vector<std::wstring>& vecNumberFileName);
public:
	void SetShieldInvImage(const std::string& TextureName)
	{
		m_ShieldInvImage->SetTexture(TextureName);
	}
private:
	class   CNumberWidget* m_ShieldInvTenWidget;
	class   CNumberWidget* m_ShieldInvOneWidget;
public:
	void SetShieldInvTenWidget(int Number)
	{
		m_ShieldInvTenWidget->SetNumber(Number);
	}
	void SetShieldInvTenRenderEnable(bool State)
	{
		m_ShieldInvTenWidget->SetRenderEnable(State);
	}
	void SetShieldInvOneWidget(int Number)
	{
		m_ShieldInvOneWidget->SetNumber(Number);
	}
	void SetShieldInvOneRenderEnable(bool State)
	{
		m_ShieldInvOneWidget->SetRenderEnable(State);
	}
// Bullet Info ---
private :
	void SetGunBulletUI(std::vector<std::wstring>& vecNumberFileName);
private:
	// Gun 
	class   CNumberWidget* m_BulletHunderedWidget;
	class   CNumberWidget* m_BulletTenWidget;
	class   CNumberWidget* m_BulletOneWidget;
public:
	void SetBulletHundredWidget(int Number)
	{
		m_BulletHunderedWidget->SetNumber(Number);
	}
	void SetBulletHundredRenderEnable(bool State)
	{
		m_BulletHunderedWidget->SetRenderEnable(State);
	}

	void SetBulletTenWidget(int Number)
	{
		m_BulletTenWidget->SetNumber(Number);
	}
	void SetBulletTenRenderEnable(bool State)
	{
		m_BulletTenWidget->SetRenderEnable(State);
	}

	void SetBulletOneWidget(int Number)
	{
		m_BulletOneWidget->SetNumber(Number);
	}
	void SetBulletOneRenderEnable(bool State)
	{
		m_BulletOneWidget->SetRenderEnable(State);
	}
private:
	class   CNumberWidget* m_FullBulletHunderedWidget;
	class   CNumberWidget* m_FullBulletTenWidget;
	class   CNumberWidget* m_FullBulletOneWidget;
public:
	void SetFullBulletHundredWidget(int Number)
	{
		m_FullBulletHunderedWidget->SetNumber(Number);
	}
	void SetFullBulletHundredRenderEnable(bool State)
	{
		m_FullBulletHunderedWidget->SetRenderEnable(State);
	}

	void SetFullBulletTenWidget(int Number)
	{
		m_FullBulletTenWidget->SetNumber(Number);
	}
	void SetFullBulletTenRenderEnable(bool State)
	{
		m_FullBulletTenWidget->SetRenderEnable(State);
	}

	void SetFullBulletOneWidget(int Number)
	{
		m_FullBulletOneWidget->SetNumber(Number);
	}
	void SetFullBulletOneRenderEnable(bool State)
	{
		m_FullBulletOneWidget->SetRenderEnable(State);
	}
public:
	virtual bool Init();
};

