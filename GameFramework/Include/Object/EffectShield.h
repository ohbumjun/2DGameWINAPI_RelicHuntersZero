#pragma once
#include "GameObject.h"
class CEffectShield :
    public CGameObject
{
	friend class CScene;
protected:
	CEffectShield();
	CEffectShield(const CEffectShield& obj);
	virtual ~CEffectShield();
private :
	EShield_Type m_ShieldType;
public :
	void SetShieldType(EShield_Type Type)
	{
		m_ShieldType = Type;
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

