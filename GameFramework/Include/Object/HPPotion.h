#pragma once
#include "Potion.h"
class CHPPotion :
	public CPotion
{
public:
	CHPPotion();
	virtual ~CHPPotion();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	// virtual CHPPotion* Clone();
};

