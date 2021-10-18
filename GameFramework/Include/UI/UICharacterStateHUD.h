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
// HP,MP
private:
	CSharedPtr<CProgressBar> m_HPBar;
	CSharedPtr<CProgressBar> m_MPBar;
	CSharedPtr<CProgressBar> m_SteminaBar;
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
// Gold
private :
	CSharedPtr<CUIImage> m_GoldImage;
public:
	void SetGoldImage(const std::string& TextureName)
	{
		m_GoldImage->SetTexture(TextureName);
	}
private :
	// Gun 
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

public:
	virtual bool Init();
};

