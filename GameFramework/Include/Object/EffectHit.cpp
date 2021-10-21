
#include "EffectHit.h"
#include "Bullet.h"
#include "../Scene/Scene.h"
#include "../Collision/ColliderBox.h"

CEffectHit::CEffectHit() : m_IsBossGun(false)
{
	m_ObjType = EObject_Type::Effect;
}

CEffectHit::CEffectHit(const CEffectHit& obj) :
	CGameObject(obj)
{
	m_IsBossGun = false;
}

CEffectHit::~CEffectHit()
{
}

void CEffectHit::SetIsBossGun()
{
	m_IsBossGun = true;
	SetCurrentAnimation(BOSS_BULLET_HIT_EFFECT);
}

void CEffectHit::Start()
{
	CGameObject::Start();
	SetAnimationEndNotify<CEffectHit>(HIT_EFFECT, this, &CEffectHit::AnimationFinish);
	SetAnimationEndNotify<CEffectHit>(BOSS_BULLET_HIT_EFFECT, this, &CEffectHit::AnimationFinish);
}

bool CEffectHit::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);
	CreateAnimation();
	AddAnimation(HIT_EFFECT, false, 0.3f);
	AddAnimation(BOSS_BULLET_HIT_EFFECT, false, 0.5f);
	SetCurrentAnimation(HIT_EFFECT);

	return true;
}

void CEffectHit::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CEffectHit::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CEffectHit::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CEffectHit::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CEffectHit* CEffectHit::Clone()
{
	return new CEffectHit(*this);
}

void CEffectHit::AnimationFinish()
{
	Destroy();
}
