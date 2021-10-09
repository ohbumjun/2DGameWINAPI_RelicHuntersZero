#pragma once
#include "GameObject.h"
class CEffectSurprise :
    public CGameObject
{
	friend class CScene;

protected:
	CEffectSurprise();
	CEffectSurprise(const CEffectSurprise& obj);
	virtual ~CEffectSurprise();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CEffectSurprise* Clone();

public:
	void AnimationFinish();
};

