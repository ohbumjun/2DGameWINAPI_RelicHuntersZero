#include "EffectGrenade.h"
#include "EffectText.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CEffectGrenade::CEffectGrenade() :
	m_SpeedX(100.f),
	m_FallTime(0.f)
{
}

CEffectGrenade::CEffectGrenade(const CEffectGrenade& obj) :
	CGameObject(obj)
{
	m_SpeedX = obj.m_SpeedX;
	m_FallTime = 0.f;
}

CEffectGrenade::~CEffectGrenade()
{
}

void CEffectGrenade::Start()
{
	CGameObject::Start();
	// Random Jump Velocity
	SetJumpVelocity(10.f);
	Jump();
}

bool CEffectGrenade::Init()
{
	if (!CGameObject::Init()) return false;

	SetPhysicsSimulate(true);
	return true;
}

void CEffectGrenade::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	
	m_FallTime += DeltaTime;
	if (m_FallTime >= 0.7f)
	{
		ChangeExplosionAnimation();
	}
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
	m_IsGround = true;

	SetOffset(Vector2(-m_Size.x * 0.45f, 0));
	
	// Activate Animation
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