#pragma once
#include "Scene.h"
class CMenuScene :
    public CScene
{
	friend class CSceneManager;

private:
	CMenuScene();
	virtual ~CMenuScene();

public:
	virtual bool Init();

private:
	void LoadAnimationSequence();
	void LoadSound();

};

