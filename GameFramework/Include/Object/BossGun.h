#pragma once
#include "Gun.h"
class CBossGun :
    public CGun
{
	friend class CScene;
public:
	CBossGun();
	CBossGun(const CBossGun& obj);
	virtual ~CBossGun();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CBossGun* Clone();
};

