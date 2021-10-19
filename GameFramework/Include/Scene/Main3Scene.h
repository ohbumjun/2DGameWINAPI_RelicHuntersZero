#pragma once
#include "Scene.h"
class CMain3Scene :
    public CScene
{
	friend class CSceneManager;

private:
	CMain3Scene();
	virtual ~CMain3Scene();

public:
	virtual bool Init();

private:
	void LoadAnimationSequence();
	void LoadSound();
private:
	void GoBackToWaitingScene();
};

