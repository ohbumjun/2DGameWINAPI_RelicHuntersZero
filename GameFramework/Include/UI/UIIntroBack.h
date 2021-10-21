#pragma once
#include "UIWidget.h"
#include "UIAnimation.h"
class CUIIntroBack :
    public CUIAnimation
{
public:
	CUIIntroBack();
	virtual ~CUIIntroBack();
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC);
};

