#include "PistolGun.h"
#include "Gun.h"

CPistolGun::CPistolGun()
{
	m_FireTimeMax = 0.5f;
	m_GunInfo.m_GunType = EGun_Type::Pistol;
	m_GunInfo.m_BulletsLoaded = 200;
	m_GunInfo.m_BulletsFullNum = 200;
}


CPistolGun::CPistolGun(const CPistolGun& obj) : CGun(obj)
{
	m_FireTimeMax = obj.m_FireTimeMax;
	m_GunInfo.m_BulletsLoaded = 200;
	m_GunInfo.m_BulletsFullNum = 200;
}

CPistolGun::~CPistolGun()
{
}

void CPistolGun::Start()
{
	CGun::Start();
}

bool CPistolGun::Init()
{
	if (!CGun::Init()) return false;
	return true;
}

void CPistolGun::Update(float DeltaTime)
{
	CGun::Update(DeltaTime);
}

void CPistolGun::PostUpdate(float DeltaTime)
{
	CGun::PostUpdate(DeltaTime);
}

void CPistolGun::Collision(float DeltaTime)
{
	CGun::Collision(DeltaTime);
}

void CPistolGun::Render(HDC hDC)
{
	CGun::Render(hDC);
}

CPistolGun* CPistolGun::Clone()
{
	return new CPistolGun(*this);
}
