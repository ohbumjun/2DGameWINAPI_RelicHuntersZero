#include "EffectShield.h"

CEffectShield::CEffectShield()
{
	m_ObjType = EObject_Type::Effect;
	m_ShieldType = EShield_Type::Player;
}

CEffectShield::CEffectShield(const CEffectShield& obj) : CGameObject(obj)
{
	CGameObject::Start();
}

CEffectShield::~CEffectShield()
{
}

void CEffectShield::Start()
{
	CGameObject::Start();
	SetAnimationEndNotify<CEffectShield>(SHIELD_TURTLE, this, &CEffectShield::AnimationFinish);
	SetAnimationEndNotify<CEffectShield>(SHIELD_PLAYER, this, &CEffectShield::AnimationFinish);
	SetAnimationEndNotify<CEffectShield>(SHIELD_BOSS, this, &CEffectShield::AnimationFinish);
}

bool CEffectShield::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);
	CreateAnimation();
	AddAnimation(SHIELD_TURTLE, false, 0.2f);
	AddAnimation(SHIELD_PLAYER, false, 0.2f);
	AddAnimation(SHIELD_BOSS, false, 0.2f);

	return true;
}

void CEffectShield::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	switch (m_ShieldType)
	{
	case EShield_Type::Turtle:
		SetCurrentAnimation(SHIELD_TURTLE);
		break;
	case EShield_Type::Player:
		SetCurrentAnimation(SHIELD_PLAYER);
		break;
	case EShield_Type::Boss:
		SetCurrentAnimation(SHIELD_BOSS);
		break;
	default:
		break;
	}
}

void CEffectShield::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CEffectShield::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CEffectShield::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CEffectShield* CEffectShield::Clone()
{
	return new CEffectShield(*this);
}

void CEffectShield::AnimationFinish()
{
	Destroy();
}
