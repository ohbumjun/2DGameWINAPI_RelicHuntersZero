#pragma once
#include "GameObject.h"
class CEffectHit :
    public CGameObject
{
	friend class CScene;

protected:
	CEffectHit();
	CEffectHit(const CEffectHit& obj);
	virtual ~CEffectHit();
private :
	bool m_IsBossGun;
public :
	void SetIsBossGun();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CEffectHit* Clone();

public:
	void AnimationFinish();
};

