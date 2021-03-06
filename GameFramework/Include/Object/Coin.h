#pragma once
#include "GameObject.h"
class CCoin :
    public CGameObject
{
    friend class CScene;
protected:
	CCoin();
	CCoin(const CCoin& obj);
	virtual ~CCoin();
private:
	// 랜덤 방향과 속도
	float m_DirX;
	Vector2 m_InitPos;
	int m_Gold;
	bool m_MoveToPlayerEnable;
	Vector2 m_TargetPlayerPos;
public:
	void SetMoveToPlayer(bool State, Vector2 PlayerPos)
	{
		m_MoveToPlayerEnable = State;
		m_TargetPlayerPos = PlayerPos;
		m_MoveSpeed = 100.f;
	}
	void SetGold(int Gold)
	{
		m_Gold = Gold;
	}
	void SetDirX(float Dir)
	{
		m_DirX = Dir;
	}
	void SetInitPos(Vector2 InitPos)
	{
		m_InitPos = InitPos;
	}
public :
	int GetCoinGold() const
	{
		return m_Gold;
	}
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CCoin* Clone();
public :
	void MoveToPlayer();
};

