#pragma once
#include "GameObject.h"
class CEffectAbilityUp :
    public CGameObject
{
	friend class CScene;

protected:
	CEffectAbilityUp();
	CEffectAbilityUp(const CEffectAbilityUp& obj);
	virtual ~CEffectAbilityUp();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CEffectAbilityUp* Clone();

public:
	void AnimationFinish();
};

