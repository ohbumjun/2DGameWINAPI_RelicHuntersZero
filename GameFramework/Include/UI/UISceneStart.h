#pragma once
#include "UIAnimation.h"
class CUISceneStart :
    public CUIAnimation
{
public :
    CUISceneStart();
    CUISceneStart(const CUISceneStart& widget);
    virtual ~CUISceneStart();
private :
	ESceneStage m_Stage;
public :
	void SetSceneStage(ESceneStage Stage);
	
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC);
};

