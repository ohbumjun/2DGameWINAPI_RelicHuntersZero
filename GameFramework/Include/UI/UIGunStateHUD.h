#pragma once
#include "UIWindow.h"
#include "ProgressBar.h"
#include "Button.h"
#include "UIImage.h"

class CUIGunStateHUD :
    public CUIWindow
{
	friend class CScene;

private:
	CUIGunStateHUD();
	virtual ~CUIGunStateHUD();

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
private :
	CSharedPtr<CUIImage> m_CurrentGunImage;
	CSharedPtr<CUIImage> m_SecondGunImage;
	CSharedPtr<CUIImage> m_ThirdGunImage;
public:
	void SetCurrentGunImage(const std::string &TextureName)
	{
		m_CurrentGunImage->SetTexture(TextureName);
	}
	void SetSecondGunImage(const std::string &TextureName)
	{
		m_SecondGunImage->SetTexture(TextureName);
	}
	void SetThirdGunImage(const std::string &TextureName)
	{
		m_ThirdGunImage->SetTexture(TextureName);
	}

public:
	virtual bool Init();
};

