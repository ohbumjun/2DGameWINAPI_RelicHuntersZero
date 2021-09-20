#pragma once
#include "Character.h"
#include "Bullet.h"
#include "TeleportMouse.h"

class CPlayer : public CCharacter
{
	friend class CScene;

protected:
	CPlayer();
	CPlayer(const CPlayer &obj);
	virtual ~CPlayer();

protected:
	// Widget
	CSharedPtr<CWidgetComponent> m_HPBarWidget;
	CSharedPtr<CWidgetComponent> m_MPBarWidget;
	CSharedPtr<CWidgetComponent> m_NameWidget;

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
	virtual CPlayer *Clone();
	virtual float SetDamage(float Damage);

// Idle
private :
	void ChangeIdleAnimation();
	
// Move
private:
	void MoveUp(float DeltaTime);
	void MoveDown(float DeltaTime);
	void MoveLeft(float DeltaTime);
	void MoveRight(float DeltaTime);
	void Move(const Vector2& Dir);
	void Move(const Vector2& Dir, float Speed);
	void ChangeMoveAnimation(); // 오른쪽,왼쪽 only
// Run
private:
	void RunLeft(float DeltaTime);
	void RunRight(float DeltaTime);
	void RunUp(float DeltaTime);
	void RunDown(float DeltaTime);
	bool m_RunEnable;
	void RunStart();
	void RunEnd();
	void ChangeRunAnimation(); // 오른쪽 왼쪽 only
// Dash
private:
	void Dash(float DeltaTime);
	void DashEnd();
	bool m_DashEnable;
	float m_DashTime;
// Other Skills
private:
	void Pause(float DeltaTime);
	void Resume(float DeltaTime);
	void SkillSlowMotionAttack(float DeltaTime);
	void SkillSlowMotionAttackEnd();
	void SkillSlowMotionAttackEnable();
	bool m_SkillSlowMotionAttackEnable;
	float m_SkillSlowMotionAttackTime;
	std::list<CSharedPtr<CBullet>> m_SkillSlowMotionBulletList;
	CGameObject* FindClosestTarget(Vector2 PlayerPos);
// Collision
private :
	bool CollisionCheck();
	int MonsterCollisionCheck();
	void CollisionBegin(class CCollider* Src, class CCollider* Dest, float DeltaTime);
	Vector2 GetColliderPos();
	void CollideBounceBack();
// Stun
private :
	virtual void Stun();
	virtual void StunEnd();
// 순간 이동
private :
	bool    m_TeleportEnable;
	Vector2 m_TeleportPos;
	float   m_TelePortTime; // 텔레포트 마우스 커서를 담은 obj를 지워주기 위함
	CTeleportMouse* m_TeleportObj;
private:
	void Teleport(float DeltaTime);
	void SetTeleportPos(float DeltaTime);
	void DeleteTeleportObj();
// 공격
private :
	Vector2 m_TargetPos;
public  :
	void AttackEnd();
	void BulletFire(float DeltaTime);
	void Fire();
	void SetTargetPos(float DeltaTime);
	void FireTarget();
	void BulletFireTarget(float DeltaTime);
// Death
	virtual void CharacterDestroy();
	float m_DeathAnimationTime;
public:
	virtual void GoOppDirection(Vector2 Dir)
	{
		Vector2 Dist = Vector2(Dir.x * 20, Dir.y * 20);
		m_Pos += Dist;
	};
};
