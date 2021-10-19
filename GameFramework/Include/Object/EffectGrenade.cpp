#include "EffectGrenade.h"

CEffectGrenade::CEffectGrenade() :
	m_SpeedX(100.f)
{
}

CEffectGrenade::CEffectGrenade(const CEffectGrenade& obj) :
	CGameObject(obj)
{
	m_SpeedX = obj.m_SpeedX;
}

CEffectGrenade::~CEffectGrenade()
{
}

void CEffectGrenade::Start()
{
	CGameObject::Start();
	// Random Jump Velocity
	SetJumpVelocity((float)(10.f + rand() % 20));
	Jump();

	
}

bool CEffectGrenade::Init()
{
	if (!CGameObject::Init()) return false;

	// Offset
	SetOffset(Vector2(-m_Size.x / 2, 0));

	// Physics Setting
	SetJumpVelocity(40.f);
	SetPhysicsSimulate(true);
	Jump();

	return true;
}

void CEffectGrenade::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	Move(Vector2(m_SpeedX * DeltaTime, 0.f));

}

void CEffectGrenade::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CEffectGrenade::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CEffectGrenade::Render(HDC hDC)
{
	// this
	CGameObject::Render(hDC);
}

CEffectGrenade* CEffectGrenade::Clone()
{
	return new CEffectGrenade(*this);
}

void CEffectGrenade::ChangeExplosionAnimation()
{
	AddAnimation(GRENADE_ON, false, 2.f);
	// EndNotify
	SetAnimationEndNotify<CEffectGrenade>(GRENADE_ON, this, &CEffectGrenade::AnimationFinish);
}

void CEffectGrenade::Explode()
{
}

void CEffectGrenade::AnimationFinish()
{
	Destroy();
	Explode();
}
