#include "DuckMonster.h"

CDuckMonster::CDuckMonster()
{
}

CDuckMonster::CDuckMonster(const CDuckMonster& obj) : CMonster(obj)
{
}

CDuckMonster::~CDuckMonster()
{
}

void CDuckMonster::ChangeIdleAnimation()
{
	CMonster::ChangeIdleAnimation();
	if (m_Dir.x < 0)
		ChangeAnimation(MONSTER_DUCK1_LEFT_IDLE);
	else
		ChangeAnimation(MONSTER_DUCK1_RIGHT_IDLE);
}

void CDuckMonster::ChangeMoveAnimation()
{
	CMonster::ChangeMoveAnimation();
	if (m_Dir.x < 0)
		ChangeAnimation(MONSTER_DUCK1_LEFT_WALK);
	else
		ChangeAnimation(MONSTER_DUCK1_RIGHT_WALK);
}

void CDuckMonster::ChangeRunAnimation()
{
	CMonster::ChangeRunAnimation();
	if (m_Dir.x < 0)
		ChangeAnimation(MONSTER_DUCK1_LEFT_RUN);
	else
		ChangeAnimation(MONSTER_DUCK1_RIGHT_RUN);
}

void CDuckMonster::ChangeDeathAnimation()
{
	CMonster::ChangeDeathAnimation();
	if (m_Dir.x < 0)
		ChangeAnimation(MONSTER_DUCK1_LEFT_DEATH);
	else
		ChangeAnimation(MONSTER_DUCK1_RIGHT_DEATH);
}

void CDuckMonster::ChangeHitAnimation()
{
	CMonster::ChangeHitAnimation();
	if (m_Dir.x < 0)
		ChangeAnimation(MONSTER_DUCK1_LEFT_HIT);
	else
		ChangeAnimation(MONSTER_DUCK1_RIGHT_HIT);
}

void CDuckMonster::SetAnimation()
{
	// Right
	AddAnimation(MONSTER_DUCK1_RIGHT_IDLE, true, 2.f);
	AddAnimation(MONSTER_DUCK1_RIGHT_WALK, true, 1.f);
	AddAnimation(MONSTER_DUCK1_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_DUCK1_RIGHT_RUN, true, 0.6f);
	AddAnimation(MONSTER_DUCK1_RIGHT_HIT, false, 1.f);
	AddAnimation(MONSTER_DUCK1_RIGHT_DEATH, false, 1.5f);

	// Left
	AddAnimation(MONSTER_DUCK1_LEFT_IDLE, true, 2.f);
	AddAnimation(MONSTER_DUCK1_LEFT_WALK, true, 1.f);
	AddAnimation(MONSTER_DUCK1_LEFT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_DUCK1_LEFT_RUN, true, 0.6f);
	AddAnimation(MONSTER_DUCK1_LEFT_HIT, false, 1.f);
	AddAnimation(MONSTER_DUCK1_LEFT_DEATH, false, 1.5f);
}


void CDuckMonster::Start()
{
	CMonster::Start();
	SetAnimation();
	SetAnimationEndNotify<CDuckMonster>(MONSTER_DUCK1_LEFT_DEATH, this, &CDuckMonster::CharacterDestroy);
	SetAnimationEndNotify<CDuckMonster>(MONSTER_DUCK1_RIGHT_DEATH, this, &CDuckMonster::CharacterDestroy);
}

bool CDuckMonster::Init()
{
	if (!CMonster::Init()) return false;
	return true;
}

void CDuckMonster::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
}

void CDuckMonster::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

void CDuckMonster::Collision(float DeltaTime)
{
	CMonster::Collision(DeltaTime);
}

void CDuckMonster::Render(HDC hDC)
{
	CMonster::Render(hDC);
}

CDuckMonster* CDuckMonster::Clone()
{
	return new CDuckMonster(*this);
}

