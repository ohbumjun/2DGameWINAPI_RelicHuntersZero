#pragma once
#include "UIWindow.h"
#include "NumberWidget.h"

class CUIHome :
    public CUIWindow
{
	friend class CScene;

private:
	CUIHome();
	virtual ~CUIHome();
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

};

