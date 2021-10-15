#include "WallObject.h"
#include "../Scene/SceneManager.h"
#include "../Scene/HomeScene.h"
#include "../Scene/MainScene.h"
#include "../Scene/Main2Scene.h"
#include "../Scene/StartScene.h"
#include "../Collision/ColliderBox.h"
#include "Player.h"

CWallObject::CWallObject()
{
	m_ObjType = EObject_Type::WallObject;
}

CWallObject::CWallObject(const CWallObject& obj) :
	CGameObject(obj)
{
	m_DoorStageType = obj.m_DoorStageType;
}

CWallObject::~CWallObject()
{
}

void CWallObject::Start()
{
	CGameObject::Start();
}

bool CWallObject::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);
	// CreateAnimation();
	// AddAnimation("WallObject", true, 2.f);

	// Add Collider
	CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(100.f, 80.f);
	Body->SetOffset(0.f, 25.f);

	return true;
}

void CWallObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CWallObject::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CWallObject::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CWallObject::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CWallObject* CWallObject::Clone()
{
	return new CWallObject(*this);
}
