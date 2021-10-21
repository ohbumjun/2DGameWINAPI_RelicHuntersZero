#pragma once
#include "UIWindow.h"
class CUIIntro :
    public CUIWindow
{
	friend class CScene;

private:
	CUIIntro();
	virtual ~CUIIntro();

public:
	virtual bool Init();

public:
	void IntroClick();
};

