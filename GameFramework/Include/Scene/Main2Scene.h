#pragma once
#include "Scene.h"
class CMain2Scene :
    public CScene
{
	friend class CSceneManager;

private:
	CMain2Scene();
	virtual ~CMain2Scene();

public:
	virtual bool Init();

private:
	void LoadAnimationSequence();
	void LoadSound();
private:
	void GoBackToWaitingScene();
};

