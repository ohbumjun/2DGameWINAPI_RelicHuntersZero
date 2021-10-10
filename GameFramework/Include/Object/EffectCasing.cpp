#include "EffectCasing.h"

CEffectCasing::CEffectCasing() :
	m_DirX(0),
	m_SpeedX(300.f)
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
	// Random Jump Velocity
	SetJumpVelocity((float)(10.f + rand() % 30));
	Jump();
}

bool CEffectCasing::Init()
{
	if (!CGameObject::Init()) return false;

	// Offset
	SetOffset(Vector2(-m_Size.x/2,0));

	// Physics Setting
	SetJumpVelocity(40.f);
	SetPhysicsSimulate(true);
	Jump();
	SetLifeTime(0.5f);

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
