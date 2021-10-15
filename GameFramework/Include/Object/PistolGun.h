#pragma once
#include "../GameInfo.h"
#include "Gun.h"
class CPistolGun :
    public CGun
{
	friend class CScene;
public:
	CPistolGun();
	CPistolGun(const CPistolGun& obj);
	virtual ~CPistolGun();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CPistolGun* Clone();
};

