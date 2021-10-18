#include "TurtleMonster.h"
#include "EffectShield.h"
#include "../Scene/Scene.h"

CTurtleMonster::CTurtleMonster() : 
	m_ShieldEnable(false), 
	m_ShieldTime(0.f),
	m_ShieldTimeMax(3.f),
	m_ShieldEnableTime(2.f)
{
}

CTurtleMonster::CTurtleMonster(const CTurtleMonster& obj) : CMonster(obj)
{
	m_ShieldEnable = false;
	m_ShieldTime = 0.f;
	m_ShieldTimeMax = 3.f;	
	m_ShieldEnableTime = 2.f;
}

CTurtleMonster::~CTurtleMonster()
{
}

void CTurtleMonster::ChangeIdleAnimation()
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

void CTurtleMonster::ChangeMoveAnimation()
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

void CTurtleMonster::ChangeRunAnimation()
{
	CMonster::ChangeRunAnimation();
	if (m_Dir.x < 0)
	{
		std::string Anim = m_mapAnimName.find(MONSTER_LEFT_RUN)->second;
		ChangeAnimation(Anim);
	}
	else
	{
		std::string Anim = m_mapAnimName.find(MONSTER_RIGHT_RUN)->second;
		ChangeAnimation(Anim);
	}
}

void CTurtleMonster::ChangeDeathAnimation()
{
	CMonster::ChangeDeathAnimation();
	if (m_Dir.x < 0)
	{
		std::string Anim = m_mapAnimName.find(MONSTER_LEFT_DEATH)->second;
		ChangeAnimation(Anim);
	}
	else
	{
		std::string Anim = m_mapAnimName.find(MONSTER_RIGHT_DEATH)->second;
		ChangeAnimation(Anim);
	}
}

void CTurtleMonster::ChangeHitAnimation()
{
	CMonster::ChangeHitAnimation();
	if (m_Dir.x < 0)
	{
		std::string Anim = m_mapAnimName.find(MONSTER_LEFT_HIT)->second;
		ChangeAnimation(Anim);
	}
	else
	{
		std::string Anim = m_mapAnimName.find(MONSTER_RIGHT_HIT)->second;
		ChangeAnimation(Anim);
	}
}

void CTurtleMonster::SetAnimation()
{
	// Turtle1 --
	// Right
	AddAnimation(MONSTER_TURTLE1_RIGHT_IDLE, true, 2.f);
	AddAnimation(MONSTER_TURTLE1_RIGHT_WALK, true, 1.f);
	AddAnimation(MONSTER_TURTLE1_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_TURTLE1_RIGHT_RUN, true, 0.6f);
	AddAnimation(MONSTER_TURTLE1_RIGHT_HIT, false, 1.f);
	AddAnimation(MONSTER_TURTLE1_RIGHT_DEATH, false, 1.0f);

	// Left
	AddAnimation(MONSTER_TURTLE1_LEFT_IDLE, true, 2.f);
	AddAnimation(MONSTER_TURTLE1_LEFT_WALK, true, 1.f);
	AddAnimation(MONSTER_TURTLE1_LEFT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_TURTLE1_LEFT_RUN, true, 0.6f);
	AddAnimation(MONSTER_TURTLE1_LEFT_HIT, false, 1.f);
	AddAnimation(MONSTER_TURTLE1_LEFT_DEATH, false, 1.0f);

	// Turtle2 --
	// Right
	AddAnimation(MONSTER_TURTLE2_RIGHT_IDLE, true, 2.f);
	AddAnimation(MONSTER_TURTLE2_RIGHT_WALK, true, 1.f);
	AddAnimation(MONSTER_TURTLE2_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_TURTLE2_RIGHT_RUN, true, 0.6f);
	AddAnimation(MONSTER_TURTLE2_RIGHT_HIT, false, 1.f);
	AddAnimation(MONSTER_TURTLE2_RIGHT_DEATH, false, 1.0f);

	// Left
	AddAnimation(MONSTER_TURTLE2_LEFT_IDLE, true, 2.f);
	AddAnimation(MONSTER_TURTLE2_LEFT_WALK, true, 1.f);
	AddAnimation(MONSTER_TURTLE2_LEFT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_TURTLE2_LEFT_RUN, true, 0.6f);
	AddAnimation(MONSTER_TURTLE2_LEFT_HIT, false, 1.f);
	AddAnimation(MONSTER_TURTLE2_LEFT_DEATH, false, 1.0f);

	// Turtle1 --
	// Right
	AddAnimation(MONSTER_TURTLE3_RIGHT_IDLE, true, 2.f);
	AddAnimation(MONSTER_TURTLE3_RIGHT_WALK, true, 1.f);
	AddAnimation(MONSTER_TURTLE3_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_TURTLE3_RIGHT_RUN, true, 0.6f);
	AddAnimation(MONSTER_TURTLE3_RIGHT_HIT, false, 1.f);
	AddAnimation(MONSTER_TURTLE3_RIGHT_DEATH, false, 1.0f);

	// Left
	AddAnimation(MONSTER_TURTLE3_LEFT_IDLE, true, 2.f);
	AddAnimation(MONSTER_TURTLE3_LEFT_WALK, true, 1.f);
	AddAnimation(MONSTER_TURTLE3_LEFT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_TURTLE3_LEFT_RUN, true, 0.6f);
	AddAnimation(MONSTER_TURTLE3_LEFT_HIT, false, 1.f);
	AddAnimation(MONSTER_TURTLE3_LEFT_DEATH, false, 1.0f);
}

