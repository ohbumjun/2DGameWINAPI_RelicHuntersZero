
#include "EffectBulletStart.h"
#include "Bullet.h"
#include "../Scene/Scene.h"
#include "../Collision/ColliderBox.h"

CEffectBulletStart::CEffectBulletStart()
{
	m_ObjType = EObject_Type::Effect;
}

CEffectBulletStart::CEffectBulletStart(const CEffectBulletStart& obj) :
	CGameObject(obj)
{
}

CEffectBulletStart::~CEffectBulletStart()
{
}

void CEffectBulletStart::Start()
{
	CGameObject::Start();
	// Set Animation According To Player Dir
	CGameObject* Player = m_Scene->GetPlayer();
	Vector2 PlayerDir = Player->GetDir();

	// ¿ÞÂÊ 
	if (PlayerDir.x < 0)
	{
		AddAnimation(BULLETSTART_EFFECT_L, false, 0.1f);
		SetAnimationEndNotify<CEffectBulletStart>(BULLETSTART_EFFECT_L, this, &CEffectBulletStart::AnimationFinish);
	}
	// ¿À¸¥ÂÊ 
	else
	{
		AddAnimation(BULLETSTART_EFFECT_R, false, 0.1f);
		SetAnimationEndNotify<CEffectBulletStart>(BULLETSTART_EFFECT_R, this, &CEffectBulletStart::AnimationFinish);
	}
}

bool CEffectBulletStart::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);
	CreateAnimation();


	return true;
}

void CEffectBulletStart::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CEffectBulletStart::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CEffectBulletStart::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CEffectBulletStart::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CEffectBulletStart* CEffectBulletStart::Clone()
{
	return new CEffectBulletStart(*this);
}

void CEffectBulletStart::AnimationFinish()
{
	Destroy();
}
