#pragma once
#include "GameObject.h"
class CBullet :
    public CGameObject
{
	friend class CScene;

protected:
	CBullet();
	CBullet(const CBullet& obj);
	virtual ~CBullet();

protected:
	EBullet_Type m_BulletType;
	Vector2		m_Dir;
	float		m_Distance;
	float		m_Damage;
	CGun*		m_Owner;

public:
	void SetOwner(CGun* Owner)
	{
		m_Owner = Owner;
	}
	void SetBulletType(EBullet_Type BType);
	void SetBulletDamage(float Damage)
	{
		m_Damage = Damage;
	}
	void SetDir(float x, float y)
	{
		m_Dir.x = x;
		m_Dir.y = y;
	}
	void SetDir(Vector2 Dir)
	{
		m_Dir = Dir;
	}
	void SetDir(float Angle)
	{
		m_Dir.x = cosf(DegreeToRadian(Angle));
		m_Dir.y = sinf(DegreeToRadian(Angle));
	}
	void SetDistance(float Distance)
	{
		m_Distance = Distance;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CBullet* Clone();

public:
	RectInfo GetInterCollideRect(RectInfo Rect1, RectInfo Rect2);
	void WallCollision();
	void CollisionBegin(class CCollider* Src, class CCollider* Dest, float DeltaTime);
public :
	void UpdateDistance();
	void UpdateBossBullet();
};

