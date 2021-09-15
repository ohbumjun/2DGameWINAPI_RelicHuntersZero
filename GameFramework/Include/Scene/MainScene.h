#pragma once
#include "Scene.h"
class CMainScene :
    public CScene
{
	friend class CSceneManager;

private:
	CMainScene();
	virtual ~CMainScene();

public:
	virtual bool Init();

private:
	void LoadAnimationSequence();
	void LoadSound();
};

