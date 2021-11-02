#pragma once
#include "Scene.h"
class CHomeScene :
    public CScene
{
	friend class CSceneManager;

private:
	CHomeScene();
	virtual ~CHomeScene();

private :
	std::list<CSharedPtr<class CPlayerClone>> PlayerCloneLists;
public:
	virtual bool Init();

private:
	void LoadAnimationSequence();
	void LoadSound();

};

