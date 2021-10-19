#include "KamiKazeMonster.h"
#include "DamageFont.h"
#include "../Scene/Scene.h"

CKamiKazeMonster::CKamiKazeMonster()
{
	m_MonsterType = EMonster_Type::KamiKaze1;
	m_DashDistance = KAMIKAZE_MONSTER_DASH_DISTANCE;
}

CKamiKazeMonster::CKamiKazeMonster(const CKamiKazeMonster& obj) : CMonster(obj)
{
}

CKamiKazeMonster::~CKamiKazeMonster()
{
}

void CKamiKazeMonster::ChangeIdleAnimation()
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

void CKamiKazeMonster::ChangeMoveAnimation()
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

void CKamiKazeMonster::ChangeRunAnimation()
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

void CKamiKazeMonster::ChangeDeathAnimation()
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

void CKamiKazeMonster::ChangeHitAnimation()
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

void CKamiKazeMonster::SetAnimation()
{
	// KamiKaze1 --
	// Right
	AddAnimation(MONSTER_KAMIKAZE1_RIGHT_IDLE, true, 2.f);
	AddAnimation(MONSTER_KAMIKAZE1_RIGHT_WALK, true, 1.f);
	AddAnimation(MONSTER_KAMIKAZE1_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_KAMIKAZE1_RIGHT_RUN, true, 0.6f);
	AddAnimation(MONSTER_KAMIKAZE1_RIGHT_HIT, false, 1.f);
	AddAnimation(MONSTER_KAMIKAZE1_RIGHT_DEATH, false, 1.0f);

	// Left
	AddAnimation(MONSTER_KAMIKAZE1_LEFT_IDLE, true, 2.f);
	AddAnimation(MONSTER_KAMIKAZE1_LEFT_WALK, true, 1.f);
	AddAnimation(MONSTER_KAMIKAZE1_LEFT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_KAMIKAZE1_LEFT_RUN, true, 0.6f);
	AddAnimation(MONSTER_KAMIKAZE1_LEFT_HIT, false, 1.f);
	AddAnimation(MONSTER_KAMIKAZE1_LEFT_DEATH, false, 1.0f);

	// KamiKaze2 --
	// Right
	AddAnimation(MONSTER_KAMIKAZE2_RIGHT_IDLE, true, 2.f);
	AddAnimation(MONSTER_KAMIKAZE2_RIGHT_WALK, true, 1.f);
	AddAnimation(MONSTER_KAMIKAZE2_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_KAMIKAZE2_RIGHT_RUN, true, 0.6f);
	AddAnimation(MONSTER_KAMIKAZE2_RIGHT_HIT, false, 1.f);
	AddAnimation(MONSTER_KAMIKAZE2_RIGHT_DEATH, false, 1.0f);

	// Left
	AddAnimation(MONSTER_KAMIKAZE2_LEFT_IDLE, true, 2.f);
	AddAnimation(MONSTER_KAMIKAZE2_LEFT_WALK, true, 1.f);
	AddAnimation(MONSTER_KAMIKAZE2_LEFT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_KAMIKAZE2_LEFT_RUN, true, 0.6f);
	AddAnimation(MONSTER_KAMIKAZE2_LEFT_HIT, false, 1.f);
	AddAnimation(MONSTER_KAMIKAZE2_LEFT_DEATH, false, 1.0f);
}

void CKamiKazeMonster::SetAnimNames()
{
	switch (m_MonsterType)
	{
	case EMonster_Type::KamiKaze1:
		SetKamiKaze1AnimName();
		break;
	case EMonster_Type::KamiKaze2:
		SetKamiKaze2AnimName();
		break;
	}
	m_TypeChanged = false;

}

void CKamiKazeMonster::SetKamiKaze1AnimName()
{
	m_mapAnimName.clear();
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_IDLE, MONSTER_KAMIKAZE1_RIGHT_IDLE));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_WALK, MONSTER_KAMIKAZE1_RIGHT_WALK));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_ATTACK, MONSTER_KAMIKAZE1_RIGHT_ATTACK));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_RUN, MONSTER_KAMIKAZE1_RIGHT_RUN));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_DEATH, MONSTER_KAMIKAZE1_RIGHT_DEATH));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_HIT, MONSTER_KAMIKAZE1_RIGHT_HIT));

	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_IDLE, MONSTER_KAMIKAZE1_LEFT_IDLE));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_WALK, MONSTER_KAMIKAZE1_LEFT_WALK));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_ATTACK, MONSTER_KAMIKAZE1_LEFT_ATTACK));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_RUN, MONSTER_KAMIKAZE1_LEFT_RUN));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_DEATH, MONSTER_KAMIKAZE1_LEFT_DEATH));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_HIT, MONSTER_KAMIKAZE1_LEFT_HIT));
}

