#pragma once
#include "UIWindow.h"
class CUIEnd :
    public CUIWindow
{
	friend class CScene;

private:
	CUIBossEnd();
	virtual ~CUIBossEnd();
private:
	CSharedPtr<class CUIImage> m_UpperImg;
	CSharedPtr<class CUIImage> m_DownImg;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
};
};

