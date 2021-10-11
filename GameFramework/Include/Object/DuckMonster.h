#pragma once
#include "Monster.h"
class CDuckMonster :
    public CMonster
{
	friend class CScene;

protected:
	CDuckMonster();
	CDuckMonster(const CDuckMonster& obj);
	virtual ~CDuckMonster();

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
private :
	void SetDuck1AnimName();
	void SetDuck2AnimName();
	void SetDuck3AnimName();
public :
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CDuckMonster* Clone();
};