void CKamiKazeMonster::SetKamiKaze2AnimName()
{
	m_mapAnimName.clear();
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_IDLE, MONSTER_KAMIKAZE2_RIGHT_IDLE));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_WALK, MONSTER_KAMIKAZE2_RIGHT_WALK));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_ATTACK, MONSTER_KAMIKAZE2_RIGHT_ATTACK));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_RUN, MONSTER_KAMIKAZE2_RIGHT_RUN));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_DEATH, MONSTER_KAMIKAZE2_RIGHT_DEATH));
	m_mapAnimName.insert(std::make_pair(MONSTER_RIGHT_HIT, MONSTER_KAMIKAZE2_RIGHT_HIT));

	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_IDLE, MONSTER_KAMIKAZE2_LEFT_IDLE));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_WALK, MONSTER_KAMIKAZE2_LEFT_WALK));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_ATTACK, MONSTER_KAMIKAZE2_LEFT_ATTACK));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_RUN, MONSTER_KAMIKAZE2_LEFT_RUN));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_DEATH, MONSTER_KAMIKAZE2_LEFT_DEATH));
	m_mapAnimName.insert(std::make_pair(MONSTER_LEFT_HIT, MONSTER_KAMIKAZE2_LEFT_HIT));
}

void CKamiKazeMonster::Start()
{
	CMonster::Start();
	SetAnimNames();
	SetAnimationEndNotify<CKamiKazeMonster>(MONSTER_KAMIKAZE1_LEFT_DEATH, this, &CKamiKazeMonster::CharacterDestroy);
	SetAnimationEndNotify<CKamiKazeMonster>(MONSTER_KAMIKAZE1_RIGHT_DEATH, this, &CKamiKazeMonster::CharacterDestroy);
	SetAnimationEndNotify<CKamiKazeMonster>(MONSTER_KAMIKAZE2_LEFT_DEATH, this, &CKamiKazeMonster::CharacterDestroy);
	SetAnimationEndNotify<CKamiKazeMonster>(MONSTER_KAMIKAZE2_RIGHT_DEATH, this, &CKamiKazeMonster::CharacterDestroy);
}

bool CKamiKazeMonster::Init()
{
	if (!CMonster::Init()) return false;
	CreateAnimation();
	SetAnimation();
	SetAnimNames();

	return true;
}

void CKamiKazeMonster::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
	if (m_TypeChanged)
		SetAnimNames();
	if (PlayerCollisionCheck())
	{
		CharacterDestroy();
		CDamageFont* DamageFont = m_Scene->CreateObject<CDamageFont>("DamageFont", m_Pos);
		DamageFont->SetDamageNumber((int)(m_CharacterInfo.Attack));
	}
}

void CKamiKazeMonster::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

void CKamiKazeMonster::Collision(float DeltaTime)
{
	CMonster::Collision(DeltaTime);
}

void CKamiKazeMonster::Render(HDC hDC)
{
	CMonster::Render(hDC);
}

CKamiKazeMonster* CKamiKazeMonster::Clone()
{
	return new CKamiKazeMonster(*this);
}

void CKamiKazeMonster::AIIdle(float DeltaTime)
{
	CMonster::AIIdle(DeltaTime);
	m_MoveSpeed = 150.f;
}

void CKamiKazeMonster::AIWalk(float DeltaTime)
{
	CMonster::AIWalk(DeltaTime);
	m_MoveSpeed = 150.f;
}

void CKamiKazeMonster::AITrace(float DeltaTime, Vector2 PlayerPos)
{
	CMonster::AITrace(DeltaTime, PlayerPos);
	m_MoveSpeed = 200.f;
}

void CKamiKazeMonster::AIDeath(float DeltaTime)
{
	CMonster::AIDeath(DeltaTime);
}

void CKamiKazeMonster::AIAttack(float DeltaTime, Vector2 PlayerPos)
{
	float Angle = GetAngle(m_Pos, PlayerPos);
	SetDir(Angle);
}

void CKamiKazeMonster::AIHit(float DeltaTime)
{
	CMonster::AIHit(DeltaTime);
}

void CKamiKazeMonster::CharacterDestroy()
{
	CMonster::CharacterDestroy();
}

