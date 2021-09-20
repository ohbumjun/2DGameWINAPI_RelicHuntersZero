#pragma once
#include "GameObject.h"
class CCharacter : public CGameObject
{
	friend class CScene;

protected:
	CCharacter();
	CCharacter(const CCharacter &obj);
	virtual ~CCharacter();

protected:
	CharacterInfo m_CharacterInfo;
	// Dir 세팅 
	Vector2   m_Dir;

public:
	void SetDir(Vector2 Dir)
	{
		m_Dir = Dir;
	}
	void SetDir(float x, float y)
	{
		m_Dir = Vector2(x, y);
	}
	void SetDir(float Angle)
	{
		m_Dir.x = cosf(DegreeToRadian(Angle));
		m_Dir.y = sinf(DegreeToRadian(Angle));
	}
	void SetCharacterInfo(int Attack, int Armor, int HP, float MP, int Level,
						  int Exp, int Gold, float AttackSpeed,
						  float AttackDistance)
	{
		m_CharacterInfo.Attack = Attack;
		m_CharacterInfo.Armor = Armor;
		m_CharacterInfo.HP = HP;
		m_CharacterInfo.HPMax = HP;
		m_CharacterInfo.MP = MP;
		m_CharacterInfo.MPMax = MP;
		m_CharacterInfo.Level = Level;
		m_CharacterInfo.Exp = Exp;
		m_CharacterInfo.Gold = Gold;
		m_CharacterInfo.AttackSpeed = AttackSpeed;
		m_CharacterInfo.AttackDistance = AttackDistance;
	}

	virtual void SetAttackSpeed(float Speed)
	{
		m_CharacterInfo.AttackSpeed = Speed;
	}


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CCharacter *Clone();
	virtual float SetDamage(float Damage);


protected :
	// Move != 가상함수
	// 왜 가상함수 x ? --> Character 외, 다른 GameObject 까지
	// 가상함수 테이블에, Move함수를 들고 있게 하고 싶지 않았다 
	// 그런데.. 그냥 걸자... ㅋㅋ
	virtual void Move(const Vector2& Dir);
	virtual void Move(const Vector2& Dir, float Speed);
protected :
	virtual void Stun();
	virtual void StunEnd();
};
