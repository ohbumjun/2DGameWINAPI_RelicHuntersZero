#include "Potion.h"
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderSphere.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "Player.h"
#include "DamageFont.h"

CPotion::CPotion()
{
	m_ObjType = EObject_Type::Potion;
	m_PotionAmount = 0.f;
}

CPotion::CPotion(const CPotion& Potion) : CGameObject(Potion)
{
	m_PotionType = Potion.m_PotionType;

	m_ObjType = Potion.m_ObjType;
	m_PotionAmount = Potion.m_PotionAmount;
}

CPotion::~CPotion()
{
}

void CPotion::AddPotionToPlayer()
{
}

void CPotion::Start()
{
	CGameObject::Start();
}

bool CPotion::Init()
{
	if (!CGameObject::Init())
		return false;

	// Add Collider
	CColliderSphere* Body = AddCollider<CColliderSphere>("Body");
	Body->SetRadius(20.f);
	Body->SetOffset(25.f, 25.f);
	Body->SetCollisionProfile("Default");

	// Animation
	CreateAnimation();

	return true;
}

void CPotion::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// Potion Time Update
	if (m_PotionPauseTime >= 0.f)
		m_PotionPauseTime -= DeltaTime;
}

void CPotion::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CPotion::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);

	// 충돌 목록 중에서 Player가 있는지 확인한다
	
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		CPlayer* Player = (*iter)->IsCollisionWithPlayer();
		// 만약 Player와 충돌했다면
		if (Player)
		{
			// 일정 시간동안, Player HP를 올려준다 
			if (!m_PotionPauseTime <= 0.f)
			{
				EPotion_Type PotionType = GetPotionType();
				if (m_PotionType == EPotion_Type::HP)
				{
					Player->SetHP(Player->GetHP() + (int)m_PotionAmount);
				}
				else if(m_PotionType == EPotion_Type::HP)
				{
					Player->SetMP(Player->GetMP() + m_PotionAmount);
				}
				m_PotionPauseTime = 1.f;
			}
		}
	}
};

void CPotion::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CPotion* CPotion::Clone()
{
	return new CPotion(*this);
}
