#pragma once
#include "GameObject.h"
class CEffectDoorAbove :
    public CGameObject
{
	friend class CScene;

protected:
	CEffectDoorAbove();
	CEffectDoorAbove(const CEffectDoorAbove& obj);
	virtual ~CEffectDoorAbove();
private :
	CGameObject* m_Owner;
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
public :
	void SetOwner(CGameObject* Owner) { m_Owner = Owner; }
public:
	void AnimationFinish();
};

