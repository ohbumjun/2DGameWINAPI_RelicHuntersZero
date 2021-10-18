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

protected:
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
protected :
	CSharedPtr<CWidgetComponent> m_HPBarWidget;
	CSharedPtr<CWidgetComponent> m_MPBarWidget;
protected:
	std::unordered_map<std::string, std::string> m_mapAnimName;
	bool m_TypeChanged;
public :
	void SetMonsterType(EMonster_Type MonsterType)
	{
		m_MonsterType = MonsterType;
		m_TypeChanged = true;
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
	virtual float SetDamage(float Damage);
// Collision
public :
	bool PlayerCollisionCheck();
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
	virtual void ChangeIdleAnimation();
	virtual void ChangeMoveAnimation() ;
	virtual void ChangeRunAnimation() ;
	virtual void ChangeDeathAnimation() ;
	virtual void ChangeHitAnimation() ;
// Random Dir Setting
private :
	void SetRandomTargetDir();
	Vector2 SetRandomTargetPos();
// Set Animation
protected :
	virtual void SetAnimation() {};
	virtual void SetAnimNames() {};
// AI
protected :
	virtual void AIIdle(float DeltaTime);
	virtual void AIWalk(float DeltaTime);
	virtual void AITrace(float DeltaTime, Vector2 PlayerPos);
	virtual void AIAttack(float DeltaTime, Vector2 PlayerPos);
	virtual void AIDeath(float DeltaTime);
	virtual void AIHit(float DeltaTime);
// Gun
public :
	virtual CGun* Equip(CGun* Gun);
};

