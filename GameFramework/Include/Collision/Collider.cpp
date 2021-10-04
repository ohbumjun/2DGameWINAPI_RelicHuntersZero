
#include "Collider.h"
#include "CollisionManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneCollision.h"

<<<<<<< HEAD
CCollider::CCollider() : m_Scene(nullptr),
						 m_Owner(nullptr),
						 m_Enable(true),
						 m_Profile(nullptr),
						 m_MouseCollision(false),
						 m_Type(ECollider_Type::Box)
{
}

CCollider::CCollider(const CCollider &collider) : CRef(collider),
												  m_Scene(nullptr),
												  m_Owner(nullptr),
												  m_Enable(true)
=======
CCollider::CCollider()	:
	m_Scene(nullptr),
	m_Owner(nullptr),
	m_Enable(true),
	m_Profile(nullptr),
	m_MouseCollision(false),
	m_Type(ECollider_Type::Box)
{
}

CCollider::CCollider(const CCollider& collider)	:
	CRef(collider),
	m_Scene(nullptr),
	m_Owner(nullptr),
	m_Enable(true)
>>>>>>> a1c29f602a9d2b17309d6664e27d89b32dfee792
{
	m_Type = collider.m_Type;
	m_Offset = collider.m_Offset;
	m_Profile = collider.m_Profile;
}

CCollider::~CCollider()
{
	m_Scene->GetSceneCollision()->ClearMouseCollision(this);

<<<<<<< HEAD
	auto iter = m_CollisionList.begin();
	auto iterEnd = m_CollisionList.end();
=======
	auto	iter = m_CollisionList.begin();
	auto	iterEnd = m_CollisionList.end();
>>>>>>> a1c29f602a9d2b17309d6664e27d89b32dfee792

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->DeleteCollisionList(this);
	}
}

<<<<<<< HEAD
CGameObject *CCollider::IsCollisionWithMonster()
=======
CGameObject* CCollider::IsCollisionWithMonster()
>>>>>>> a1c29f602a9d2b17309d6664e27d89b32dfee792
{
	auto iter = m_CollisionList.begin();
	auto iterEnd = m_CollisionList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetOwner()->GetObjType() == EObject_Type::Monster)
		{
<<<<<<< HEAD
			// Damageï¿½ï¿½ ï¿½ï¿½ï¿½Â´ï¿½
=======
			// Damage¸¦ ¾ò¾î¿Â´Ù 
>>>>>>> a1c29f602a9d2b17309d6664e27d89b32dfee792
			return (*iter)->GetOwner();
		}
	}
	return nullptr;
}

<<<<<<< HEAD
CPlayer *CCollider::IsCollisionWithPlayer()
=======
CPlayer* CCollider::IsCollisionWithPlayer()
>>>>>>> a1c29f602a9d2b17309d6664e27d89b32dfee792
{
	auto iter = m_CollisionList.begin();
	auto iterEnd = m_CollisionList.end();
	for (; iter != iterEnd; ++iter)
	{
<<<<<<< HEAD
		if ((*iter)->GetOwner()->GetObjType() == EObject_Type::Player)
			return (CPlayer *)(*iter)->GetOwner();
=======
		if((*iter)->GetOwner()->GetObjType() == EObject_Type::Player)
			return (CPlayer*)(*iter)->GetOwner();
>>>>>>> a1c29f602a9d2b17309d6664e27d89b32dfee792
	}
	return nullptr;
}

<<<<<<< HEAD
CPotion *CCollider::IsCollisionWithPotion()
=======
CPotion* CCollider::IsCollisionWithPotion()
>>>>>>> a1c29f602a9d2b17309d6664e27d89b32dfee792
{
	auto iter = m_CollisionList.begin();
	auto iterEnd = m_CollisionList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetOwner()->GetObjType() == EObject_Type::Potion)
<<<<<<< HEAD
			return (CPotion *)(*iter)->GetOwner();
=======
			return (CPotion*)(*iter)->GetOwner();
>>>>>>> a1c29f602a9d2b17309d6664e27d89b32dfee792
	}
	return nullptr;
}

bool CCollider::DidCollideWithObstacles() const
{
<<<<<<< HEAD
	auto iter = m_CollisionList.begin();
=======
	auto iter   = m_CollisionList.begin();
>>>>>>> a1c29f602a9d2b17309d6664e27d89b32dfee792
	auto iterEnd = m_CollisionList.end();
	for (; iter != iterEnd; ++iter)
	{
		EObject_Type OwnerObjType = (*iter)->GetOwner()->GetObjType();
<<<<<<< HEAD
		if (OwnerObjType == EObject_Type::Monster || OwnerObjType == EObject_Type::Obstacle)
=======
		if (OwnerObjType == EObject_Type::Monster ||OwnerObjType == EObject_Type::Obstacle) 
>>>>>>> a1c29f602a9d2b17309d6664e27d89b32dfee792
			return true;
	}
	return false;
}

