#pragma once
#include "Monster.h"
class CGeneratorTower :
    public CMonster
{
	friend class CScene;
	// 0 ó������ Start , -->  Crack --> Boss Monster 
	// 	   ��� Set End notify �Լ��� ���ؼ�, Animation ��ȯ ��Ű�� 
	// 1) ����ź �̻��� : 10�ʿ� �ѹ� �߻�
	// 2) 30�ʸ��� : �ֺ� 6������ grenade ��ô --> ���� ���� �ȿ� ������ damage
	// 3) hp�� 30, 70 �� �����ϸ�, 2������ laser tower�� �ν��߸�, �ٽ� ���� ����( �̶��� ������ x )

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

