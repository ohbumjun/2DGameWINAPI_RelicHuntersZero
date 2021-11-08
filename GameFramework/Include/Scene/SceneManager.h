#pragma once

#include "../GameInfo.h"
#include "Scene.h"

class CSceneManager
{
	friend class CGameManager;
private:
	CSceneManager();
	~CSceneManager();

private:
	bool    m_NewPlay;
	CScene* m_Scene;
	CScene* m_NextScene;
	CSharedPtr<class CGameObject> m_CommonPlayer;

public:
	void SetNewPlay(bool State) { m_NewPlay = State; }
	CScene* GetScene() { return m_Scene; }
	class CGameObject* GetPlayer() {
		if (m_CommonPlayer)
			return m_CommonPlayer;
		return nullptr;
	}
	void UpdateStaticObjects(CGameObject* Object)
	{
		// 어차피 공유되는 Object는 Player 한명이기 때문이다 
		m_CommonPlayer = Object->Clone();
	}
	void DeleteStaticPlayer()
	{
		m_CommonPlayer = nullptr;
	}
	void SetCommonPlayerColliderSceneNull()
	{
		if(m_CommonPlayer)
			m_CommonPlayer->SetColliderSceneNull();
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