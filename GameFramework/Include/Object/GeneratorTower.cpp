#include "GeneratorTower.h"

CGeneratorTower::CGeneratorTower() 
{
	m_MoveSpeed = 0.f;
	m_IsCreated = true;
}

CGeneratorTower::CGeneratorTower(const CGeneratorTower& obj) : CMonster(obj)
{
	m_IsCreated = true;
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


void CGeneratorTower::CharacterDestroy()
{
	CMonster::CharacterDestroy();
}

void CGeneratorTower::Start()
{
	CMonster::Start();
	SetAnimationEndNotify<CGeneratorTower>(GENERATOR_START, this, &CGeneratorTower::ChangeIdleAnimation);
}

bool CGeneratorTower::Init()
{
	if (!CMonster::Init()) return false;

	AddAnimation(GENERATOR_IDLE, true, 2.f);
	AddAnimation(GENERATOR_START, false, 5.f);
	AddAnimation(GENERATOR_OFF, true, 1.f);
	AddAnimation(GENERATOR_HIT, false, 0.6f);
	SetCurrentAnimation(GENERATOR_START);

	return true;
}

void CGeneratorTower::Update(float DeltaTime)
{
	if (m_IsCreated) return;
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

