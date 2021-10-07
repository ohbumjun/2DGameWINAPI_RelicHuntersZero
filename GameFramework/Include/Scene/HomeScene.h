#pragma once
#include "Scene.h"
class CHomeScene :
    public CScene
{
	friend class CSceneManager;

private:
	CHomeScene();
	virtual ~CHomeScene();

public:
	virtual bool Init();

private:
	void LoadAnimationSequence();
	void LoadSound();
	void SetBasicGuns();
};

