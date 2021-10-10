#pragma once

#include "UIWindow.h"
#include "ProgressBar.h"
#include "Button.h"
#include "UIImage.h"

class CUICharacterStateHUD :
    public CUIWindow
{
	friend class CScene;

private:
	CUICharacterStateHUD();
	virtual ~CUICharacterStateHUD();

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
private:
	CSharedPtr<CProgressBar> m_GunLightBar;
	CSharedPtr<CProgressBar> m_GunMediumBar;
	CSharedPtr<CProgressBar> m_GunHeavyBar;
public:
	void SetLightGunPercent(float Percent)
	{
		m_GunLightBar->SetPercent(Percent);
	}
	void SetMediumGunPercent(float Percent)
	{
		m_GunMediumBar->SetPercent(Percent);
	}
	void SetHeavyGunPercent(float Percent)
	{
		m_GunHeavyBar->SetPercent(Percent);
	}


public:
	virtual bool Init();
};

