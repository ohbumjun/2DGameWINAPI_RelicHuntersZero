#pragma once

#include "../GameInfo.h"
#include "Scene.h"

class CSceneManager
{
private:
	CSceneManager();
	~CSceneManager();

private:
	CScene* m_Scene;
	CScene* m_NextScene;
	class CGameObject*m_StaticPlayer;

public:
	CScene* GetScene(){	return m_Scene;	}
	class CGameObject* GetPlayer() { return m_StaticPlayer; }
	void UpdateStaticPlayer(CGameObject* Player)
	{
		m_StaticPlayer = Player->Clone();
	}

public:
	bool Init();
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	bool Collision(float DeltaTime);
	bool Render(HDC hDC);

private:
	bool ChangeScene();

private:
	static CSceneManager* m_Inst;

public:
	static CSceneManager* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CSceneManager;

		return m_Inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}

public:
	template <typename T>
	bool CreateScene()
	{
		m_NextScene = new T;

		/*
		if (!m_NextScene->Init())
		{
			SAFE_DELETE(m_NextScene);
			return false;
		}
		*/

		return true;
	}
};

