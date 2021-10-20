#pragma once
#include "GameObject.h"
class CEffectShieldStart :
    public CGameObject
{
	friend class CScene;

protected:
	CEffectShieldStart();
	CEffectShieldStart(const CEffectShieldStart& obj);
	virtual ~CEffectShieldStart();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CEffectShieldStart* Clone();

public:
	void AnimationFinish();
private :
	CGameObject* m_Owner;
public :
	void SetOwner(CGameObject* Owner)
	{
		m_Owner = Owner;
	}
};

