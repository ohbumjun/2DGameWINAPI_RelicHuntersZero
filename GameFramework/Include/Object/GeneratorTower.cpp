#include "GeneratorTower.h"

CGeneratorTower::CGeneratorTower() 
{
}

CGeneratorTower::CGeneratorTower(const CGeneratorTower& obj) : CMonster(obj)
{
}

CGeneratorTower::~CGeneratorTower()
{
}

void CGeneratorTower::ChangeIdleAnimation()
{
	CMonster::ChangeIdleAnimation();
	ChangeAnimation(GENERATOR_IDLE);
}

void CGeneratorTower::ChangeMoveAnimation()
{
	CMonster::ChangeMoveAnimation();
	ChangeAnimation(GENERATOR_IDLE);
}

void CGeneratorTower::ChangeRunAnimation()
{
	CMonster::ChangeRunAnimation();
	ChangeAnimation(GENERATOR_IDLE);
}

void CGeneratorTower::ChangeDeathAnimation()
{
	CMonster::ChangeDeathAnimation();
	ChangeAnimation(GENERATOR_OFF);
}

void CGeneratorTower::ChangeHitAnimation()
{
	CMonster::ChangeHitAnimation();
	ChangeAnimation(GENERATOR_HIT);
}

void CGeneratorTower::SetAnimation()
{
	AddAnimation(GENERATOR_IDLE, true, 2.f);
	AddAnimation(GENERATOR_START, true, 4.f);
	AddAnimation(GENERATOR_OFF, false, 0.1f);
	AddAnimation(GENERATOR_HIT, false, 0.6f);

}

void CGeneratorTower::CharacterDestroy()
{
	CMonster::CharacterDestroy();
}

void CGeneratorTower::Start()
{
	CMonster::Start();
	SetAnimationEndNotify<CGeneratorTower>(GENERATOR_START, this, &CGeneratorTower::CharacterDestroy);
}

bool CGeneratorTower::Init()
{
	if (!CMonster::Init()) return false;
	SetAnimation();
	SetAnimNames();
	SetCurrentAnimation(GENERATOR_START);

	return true;
}

void CGeneratorTower::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
}

void CGeneratorTower::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

void CGeneratorTower::Collision(float DeltaTime)
{
	CMonster::Collision(DeltaTime);
}

void CGeneratorTower::Render(HDC hDC)
{
	CMonster::Render(hDC);
}

CGeneratorTower* CGeneratorTower::Clone()
{
	return new CGeneratorTower(*this);
}

