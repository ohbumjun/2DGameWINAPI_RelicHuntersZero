#pragma once
#include "Monster.h"
class CTurtleMonster :
    public CMonster
{
	friend class CScene;

protected:
	CTurtleMonster();
	CTurtleMonster(const CTurtleMonster& obj);
	virtual ~CTurtleMonster();

// Animation
public:
	virtual void ChangeIdleAnimation();
	virtual void ChangeMoveAnimation();
	virtual void ChangeRunAnimation();
	virtual void ChangeDeathAnimation();
	virtual void ChangeHitAnimation();

// Set Animation
private:
	virtual void SetAnimation();
	virtual void SetAnimNames();
private:
	void SetTurtle1AnimName();
	void SetTurtle2AnimName();
	void SetTurtle3AnimName();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CTurtleMonster* Clone();
private :
	virtual void CharacterDestroy();
};

