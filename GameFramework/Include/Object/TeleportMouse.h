#pragma once
#include "GameObject.h"

class CTeleportMouse :
	public CGameObject
{
	friend class CScene;

protected:
	CTeleportMouse();
	CTeleportMouse(const CTeleportMouse& obj);
	virtual ~CTeleportMouse();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CTeleportMouse* Clone();

public:
	void AnimationFinish();
};

