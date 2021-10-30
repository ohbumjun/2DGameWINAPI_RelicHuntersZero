#pragma once
#include "Scene.h"
class CEndScene :
    public CScene
{
public:
	CEndScene();
	virtual ~CEndScene();

public:
	virtual bool Init();

private:
	void LoadAnimationSequence();
	void LoadSound();
};

