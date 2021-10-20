#include "EffectReload.h"
#include "../Scene/Scene.h"
#include "../Object/Player.h"
#include "../Object/Gun.h"
#include "../Collision/ColliderSphere.h"

CEffectReload::CEffectReload()
{
	m_ObjType = EObject_Type::Effect;
}

CEffectReload::CEffectReload(const CEffectReload& obj) : CGameObject(obj)
{
}

CEffectReload::~CEffectReload()
{
}

void CEffectReload::Start()
{
	CGameObject::Start();
	SetAnimationEndNotify<CEffectReload>(RELOAD_EFFECT, this, &CEffectReload::AnimationFinish);
}

bool CEffectReload::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);
	CreateAnimation();
	AddAnimation(RELOAD_EFFECT, false, 2.0f);

	return true;
}

void CEffectReload::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	if (m_Owner)
		m_Pos = m_Owner->GetPos();
}

void CEffectReload::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CEffectReload::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CEffectReload::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CEffectReload* CEffectReload::Clone()
{
	return new CEffectReload(*this);
}

void CEffectReload::AnimationFinish()
{
	Destroy();
	
	// Player Gun Reload
	CPlayer* Player = (CPlayer*)m_Scene->GetPlayer();
	CGun* PlayerCurGun = Player->GetCurrentGun();
	if (PlayerCurGun)
	{
		PlayerCurGun->SetReloadBullet();
	}
}
