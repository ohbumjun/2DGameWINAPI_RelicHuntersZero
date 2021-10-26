#pragma once
#include "GameObject.h"
class CEffectGrenade :
    public CGameObject
{
public :
    CEffectGrenade();
    CEffectGrenade(const CEffectGrenade& Grenade);
    virtual ~CEffectGrenade();
private :
	bool m_IsPlayerGrenade;
private:
	// 랜덤 방향과 속도
	float m_SpeedX;
	float m_FallTime;
	int   m_Damage;
	float m_ExplodeTime;
	float m_ExplodeMaxTime;
	bool  m_Explode;
	float m_ExplodeDist;
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CEffectGrenade* Clone();
public :
	void SetPlayerGrenage(bool State)
	{
		m_IsPlayerGrenade = State;
	}
public :
	void ChangeExplosionAnimation();
	void ChangePlayerExplosionAnimation();
	void Explode();
	void DrawExplodeTrace();
	void ExplodeDamagePlayer();
	void ExplodeDamageMonster();
	void ExplodeHitPlayer(class CPlayer* const Player);
	void ExplodeHitMonster();
};

