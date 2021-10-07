#include "Gun.h"

CGun::CGun()
{
	m_GunInfo.m_GunType        = EGun_Type::Light_Pistol;
	m_GunInfo.m_Damage         = NORMAL_MONSTER_ATTACK;
	m_GunInfo.m_BulletsLoaded  = true;
	m_GunInfo.m_BulletLoadTime = 0.1f;
	m_GunInfo.m_BulletDistance = NORMAL_BULLET_DISTANCE;
	EGunInfo m_GunInfo;
}

CGun::CGun(const CGun& obj) : CGameObject(obj)
{
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
	return false;
}

void CGun::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
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
