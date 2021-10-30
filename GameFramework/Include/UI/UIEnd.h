#pragma once
#include "UIWindow.h"
class CUIEnd :
    public CUIWindow
{
	friend class CScene;

private:
	CUIEnd();
	virtual ~CUIEnd();
private:
	CSharedPtr<class CUIImage> m_UpperImg;
	CSharedPtr<class CUIImage> m_DownImg;

public:
	virtual bool Init();
};

