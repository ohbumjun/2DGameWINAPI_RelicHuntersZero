#pragma once
#include "Scene.h"
class CEditorScene :
    public CScene
{
	friend class CSceneManager;

private:
	CEditorScene();
	virtual ~CEditorScene();

private:
	bool m_Start;
	bool m_OpenEditor;

public :
	virtual bool Init();
	virtual bool Update(float DeltaTime);
	virtual bool PostUpdate(float DeltaTime);
public :
	void OnEditor(float DeltaTime);
	void LoadAnimationSequence();
	void LoadSound();
};

