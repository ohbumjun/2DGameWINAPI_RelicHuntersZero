#pragma once
#include "Monster.h"
class CBossEgg :
    public CMonster
{
	friend class CScene;
	// 0 처음에는 Start , -->  Crack --> Boss Monster 
	// 	   모두 Set End notify 함수를 통해서, Animation 변환 시키기 
	// 1) 유도탄 미사일 : 10초에 한번 발사
	// 2) 30초마다 : 주변 6군데에 grenade 투척 --> 일정 범위 안에 있으면 damage
	// 3) hp가 30, 70 에 도달하면, 2군데의 laser tower를 부숴야만, 다시 공격 가능( 이때는 데미지 x )

protected:
	CBossEgg();
	CBossEgg(const CBossEgg& obj);
	virtual ~CBossEgg();
private:
	float m_EggStartTime;
	float m_EggStartEnable;
	void EggStartUpdate(float DeltaTime);
private:
	float m_EggCrackTime;
	float m_EggCrackEnable;
	void EggCrackUpdate(float DeltaTime);
private:
	void EggCrack();
	void MonsterAppear();
	// Set Animation
private:
	virtual void SetAnimation();
private:
	virtual void CharacterDestroy();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
};

