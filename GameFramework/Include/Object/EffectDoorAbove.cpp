#include "EffectDoorAbove.h"
#include "../Scene/Scene.h"
#include "../Collision/ColliderSphere.h"

CEffectDoorAbove::CEffectDoorAbove()
{
	m_ObjType = EObject_Type::Effect;
}

CEffectDoorAbove::CEffectDoorAbove(const CEffectDoorAbove& obj) : CGameObject(obj)
{
}

CEffectDoorAbove::~CEffectDoorAbove()
{
}

void CEffectDoorAbove::Start()
{
	CGameObject::Start();
	SetAnimationEndNotify<CEffectDoorAbove>(STAGE_DOOR_ABOVE_EFFECT, this, &CEffectDoorAbove::AnimationFinish);
}

bool CEffectDoorAbove::Init()
{
	if (!CGameObject::Init()) return false;

	CreateAnimation();
	AddAnimation(STAGE_DOOR_ABOVE_EFFECT, false, 2.0f);

	return true;
}

void CEffectDoorAbove::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	if (m_Owner)
	{
		Vector2 OwnerSize = m_Owner->GetSize();
		Vector2 OwnerPos = m_Owner->GetPos();
		m_Pos.y = OwnerPos.y - OwnerSize.y * 0.5f;
		m_Pos.x = OwnerPos.x;
	}
}

void CEffectDoorAbove::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

}

void CEffectDoorAbove::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CEffectDoorAbove::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

void CEffectDoorAbove::AnimationFinish()
{
	Destroy();
}
