#pragma once
#include "Character.h"
class CPlayerClone :
    public CCharacter
{
public:
	CPlayerClone();
	CPlayerClone(const CPlayerClone& obj);
	virtual ~CPlayerClone();
protected:
	CSharedPtr<CWidgetComponent> m_HPBarWidget;
	CSharedPtr<CWidgetComponent> m_MPBarWidget;
	CSharedPtr<CWidgetComponent> m_NameWidget;
private :
	EChar_Type m_CharType;
	EMonsterAI m_AI;
	int m_DashDistance;
	int m_AttackDistance;
private :
	CGameObject* m_ClosestMonster;
public :
	void SetCharType(EChar_Type Type)
	{
		m_CharType = Type;
	}
private :
	void SetNotifyFunctions();
	void SetCharName();
public:
	void SetAnimName();
private:
	void SetAssAnimName();
	void SetJimmyAnimName();
	void SetBiuAnimName();
	void SetPinkyAnimName();
	void SetPunnyAnimName();
	void SetRaffAnimName();
private:
	void AddAssAnimName();
	void AddJimmyAnimName();
	void AddBiuAnimName();
	void AddPinkyAnimName();
	void AddPunnyAnimName();
	void AddRaffAnimName();
public :
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual float SetDamage(float Damage);
	virtual void CharacterDestroy();
public:
	void Fire();
	void AttackEnd();
	// Move
public:
	virtual void Move(const Vector2& Dir);
	virtual void Move(const Vector2& Dir, float Speed);
	// Animation
public:
	virtual void ChangeIdleAnimation();
	virtual void ChangeMoveAnimation();
	virtual void ChangeRunAnimation();
	virtual void ChangeDeathAnimation();
	virtual void ChangeHitAnimation();
// Random Dir Setting
private:
	void SetPlayerTargetDir();
// AI
protected:
	virtual void AIIdle(float DeltaTime);
	virtual void AIWalk(float DeltaTime);
	virtual void AITrace(float DeltaTime, Vector2 PlayerPos);
	virtual void AIAttack(float DeltaTime, Vector2 PlayerPos);
	virtual void AIDeath(float DeltaTime);
	virtual void AIHit(float DeltaTime);
// Gun
public:
	virtual CGun* Equip(CGun* Gun);
};

