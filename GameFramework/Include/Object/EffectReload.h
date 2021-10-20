#pragma once
#include "GameObject.h"
class CEffectReload :
    public CGameObject
{
	friend class CScene;

protected:
	CEffectReload();
	CEffectReload(const CEffectReload& obj);
	virtual ~CEffectReload();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CEffectReload* Clone();
private :
	CGameObject* m_Owner;
public :
	void SetOwner(CGameObject* Obj)
	{
		m_Owner = Obj;
	}
public:
	void AnimationFinish();
};

