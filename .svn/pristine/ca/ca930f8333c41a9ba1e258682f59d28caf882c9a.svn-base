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

public:
	void SetHPPercent(float Percent)
	{
		m_HPBar->SetPercent(Percent);
	}

public:
	virtual bool Init();
};