<<<<<<< HEAD
void CCollider::SetCollisionProfile(const std::string &Name)
=======
void CCollider::SetCollisionProfile(const std::string& Name)
>>>>>>> a1c29f602a9d2b17309d6664e27d89b32dfee792
{
	m_Profile = CCollisionManager::GetInst()->FindProfile(Name);
}

<<<<<<< HEAD
void CCollider::AddCollisionList(CCollider *Collider)
=======
void CCollider::AddCollisionList(CCollider* Collider)
>>>>>>> a1c29f602a9d2b17309d6664e27d89b32dfee792
{
	m_CollisionList.push_back(Collider);
}

<<<<<<< HEAD
bool CCollider::CheckCollisionList(CCollider *Collider)
{
	auto iter = m_CollisionList.begin();
	auto iterEnd = m_CollisionList.end();
=======
bool CCollider::CheckCollisionList(CCollider* Collider)
{
	auto	iter = m_CollisionList.begin();
	auto	iterEnd = m_CollisionList.end();
>>>>>>> a1c29f602a9d2b17309d6664e27d89b32dfee792

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == Collider)
			return true;
	}

	return false;
}

<<<<<<< HEAD
void CCollider::DeleteCollisionList(CCollider *Collider)
{
	auto iter = m_CollisionList.begin();
	auto iterEnd = m_CollisionList.end();
=======
void CCollider::DeleteCollisionList(CCollider* Collider)
{
	auto	iter = m_CollisionList.begin();
	auto	iterEnd = m_CollisionList.end();
>>>>>>> a1c29f602a9d2b17309d6664e27d89b32dfee792

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == Collider)
		{
			m_CollisionList.erase(iter);
			return;
		}
	}
}

void CCollider::ClearCollisionList()
{
<<<<<<< HEAD
	auto iter = m_CollisionList.begin();
	auto iterEnd = m_CollisionList.end();
=======
	auto	iter = m_CollisionList.begin();
	auto	iterEnd = m_CollisionList.end();
>>>>>>> a1c29f602a9d2b17309d6664e27d89b32dfee792

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->DeleteCollisionList(this);
	}
<<<<<<< HEAD

	m_CollisionList.clear();
}

void CCollider::CallCollisionBegin(CCollider *Dest, float DeltaTime)
=======
	
	m_CollisionList.clear();
}

void CCollider::CallCollisionBegin(CCollider* Dest, float DeltaTime)
>>>>>>> a1c29f602a9d2b17309d6664e27d89b32dfee792
{
	if (m_BeginFunction)
		m_BeginFunction(this, Dest, DeltaTime);
}

<<<<<<< HEAD
void CCollider::CallCollisionEnd(CCollider *Dest, float DeltaTime)
=======
void CCollider::CallCollisionEnd(CCollider* Dest, float DeltaTime)
>>>>>>> a1c29f602a9d2b17309d6664e27d89b32dfee792
{
	if (m_EndFunction)
		m_EndFunction(this, Dest, DeltaTime);
}

<<<<<<< HEAD
void CCollider::CallMouseCollisionBegin(const Vector2 &MousePos, float DeltaTime)
=======
void CCollider::CallMouseCollisionBegin(const Vector2& MousePos, float DeltaTime)
>>>>>>> a1c29f602a9d2b17309d6664e27d89b32dfee792
{
	if (m_MouseBeginFunction)
		m_MouseBeginFunction(this, MousePos, DeltaTime);
}

<<<<<<< HEAD
void CCollider::CallMouseCollisionEnd(const Vector2 &MousePos, float DeltaTime)
=======
void CCollider::CallMouseCollisionEnd(const Vector2& MousePos, float DeltaTime)
>>>>>>> a1c29f602a9d2b17309d6664e27d89b32dfee792
{
	if (m_MouseEndFunction)
		m_MouseEndFunction(this, MousePos, DeltaTime);
}

bool CCollider::Init()
{
	m_Profile = CCollisionManager::GetInst()->FindProfile("Default");

	return true;
}

void CCollider::Update(float DeltaTime)
{
}

void CCollider::PostUpdate(float DeltaTime)
{
}

void CCollider::Render(HDC hDC)
{
}

<<<<<<< HEAD
CCollider *CCollider::Clone()
=======
CCollider* CCollider::Clone()
>>>>>>> a1c29f602a9d2b17309d6664e27d89b32dfee792
{
	return nullptr;
}
