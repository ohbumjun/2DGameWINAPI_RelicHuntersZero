#pragma once
#include "UIWindow.h"
class CUISelect :
    public CUIWindow
{
	friend class CScene;

private:
	CUISelect();
	virtual ~CUISelect();
private:
	void CharacterClick();
private:
	void CharacterBarInit();
	void AssInit();
	void BiuInit();
	void JimmyInit();
	void PingkyInit();
	void PunnyInit();
	void RaffInit();
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
private:
	void StartClick();
	void ExitClick();
};

