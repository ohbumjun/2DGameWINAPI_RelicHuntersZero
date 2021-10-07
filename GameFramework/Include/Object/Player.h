#pragma once
#include "Character.h"
#include "Bullet.h"
#include "LaserObject.h"
#include "TeleportMouse.h"

class CPlayer : public CCharacter
{
	friend class CScene;
	friend class CSceneManager;

protected:
	CPlayer();
	CPlayer(const CPlayer &obj);
	virtual ~CPlayer();

protected:
	// Widget ---
	CSharedPtr<CWidgetComponent> m_HPBarWidget;
	CSharedPtr<CWidgetComponent> m_MPBarWidget;
	CSharedPtr<CWidgetComponent> m_NameWidget;

	// Laser Obj
	CSharedPtr<CLaserObject> m_LaserBulletObj;

public:
	virtual void Start();
	virtual void SetNotifyFunctions();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CPlayer *Clone();
	virtual float SetDamage(float Damage);
// Gun
private :
	void ChangeGun();

// Idle ---
private:
	void ChangeIdleAnimation();
	void ChangeDashAnimation();
// Move ---
private:
	void MoveUp(float DeltaTime);
	void MoveDown(float DeltaTime);
	void MoveLeft(float DeltaTime);
	void MoveRight(float DeltaTime);
	virtual void Move(const Vector2 &Dir);
	virtual void Move(const Vector2 &Dir, float Speed);
	void ChangeMoveAnimation(); 

// Run ---
private:
	void RunLeft(float DeltaTime);
	void RunRight(float DeltaTime);
	void RunUp(float DeltaTime);
	void RunDown(float DeltaTime);
	bool m_RunEnable;
	void RunStart();
	void RunEnd();
	void ChangeRunAnimation();

// Dash ---
private:
	void DashStart();
	void DashLeft(float DeltaTime);
	void DashRight(float DeltaTime);
	void DashUp(float DeltaTime);
	void DashDown(float DeltaTime);
	void DashEnd();
	bool m_DashEnable;
	float m_DashTime;

// Other Skills ---
private:
	// Pause, Resume
	void Pause(float DeltaTime);
	void Resume(float DeltaTime);

	// Slow Motion Attack
	void SkillSlowMotionAttack(float DeltaTime);
	void SkillSlowMotionAttackEnd();
	void SkillSlowMotionAttackEnable();
	bool m_SkillSlowMotionAttackEnable;
	float m_SkillSlowMotionAttackTime;

	// Destory All
	void SkillDestroyAllAttack(float DeltaTime);
	void SkillDestroyAllAttackEnd();
	void SkillDestoryAllAttackEnable();
	bool m_SkillDestoryAllAttackEnable;
	float m_SkillDestoryAllAttackTime;

	// Attack Speed
	virtual void SetAttackSpeed(float Speed)
	{
		CCharacter::SetAttackSpeed(Speed);
		SetAnimationPlayScale(PLAYER_RIGHT_ATTACK, Speed);
	}

// Collision ---
private:
	void CollisionBegin(class CCollider *Src, class CCollider *Dest, float DeltaTime);
	Vector2 GetColliderPos();
	virtual void CollideBounceBack(Vector2 Dir);
// Stun ---
private:
	virtual void Stun();
	virtual void StunEnd();
	void ChangeStunAnimation();

// Teleport ---
private:
	bool m_TeleportEnable;
	Vector2 m_TeleportPos;
	float m_TelePortTime; // �ڷ���Ʈ ���콺 Ŀ���� ���� obj�� �����ֱ� ����
	CTeleportMouse *m_TeleportObj;

private:
	void Teleport(float DeltaTime);
	void SetTeleportPos(float DeltaTime);
	void DeleteTeleportObj();

// Attack ---
private:
	Vector2 m_TargetPos;

public:
	bool m_TargetEnable;
	void AttackEnd();
	void Fire();
	void SetTargetPos(float DeltaTime);
	void RemoveTargetPos(float DeltaTime);
	void FireTarget();
	void BulletFireTarget(float);
	CGameObject *FindClosestTarget(Vector2 PlayerPos);

// Death ---
public:
	virtual void CharacterDestroy();
	float m_DeathAnimationTime;
	virtual void GoOppDirection(Vector2 Dir)
	{
		Vector2 Dist = Vector2(Dir.x * 20, Dir.y * 20);
		m_Pos += Dist;
	};

// Item --
public:
	void AcquireItem(float);
// Weapon
public :
	void ChangeWeapon(float);
};