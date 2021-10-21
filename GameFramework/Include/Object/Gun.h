#pragma once
#include "../GameInfo.h"
#include "GameObject.h"
class CGun :
    public CGameObject
{
	friend class CScene;
public :
	CGun();
	CGun(const CGun& obj);
	virtual ~CGun();
protected :
	bool m_FireToggle;
	EGunInfo m_GunInfo;
	CGameObject* m_Owner;
	const char* m_TextureImgNames[2];
protected :
	float m_FireTime;
	float m_FireTimeMax;
public :
	float GetFireTime() const { return m_FireTime; }
	float GetFireTimeMax() const { return m_FireTimeMax; }
public:
	void SetLeftTextureName(const char* Name)
	{
		m_TextureImgNames[ETexture_Dir::Texture_Left] = Name;
	}
	void SetRightTextureName(const char* Name)
	{
		m_TextureImgNames[ETexture_Dir::Texture_Right] = Name;
	}
	void SetGunClass(EGunClass GunClass)
	{
		m_GunInfo.m_GunClass = GunClass;
	}
	void SetOwner(CGameObject* Obj)
	{
		m_Owner = Obj;
	}
	void SetGunDamage(float &Damage)
	{
		m_GunInfo.m_Damage = Damage;
	}
	void SetBulletDistance(float &Dist)
	{
		m_GunInfo.m_BulletDistance = Dist;
	}
	void SetReloadBullet()
	{
		m_GunInfo.m_BulletsLoaded = m_GunInfo.m_BulletsFullNum;
	}
public :
	std::string GetLeftTextureName()
	{
		return m_TextureImgNames[ETexture_Dir::Texture_Left];
	}
	std::string GetRightTextureName()
	{
		return m_TextureImgNames[ETexture_Dir::Texture_Right];
	}
	CGameObject* GetOwner() const
	{
		return m_Owner;
	}
	int GetGunFullBullet() const
	{
		return m_GunInfo.m_BulletsFullNum;
	}
	int GetGunLeftBullet() const
	{
		return m_GunInfo.m_BulletsLoaded;
	}
	EGunClass GetGunClass() const
	{
		return m_GunInfo.m_GunClass;
	}
	float GetGunDamage() const
	{
		return	m_GunInfo.m_Damage;
	}
	bool IsGunBulletEmpty() const
	{
		return	m_GunInfo.m_BulletEmpty;
	}
	
public :
	void PlayerFire(Vector2 TargetPos, float OwnerAttackDamage);
	void MonsterFire(Vector2 TargetPos, float OwnerAttackDamage);
private :
	void CreateBulletEffect();
	void ShowNoBulletSign();
	void AdjustGunTexture();
protected:
	virtual void CreateCasing(class CBullet* Bullet);
private :
	EBullet_Type MatchBulletToGun();
public :
	void RemoveGunCollider();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PrevRender();
	virtual void Render(HDC hDC);
	virtual CGun* Clone();
};

