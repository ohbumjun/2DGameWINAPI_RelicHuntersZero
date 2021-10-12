#include "Bullet.h"
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderSphere.h"
#include "EffectHit.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../UI//NumberWidget.h"
#include "../UI/WidgetComponent.h"
#include "DamageFont.h"

CBullet::CBullet()
{
	m_ObjType   = EObject_Type::Bullet;
	m_BulletType = EBullet_Type::Light;
	m_Dir.x     = 1.f;
	m_Dir.y     = 0.f;
	m_Damage    = NORMAL_MONSTER_ATTACK;
	m_Distance  = NORMAL_BULLET_DISTANCE;
	m_MoveSpeed = BULLET_SPEED;
}

CBullet::CBullet(const CBullet& obj) : CGameObject(obj)
{
	m_Damage = obj.m_Damage;
	m_Dir = obj.m_Dir;
	m_Distance = obj.m_Distance;
	// m_TimeScale = obj.m_TimeScale;
	m_TimeScale = 1.f;
}

CBullet::~CBullet()
{
}

void CBullet::SetBulletType(EBullet_Type BType)
{
	m_BulletType = BType;
	switch (m_BulletType)
	{
	case EBullet_Type::Light:
		SetCurrentAnimation(BULLET_LIGHT);
		break;
	case EBullet_Type::Medium:
		SetCurrentAnimation(BULLET_MEDIUM);
		break;
	case EBullet_Type::Heavy:
		SetCurrentAnimation(BULLET_HEAVY);
		break;
	}
}

void CBullet::Start()
{
	CGameObject::Start();
	CCollider* Body = FindCollider("Body");
	Body->SetCollisionBeginFunction<CBullet>(this, &CBullet::CollisionBegin);
}

bool CBullet::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);

	CColliderSphere* Body = AddCollider<CColliderSphere>("Body");
	Body->SetRadius(25.f);
	Body->SetOffset(0.f, 0.f);

	CreateAnimation();
	AddAnimation(BULLET_LIGHT,true,0.1f);
	AddAnimation(BULLET_MEDIUM,true,0.1f);
	AddAnimation(BULLET_HEAVY,true,0.1f);
	return true;
}

void CBullet::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	Vector2 Dir = m_Dir;
	Dir.Normalize();
	Move(Dir);
	m_Distance -= GetMoveSpeedFrame();
	if (m_Distance <= 0.f)
		Destroy();

}

void CBullet::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CBullet::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CBullet::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CBullet* CBullet::Clone()
{
	return new CBullet(*this);
}

void CBullet::CollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	Destroy();

	Dest->GetOwner()->SetDamage(m_Damage);

	CGameObject* DestOwner = Dest->GetOwner();
	Vector2 DestSize = DestOwner->GetSize();
	Vector2 BulletDir = m_Dir;

	if (DestOwner->GetObjType() == EObject_Type::Player ||
		DestOwner->GetObjType() == EObject_Type::Monster)
	{
		CCharacter* DestChar = (CCharacter*)Dest->GetOwner();
		DestChar->SetHitDir(BulletDir);
		DestChar->Hit();
	}

	CEffectHit* Hit = m_Scene->CreateObject<CEffectHit>("HitEffect", EFFECT_HIT_PROTO,
		m_Pos, Vector2(178.f, 164.f));
	m_Scene->GetSceneResource()->SoundPlay("Fire");

	int Armor = 0;
	if (Dest->GetOwner()->GetObjType() == EObject_Type::Monster ||
		Dest->GetOwner()->GetObjType() == EObject_Type::Player)
		Armor = Dest->GetOwner()->GetArmor();

	CDamageFont* DamageFont = m_Scene->CreateObject<CDamageFont>("DamageFont", m_Pos);
	DamageFont->SetDamageNumber((int)(m_Damage - Armor));
}