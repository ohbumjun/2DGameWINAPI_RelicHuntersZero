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
		// 현재 player 정보로 static 정보를 update 해준다
		CGameObject* CurPlayer = m_Scene->GetPlayer();
		if (CurPlayer)
		{
			UpdateStaticObjects(CurPlayer);
		}

		// 기존 장면을 제거한다.
		SAFE_DELETE(m_Scene);
		CInput::GetInst()->ClearCallback();

		// 전제 : m_NextScene은 init이 실패할 일이 절대 없다는 것을 전재로 한다 
		// m_Scene에 있는 기존 resource를 모두 지운 다음
		// 그 다음 m_NextScene을 init 해주어야 한다 
		m_NextScene->Init();
		// 생성되어 있는 다음 장면을 기존 장면으로 지정한다.
		m_Scene = m_NextScene;
		// 변수를 초기화한다.
		m_NextScene = nullptr;

		return true;
	}

	return false;
}