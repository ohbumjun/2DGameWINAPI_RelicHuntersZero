#pragma once

#include "UIWindow.h"
#include "ProgressBar.h"
#include "Button.h"
#include "UIImage.h"

class CUICharacterStateHUD : public CUIWindow
{
	friend class CScene;

private:
	CUICharacterStateHUD();
	virtual ~CUICharacterStateHUD();

private:
	CSharedPtr<CProgressBar> m_HPBar;
	CSharedPtr<CProgressBar> m_SteminaBar;

public:
	void SetHPPercent(float Percent)
	{
		m_HPBar->SetPercent(Percent);
	}
	void SetSteminaPercent(float Percent)
	{
		m_SteminaBar->SetPercent(Percent);
	}

public:
	virtual bool Init();
};
