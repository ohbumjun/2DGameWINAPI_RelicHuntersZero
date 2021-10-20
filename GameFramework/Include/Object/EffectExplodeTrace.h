#pragma once
#include "GameObject.h"
class CEffectExplodeTrace :
	public CGameObject
{
	friend class CScene;

protected:
	CEffectExplodeTrace();
	CEffectExplodeTrace(const CEffectExplodeTrace& obj);
	virtual ~CEffectExplodeTrace();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CEffectExplodeTrace* Clone();

public:
	void AnimationFinish();
};