#pragma once
#include "Monster.h"
class CGeneratorTower :
    public CMonster
{
	friend class CScene;
	// 0 처음에는 Start , -->  Crack --> Boss Monster 
	// 	   모두 Set End notify 함수를 통해서, Animation 변환 시키기 
	// 1) 유도탄 미사일 : 10초에 한번 발사
	// 2) 30초마다 : 주변 6군데에 grenade 투척 --> 일정 범위 안에 있으면 damage
	// 3) hp가 30, 70 에 도달하면, 2군데의 laser tower를 부숴야만, 다시 공격 가능( 이때는 데미지 x )

protected:
	CGeneratorTower();
	CGeneratorTower(const CGeneratorTower& obj);
	virtual ~CGeneratorTower();
	// Animation
private :
	bool m_IsCreated;
public:
	virtual void ChangeIdleAnimation();
	virtual void ChangeMoveAnimation();
	virtual void ChangeRunAnimation();
	virtual void ChangeDeathAnimation();
	virtual void ChangeHitAnimation();
private:
	virtual void CharacterDestroy();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CGeneratorTower* Clone();
};

