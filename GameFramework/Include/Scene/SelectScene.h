#pragma once
#include "Scene.h"
class CSelectScene :
    public CScene
{
	friend class CSceneManager;

private:
	CSelectScene();
	virtual ~CSelectScene();

public:
	virtual bool Init();

private:
	void LoadAnimationSequence();
	void LoadSound();
};

