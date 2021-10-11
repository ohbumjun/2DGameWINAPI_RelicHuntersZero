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
	{
		std::string Anim = m_mapAnimName.find(MONSTER_LEFT_IDLE)->second;
		ChangeAnimation(Anim);
	}
	else
	{
		std::string Anim = m_mapAnimName.find(MONSTER_RIGHT_IDLE)->second;
		ChangeAnimation(Anim);
	}
}

void CDuckMonster::ChangeMoveAnimation()
{
	CMonster::ChangeMoveAnimation();
	if (m_Dir.x < 0)
	{
		std::string Anim = m_mapAnimName.find(MONSTER_LEFT_WALK)->second;
		ChangeAnimation(Anim);
	}
	else
	{
		std::string Anim = m_mapAnimName.find(MONSTER_RIGHT_WALK)->second;
		ChangeAnimation(Anim);
	}
}

void CDuckMonster::ChangeRunAnimation()
{
	CMonster::ChangeRunAnimation();
	if (m_Dir.x < 0)
	{
		std::string Anim = m_mapAnimName.find(MONSTER_LEFT_WALK)->second;
		ChangeAnimation(Anim);
	}
	else
	{
		std::string Anim = m_mapAnimName.find(MONSTER_RIGHT_WALK)->second;
		ChangeAnimation(Anim);
	}
}

void CDuckMonster::ChangeDeathAnimation()
{
	CMonster::ChangeDeathAnimation();
	if (m_Dir.x < 0)
	{
		std::string Anim = m_mapAnimName.find(MONSTER_LEFT_WALK)->second;
		ChangeAnimation(Anim);
	}
	else
	{
		std::string Anim = m_mapAnimName.find(MONSTER_RIGHT_WALK)->second;
		ChangeAnimation(Anim);
	}
}

void CDuckMonster::ChangeHitAnimation()
{
	CMonster::ChangeHitAnimation();
	if (m_Dir.x < 0)
	{
		std::string Anim = m_mapAnimName.find(MONSTER_LEFT_WALK)->second;
		ChangeAnimation(Anim);
	}
	else
	{
		std::string Anim = m_mapAnimName.find(MONSTER_RIGHT_WALK)->second;
		ChangeAnimation(Anim);
	}
}

void CDuckMonster::SetAnimation()
{
	// Duck1 --
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

	// Duck2 --
	// Right
	AddAnimation(MONSTER_DUCK2_RIGHT_IDLE, true, 2.f);
	AddAnimation(MONSTER_DUCK2_RIGHT_WALK, true, 1.f);
	AddAnimation(MONSTER_DUCK2_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_DUCK2_RIGHT_RUN, true, 0.6f);
	AddAnimation(MONSTER_DUCK2_RIGHT_HIT, false, 1.f);
	AddAnimation(MONSTER_DUCK2_RIGHT_DEATH, false, 1.5f);

	// Left
	AddAnimation(MONSTER_DUCK2_LEFT_IDLE, true, 2.f);
	AddAnimation(MONSTER_DUCK2_LEFT_WALK, true, 1.f);
	AddAnimation(MONSTER_DUCK2_LEFT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_DUCK2_LEFT_RUN, true, 0.6f);
	AddAnimation(MONSTER_DUCK2_LEFT_HIT, false, 1.f);
	AddAnimation(MONSTER_DUCK2_LEFT_DEATH, false, 1.5f);

	// Duck1 --
	// Right
	AddAnimation(MONSTER_DUCK3_RIGHT_IDLE, true, 2.f);
	AddAnimation(MONSTER_DUCK3_RIGHT_WALK, true, 1.f);
	AddAnimation(MONSTER_DUCK3_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_DUCK3_RIGHT_RUN, true, 0.6f);
	AddAnimation(MONSTER_DUCK3_RIGHT_HIT, false, 1.f);
	AddAnimation(MONSTER_DUCK3_RIGHT_DEATH, false, 1.5f);

	// Left
	AddAnimation(MONSTER_DUCK3_LEFT_IDLE, true, 2.f);
	AddAnimation(MONSTER_DUCK3_LEFT_WALK, true, 1.f);
	AddAnimation(MONSTER_DUCK3_LEFT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_DUCK3_LEFT_RUN, true, 0.6f);
	AddAnimation(MONSTER_DUCK3_LEFT_HIT, false, 1.f);
	AddAnimation(MONSTER_DUCK3_LEFT_DEATH, false, 1.5f);
}

