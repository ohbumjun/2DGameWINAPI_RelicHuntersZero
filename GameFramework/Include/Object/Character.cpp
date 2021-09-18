
#include "Character.h"

CCharacter::CCharacter() : 
	m_CharacterInfo{},
	m_Stun(false),
	m_StunTime(0.f)
{
	m_ObjType = EObject_Type::Character;
	m_MoveSpeed = m_SpeedInfo.Normal;
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
	if (m_Stun)
	{
		m_Stun -= DeltaTime;
		if (m_Stun <= 0.f)
			m_Stun = false;
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
	// ���� �ɸ��� �������� ���ϰ� �Ѵ� 
	if(m_Stun) return;
	SetDir(Dir);
	CGameObject::Move(Dir);
}

void CCharacter::Move(const Vector2& Dir, float Speed)
{
	// ���� �ɸ��� �������� ���ϰ� �Ѵ� 
	if(m_Stun) return;
	SetDir(Dir);
	CGameObject::Move(Dir, Speed);
}
