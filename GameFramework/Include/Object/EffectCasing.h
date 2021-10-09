#pragma once
#include "GameObject.h"
class CEffectCasing :
	public CGameObject
{
	friend class CScene;
protected:
	CEffectCasing();
	CEffectCasing(const CEffectCasing& obj);
	virtual ~CEffectCasing();
private:
	// 랜덤 방향과 속도
	float m_DirX;
	float m_SpeedX;
public:
	void SetDirX(float Dir)
	{
		m_DirX = Dir;
	}
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CEffectCasing* Clone();

};

