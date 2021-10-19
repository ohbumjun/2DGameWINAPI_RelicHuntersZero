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

// Widget ---
protected:
	CSharedPtr<CWidgetComponent> m_HPBarWidget;
	CSharedPtr<CWidgetComponent> m_MPBarWidget;
	CSharedPtr<CWidgetComponent> m_SteminaBarWidget;
	CSharedPtr<CWidgetComponent> m_NameWidget;

// Laser Obj
	CSharedPtr<CLaserObject> m_LaserBulletObj;
// Inv 
private :
	int m_HpPotionInv;
	int m_MpPotionInv;
	int m_ShieldInv;
private :
	int GetHpPotionInv() const
	{
		return m_HpPotionInv;
	}
	int GetMpPotionInv() const
	{
		return m_MpPotionInv;
	}
	int GetShieldInv() const
	{
		return m_ShieldInv;
	}
private :
	void SetHpPotionInv(int Hp)
	{
		m_HpPotionInv = Hp;
	}
	void SetMpPotionInv(int Mp)
	{
		m_MpPotionInv = Mp;
	}
	void SetShieldPotionInv(int Shield)
	{
		m_ShieldInv = Shield;
	}
private :
	void UpdateHpPotionInv(class CUICharacterStateHUD* const State);
	void UpdateMpPotionInv(class CUICharacterStateHUD* const State);
	void UpdateShieldInv(class CUICharacterStateHUD* const State);
// Functions
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

// Hp,Mp,Stemina (Ability)
public :
	void AbilityUpdate(float DeltaTime);
	void AbilityStateUIUpdate(class CUICharacterStateHUD* State);
// Gold
private :
	void  CurGoldNumUpdate(class CUICharacterStateHUD* State);
// Animation 
private :
	virtual void ChangeMoveAnimation();
	virtual void ChangeRunAnimation();
	virtual void ChangeHitAnimation();
	virtual void ChangeIdleAnimation();
	virtual void ChangeDeathAnimation();
	void ChangeDashAnimation();


// Gun
private :
	void  ChangeGunToLight(float DeltaTime);
	void  ChangeGunToMedium(float DeltaTime);
	void  ChangeGunToHeavy(float DeltaTime);
	void  ReloadGun(float DeltaTime);
	void  GunStateUIUpdate(class CUIGunStateHUD* State);
	void  GunCurBulletNumUpdate();
public :
	CGun* GetCurrentGun() const
	{
		return m_CurrentGun;
	}

// Move ---
private:
	void MoveUp(float DeltaTime);
	void MoveDown(float DeltaTime);
	void MoveLeft(float DeltaTime);
	void MoveRight(float DeltaTime);
	virtual void Move(const Vector2 &Dir);
	virtual void Move(const Vector2 &Dir, float Speed);
	void JumpKey(float DeltaTime);

// Run ---
private:
	void RunLeft(float DeltaTime);
	void RunRight(float DeltaTime);
	void RunUp(float DeltaTime);
	void RunDown(float DeltaTime);
	void RunUpdate(float DeltaTime);
	bool m_RunEnable;
	void RunStart();
	void RunEnd();

// Dash ---
private:
	void DashStart();
	void DashLeft(float DeltaTime);
	void DashRight(float DeltaTime);
	void DashUp(float DeltaTime);
	void DashDown(float DeltaTime);
	void DashEnd();
	void DashUpdate(float DeltaTime);
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
	void SkillSlowMotionUpdate(float DeltaTime);
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
	void CollideMonsterBody(CGameObject* Monster);
	float m_MonsterCollideTime;

// Teleport ---
private:
	bool m_TeleportEnable;
	Vector2 m_TeleportPos;
	float m_TelePortTime;
	CTeleportMouse *m_TeleportObj;
private:
	void Teleport(float DeltaTime);
	void SetTeleportPos(float DeltaTime);
	void DeleteTeleportObj();
	void TeleportUpdate(float DeltaTime);
// Attack ---
private:
	Vector2 m_TargetPos;
public:
	bool m_TargetEnable;
	void AttackEnd();
	void SetTargetPos(float DeltaTime);
	void RemoveTargetPos(float DeltaTime);
	void FireTarget();
	void BulletFireTarget(float);
	CGameObject *FindClosestTarget(Vector2 PlayerPos);

// Death ---
public:
	bool m_PlayerDeath;
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
	void BuyItem(float);
// Gun
public :
	virtual CGun* Equip(CGun* Gun);
// Direction
private :
	void ChangeDirToMouse();
};