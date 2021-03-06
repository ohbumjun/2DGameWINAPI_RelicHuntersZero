#pragma once
#include "Collider.h"
class CColliderBox : public CCollider
{
	friend class CGameObject;
	friend class CScene;

protected:
	CColliderBox();
	CColliderBox(const CColliderBox &collider);
	virtual ~CColliderBox();

protected:
	float m_Width;
	float m_Height;
	RectInfo m_Info;
	// bool m_IsWallCollider;

public:
	/*
	void SetWallCollider(bool State)
	{
		m_IsWallCollider = State;
	}
	*/
	void SetExtent(float Width, float Height)
	{
		m_Width = Width;
		m_Height = Height;
	}
	RectInfo GetInfo() const
	{
		return m_Info;
	}
	virtual float GetBottom() const
	{
		return m_Info.Bottom;
	}
public :
	/*
	bool IsWallCollider() const
	{
		return m_IsWallCollider;
	}
	*/

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CColliderBox *Clone();
	virtual bool Collision(CCollider *Dest);
	virtual bool CollisionMouse(const Vector2 &MousePos);
	virtual bool IsCollisionWithLaser(const Vector2 &LaserPos);
};
