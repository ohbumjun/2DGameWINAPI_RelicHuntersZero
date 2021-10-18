#include "EffectShield.h"

CEffectShield::CEffectShield() : m_Owner(nullptr)
{
	m_ObjType = EObject_Type::Effect;
	m_ShieldType = EShield_Type::Player;
}

CEffectShield::CEffectShield(const CEffectShield& obj) : CGameObject(obj)
{
	CGameObject::Start();
	m_Owner = nullptr;
}

CEffectShield::~CEffectShield()
{
}

void CEffectShield::Start()
{
	CGameObject::Start();
	// SetAnimationEndNotify<CEffectShield>(SHIELD_TURTLE, this, &CEffectShield::AnimationFinish);
	// SetAnimationEndNotify<CEffectShield>(SHIELD_PLAYER, this, &CEffectShield::AnimationFinish);
	// SetAnimationEndNotify<CEffectShield>(SHIELD_BOSS,   this, &CEffectShield::AnimationFinish);
}

bool CEffectShield::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);
	CreateAnimation();
	AddAnimation(SHIELD_TURTLE, true, 0.5f);
	AddAnimation(SHIELD_PLAYER, true, 0.5f);
	AddAnimation(SHIELD_BOSS,   true, 0.5f);
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

	if (m_Owner)
	{
		Vector2 OwnerSize = m_Owner->GetSize();
		Vector2 OwnerPos = m_Owner->GetPos();
		m_Pos.y = OwnerPos.y - OwnerSize.y * 0.5f;
		m_Pos.x = OwnerPos.x;
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
