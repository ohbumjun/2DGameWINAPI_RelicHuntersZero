#include "Potion.h"
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderSphere.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CPotion::CPotion()
{
}

CPotion::CPotion(const CPotion& Potion) : CGameObject(Potion)
{
}

CPotion::~CPotion()
{
}

void CPotion::Start()
{
	CGameObject::Start();
}

bool CPotion::Init()
{
	if (!CGameObject::Init())
		return false;

	// Add Collider
	CColliderSphere* Body = AddCollider<CColliderSphere>("Body");
	Body->SetRadius(30.f);
	Body->SetOffset(25.f, 25.f);
	Body->SetCollisionProfile("Default");


	return true;
}

void CPotion::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CPotion::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CPotion::Collision(float DeltaTime)
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
			// if (PlayerDir.x == 0 && PlayerDir.y == -1)
				// ChangeScene();
		}
	}
}

void CPotion::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CPotion* CPotion::Clone()
{
	return new CPotion();
}
