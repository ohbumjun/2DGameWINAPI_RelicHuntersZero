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
	// ���� ����� �ӵ�
	float m_DirX;
	Vector2 m_InitPos;
public:
	void SetDirX(float Dir)
	{
		m_DirX = Dir;
	}
	void SetInitPos(Vector2 InitPos)
	{
		m_InitPos = InitPos;
	}
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CCoin* Clone();
};

