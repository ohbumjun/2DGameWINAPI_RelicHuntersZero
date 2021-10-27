#include "BossGun.h"

CBossGun::CBossGun()
{
    m_FireTimeMax = 3.f;
    m_GunInfo.m_GunClass = EGunClass::Boss;
    m_GunInfo.m_GunType = EGun_Type::BossGun;
}

CBossGun::CBossGun(const CBossGun& obj) : CGun(obj)
{
    m_FireTimeMax = obj.m_FireTimeMax;
    m_GunInfo.m_GunClass = EGunClass::Boss;
}

CBossGun::~CBossGun()
{
}

void CBossGun::Start()
{
}

bool CBossGun::Init()
{
    return false;
}

void CBossGun::Update(float DeltaTime)
{
}

void CBossGun::PostUpdate(float DeltaTime)
{
}

void CBossGun::Collision(float DeltaTime)
{
}

void CBossGun::Render(HDC hDC)
{
}

CBossGun* CBossGun::Clone()
{
    return nullptr;
}
