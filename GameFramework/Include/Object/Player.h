#pragma once
#include "Character.h"
#include "Bullet.h"
class CPlayer :
    public CCharacter
{
	friend class CScene;

protected:
	CPlayer();
	CPlayer(const CPlayer& obj);
	virtual ~CPlayer();

protected:
	bool	m_Skill1Enable;
	float	m_Skill1Time;
	std::list<CSharedPtr<CBullet>>	m_Skill1BulletList;

public:
	virtual void SetAttackSpeed(float Speed)
	{
		CCharacter::SetAttackSpeed(Speed);

		SetAnimationPlayScale("LucidNunNaRightAttack", Speed);
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CPlayer* Clone();
	virtual float SetDamage(float Damage);

private:
	void MoveUp(float DeltaTime);
	void MoveDown(float DeltaTime);
	void MoveLeft(float DeltaTime);
	void MoveRight(float DeltaTime);
	void BulletFire(float DeltaTime);
	void Pause(float DeltaTime);
	void Resume(float DeltaTime);
	void Skill1(float DeltaTime);

// run
	void RunRight(float DeltaTime);
	void RunLeft(float DeltaTime);
	void RunUp(float DeltaTime);
	void RunDown(float DeltaTime);
	void RunStart(bool IsRight);
	void RunEnd();
	bool m_RunEnable;
	float m_RunTime;

public:
	void AttackEnd();
	void Fire();

	void Skill1End();
	void Skill1Enable();
};

