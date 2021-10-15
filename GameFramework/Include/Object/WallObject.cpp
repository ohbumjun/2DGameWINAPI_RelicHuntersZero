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
}

CWallObject::~CWallObject()
{
}

CCollider* CWallObject::GetColliderBox()
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == "Body")
			return (*iter);
	}
	return nullptr;
}

void CWallObject::Start()
{
	CGameObject::Start();
}

bool CWallObject::Init()
{
	if (!CGameObject::Init())
		return false;

	// SetPivot(1.f, 0.0f);
	// CreateAnimation();
	// AddAnimation("WallObject", true, 2.f);

	// Add Collider
	CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(64.f,64.f);
	Body->SetCollisionProfile("Wall");

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
