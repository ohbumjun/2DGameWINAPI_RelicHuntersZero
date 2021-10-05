#pragma once
#include "Potion.h"
class CMPPotion :
    public CPotion
{
public:
	CMPPotion();
	virtual ~CMPPotion();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	// virtual CMPPotion* Clone();
};

