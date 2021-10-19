#pragma once
#include "UIWindow.h"
#include "NumberWidget.h"
class CUICommon :
    public CUIWindow
{
	friend class CScene;

private:
	CUICommon();
	virtual ~CUICommon();
public:
	virtual bool Init();
};

