#include "EffectSurprise.h"
#include "../Scene/Scene.h"
#include "../Collision/ColliderSphere.h"

CEffectSurprise::CEffectSurprise()
{
    m_ObjType = EObject_Type::Effect;
}

CEffectSurprise::CEffectSurprise(const CEffectSurprise& obj) : CGameObject(obj)
{
}

CEffectSurprise::~CEffectSurprise()
{
}

void CEffectSurprise::Start()
{
    CGameObject::Start();
    SetAnimationEndNotify<CEffectSurprise>(SURPRISE_EFFECT, this, &CEffectSurprise::AnimationFinish);
}

bool CEffectSurprise::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);
	CreateAnimation();
	AddAnimation(SURPRISE_EFFECT, false, 0.7f);

	return true;
}

void CEffectSurprise::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CEffectSurprise::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CEffectSurprise::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CEffectSurprise::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CEffectSurprise* CEffectSurprise::Clone()
{
    return new CEffectSurprise(*this);
}

void CEffectSurprise::AnimationFinish()
{
	Destroy();
}
