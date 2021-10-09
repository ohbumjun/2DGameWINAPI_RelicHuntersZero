#include "EffectCasing.h"

CEffectCasing::CEffectCasing() :
	m_DirX(0),
	m_SpeedX(0)
{
}

CEffectCasing::CEffectCasing(const CEffectCasing& obj) :
	CGameObject(obj)
{
	m_DirX = obj.m_DirX;
	m_SpeedX = obj.m_SpeedX;
}

CEffectCasing::~CEffectCasing()
{
}

void CEffectCasing::Start()
{
	CGameObject::Start();
	m_SpeedX = (float)(rand() % 300);

	SetJumpVelocity(30.f);
	SetPhysicsSimulate(true);
	Jump();
	SetLifeTime(3.f);
}

bool CEffectCasing::Init()
{
	if (!CGameObject::Init()) return false;

	// Physics Setting
	SetJumpVelocity(30.f);
	SetPhysicsSimulate(true);
	Jump();
	SetLifeTime(3.f);

	return true;
}

void CEffectCasing::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	Move(Vector2(m_DirX * m_SpeedX * DeltaTime, 0.f));
}

void CEffectCasing::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CEffectCasing::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CEffectCasing::Render(HDC hDC)
{
	// this
	CGameObject::Render(hDC);
}

CEffectCasing* CEffectCasing::Clone()
{
	return new CEffectCasing(*this);
}
