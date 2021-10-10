#pragma once
#include "Character.h"
class CMonster :
    public CCharacter
{
	friend class CScene;

protected:
	CMonster();
	CMonster(const CMonster& obj);
	virtual ~CMonster();

private:
	float	m_FireTime;
	float	m_FireTimeMax;
	float   m_RandomMoveTime;
	Vector2 m_MoveTargetPos;
	int		m_Count;
	EMonster_Type m_MonsterType;
	EMonsterAI m_AI;
	float m_DashDistance;
	float m_AttackDistance;
	bool m_TraceSurprise;
	float m_SurpriseStopTime;
private :
	CSharedPtr<CWidgetComponent> m_HPBarWidget;
	CSharedPtr<CWidgetComponent> m_MPBarWidget;
private :
	std::unordered_map<std::string, std::string> m_mapAnimName;
public :
	void SetMonsterType(EMonster_Type MonsterType)
	{
		m_MonsterType = MonsterType;
	}
	EMonster_Type GetMonsterType() const
	{
		return m_MonsterType;
	}
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CMonster* Clone();
	virtual float SetDamage(float Damage);

// Attack
public :
	void AttackEnd();
	void Fire();

// Move
public :
	virtual void Move(const Vector2& Dir);
	virtual void Move(const Vector2& Dir, float Speed);
// Animation
public :
	void ChangeIdleAnimation();
	void ChangeWalkAnimation();
	void ChangeTraceAnimation();
	void ChangeDeathAnimation();

// Random Dir Setting
private :
	void SetRandomTargetDir();
	Vector2 SetRandomTargetPos();
// Set Animation
private :
	void SetAnimation();
	void SetAnimNames();
	void SetDuck1Animation();
	void SetDuck1AnimName();
// AI
private :
	void AIIdle(float DeltaTime);
	void AIWalk(float DeltaTime);
	void AITrace(float DeltaTime, Vector2 PlayerPos);
	void AIAttack(float DeltaTime, Vector2 PlayerPos);
	void AIDeath(float DeltaTime);
	void AIHit(float DeltaTime);
// Gun
public :
	virtual CGun* Equip(CGun* Gun);
// Stun
private:
	void ChangeStunAnimation();
};

