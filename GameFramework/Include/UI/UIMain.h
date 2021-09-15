#pragma once
#include "UIWindow.h"
class CUIMain :
    public CUIWindow
{
	friend class CScene;

private:
	CUIMain();
	virtual ~CUIMain();

public:
	virtual bool Init();
};

