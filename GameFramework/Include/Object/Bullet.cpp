
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
	m_ObjType = EObject_Type::Bullet;
	m_Dir.x = 1.f;
	m_Dir.y = 0.f;
	m_Damage = NORMAL_MONSTER_ATTACK;
	m_Distance = NORMAL_BULLET_DISTANCE;
	SetMoveSpeed(BULLET_SPEED);
}

CBullet::CBullet(const CBullet& obj)	:
	CGameObject(obj)
{
	m_Damage = obj.m_Damage;
	m_Dir = obj.m_Dir;
	m_Distance = obj.m_Distance;
}

CBullet::~CBullet()
{
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

	CreateAnimation();
	AddAnimation("Bullet", true, 1.f);

	CColliderSphere* Body = AddCollider<CColliderSphere>("Body");
	Body->SetRadius(25.f);
	Body->SetOffset(0.f, 0.f);

	return true;
}

void CBullet::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	Vector2	Dir = m_Dir;
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

	// Damage 주기 
	Dest->GetOwner()->SetDamage(m_Damage);

	// 튕겨나가게 하기 ( 자신의 size 만큼 )
	Vector2 DestSize = Dest->GetOwner()->GetSize();
	Vector2 BulletDir = m_Dir;

	// 총알 반대 방향으로 이동시키기 ( Stun 걸기 )
	Dest->GetOwner()->SetStunDir(BulletDir);
	Dest->GetOwner()->Stun();
	
	CEffectHit* Hit = m_Scene->CreateObject<CEffectHit>("HitEffect", "HitEffect",
		m_Pos, Vector2(178.f, 164.f));
	m_Scene->GetSceneResource()->SoundPlay("Fire");

	// Dest target의 Armor 가져오기
	int Armor = 0;
	if (Dest->GetOwner()->GetObjType() == EObject_Type::Monster ||
		Dest->GetOwner()->GetObjType() == EObject_Type::Player)
		Armor = Dest->GetOwner()->GetArmor();

	CDamageFont* DamageFont = m_Scene->CreateObject<CDamageFont>("DamageFont",m_Pos);
	DamageFont->SetDamageNumber((int)(m_Damage - Armor));
	
}
