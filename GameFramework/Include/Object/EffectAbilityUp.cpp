#include "EffectAbilityUp.h"
#include "../Scene/Scene.h"
#include "../Collision/ColliderSphere.h"

CEffectAbilityUp::CEffectAbilityUp()
{
	m_ObjType = EObject_Type::Effect;
}

CEffectAbilityUp::CEffectAbilityUp(const CEffectAbilityUp& obj) : CGameObject(obj)
{
}

CEffectAbilityUp::~CEffectAbilityUp()
{
}

void CEffectAbilityUp::Start()
{
	CGameObject::Start();
	SetAnimationEndNotify<CEffectAbilityUp>(ABILITYUP_EFFECT, this, &CEffectAbilityUp::AnimationFinish);
}

bool CEffectAbilityUp::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);
	CreateAnimation();
	AddAnimation(ABILITYUP_EFFECT, false, 0.7f);

	CColliderSphere* Body = AddCollider<CColliderSphere>("Body");
	Body->SetRadius(25.f);
	Body->SetOffset(0.f, 0.f);

	return true;
}

void CEffectAbilityUp::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CEffectAbilityUp::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CEffectAbilityUp::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CEffectAbilityUp::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CEffectAbilityUp* CEffectAbilityUp::Clone()
{
	return new CEffectAbilityUp(*this);
}

void CEffectAbilityUp::AnimationFinish()
{
	Destroy();
}
