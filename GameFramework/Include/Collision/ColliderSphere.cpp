
#include "ColliderSphere.h"
#include "../Object/GameObject.h"
#include "../GameManager.h"
#include "Collision.h"
#include "ColliderBox.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"

CColliderSphere::CColliderSphere()
{
	m_Info.Radius = 50.f;
	m_Type = ECollider_Type::Sphere;
}

CColliderSphere::CColliderSphere(const CColliderSphere& collider) :
	CCollider(collider)
{
	m_Info = collider.m_Info;
}

CColliderSphere::~CColliderSphere()
{
}

bool CColliderSphere::Init()
{
	if (!CCollider::Init())
		return false;

	return true;
}

void CColliderSphere::Update(float DeltaTime)
{
	CCollider::Update(DeltaTime);
}

void CColliderSphere::PostUpdate(float DeltaTime)
{
	CCollider::PostUpdate(DeltaTime);

	m_Info.Center = m_Owner->GetPos() + m_Offset;
}

void CColliderSphere::Render(HDC hDC)
{
	CCollider::Render(hDC);

#ifdef _DEBUG
	HPEN	Pen = CGameManager::GetInst()->GetGreenPen();

	if (!m_CollisionList.empty() || m_MouseCollision)
		Pen = CGameManager::GetInst()->GetRedPen();

	CCamera* Camera = m_Scene->GetCamera();

	SphereInfo	RenderInfo = m_Info;

	RenderInfo.Center -= Camera->GetPos();
	
	HGDIOBJ	Prev = SelectObject(hDC, Pen);

	MoveToEx(hDC, (int)(RenderInfo.Center.x + RenderInfo.Radius), (int)RenderInfo.Center.y, nullptr);

	for (int i = 0; i < 12; ++i)
	{
		float	Radian = DegreeToRadian((i + 1) * (360.f / 12.f));

		float	x = RenderInfo.Center.x + cosf(Radian) * RenderInfo.Radius;
		float	y = RenderInfo.Center.y + sinf(Radian) * RenderInfo.Radius;

		LineTo(hDC, (int)x, (int)y);
	}

	SelectObject(hDC, Prev);
#endif // _DEBUG

}

CColliderSphere* CColliderSphere::Clone()
{
	return new CColliderSphere(*this);
}

bool CColliderSphere::Collision(CCollider* Dest)
{
	switch (Dest->GetColliderType())
	{
	case ECollider_Type::Box:
		return CCollision::CollisionBoxToSphere((CColliderBox*)Dest, this);
		break;
	case ECollider_Type::Sphere:
		return CCollision::CollisionSphereToSphere(this, (CColliderSphere*)Dest);
	case ECollider_Type::Point:
		break;
	}

	return false;
}

bool CColliderSphere::CollisionMouse(const Vector2& MousePos)
{
	float	Dist = Distance(m_Info.Center, MousePos);

	return Dist <= m_Info.Radius;
}
