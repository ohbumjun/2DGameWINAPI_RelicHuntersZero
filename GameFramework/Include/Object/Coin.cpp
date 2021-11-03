#include "Coin.h"
#include "../Collision/ColliderSphere.h"
#include "../Scene/Scene.h"

CCoin::CCoin() :
	m_DirX(0),
	m_Gold(50),
	m_MoveToPlayerEnable(false)
{
	m_MoveSpeed = 0.f;
	m_ObjType = EObject_Type::Coin;
}

CCoin::CCoin(const CCoin& obj) :
	CGameObject(obj)
{
	m_DirX = obj.m_DirX;
	m_Gold = obj.m_Gold;
	m_MoveToPlayerEnable = false;
}

CCoin::~CCoin()
{
}

void CCoin::Start()
{
	CGameObject::Start();
	SetJumpVelocity((float)(30.f + (float)(rand() % 20)));
	Jump();

}

bool CCoin::Init()
{
	if (!CGameObject::Init()) return false;

	// Offset
	SetOffset(Vector2(-m_Size.x / 2, 0));

	// Physics Setting
	SetPhysicsSimulate(true);
	Jump();

	// Coin lasts for 5 secs
	SetLifeTime(5.f);

	CreateAnimation();
	AddAnimation("Coin", true, 1.f);

	// Collider
	CColliderSphere* Head = AddCollider<CColliderSphere>("Head");
	Head->SetRadius(40.f);
	Head->SetOffset(23.f, 20.f);
	Head->SetCollisionProfile("Default");

	return true;
}

void CCoin::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	Move(Vector2(m_DirX * 500.f * DeltaTime, 0.f));

	// Stop Falling
	if (m_Pos.y > m_InitPos.y + 10.f)
		m_IsGround = true;

	MoveToPlayer();
}

void CCoin::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CCoin::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CCoin::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CCoin* CCoin::Clone()
{
	return new CCoin(*this);
}

void CCoin::MoveToPlayer()
{
	if (m_MoveToPlayerEnable)
	{
		Vector2 CenterPos = Vector2(m_Pos.x + m_Size.x * 0.5f ,m_Pos.y);
		float Angle = GetAngle(CenterPos, m_TargetPlayerPos);
		SetDir(Angle);
		Move(m_Dir, 500.f);
		
		if (Distance(CenterPos, m_TargetPlayerPos) <= 5.f)
		{
			Destroy();
			CPlayer* Player = (CPlayer*)m_Scene->GetPlayer();
			Player->AddCoin(m_Gold);
		}
	}
}

