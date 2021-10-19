#pragma once
#include "GameObject.h"
class CEffectGrenade :
    public CGameObject
{
public :
    CEffectGrenade();
    CEffectGrenade(const CEffectGrenade& Grenade);
    virtual ~CEffectGrenade();

private:
	// ���� ����� �ӵ�
	float m_SpeedX;
	float m_FallTime;
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CEffectGrenade* Clone();
public :
	void ChangeExplosionAnimation();
	void Explode();
	void AnimationFinish();
};

