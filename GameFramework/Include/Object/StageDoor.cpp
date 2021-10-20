#include "StageDoor.h"
#include "../Scene/SceneManager.h"
#include "../Scene/HomeScene.h"
#include "../Scene/MainScene.h"
#include "../Scene/Main2Scene.h"
#include "../Scene/Main3Scene.h"
#include "../Scene/StartScene.h"
#include "../Collision/ColliderBox.h"
#include "Player.h"
#include "EffectDoorAbove.h"

CStageDoor::CStageDoor() : m_DoorEffectEnable(false)
{
    m_DoorStageType = EDoorStage_Type::Stage_Default;
	m_ObjType = EObject_Type::GameObject;
}

CStageDoor::CStageDoor(const CStageDoor& obj) :
    CGameObject(obj)
{
	m_DoorStageType = obj.m_DoorStageType;
	m_DoorEffectEnable = false;
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

	return true;
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
	bool m_PlayerCollision = false;
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		CPlayer* Player   = (*iter)->IsCollisionWithPlayer();
		// 만약 Player와 충돌했다면
		if (Player)
		{
			Vector2 PPos = Player->GetPos();
			Vector2 PSize = Player->GetSize();
			// Vector2 POffSet = Player->GetOffset();
			Vector2 PPivot = Player->GetPivot();
			// Stage Door Above Effect 
			if (!m_DoorEffectEnable)
			{
				CEffectDoorAbove* AboveEffect = m_Scene->CreateObject<CEffectDoorAbove>(
					"DoorAboveEffect",
					STAGE_DOOR_ABOVE_PROTO,
					Vector2(m_Pos.x, m_Pos.y - m_Pivot.y * m_Size.y)
					);
				m_DoorEffectEnable = true;
			}
			m_PlayerCollision = true;

			// 위로 가기 버튼을 클릭했다면( Dir )
			Vector2 PlayerDir = Player->GetDir();
			if (PlayerDir.x == 0 && PlayerDir.y == -1)
				ChangeScene();
		}
	}
	// Did not collide with Player && DoorEffectEnabled = Player Out 
	if (!m_PlayerCollision && m_DoorEffectEnable)
		m_DoorEffectEnable = false;
		
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
		CSceneManager::GetInst()->CreateScene<CMain2Scene>();
		break;
	case EDoorStage_Type::Stage_Three:
		CSceneManager::GetInst()->CreateScene<CMain3Scene>();
		break;
	}
}
