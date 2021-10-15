#pragma once
#include "Gun.h"
class CShotGun :
    public CGun
{
	friend class CScene;
public:
	CShotGun();
	CShotGun(const CShotGun& obj);
	virtual ~CShotGun();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CShotGun* Clone();
};

