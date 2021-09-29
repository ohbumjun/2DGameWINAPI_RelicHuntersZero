
#include "EditorScene.h"
#include "SceneResource.h"
#include "../Object/EffectHit.h"
#include "Camera.h"
#include "../UI/UIStart.h"
#include "../Input.h"

CEditorScene::CEditorScene()
{
}

CEditorScene::~CEditorScene()
{
}

bool CEditorScene::Init()
{
	LoadSound();

	LoadAnimationSequence();

	m_Start = false;
	m_OpenEditor = false;

	// 단축키를 통해, Dialog 열어서, 
	// Dialog 상에서 타일 생성하기 
	CInput::GetInst()->SetCallback<CEditorScene>("Editor", KeyState_Down,
		this, &CEditorScene::OnEditor);
	


	return true;
}

bool CEditorScene::Update(float DeltaTime)
{
	if (!m_Start)
	{
		m_Start = true;
	}
	return false;
}

bool CEditorScene::PostUpdate(float DeltaTime)
{
	return false;
}



void CEditorScene::OnEditor(float DeltaTime)
{
	if (!m_OpenEditor)
	{
		m_OpenEditor = true;
	}
	else
	{
		m_OpenEditor = false;
	}
}

void CEditorScene::LoadAnimationSequence()
{
}

void CEditorScene::LoadSound()
{
	GetSceneResource()->LoadSound("UI", false, "ButtonMouseOn", "TeemoSmile.mp3");
	GetSceneResource()->LoadSound("UI", false, "ButtonClick", "TeemoEditorClicck.mp3");

	GetSceneResource()->SetVolume("UI", 3);
}
