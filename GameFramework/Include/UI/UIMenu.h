#pragma once
#include "UIWindow.h"
class CUIMenu :
    public CUIWindow
{
	friend class CScene;

private:
	CUIMenu();
	virtual ~CUIMenu();

public:
	virtual bool Init();

};

