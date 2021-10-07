
#include "Character.h"
#include "../Scene/Camera.h"
#include "../Scene/Scene.h"
#include "../Object/Gun.h"

CCharacter::CCharacter() : 
	m_CharacterInfo{},
	m_Gun{}
{
	m_ObjType = EObject_Type::Character;

}

CCharacter::CCharacter(const CCharacter &obj) : CGameObject(obj)
{
	m_CharacterInfo = obj.m_CharacterInfo;
	m_Gun = obj.m_Gun->Clone();
}

CCharacter::~CCharacter()
{
	SAFE_DELETE(m_Gun);
}


void CCharacter::Start()
{
	CGameObject::Start();
}

bool CCharacter::Init()
{
	if (!CGameObject::Init())
		return false;

	m_Gun = new CGun;

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
}

void CCharacter::Move(const Vector2& Dir, float Speed)
{
	CGameObject::Move(Dir, Speed);
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

bool CCharacter::CollisionCheck()
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsCollisionListEmpty())
		{
			return true;
		}
	}
	return false;
}

bool CCharacter::ObstacleCollisionCheck() const
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->DidCollideWithObstacles())
			return true;
	}

	return false;
}

CGameObject* CCharacter::MonsterCollisionCheck()
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		CGameObject* Monster = (*iter)->IsCollisionWithMonster();
		if (Monster)
		{
			return Monster;
		}
	}
	return nullptr;
}

void CCharacter::CollideBounceBack(Vector2 Dir)
{
	Vector2 OppDir = Dir;
	OppDir.Normalize();
	SetStunDir(OppDir);
	Stun();
}

void CCharacter::Stun()
{
	CGameObject::Stun();
}

void CCharacter::StunEnd()
{
	CGameObject::StunEnd();
}

void CCharacter::SetGunTexture(const std::string& Name)
{
	m_Gun->SetTexture(Name);
}

void CCharacter::SetGunTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	m_Gun->SetTexture(Name, FileName, PathName);
}

void CCharacter::SetGunTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	m_Gun->SetTexture(Name, FullPath);
}

void CCharacter::SetGunTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	m_Gun->SetTexture(Name, vecFileName, PathName);
}

void CCharacter::SetGunTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
	m_Gun->SetTextureColorKey(r, g, b, Index);
}
