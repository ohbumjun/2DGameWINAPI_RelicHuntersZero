#pragma once
#include "GameObject.h"
class CEffectBulletStart :
    public CGameObject
{
	friend class CScene;

protected:
	CEffectBulletStart();
	CEffectBulletStart(const CEffectBulletStart& obj);
	virtual ~CEffectBulletStart();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CEffectBulletStart* Clone();

public:
	void AnimationFinish();
};

