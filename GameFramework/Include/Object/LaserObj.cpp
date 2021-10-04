#include "LaserObj.h"
#include "../Collision/ColliderSphere.h"


CLaserObj::CLaserObj()
{
	m_ObjType = EObject_Type::Laser;
	m_Dir.x = 1.f;
	m_Dir.y = 0.f;
	m_Distance = LASER_DISTANCE;
	SetMoveSpeed(LASER_SPEED);
}

CLaserObj::CLaserObj(const CLaserObj& obj)
{
	m_Dir = obj.m_Dir;
	m_Distance = obj.m_Distance;
}

CLaserObj::~CLaserObj()
{
}

void CLaserObj::Start()
{
	CGameObject::Start();
	CCollider* Body = FindCollider("Body");
}

bool CLaserObj::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);

	// Animation 은 option
	CreateAnimation();
	AddAnimation("Bullet", true, 1.f);

	CColliderSphere* Body = AddCollider<CColliderSphere>("Body");
	Body->SetRadius(25.f);
	Body->SetOffset(0.f, 0.f);

	return true;
}

void CLaserObj::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	Vector2 Dir = m_Dir;
	Dir.Normalize();
	Move(Dir);
	m_Distance -= GetMoveSpeedFrame();
	if (m_Distance <= 0.f)
		Destroy();
}

void CLaserObj::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CLaserObj::Collision(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CLaserObj::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CLaserObj* CLaserObj::Clone()
{
	return new CLaserObj(*this);
}
