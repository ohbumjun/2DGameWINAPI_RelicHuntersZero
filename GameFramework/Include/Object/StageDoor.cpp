#include "StageDoor.h"
#include "../Scene/SceneManager.h"
#include "../Scene/HomeScene.h"
#include "../Scene/MainScene.h"

CStageDoor::CStageDoor()
{
    m_DoorStageType = EDoorStage_Type::Stage_Default;
}

CStageDoor::CStageDoor(const CStageDoor& obj) :
    CGameObject(obj)
{
}

CStageDoor::~CStageDoor()
{
}

void CStageDoor::Start()
{
	CGameObject::Start();
}

bool CStageDoor::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);
	CreateAnimation();
	AddAnimation("StageDoor", true, 2.f);
}

void CStageDoor::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CStageDoor::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CStageDoor::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CStageDoor::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CStageDoor* CStageDoor::Clone()
{
	return new CStageDoor(*this);
}

void CStageDoor::ChangeStage()
{
	switch (m_DoorStageType)
	{
	case EDoorStage_Type::Stage_Home:
		CSceneManager::GetInst()->CreateScene<CHomeScene>();
		break;
	case EDoorStage_Type::Stage_One:
		CSceneManager::GetInst()->CreateScene<CMainScene>();
		break;
	case EDoorStage_Type::Stage_Two:
		// CSceneManager::GetInst()->CreateScene<CHomeScene>();
		break;
	case EDoorStage_Type::Stage_Three:
		// CSceneManager::GetInst()->CreateScene<CHomeScene>();
		break;
	}
}
