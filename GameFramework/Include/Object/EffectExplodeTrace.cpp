#include "EffectExplodeTrace.h"
#include "../Scene/Scene.h"
#include "../Collision/ColliderSphere.h"

CEffectExplodeTrace::CEffectExplodeTrace()
{
	m_ObjType = EObject_Type::Effect;
}

CEffectExplodeTrace::CEffectExplodeTrace(const CEffectExplodeTrace& obj) : CGameObject(obj)
{
}

CEffectExplodeTrace::~CEffectExplodeTrace()
{
}

void CEffectExplodeTrace::Start()
{
	CGameObject::Start();
	SetAnimationEndNotify<CEffectExplodeTrace>(GRENADE_OFF, this, &CEffectExplodeTrace::AnimationFinish);
}

bool CEffectExplodeTrace::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);
	CreateAnimation();
	AddAnimation(GRENADE_OFF, true, 10.f);

	return true;
}

void CEffectExplodeTrace::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CEffectExplodeTrace::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CEffectExplodeTrace::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CEffectExplodeTrace::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CEffectExplodeTrace* CEffectExplodeTrace::Clone()
{
	return new CEffectExplodeTrace(*this);
}

void CEffectExplodeTrace::AnimationFinish()
{
	Destroy();
}
