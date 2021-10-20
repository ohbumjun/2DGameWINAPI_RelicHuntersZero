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
	// SetAnimationEndNotify<CEffectDoorAbove>(STAGE_DOOR_ABOVE_EFFECT, this, &CEffectDoorAbove::AnimationFinish);
}

bool CEffectDoorAbove::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);
	CreateAnimation();
	AddAnimation(STAGE_DOOR_ABOVE_EFFECT, false, 0.7f);

	return true;
}

void CEffectDoorAbove::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
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
