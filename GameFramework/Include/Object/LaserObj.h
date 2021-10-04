#pragma once
#include "GameObject.h"
class CLaserObj :
    public CGameObject
{
	friend class CScene;

protected:
	CLaserObj();
	CLaserObj(const CLaserObj& obj);
	virtual ~CLaserObj();

protected:
	Vector2		m_Dir;
	float		m_Distance;

public:
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
	virtual CLaserObj* Clone();
};

