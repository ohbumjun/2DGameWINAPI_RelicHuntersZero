#pragma once
#include"GameInfo.h"

class CInventory
{
public:
	CInventory();
	~CInventory();
private:
	class CGun* m_GunItems[EGunClass::End];
public:
	void AddGun(class CGun* Item);
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void PrevRender();
	virtual void Render(HDC hDC);
	virtual CInventory* Clone();
};