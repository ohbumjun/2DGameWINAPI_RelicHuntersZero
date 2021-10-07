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
	EGunInfo m_GunInfo;
	CGameObject* m_Owner;
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
	void SetOwner(CGameObject* Obj)
	{
		m_Owner = Obj;
	}
	void SetGunDamage(float &Damage)
	{
		m_GunInfo.m_Damage = Damage;
	}
	void SetGunType(EGun_Type &GunType)
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
	void Shoot();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CGun* Clone();
};

