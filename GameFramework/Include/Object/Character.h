#pragma once
#include "GameObject.h"
#include "Gun.h"
class CCharacter : public CGameObject
{
	friend class CScene;

protected:
	CCharacter();
	CCharacter(const CCharacter &obj);
	virtual ~CCharacter();

protected:
	CharacterInfo m_CharacterInfo;
	class CSharedPtr<CGun> m_GunEquipment[EGunClass::End];
	CSharedPtr<CGun> m_CurrentGun;
public :
	void SetCharacterInfo(int Attack, int Armor, int HP, float MP, int Level,
						  int Exp, int Gold, float AttackSpeed,
						  float AttackDistance, float DashDistance = 0.f)
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
	// Get
	void SetAttackSpeed(float Speed)
	{
		m_CharacterInfo.AttackSpeed = Speed;
	}
	void SetHP(int newHP)
	{
		if (newHP > m_CharacterInfo.HPMax) newHP = m_CharacterInfo.HPMax;
		m_CharacterInfo.HP = newHP;
	}
	void SetMP(float newMP)
	{
		m_CharacterInfo.MP = newMP;
	}
	// Get 
	int GetHP() { return m_CharacterInfo.HP; }
	float GetMP() { return m_CharacterInfo.MP; }
	int GetArmor()const 
	{
		return m_CharacterInfo.Armor;
	}
	int GetAttack()const
	{
		return m_CharacterInfo.Attack;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PrevRender();
	virtual void Render(HDC hDC);
	virtual CCharacter *Clone();
	virtual float SetDamage(float Damage);
	
protected :
	void SetScene(class CScene* Scene);
// Animation
protected :
	virtual void ChangeHitAnimation();
	virtual void ChangeIdleAnimation();
	virtual void ChangeRunAnimation();
	virtual void ChangeMoveAnimation();
	virtual void ChangeDeathAnimation();

protected :
// Move != 가상함수
// 왜 가상함수 x ? --> Character 외, 다른 GameObject 까지
// 가상함수 테이블에, Move함수를 들고 있게 하고 싶지 않았다 
// 그런데.. 그냥 걸자... ㅋㅋ
	virtual void Move(const Vector2& Dir);
	virtual void Move(const Vector2& Dir, float Speed);
	void MoveWithinWorldResolution();
// Collision
protected :
	bool CollisionCheck();
	bool ObstacleCollisionCheck() const;
	CGameObject* MonsterCollisionCheck();
	virtual void CollideBounceBack(Vector2 Dir);
// Hit
protected:
	bool m_HitEnable;
	float m_HitTime;
	Vector2 m_HitDir;
public:
	void SetHitDir(Vector2 Dir);
	virtual void Hit();
	virtual void HitMove();
	virtual void HitEnd();
// Death
public:
	void CharacterDestroy();
// Gun
protected :
	void SetInitGun(CGun* Gun);
	void SetGunPos(Vector2 Pos)
	{
		m_CurrentGun->SetPos(Pos);
	}
	void SetTexture(CTexture* Texture)
	{
		m_Texture = Texture;

		if (m_Texture)
		{
			m_Size.x = (float)m_Texture->GetWidth();
			m_Size.y = (float)m_Texture->GetHeight();
		}
	}
// Equip
public:
	virtual CGun* Equip(CGun* Gun);
};
