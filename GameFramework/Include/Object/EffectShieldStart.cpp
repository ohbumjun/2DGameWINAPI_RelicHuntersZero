
#include "EffectShieldStart.h"
#include "Bullet.h"
#include "../Scene/Scene.h"
#include "../Collision/ColliderBox.h"

CEffectShieldStart::CEffectShieldStart()
{
	m_ObjType = EObject_Type::Effect;
}

CEffectShieldStart::CEffectShieldStart(const CEffectShieldStart& obj) :
	CGameObject(obj)
{
}

CEffectShieldStart::~CEffectShieldStart()
{
}

void CEffectShieldStart::Start()
{
	CGameObject::Start();
	
}

bool CEffectShieldStart::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);
	CreateAnimation();
	AddAnimation(SHIELD_START,false,1.f);

	return true;
}

void CEffectShieldStart::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	if (m_Owner)
	{
		Vector2 OwnerPos = m_Owner->GetPos();
		m_Pos = Vector2(OwnerPos.x, OwnerPos.y - m_Owner->GetSize().y * 0.5f);
	}
}

void CEffectShieldStart::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CEffectShieldStart::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CEffectShieldStart::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CEffectShieldStart* CEffectShieldStart::Clone()
{
	return new CEffectShieldStart(*this);
}

void CEffectShieldStart::AnimationFinish()
{
	Destroy();
}
