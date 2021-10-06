#pragma once
#include "GameObject.h"
class CEffectDash :
    public CGameObject
{
    friend class CScene;
protected:
	CEffectDash();
	CEffectDash(const CEffectDash& obj);
	virtual ~CEffectDash();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CEffectDash* Clone();

public:
	void AnimationFinish();
};

