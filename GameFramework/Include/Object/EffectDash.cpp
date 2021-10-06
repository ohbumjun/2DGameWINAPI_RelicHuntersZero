#include "EffectDash.h"

CEffectDash::CEffectDash()
{
	m_ObjType = EObject_Type::Effect;
}

CEffectDash::CEffectDash(const CEffectDash& obj) : CGameObject(obj)
{
	CGameObject::Start();
}

CEffectDash::~CEffectDash()
{
}

void CEffectDash::Start()
{
	CGameObject::Start();
	SetAnimationEndNotify<CEffectDash>(DASH_EFFECT, this, &CEffectDash::AnimationFinish);
}

bool CEffectDash::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);
	CreateAnimation();
	AddAnimation(DASH_EFFECT, false, 0.2f);

	return true;
}

void CEffectDash::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CEffectDash::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CEffectDash::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CEffectDash::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CEffectDash* CEffectDash::Clone()
{
	return new CEffectDash(*this);
}

void CEffectDash::AnimationFinish()
{
	Destroy();
}
