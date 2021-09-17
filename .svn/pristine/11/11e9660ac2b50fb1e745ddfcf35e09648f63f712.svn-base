
#include "ColliderBox.h"
#include "../Object/GameObject.h"
#include "../GameManager.h"
#include "Collision.h"
#include "ColliderSphere.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"

CColliderBox::CColliderBox()	:
	m_Width(100.f),
	m_Height(100.f)
{
	m_Type = ECollider_Type::Box;
}

CColliderBox::CColliderBox(const CColliderBox& collider)	:
	CCollider(collider)
{
	m_Width = collider.m_Width;
	m_Height = collider.m_Height;
	m_Info = collider.m_Info;
}

CColliderBox::~CColliderBox()
{
}

bool CColliderBox::Init()
{
	if (!CCollider::Init())
		return false;

	return true;
}

void CColliderBox::Update(float DeltaTime)
{
	CCollider::Update(DeltaTime);
}

void CColliderBox::PostUpdate(float DeltaTime)
{
	CCollider::PostUpdate(DeltaTime);

	Vector2	Pos = m_Owner->GetPos();

	m_Info.Left = Pos.x - m_Width / 2.f + m_Offset.x;
	m_Info.Top = Pos.y - m_Height / 2.f + m_Offset.y;
	m_Info.Right = Pos.x + m_Width / 2.f + m_Offset.x;
	m_Info.Bottom = Pos.y + m_Height / 2.f + m_Offset.y;
}

void CColliderBox::Render(HDC hDC)
{
	CCollider::Render(hDC);

#ifdef _DEBUG
	HBRUSH	Brush = CGameManager::GetInst()->GetGreenBrush();

	if(!m_CollisionList.empty() || m_MouseCollision)
		Brush = CGameManager::GetInst()->GetRedBrush();

	CCamera* Camera = m_Scene->GetCamera();

	RectInfo	RenderInfo = m_Info;

	RenderInfo.Left -= Camera->GetPos().x;
	RenderInfo.Right -= Camera->GetPos().x;
	RenderInfo.Top -= Camera->GetPos().y;
	RenderInfo.Bottom -= Camera->GetPos().y;

	RECT	rc = { (long)RenderInfo.Left, (long)RenderInfo.Top,
		(long)RenderInfo.Right, (long)RenderInfo.Bottom };

	FrameRect(hDC, &rc, Brush);
#endif // _DEBUG

}

CColliderBox* CColliderBox::Clone()
{
	return new CColliderBox(*this);
}

bool CColliderBox::Collision(CCollider* Dest)
{
	switch (Dest->GetColliderType())
	{
	case ECollider_Type::Box:
		return CCollision::CollisionBoxToBox(this, (CColliderBox*)Dest);
	case ECollider_Type::Sphere:
		return CCollision::CollisionBoxToSphere(this, (CColliderSphere*)Dest);
	case ECollider_Type::Point:
		break;
	}

	return false;
}

bool CColliderBox::CollisionMouse(const Vector2& MousePos)
{
	if (m_Info.Left > MousePos.x)
		return false;

	else if (m_Info.Right < MousePos.x)
		return false;

	else if (m_Info.Top > MousePos.y)
		return false;

	else if (m_Info.Bottom < MousePos.y)
		return false;

	return true;
}
