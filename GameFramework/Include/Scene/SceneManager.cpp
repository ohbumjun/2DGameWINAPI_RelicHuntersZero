#include "SceneManager.h"
#include "Scene.h"
#include "../Input.h"

CSceneManager* CSceneManager::m_Inst = nullptr;

CSceneManager::CSceneManager() :
	m_Scene(nullptr),
	m_NextScene(nullptr),
	m_CommonPlayer{}
{
}

CSceneManager::~CSceneManager()
{
	SAFE_DELETE(m_NextScene);
	SAFE_DELETE(m_Scene);
}

bool CSceneManager::Init()
{
	m_Scene = new CScene;
	m_Scene->Init();
	return true;
}

bool CSceneManager::Update(float DeltaTime)
{
	m_Scene->Update(DeltaTime);
	return ChangeScene();
}

bool CSceneManager::PostUpdate(float DeltaTime)
{
	m_Scene->PostUpdate(DeltaTime);
	return ChangeScene();
}

bool CSceneManager::Collision(float DeltaTime)
{
	m_Scene->Collision(DeltaTime);
	return ChangeScene();
}

bool CSceneManager::Render(HDC hDC)
{
	m_Scene->Render(hDC);
	return ChangeScene();
}

bool CSceneManager::ChangeScene()
{
	if (m_NextScene)
	{
		CPlayer* CurPlayer = (CPlayer*)m_Scene->GetPlayer();
		
		if (CurPlayer)
		{
			if (CurPlayer->m_PlayerDeath)
				CurPlayer->m_CharacterInfo.HP = CurPlayer->m_CharacterInfo.HPMax;
			UpdateStaticObjects(CurPlayer);
		}

		SAFE_DELETE(m_Scene);
		CInput::GetInst()->ClearCallback();

		m_NextScene->Init();
		m_Scene = m_NextScene;
		m_NextScene = nullptr;

		return true;
	}

	return false;
}