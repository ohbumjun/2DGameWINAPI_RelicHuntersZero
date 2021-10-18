#pragma once
#include "GameObject.h"
class CEffectShield :
    public CGameObject
{
	friend class CScene;
public:
	CEffectShield();
	CEffectShield(const CEffectShield& obj);
	virtual ~CEffectShield();
private :
	EShield_Type m_ShieldType;
	CGameObject* m_Owner;
public :
	void SetShieldType(EShield_Type Type)
	{
		m_ShieldType = Type;
	}
	void SetOwner(CGameObject* Owner)
	{
		m_Owner = Owner;
	}
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CEffectShield* Clone();

public:
	void AnimationFinish();
};

