#include "StageDoor.h"
#include "../Scene/SceneManager.h"
#include "../Scene/HomeScene.h"
#include "../Scene/MainScene.h"
#include "../Scene/StartScene.h"
#include "../Collision/ColliderBox.h"

CStageDoor::CStageDoor()
{
    m_DoorStageType = EDoorStage_Type::Stage_Default;
	m_ObjType = EObject_Type::GameObject;
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
	
	// Add Animation
	SetPivot(0.5f, 0.5f);
	CreateAnimation();
	AddAnimation("StageDoor", true, 2.f);

	// Add Collider
	CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(100.f, 80.f);
	Body->SetOffset(0.f, 25.f);
	Body->SetCollisionProfile("Default");
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

	// 충돌 목록 중에서 Player가 있는지 확인한다
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		CGameObject* Player = (*iter)->IsCollisionWithPlayer();
		// 만약 Player와 충돌했다면
		if (Player)
		{
			// 위로 가기 버튼을 클릭했다면( Dir )
			Vector2 PlayerDir = Player->GetDir();

			if (PlayerDir.x == 0 && PlayerDir.y == -1)
				ChangeScene();
		}
	}
}

void CStageDoor::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CStageDoor* CStageDoor::Clone()
{
	return new CStageDoor(*this);
}

void CStageDoor::ChangeScene()
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
