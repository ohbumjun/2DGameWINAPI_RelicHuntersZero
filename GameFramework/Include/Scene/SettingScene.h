#pragma once
#include "Scene.h"
class CSettingScene :
    public CScene
{
	friend class CSceneManager;

private:
	CSettingScene();
	virtual ~CSettingScene();

public:
	virtual bool Init();

private:
	void LoadAnimationSequence();
	void LoadSound();

};

