#pragma once
#include "GameObject.h"
class CPotion :
    public CGameObject
{
public:
    CPotion();
    CPotion(const CPotion& Potion);
    virtual ~CPotion();
protected :
    EPotionType PotionType;
    float PlusAmount;
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CPotion* Clone();
};

