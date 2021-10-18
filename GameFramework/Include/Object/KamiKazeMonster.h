#pragma once
#include "Monster.h"
class CKamiKazeMonster :
    public CMonster
{
	friend class CScene;

protected:
	CKamiKazeMonster();
	CKamiKazeMonster(const CKamiKazeMonster& obj);
	virtual ~CKamiKazeMonster();

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
	void SetKamiKaze1AnimName();
	void SetKamiKaze2AnimName();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CKamiKazeMonster* Clone();
};

