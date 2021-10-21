#pragma once
#include "UIAnimation.h"

class CUIIntroLogo :
    public CUIAnimation
{
public:
	CUIIntroLogo();
	virtual ~CUIIntroLogo();
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC);
private :
	void ChangeToNextAnimation();
};

