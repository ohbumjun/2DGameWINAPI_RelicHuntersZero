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
private :
	bool m_FireToggle;
	EGunInfo m_GunInfo;
	CGameObject* m_Owner;
	const char* m_TextureImgNames[2];
public :
	/*
	
struct GunInfo
{
	EGun_Type m_GunType;
	float m_Damage;
	int   m_BulletsLoaded;
	bool  m_BulletEmpty;
	float m_BulletLoadTime;
	float m_BulletDistance;
};
	*/
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
	void SetGunType(EGun_Type GunType)
	{
		m_GunInfo.m_GunType = GunType;
	}
	void SetBulletDistance(float &Dist)
	{
		m_GunInfo.m_BulletDistance = Dist;
	}
public :
	CGameObject* GetOwner() const
	{
		return m_Owner;
	}
	EGunClass GetGunClass() const
	{
		return m_GunInfo.m_GunClass;
	}
	float GetGunDamage() const
	{
		return	m_GunInfo.m_Damage;
	}
	EGun_Type GetGunTypee() const
	{
		return	m_GunInfo.m_GunType;
	}
	bool IsGunBulletEmpty() const
	{
		return	m_GunInfo.m_BulletEmpty;
	}
public :
	void PlayerFire(Vector2 TargetPos, float OwnerAttackDamage);
	void MonsterFire(Vector2 TargetPos, float OwnerAttackDamage);
	void CreateCasing(class CBullet* Bullet);
	void CreateBulletEffect();
	void ShowNoBulletSign();
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

