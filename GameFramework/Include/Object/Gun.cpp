#include "Gun.h"
#include "../Collision/ColliderSphere.h"

CGun::CGun()
{
	m_GunInfo.m_GunClass = EGunClass::Light;
	m_GunInfo.m_GunType        = EGun_Type::Light_Pistol;
	m_GunInfo.m_Damage         = NORMAL_MONSTER_ATTACK;
	m_GunInfo.m_BulletsLoaded  = true;
	m_GunInfo.m_BulletLoadTime = 0.1f;
	m_GunInfo.m_BulletDistance = NORMAL_BULLET_DISTANCE;

	m_Owner = nullptr;
	m_ObjType = EObject_Type::Weapon;

}

CGun::CGun(const CGun& obj) : CGameObject(obj)
{
	m_Owner = nullptr;
	m_GunInfo = obj.m_GunInfo;
}

CGun::~CGun()
{
}

void CGun::Shoot()
{
}

void CGun::Start()
{
}

bool CGun::Init()
{
	if (!CGameObject::Init()) return false;

	CreateAnimation();

	SetPivot(0.5f, 0.5f);
	CColliderSphere* Body = AddCollider<CColliderSphere>("Body");
	Body->SetRadius(25.f);
	Body->SetOffset(0.f, 0.f);

	return true;
}

void CGun::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	if (m_Owner)
		SetPos(m_Owner->GetPos());
}

void CGun::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CGun::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CGun::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CGun* CGun::Clone()
{
	return new CGun(*this);
}
