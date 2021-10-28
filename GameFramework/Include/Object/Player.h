#pragma once
#include "Character.h"
#include "Bullet.h"
#include "LaserObject.h"
#include "TeleportMouse.h"
#include "../UI/UIPause.h"

class CPlayer : public CCharacter
{
	friend class CScene;
	friend class CSceneManager;
	friend class CUIPause;
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
private :
	void UseHpPotionInv(float DeltaTime);
	void UseMpPotionInv(float DeltaTime);
	void UseShieldInv(float DeltaTime);
private :
	void ShieldUpdate(float DeltaTime);
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
	virtual float SetDamage(int Damage);
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
	void  GunTimeUpdate(float DeltaTime);
	void  ChangeGunToLight(float DeltaTime);
	void  ChangeGunToMedium(float DeltaTime);
	void  ChangeGunToHeavy(float DeltaTime);
	void  ReloadGun(float DeltaTime);
	void  GunStateUIUpdate(class CUIGunStateHUD* State);
	void  GunCurBulletNumUpdate();
public :
	CGun* GetCurrentGun() const{return m_CurrentGun;}
// Move ---
private:
	void MoveUp(float DeltaTime);
	void MoveDown(float DeltaTime);
	void MoveLeft(float DeltaTime);
	void MoveRight(float DeltaTime);
	virtual void Move(const Vector2 &Dir);
	virtual void Move(const Vector2 &Dir, float Speed);
	virtual void HitMove();
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
	class CUIPause* m_UIPause;

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
	void AutoAcquireCoin(float);
	void BuyItem(float);
	void AddCoin(int Coin)
	{
		m_CharacterInfo.Gold += Coin;
	}
private :
	void ShowNoGoldSign();
// Gun
public :
	virtual CGun* Equip(CGun* Gun);
// Direction
private :
	void ChangeDirToMouse();
// SetName
private :
	void SetCharName();
// SetAnimation
public :
	void SetAnimName();
private :
	void SetAssAnimName();
	void SetJimmyAnimName();
	void SetBiuAnimName();
	void SetPinkyAnimName();
	void SetPunnyAnimName();
	void SetRaffAnimName();
private :
	void AddAssAnimName();
	void AddJimmyAnimName();
	void AddBiuAnimName();
	void AddPinkyAnimName();
	void AddPunnyAnimName();
	void AddRaffAnimName();
// Skills
private :
	float m_SkillTime;
	float m_SkillTimeMax;
	bool  m_SkillEnable;
	void SkillTimeUpdate(float DeltaTime);
private :
	void ActivateSkills(float DeltaTime);
	void DeActivateSkills(float DeltaTime);
	// Ass : Make Clones 
	void SkillMakeGrenades(float DeltaTime);
	// Biu : Drop Rocks
	void SkillClone(float DeltaTime);
	// Pinky : Slow Motion 
	void SkillSlowMotion(float DeltaTime);
	// Jimmy :  Slow Motion Attack
	void SkillMultipleBulletAttack(float DeltaTime);
	// Punny : Destory All
	void SkillDestroyAllAttack(float DeltaTime);
	// Raff : Attack Speed Up
	void SkillIncAbility();
// Static
private :
	static CharacterInfo m_SelectedCharacterInfo;
	static EChar_Type m_CharType ;
public :
	static EChar_Type GetStaticCharType() { return m_CharType; }
	static CharacterInfo GetCharacterInfo() { return m_SelectedCharacterInfo; }
	static void SetStaticPlayerCharType(EChar_Type CharType)
	{
		m_CharType = CharType;
	}
	static void SetStaticCharacterInfo(CharacterInfo CharInfo)
	{
		m_SelectedCharacterInfo = CharInfo;
	}
};