void CDuckMonster::SetAnimNames()
{
	switch (m_MonsterType)
	{
	case EMonster_Type::Duck1:
		SetDuck1AnimName();
		break;
	case EMonster_Type::Duck2:
		SetDuck2AnimName();
		break;
	case EMonster_Type::Duck3:
		SetDuck3AnimName();
	}
	m_TypeChanged = false;

}

void CDuckMonster::SetDuck1AnimName()
{
	m_mapAnimName.clear();
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_IDLE, MONSTER_DUCK1_RIGHT_IDLE));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_WALK, MONSTER_DUCK1_RIGHT_WALK));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_ATTACK, MONSTER_DUCK1_RIGHT_ATTACK));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_RUN, MONSTER_DUCK1_RIGHT_RUN));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_DEATH, MONSTER_DUCK1_RIGHT_DEATH));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_HIT, MONSTER_DUCK1_RIGHT_HIT));

	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_IDLE, MONSTER_DUCK1_LEFT_IDLE));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_WALK, MONSTER_DUCK1_LEFT_WALK));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_ATTACK, MONSTER_DUCK1_LEFT_ATTACK));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_RUN, MONSTER_DUCK1_LEFT_RUN));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_DEATH, MONSTER_DUCK1_LEFT_DEATH));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_HIT, MONSTER_DUCK1_LEFT_HIT));
}

void CDuckMonster::SetDuck2AnimName()
{
	m_mapAnimName.clear();
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_IDLE, MONSTER_DUCK2_RIGHT_IDLE));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_WALK, MONSTER_DUCK2_RIGHT_WALK));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_ATTACK, MONSTER_DUCK2_RIGHT_ATTACK));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_RUN, MONSTER_DUCK2_RIGHT_RUN));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_DEATH, MONSTER_DUCK2_RIGHT_DEATH));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_HIT, MONSTER_DUCK2_RIGHT_HIT));

	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_IDLE, MONSTER_DUCK2_LEFT_IDLE));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_WALK, MONSTER_DUCK2_LEFT_WALK));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_ATTACK, MONSTER_DUCK2_LEFT_ATTACK));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_RUN, MONSTER_DUCK2_LEFT_RUN));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_DEATH, MONSTER_DUCK2_LEFT_DEATH));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_HIT, MONSTER_DUCK2_LEFT_HIT));
}

void CDuckMonster::SetDuck3AnimName()
{
	m_mapAnimName.clear();
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_IDLE, MONSTER_DUCK3_RIGHT_IDLE));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_WALK, MONSTER_DUCK3_RIGHT_WALK));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_ATTACK, MONSTER_DUCK3_RIGHT_ATTACK));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_RUN, MONSTER_DUCK3_RIGHT_RUN));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_DEATH, MONSTER_DUCK3_RIGHT_DEATH));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_HIT, MONSTER_DUCK3_RIGHT_HIT));

	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_IDLE, MONSTER_DUCK3_LEFT_IDLE));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_WALK, MONSTER_DUCK3_LEFT_WALK));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_ATTACK, MONSTER_DUCK3_LEFT_ATTACK));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_RUN, MONSTER_DUCK3_LEFT_RUN));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_DEATH, MONSTER_DUCK3_LEFT_DEATH));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_HIT, MONSTER_DUCK3_LEFT_HIT));
}


void CDuckMonster::Start()
{
	CMonster::Start();
	SetAnimationEndNotify<CDuckMonster>(MONSTER_DUCK1_LEFT_DEATH, this, &CDuckMonster::CharacterDestroy);
	SetAnimationEndNotify<CDuckMonster>(MONSTER_DUCK1_RIGHT_DEATH, this, &CDuckMonster::CharacterDestroy);
	SetAnimationEndNotify<CDuckMonster>(MONSTER_DUCK2_LEFT_DEATH, this, &CDuckMonster::CharacterDestroy);
	SetAnimationEndNotify<CDuckMonster>(MONSTER_DUCK2_RIGHT_DEATH, this, &CDuckMonster::CharacterDestroy);
	SetAnimationEndNotify<CDuckMonster>(MONSTER_DUCK3_LEFT_DEATH, this, &CDuckMonster::CharacterDestroy);
	SetAnimationEndNotify<CDuckMonster>(MONSTER_DUCK3_RIGHT_DEATH, this, &CDuckMonster::CharacterDestroy);
}

bool CDuckMonster::Init()
{
	if (!CMonster::Init()) return false;
	SetAnimation();
	SetAnimNames();
	return true;
}

void CDuckMonster::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
	if (m_TypeChanged)
		SetAnimNames();
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

