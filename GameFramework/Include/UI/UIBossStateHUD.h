#pragma once
#include "UIWindow.h"
#include "ProgressBar.h"
#include "UIImage.h"

class CUIBossStateHUD :
    public CUIWindow
{
	friend class CScene;

private:
	CUIBossStateHUD();
	virtual ~CUIBossStateHUD();
	// HP,MP Bar
private:
	CSharedPtr<CProgressBar> m_HPBar;
private:
	void SetHpBarUI();
public:
	void SetHPPercent(float Percent)
	{
		m_HPBar->SetPercent(Percent);
	}
// Gold
private:
	CSharedPtr<CUIImage> m_GeneratorImage;
private:
	void SetGeneratorUI();
public:
	void SetGeneratorImage(const std::string& TextureName)
	{
		m_GeneratorImage->SetTexture(TextureName);
	}
public:
	virtual bool Init();
};

