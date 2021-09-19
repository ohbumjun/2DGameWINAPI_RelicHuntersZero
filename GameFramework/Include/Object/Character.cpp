
#include "Character.h"

CCharacter::CCharacter() : 
	m_CharacterInfo{}
{
	m_ObjType = EObject_Type::Character;
}

CCharacter::CCharacter(const CCharacter &obj) : CGameObject(obj)
{
	m_CharacterInfo = obj.m_CharacterInfo;
}

CCharacter::~CCharacter()
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
	// stun ���� update
	if (m_StunEnable)
	{
		m_StunTime -= DeltaTime;
		if (m_StunTime <= 0.f)
			m_StunEnable = false;
	}
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
