#include "Potion.h"
#include "../Collision/ColliderSphere.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CPotion::CPotion()
{
}

CPotion::CPotion(const CPotion& Potion)
{
}

CPotion::~CPotion()
{
}

void CPotion::Start()
{
}

bool CPotion::Init()
{
	if (!CGameObject::Init())
		return false;

	// Add Collider
	CColliderSphere* Body = AddCollider<CColliderSphere>("Body");
	Body->SetRadius(20.f);
	Body->SetOffset(0.f, 25.f);
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
}

void CPotion::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CPotion* CPotion::Clone()
{
	return new CPotion();
}
