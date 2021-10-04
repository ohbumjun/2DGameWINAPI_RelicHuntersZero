
#include "LaserObject.h"
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderSphere.h"
#include "EffectHit.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../UI//NumberWidget.h"
#include "../UI/WidgetComponent.h"
#include "DamageFont.h"

CLaserObject::CLaserObject()
{
	m_ObjType = EObject_Type::Laser;
	m_Dir.x = 1.f;
	m_Dir.y = 0.f;
	m_Damage = NORMAL_MONSTER_ATTACK;
	m_Distance = NORMAL_BULLET_DISTANCE;
	SetMoveSpeed(NORMAL_MONSTER_ATTACK_SPEED);
}

CLaserObject::CLaserObject(const CLaserObject &obj) : CGameObject(obj)
{
	m_Damage = obj.m_Damage;
	m_Dir = obj.m_Dir;
	m_Distance = obj.m_Distance;
}

CLaserObject::~CLaserObject()
{
}

void CLaserObject::Start()
{
	CGameObject::Start();
	CCollider *Body = FindCollider("Body");
	Body->SetCollisionBeginFunction<CLaserObject>(this, &CLaserObject::CollisionBegin);
}

bool CLaserObject::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);

	CreateAnimation();
	AddAnimation("Bullet", true, 1.f);

	CColliderSphere *Body = AddCollider<CColliderSphere>("Body");
	Body->SetRadius(25.f);
	Body->SetOffset(0.f, 0.f);

	return true;
}

void CLaserObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	Vector2 Dir = m_Dir;
	Dir.Normalize();
	Move(Dir);
	m_Distance -= GetMoveSpeedFrame();
	if (m_Distance <= 0.f)
		Destroy();
}

void CLaserObject::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CLaserObject::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CLaserObject::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CLaserObject *CLaserObject::Clone()
{
	return new CLaserObject(*this);
}

void CLaserObject::CollisionBegin(CCollider *Src, CCollider *Dest, float DeltaTime)
{
	// Damage �ֱ�
	Dest->GetOwner()->SetDamage(m_Damage);

	CGameObject* DestOwner = Dest->GetOwner();
	Vector2 DestSize = DestOwner->GetSize();
	Vector2 LaserObjectDir = m_Dir;

	if (DestOwner->GetObjType() == EObject_Type::Player ||
		DestOwner->GetObjType() == EObject_Type::Monster)
	{
		DestOwner->SetStunDir(LaserObjectDir);
		DestOwner->Stun();
	}

}
