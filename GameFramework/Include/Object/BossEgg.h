#pragma once
#include "Monster.h"
class CBossEgg :
    public CMonster
{
	friend class CScene;
	// 0 ó������ Start , -->  Crack --> Boss Monster 
	// 	   ��� Set End notify �Լ��� ���ؼ�, Animation ��ȯ ��Ű�� 
	// 1) ����ź �̻��� : 10�ʿ� �ѹ� �߻�
	// 2) 30�ʸ��� : �ֺ� 6������ grenade ��ô --> ���� ���� �ȿ� ������ damage
	// 3) hp�� 30, 70 �� �����ϸ�, 2������ laser tower�� �ν��߸�, �ٽ� ���� ����( �̶��� ������ x )

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

