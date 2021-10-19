#pragma once
#include "GeneratorTower.h"
#include "Monster.h"
class CBossMonster :
    public CMonster
{
	friend class CScene;
// 0 ó������ Start , -->  Crack --> Boss Monster 
// 	   ��� Set End notify �Լ��� ���ؼ�, Animation ��ȯ ��Ű�� 
// 1) ����ź �̻��� : 10�ʿ� �ѹ� �߻�
// 2) 30�ʸ��� : �ֺ� 6������ grenade ��ô --> ���� ���� �ȿ� ������ damage
// 3) hp�� 30, 70 �� �����ϸ�, 2������ laser tower�� �ν��߸�, �ٽ� ���� ����( �̶��� ������ x )

protected:
	CBossMonster();
	CBossMonster(const CBossMonster& obj);
	virtual ~CBossMonster();
private :
	std::list<CSharedPtr<CGeneratorTower>> m_GeneratorList;
private :
	float m_MissileAttackTime;
	float m_MissileAttackMaxTime;
	void MissileUpdate(float DeltaTime);
	void MissileAttack(float DeltaTime);
private :
	float m_GrenadeTime;
	float m_GrenadMaxTime;
	void GrenadeUpdate(float DeltaTime);
	void GrenadeAttack(float DeltaTime);
private :
	bool m_IsGeneratorAlive;
	void GeneratorUpdate(float DeltaTime);
	void GeneratorAttack(float DeltaTime);
// Animation
public:
	virtual void ChangeIdleAnimation();
	virtual void ChangeMoveAnimation();
	virtual void ChangeRunAnimation();
	virtual void ChangeDeathAnimation();
	virtual void ChangeHitAnimation();
private :
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
	virtual CBossMonster* Clone();
};

