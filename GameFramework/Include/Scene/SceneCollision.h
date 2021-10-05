#pragma once

#include "../GameInfo.h"

class CSceneCollision
{
	friend class CScene;

private:
	CSceneCollision();
	~CSceneCollision();

private:
	class CScene* m_Scene;
	std::vector<class CCollider*>	m_vecCollider;
	std::vector<class CUIWindow*>	m_vecUIWindow;
	class CUIWidget* m_MouseHoveredWidget;
	class CUIWidget* m_SelectWidget;
	class CCollider* m_MouseCollision;

	// Player Laser 충돌 위치 : Vector2
	Vector2 m_LaserCollidePos;

public:
	void SetSelectWidget(class CUIWidget* Widget)
	{
		m_SelectWidget = Widget;
	}

	void ClearMouseCollision(class CCollider* Collider)
	{
		if (m_MouseCollision == Collider)
			m_MouseCollision = nullptr;
	}
	Vector2 GetLaserCollidePos() { return m_LaserCollidePos; }
	void SetLaserCollidePos(const Vector2& Pos)
	{
		m_LaserCollidePos = Pos;
	}

public:
	void AddCollider(class CCollider* Collider);
	void AddUIWindow(class CUIWindow* Window);
	void CollisionMouse(float DeltaTime);
	void Collision(float DeltaTime);
	bool CollisionLaser(Vector2 LaserPos);

private:
	static int SortY(const void* Src, const void* Dest);
};

