#pragma once
#include "Potion.h"
class CHPPotion :
	public CPotion
{
public:
	CHPPotion();
	const CHPPotion(const CPotion& Potion);
	~CHPPotion();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CHPPotion* Clone();
};

