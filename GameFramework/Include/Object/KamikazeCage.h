#pragma once
#include "Monster.h"
class CKamiKazeCage :
    public CMonster
{
	friend class CScene;

protected:
	CKamiKazeCage();
	CKamiKazeCage(const CKamiKazeCage& obj);
	virtual ~CKamiKazeCage();

	// Animation
public:
	virtual void ChangeIdleAnimation();
	virtual void ChangeHitAnimation();
	virtual void ChangeDeathAnimation();
	// Set Animation
private:
	virtual void SetAnimation();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CKamiKazeCage* Clone();
private:
	virtual void AIIdle(float DeltaTime);
	virtual void AIWalk(float DeltaTime);
	virtual void AITrace(float DeltaTime, Vector2 PlayerPos);
	virtual void AIDeath(float DeltaTime);
	virtual void AIAttack(float DeltaTime, Vector2 PlayerPos);
	virtual void AIHit(float DeltaTime);
private :
	void CharacterDestroy();
};

