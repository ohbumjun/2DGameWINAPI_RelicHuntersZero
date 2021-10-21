#pragma once
#include "Scene.h"
class CIntroScene :
    public CScene
{
public:
	CIntroScene();
	virtual ~CIntroScene();

public:
	virtual bool Init();

private:
	void LoadAnimationSequence();
	void LoadSound();
};

