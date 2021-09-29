#pragma once
#include "UIWindow.h"
class CUIStart :
    public CUIWindow
{
	friend class CScene;

private:
	CUIStart();
	virtual ~CUIStart();

public:
	virtual bool Init();

public:
	void StartClick();
	void ExitClick();
	void EditorClick();
};

