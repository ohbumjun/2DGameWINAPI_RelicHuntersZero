#include "ShotGun.h"
#include "Gun.h"

CShotGun::CShotGun()
{
	m_GunInfo.m_GunType = EGun_Type::Light_ShotGun;
}

CShotGun::CShotGun(const CShotGun& obj) : CGun(obj)
{
}

CShotGun::~CShotGun()
{
}

void CShotGun::Start()
{
	CGun::Start();
}

bool CShotGun::Init()
{
	if (!CGun::Init()) return false;
	return true;
}

void CShotGun::Update(float DeltaTime)
{
	CGun::Update(DeltaTime);
}

void CShotGun::PostUpdate(float DeltaTime)
{
	CGun::PostUpdate(DeltaTime);
}

void CShotGun::Collision(float DeltaTime)
{
	CGun::Collision(DeltaTime);
}

void CShotGun::Render(HDC hDC)
{
	CGun::Render(hDC);
}

CShotGun* CShotGun::Clone()
{
	return new CShotGun(*this);
}
