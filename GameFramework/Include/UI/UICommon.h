#pragma once
#include "UIWindow.h"
#include "UISceneStart.h"
class CUICommon :
    public CUIWindow
{
	friend class CScene;

private:
	CUICommon();
	virtual ~CUICommon();
private :
	CSharedPtr<CUISceneStart> StartSceneWidget;
public :
	void SetSceneStage(ESceneStage Stage);
public:
	virtual bool Init();
};