void CTurtleMonster::SetAnimNames()
{
	switch (m_MonsterType)
	{
	case EMonster_Type::Turtle1:
		SetTurtle1AnimName();
		break;
	case EMonster_Type::Turtle2:
		SetTurtle2AnimName();
		break;
	case EMonster_Type::Turtle3:
		SetTurtle3AnimName();
	}
	m_TypeChanged = false;

}

void CTurtleMonster::SetTurtle1AnimName()
{
	m_mapAnimName.clear();
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_IDLE, MONSTER_TURTLE1_RIGHT_IDLE));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_WALK, MONSTER_TURTLE1_RIGHT_WALK));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_ATTACK, MONSTER_TURTLE1_RIGHT_ATTACK));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_RUN, MONSTER_TURTLE1_RIGHT_RUN));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_DEATH, MONSTER_TURTLE1_RIGHT_DEATH));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_HIT, MONSTER_TURTLE1_RIGHT_HIT));

	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_IDLE, MONSTER_TURTLE1_LEFT_IDLE));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_WALK, MONSTER_TURTLE1_LEFT_WALK));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_ATTACK, MONSTER_TURTLE1_LEFT_ATTACK));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_RUN, MONSTER_TURTLE1_LEFT_RUN));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_DEATH, MONSTER_TURTLE1_LEFT_DEATH));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_HIT, MONSTER_TURTLE1_LEFT_HIT));
}

void CTurtleMonster::SetTurtle2AnimName()
{
	m_mapAnimName.clear();
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_IDLE, MONSTER_TURTLE2_RIGHT_IDLE));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_WALK, MONSTER_TURTLE2_RIGHT_WALK));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_ATTACK, MONSTER_TURTLE2_RIGHT_ATTACK));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_RUN, MONSTER_TURTLE2_RIGHT_RUN));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_DEATH, MONSTER_TURTLE2_RIGHT_DEATH));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_HIT, MONSTER_TURTLE2_RIGHT_HIT));

	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_IDLE, MONSTER_TURTLE2_LEFT_IDLE));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_WALK, MONSTER_TURTLE2_LEFT_WALK));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_ATTACK, MONSTER_TURTLE2_LEFT_ATTACK));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_RUN, MONSTER_TURTLE2_LEFT_RUN));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_DEATH, MONSTER_TURTLE2_LEFT_DEATH));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_HIT, MONSTER_TURTLE2_LEFT_HIT));
}

