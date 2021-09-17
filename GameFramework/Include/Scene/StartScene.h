#pragma once
#include "Scene.h"
class CStartScene :
    public CScene
{
	friend class CSceneManager;

private:
	CStartScene();
	virtual ~CStartScene();

public:
	virtual bool Init();

private:
	void LoadAnimationSequence();
	void LoadSound();
};

