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
	std::unordered_map<std::string, std::string> m_mapAnimName;
protected:
	bool m_FireEnable;
	CharacterInfo m_CharacterInfo;
	class CSharedPtr<CGun> m_GunEquipment[EGunClass::End];
	CSharedPtr<CGun> m_CurrentGun;
public :
	void SetCharacterInfo(CharacterInfo Info)
	{
		m_CharacterInfo = Info;
	}
	void SetCharacterInfo(int Attack, int Armor, int HP, int MP, float Stemina,int Level,
						  int Exp, int Gold, float AttackSpeed,
						  float AttackDistance, float DashDistance = 700.f)
	{
		m_CharacterInfo.Attack = Attack;
		m_CharacterInfo.Armor = Armor;
		m_CharacterInfo.HP = HP;
		m_CharacterInfo.HPMax = HP;
		m_CharacterInfo.MP = MP;
		m_CharacterInfo.MPMax = MP;
		m_CharacterInfo.Stemina = Stemina;
		m_CharacterInfo.SteminaMax = Stemina;
		m_CharacterInfo.Level = Level;
		m_CharacterInfo.Exp = Exp;
		m_CharacterInfo.Gold = Gold;
		m_CharacterInfo.AttackSpeed = AttackSpeed;
		m_CharacterInfo.AttackDistance = AttackDistance;
		m_CharacterInfo.DashDistance = DashDistance;
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
	void SetMP(int newMP)
	{
		m_CharacterInfo.MP = newMP;
	}
	// Get 
	int GetHP()      { return m_CharacterInfo.HP; }
	int GetHPMax() { return m_CharacterInfo.HPMax; }
	int GetMP()    { return m_CharacterInfo.MP; }
	int GetMPMax() { return m_CharacterInfo.MPMax; }
	int GetArmor()const 
	{
		return m_CharacterInfo.Armor;
	}
	int GetAttack()const
	{
		return m_CharacterInfo.Attack;
	}
	CSharedPtr<CGun> GetCurrentGun() const { return m_CurrentGun; }
protected:
	float m_ShieldTime;
	float m_ShieldTimeMax;
	float m_ShieldEnableTime;
	bool  m_ShieldEnable;
	CSharedPtr<class CEffectShield> m_Shield;
public :
	void SetShieldEnable(bool State)
	{
		m_ShieldEnable = State;
	}
	bool GetShieldEnable() const
	{
		return m_ShieldEnable;
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
	virtual int SetDamage(int Damage);
	
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
	virtual void Move(const Vector2& Dir);
	virtual void Move(const Vector2& Dir, float Speed);
	void MoveWithinWorldResolution();

// Collision
protected :
	bool CollisionCheck();
	bool ObstacleCollisionCheck() const;
	CGameObject* MonsterCollisionCheck();
	std::vector<CGameObject*> WallCollisionCheck();
	virtual void CollideBounceBack(Vector2 Dir);
	// class CCollider* GetColliderBox();
	RectInfo GetInterCollideRect(RectInfo Rect1, RectInfo Rect2);
	virtual bool PreventWallMove();
// Hit
protected:
	bool m_HitEnable;
	float m_HitTime;
	Vector2 m_HitDir;
public :
	void SetHitEnable(bool state) { m_HitEnable = state; }
public:
	void SetHitDir(Vector2 Dir);
	virtual void Hit();
	virtual void HitMove();
	virtual void HitEnd();
// Death
public:
	virtual void CharacterDestroy();
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
protected:
	float m_FireTime;
	float m_FireTimeMax;
public:
	virtual CGun* Equip(CGun* Gun);
};
