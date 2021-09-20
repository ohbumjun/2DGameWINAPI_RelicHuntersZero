
#include "Character.h"
#include "../Scene/Camera.h"
#include "../Scene/Scene.h"

CCharacter::CCharacter() : 
	m_CharacterInfo{}
{
	m_ObjType = EObject_Type::Character;
}

CCharacter::CCharacter(const CCharacter &obj) : CGameObject(obj)
{
	m_CharacterInfo = obj.m_CharacterInfo;
}

CCharacter::~CCharacter()// 
{
}


void CCharacter::Start()
{
	CGameObject::Start();
}

bool CCharacter::Init()
{
	if (!CGameObject::Init())
		return false;

	return true;
}

void CCharacter::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CCharacter::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CCharacter::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CCharacter::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CCharacter *CCharacter::Clone()
{
	return nullptr;
}

float CCharacter::SetDamage(float Damage)
{
	Damage = CGameObject::SetDamage(Damage);
	Damage -= m_CharacterInfo.Armor;
	if (Damage < 0) Damage = 0;
	m_CharacterInfo.HP -= (int)Damage;
	return Damage;
}

void CCharacter::Move(const Vector2& Dir)
{
	CGameObject::Move(Dir);
	SetDir(Dir);
}

void CCharacter::Move(const Vector2& Dir, float Speed)
{
	CGameObject::Move(Dir, Speed);
	SetDir(Dir);
}

void CCharacter::MoveWithinWorldResolution()
{
	{
		Vector2 WorldResolution = m_Scene->GetCamera()->GetWorldResolution();
		if (m_Pos.x <= 0)
			m_Pos.x = 0.f;
		if (m_Pos.x + m_Size.x >= WorldResolution.x)
			m_Pos.x = WorldResolution.x - m_Size.x;
		if (m_Pos.y <= 0)
			m_Pos.y = 0.f;
		if (m_Pos.y + m_Size.y >= WorldResolution.y)
			m_Pos.y = WorldResolution.y - m_Size.y;
	}
}

void CCharacter::Stun()
{
	CGameObject::Stun();
}

void CCharacter::StunEnd()
{
	CGameObject::StunEnd();
}