void CTurtleMonster::SetTurtle3AnimName()
{
	m_mapAnimName.clear();
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_IDLE, MONSTER_TURTLE3_RIGHT_IDLE));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_WALK, MONSTER_TURTLE3_RIGHT_WALK));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_ATTACK, MONSTER_TURTLE3_RIGHT_ATTACK));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_RUN, MONSTER_TURTLE3_RIGHT_RUN));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_DEATH, MONSTER_TURTLE3_RIGHT_DEATH));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_HIT, MONSTER_TURTLE3_RIGHT_HIT));

	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_IDLE, MONSTER_TURTLE3_LEFT_IDLE));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_WALK, MONSTER_TURTLE3_LEFT_WALK));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_ATTACK, MONSTER_TURTLE3_LEFT_ATTACK));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_RUN, MONSTER_TURTLE3_LEFT_RUN));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_DEATH, MONSTER_TURTLE3_LEFT_DEATH));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_HIT, MONSTER_TURTLE3_LEFT_HIT));
}


void CTurtleMonster::Start()
{
	CMonster::Start();
	SetAnimNames();
	SetAnimationEndNotify<CTurtleMonster>(MONSTER_TURTLE1_LEFT_DEATH, this, &CTurtleMonster::CharacterDestroy);
	SetAnimationEndNotify<CTurtleMonster>(MONSTER_TURTLE1_RIGHT_DEATH, this, &CTurtleMonster::CharacterDestroy);

	SetAnimationEndNotify<CTurtleMonster>(MONSTER_TURTLE2_LEFT_DEATH, this, &CTurtleMonster::CharacterDestroy);
	SetAnimationEndNotify<CTurtleMonster>(MONSTER_TURTLE2_RIGHT_DEATH, this, &CTurtleMonster::CharacterDestroy);

	SetAnimationEndNotify<CTurtleMonster>(MONSTER_TURTLE3_LEFT_DEATH, this, &CTurtleMonster::CharacterDestroy);
	SetAnimationEndNotify<CTurtleMonster>(MONSTER_TURTLE3_RIGHT_DEATH, this, &CTurtleMonster::CharacterDestroy);
}

bool CTurtleMonster::Init()
{
	if (!CMonster::Init()) return false;
	SetAnimation();
	SetAnimNames();
	return true;
}

void CTurtleMonster::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
	if (m_TypeChanged)
		SetAnimNames();

	m_ShieldTime += DeltaTime;
	// Make Shield per 3 sec
	if (m_ShieldTime >= m_ShieldTimeMax)
	{
		m_ShieldTime -= m_ShieldTimeMax;
		m_Shield = m_Scene->CreateObject<CEffectShield>("Shield",SHIELD_PROTO, m_Pos);
		m_Shield->SetShieldType(EShield_Type::Turtle);
		m_Shield->SetOwner(this);
		m_ShieldEnable = true;
		m_ShieldEnableTime = 1.f;
	}
	// Maintain Shield for 1sec
	if (m_ShieldEnable && m_ShieldEnableTime > 0.f)
	{
		m_ShieldEnableTime -= DeltaTime;
		if (m_ShieldEnableTime <= 0.f)
		{
			m_ShieldEnable = false;
			m_Shield->Destroy();
		}
	}
}

void CTurtleMonster::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

void CTurtleMonster::Collision(float DeltaTime)
{
	CMonster::Collision(DeltaTime);
}

void CTurtleMonster::Render(HDC hDC)
{
	CMonster::Render(hDC);
}

CTurtleMonster* CTurtleMonster::Clone()
{
	return new CTurtleMonster(*this);
}

void CTurtleMonster::CharacterDestroy()
{
	CMonster::CharacterDestroy();
}

