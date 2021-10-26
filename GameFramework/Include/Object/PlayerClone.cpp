#include "PlayerClone.h"
#include "Monster.h"
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderSphere.h"
#include "../UI/ProgressBar.h"
#include "../Scene/Scene.h"
#include "../UI/UIText.h"

CPlayerClone::CPlayerClone() :
	m_AI(EMonsterAI::Idle),
	m_CharType(EChar_Type::Ass),
	m_DashDistance((int)NORMAL_MONSTER_DASH_DISTANCE),
	m_AttackDistance((int)NORMAL_ATTACK_DISTANCE),
	m_ClosestMonster(nullptr)
{
}

CPlayerClone::CPlayerClone(const CPlayerClone& obj) : CCharacter(obj)
{
	m_AI             = obj.m_AI;
	m_CharType       = obj.m_CharType;
	m_DashDistance   = obj.m_DashDistance;
	m_AttackDistance = obj.m_AttackDistance;
	m_ClosestMonster = nullptr;
}

CPlayerClone::~CPlayerClone()
{
}

bool CPlayerClone::Init()
{
    if (!CCharacter::Init()) return false;

	// Animation ---
	CreateAnimation();
	AddAssAnimName();
	AddJimmyAnimName();
	AddBiuAnimName();
	AddPinkyAnimName();
	AddPunnyAnimName();
	AddRaffAnimName();

	// Name
	SetCharName();

	// Collider ---
	CColliderSphere* Head = AddCollider<CColliderSphere>("Head");
	Head->SetRadius(20.f);
	Head->SetOffset(0.f, -60.f);
	Head->SetCollisionProfile("Player");

	CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(80.f, 45.f);
	Body->SetOffset(0.f, -22.5f);
	Body->SetCollisionProfile("Player");

	// Widget ---
	// HPBar
	m_HPBarWidget = CreateWidgetComponent(HPWIDGET_COMPONENET);
	CProgressBar* HPBar = m_HPBarWidget->CreateWidget<CProgressBar>("HPBar");
	HPBar->SetTexture("WorldHPBar", TEXT("CharacterHPBar.bmp"));
	m_HPBarWidget->SetPos(-25.f, -105.f);

	// MPBar
	m_MPBarWidget = CreateWidgetComponent(MPWIDGET_COMPONENET);
	CProgressBar* MPBar = m_MPBarWidget->CreateWidget<CProgressBar>("MPBar");
	MPBar->SetTexture("WorldMPBar", TEXT("CharacterMPBar.bmp"));
	m_MPBarWidget->SetPos(-25.f, -95.f);

    return true;
}

void CPlayerClone::Update(float DeltaTime)
{
    CCharacter::Update(DeltaTime);
	// Wall
	PreventWallMove();
	// Death
	if (m_CharacterInfo.HP <= 0)
	{
		ChangeDeathAnimation();
		return;
	}
	// Widgets
	CProgressBar* MPBar = (CProgressBar*)m_MPBarWidget->GetWidget();
	MPBar->SetPercent(m_CharacterInfo.MP / (float)m_CharacterInfo.MPMax);

	CProgressBar* HPBar = (CProgressBar*)m_HPBarWidget->GetWidget();
	HPBar->SetPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);

	// Move
	float DirLength = m_Dir.Length();
	if (DirLength != 0.f)
		m_Dir.Normalize();
	Move(m_Dir);

	m_ClosestMonster = m_Scene->FindClosestMonster(m_Pos);
	if (m_ClosestMonster)
	{
		Vector2 MonsterPos = m_ClosestMonster->GetPos();
		float DistToPlayer = Distance(m_Pos, MonsterPos);
		if (DistToPlayer   <= m_DashDistance)
		{
			if (DistToPlayer < m_AttackDistance)
				m_AI = EMonsterAI::Attack;
			else
				m_AI = EMonsterAI::Trace;
		}
		else
		{
			SetPlayerTargetDir();
			if (m_Dir.x == 0.f && m_Dir.y == 0.f)
				m_AI = EMonsterAI::Idle;
			else
				m_AI = EMonsterAI::Walk;
		}
	}
	else
	{
		SetPlayerTargetDir();
		if (m_Dir.x == 0.f && m_Dir.y == 0.f)
			m_AI = EMonsterAI::Idle;
		else
			m_AI = EMonsterAI::Walk;
	}

	if (m_CharacterInfo.HP <= 0)
	{
		m_AI = EMonsterAI::Death;
	}


	switch (m_AI)
	{
	case EMonsterAI::Idle:
	{
		AIIdle(DeltaTime);
	}
	break;
	case EMonsterAI::Walk:
	{
		AIWalk(DeltaTime);
	}
	break;
	case EMonsterAI::Trace:
	{
		if (m_ClosestMonster)
		{
			Vector2 MonsterPos = m_ClosestMonster->GetPos();
			AITrace(DeltaTime, MonsterPos);
		}
	}
	break;
	case EMonsterAI::Attack:
	{
		if (m_ClosestMonster)
		{
			Vector2 MonsterPos = m_ClosestMonster->GetPos();
			AIAttack(DeltaTime, MonsterPos);
		}
	}
	break;
	case EMonsterAI::Hit:
	{
		AIHit(DeltaTime);
	}
	break;
	case EMonsterAI::Death:
	{
		// Death Animation
		AIDeath(DeltaTime);
	}
	break;
	}
}

void CPlayerClone::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);
}

void CPlayerClone::Collision(float DeltaTime)
{
	CCharacter::Collision(DeltaTime);
}

void CPlayerClone::Render(HDC hDC)
{
	CCharacter::Render(hDC);
}

float CPlayerClone::SetDamage(float Damage)
{
	Damage = CCharacter::SetDamage(Damage);
	CProgressBar* HPBar = (CProgressBar*)m_HPBarWidget->GetWidget();
	HPBar->SetPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);
	return Damage;
}

void CPlayerClone::CharacterDestroy()
{
	CCharacter::CharacterDestroy();
}

void CPlayerClone::Fire()
{
	if (m_ClosestMonster)
	{
		Vector2 Pos = m_ClosestMonster->GetPos();
		m_CurrentGun->SetReloadBullet();
		m_CurrentGun->PlayerFire(Pos, (float)m_CharacterInfo.Attack);
	}
}

void CPlayerClone::AttackEnd()
{
	if (m_Dir.x < 0)
	{
		std::string Anim = m_mapAnimName.find(PLAYER_RIGHT_IDLE)->second;
		ChangeAnimation(Anim);
	}
	else
	{
		std::string Anim = m_mapAnimName.find(PLAYER_LEFT_IDLE)->second;
		ChangeAnimation(Anim);
	}
}

void CPlayerClone::Move(const Vector2& Dir)
{
	CCharacter::Move(Dir);
}

void CPlayerClone::Move(const Vector2& Dir, float Speed)
{
	CCharacter::Move(Dir, Speed);
}

void CPlayerClone::ChangeIdleAnimation()
{
	CCharacter::ChangeIdleAnimation();
	if (m_Dir.x < 0)
	{
		std::string Anim = m_mapAnimName.find(PLAYER_LEFT_IDLE)->second;
		ChangeAnimation(Anim);
	}
	else
	{
		std::string Anim = m_mapAnimName.find(PLAYER_RIGHT_IDLE)->second;
		ChangeAnimation(Anim);
	}
}

void CPlayerClone::ChangeMoveAnimation()
{
	if (m_HitEnable) return;
	CCharacter::ChangeMoveAnimation();
	if (m_Dir.x < 0)
	{
		std::string Anim = m_mapAnimName.find(PLAYER_LEFT_WALK)->second;
		ChangeAnimation(Anim);
	}
	else
	{
		std::string Anim = m_mapAnimName.find(PLAYER_RIGHT_WALK)->second;
		ChangeAnimation(Anim);
	}
}

void CPlayerClone::ChangeRunAnimation()
{
	CCharacter::ChangeRunAnimation();
	if (m_Dir.x < 0)
	{
		std::string Anim = m_mapAnimName.find(PLAYER_LEFT_RUN)->second;
		ChangeAnimation(Anim);
	}
	else
	{
		std::string Anim = m_mapAnimName.find(PLAYER_RIGHT_RUN)->second;
		ChangeAnimation(Anim);
	}
}

void CPlayerClone::ChangeDeathAnimation()
{
	if (m_Dir.x < 0)
	{
		std::string Anim = m_mapAnimName.find(PLAYER_LEFT_DEATH)->second;
		ChangeAnimation(Anim);
	}
	else
	{
		std::string Anim = m_mapAnimName.find(PLAYER_RIGHT_DEATH)->second;
		ChangeAnimation(Anim);
	}
}

void CPlayerClone::ChangeHitAnimation()
{
	if (m_Dir.x < 0)
	{
		std::string Anim = m_mapAnimName.find(PLAYER_LEFT_HIT)->second;
		ChangeAnimation(Anim);
	}
	else
	{
		std::string Anim = m_mapAnimName.find(PLAYER_RIGHT_HIT)->second;
		ChangeAnimation(Anim);
	}
}


void CPlayerClone::SetPlayerTargetDir()
{
	CPlayer* Player = (CPlayer*)m_Scene->GetPlayer();
	float Angle = GetAngle(m_Pos, Player->GetPos());
	SetDir(Angle);
}

void CPlayerClone::AIIdle(float DeltaTime)
{
	m_MoveSpeed = m_CharacterInfo.MoveSpeed;
	ChangeIdleAnimation();
}

void CPlayerClone::AIWalk(float DeltaTime)
{
	m_MoveSpeed = m_CharacterInfo.MoveSpeed;
	ChangeMoveAnimation();
}

void CPlayerClone::AITrace(float DeltaTime, Vector2 MonsterPos)
{
	m_MoveSpeed = m_CharacterInfo.MoveSpeed;
	ChangeRunAnimation();
	float Angle = GetAngle(m_Pos, MonsterPos);
	SetDir(Angle);
}

void CPlayerClone::AIAttack(float DeltaTime, Vector2 MonsterPos)
{
	EObject_Type ObjType = m_ClosestMonster->GetObjType();
	if (ObjType == EObject_Type::Player) return;
	float Angle = GetAngle(m_Pos, MonsterPos);
	SetDir(Angle);
	m_MoveSpeed = 0.f;
	ChangeIdleAnimation();
	m_FireTime += DeltaTime;
	if (m_FireTime >= m_FireTimeMax)
	{
		m_FireTime -= m_FireTimeMax;
		Fire();
		AttackEnd();
	}
}

void CPlayerClone::AIDeath(float DeltaTime)
{
	m_MoveSpeed = 0.f;
	m_ColliderList.clear();
	ChangeDeathAnimation();
}

void CPlayerClone::AIHit(float DeltaTime)
{
}

CGun* CPlayerClone::Equip(CGun* Gun)
{
	CGun* ExitingGun = CCharacter::Equip(Gun);
	CCollider* GunBody = m_CurrentGun->FindCollider("Body");
	if(GunBody)
		GunBody->SetCollisionProfile("PlayerAttack");
	return ExitingGun;
}


void CPlayerClone::SetAnimName()
{
	switch (m_CharType)
	{
	case EChar_Type::Ass:
		SetAssAnimName();
		break;
	case EChar_Type::Biu:
		SetBiuAnimName();
		break;
	case EChar_Type::Jimmy:
		SetJimmyAnimName();
		break;
	case EChar_Type::Pinky:
		SetPinkyAnimName();
		break;
	case EChar_Type::Punny:
		SetPunnyAnimName();
		break;
	case EChar_Type::Raff:
		SetRaffAnimName();
		break;
	}
	// Set Notify Functions
	SetNotifyFunctions();
	// Set Init Animation
	std::string Anim = m_mapAnimName.find(PLAYER_RIGHT_IDLE)->second;
	ChangeAnimation(Anim);
}

void CPlayerClone::SetAssAnimName()
{
	m_mapAnimName.clear();

	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_IDLE, ASS_PLAYER_RIGHT_IDLE));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_WALK, ASS_PLAYER_RIGHT_WALK));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_ATTACK, ASS_PLAYER_RIGHT_ATTACK));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_RUN, ASS_PLAYER_RIGHT_RUN));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_DEATH, ASS_PLAYER_RIGHT_DEATH));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_DASH, ASS_PLAYER_RIGHT_DASH));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_HIT, ASS_PLAYER_RIGHT_HIT));

	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_IDLE, ASS_PLAYER_LEFT_IDLE));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_WALK, ASS_PLAYER_LEFT_WALK));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_ATTACK, ASS_PLAYER_LEFT_ATTACK));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_RUN, ASS_PLAYER_LEFT_RUN));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_DEATH, ASS_PLAYER_LEFT_DEATH));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_DASH, ASS_PLAYER_LEFT_DASH));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_HIT, ASS_PLAYER_LEFT_HIT));

	m_mapAnimName.insert(std::make_pair(PLAYER_TELEPORT, ASS_PLAYER_TELEPORT));
}

void CPlayerClone::SetJimmyAnimName()
{
	m_mapAnimName.clear();

	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_IDLE, JIMMY_PLAYER_RIGHT_IDLE));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_WALK, JIMMY_PLAYER_RIGHT_WALK));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_ATTACK, JIMMY_PLAYER_RIGHT_ATTACK));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_RUN, JIMMY_PLAYER_RIGHT_RUN));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_DEATH, JIMMY_PLAYER_RIGHT_DEATH));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_DASH, JIMMY_PLAYER_RIGHT_DASH));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_HIT, JIMMY_PLAYER_RIGHT_HIT));

	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_IDLE, JIMMY_PLAYER_LEFT_IDLE));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_WALK, JIMMY_PLAYER_LEFT_WALK));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_ATTACK, JIMMY_PLAYER_LEFT_ATTACK));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_RUN, JIMMY_PLAYER_LEFT_RUN));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_DEATH, JIMMY_PLAYER_LEFT_DEATH));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_DASH, JIMMY_PLAYER_LEFT_DASH));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_HIT, JIMMY_PLAYER_LEFT_HIT));

	m_mapAnimName.insert(std::make_pair(PLAYER_TELEPORT, JIMMY_PLAYER_TELEPORT));
}

void CPlayerClone::SetBiuAnimName()
{
	m_mapAnimName.clear();

	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_IDLE, BIU_PLAYER_RIGHT_IDLE));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_WALK, BIU_PLAYER_RIGHT_WALK));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_ATTACK, BIU_PLAYER_RIGHT_ATTACK));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_RUN, BIU_PLAYER_RIGHT_RUN));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_DEATH, BIU_PLAYER_RIGHT_DEATH));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_DASH, BIU_PLAYER_RIGHT_DASH));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_HIT, BIU_PLAYER_RIGHT_HIT));

	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_IDLE, BIU_PLAYER_LEFT_IDLE));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_WALK, BIU_PLAYER_LEFT_WALK));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_ATTACK, BIU_PLAYER_LEFT_ATTACK));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_RUN, BIU_PLAYER_LEFT_RUN));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_DEATH, BIU_PLAYER_LEFT_DEATH));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_DASH, BIU_PLAYER_LEFT_DASH));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_HIT, BIU_PLAYER_LEFT_HIT));

	m_mapAnimName.insert(std::make_pair(PLAYER_TELEPORT, BIU_PLAYER_TELEPORT));
}

void CPlayerClone::SetPinkyAnimName()
{
	m_mapAnimName.clear();

	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_IDLE, PINKY_PLAYER_RIGHT_IDLE));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_WALK, PINKY_PLAYER_RIGHT_WALK));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_ATTACK, PINKY_PLAYER_RIGHT_ATTACK));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_RUN, PINKY_PLAYER_RIGHT_RUN));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_DEATH, PINKY_PLAYER_RIGHT_DEATH));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_DASH, PINKY_PLAYER_RIGHT_DASH));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_HIT, PINKY_PLAYER_RIGHT_HIT));

	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_IDLE, PINKY_PLAYER_LEFT_IDLE));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_WALK, PINKY_PLAYER_LEFT_WALK));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_ATTACK, PINKY_PLAYER_LEFT_ATTACK));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_RUN, PINKY_PLAYER_LEFT_RUN));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_DEATH, PINKY_PLAYER_LEFT_DEATH));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_DASH, PINKY_PLAYER_LEFT_DASH));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_HIT, PINKY_PLAYER_LEFT_HIT));

	m_mapAnimName.insert(std::make_pair(PLAYER_TELEPORT, PINKY_PLAYER_TELEPORT));
}

void CPlayerClone::SetPunnyAnimName()
{
	m_mapAnimName.clear();

	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_IDLE, PUNNY_PLAYER_RIGHT_IDLE));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_WALK, PUNNY_PLAYER_RIGHT_WALK));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_ATTACK, PUNNY_PLAYER_RIGHT_ATTACK));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_RUN, PUNNY_PLAYER_RIGHT_RUN));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_DEATH, PUNNY_PLAYER_RIGHT_DEATH));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_DASH, PUNNY_PLAYER_RIGHT_DASH));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_HIT, PUNNY_PLAYER_RIGHT_HIT));

	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_IDLE, PUNNY_PLAYER_LEFT_IDLE));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_WALK, PUNNY_PLAYER_LEFT_WALK));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_ATTACK, PUNNY_PLAYER_LEFT_ATTACK));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_RUN, PUNNY_PLAYER_LEFT_RUN));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_DEATH, PUNNY_PLAYER_LEFT_DEATH));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_DASH, PUNNY_PLAYER_LEFT_DASH));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_HIT, PUNNY_PLAYER_LEFT_HIT));

	m_mapAnimName.insert(std::make_pair(PLAYER_TELEPORT, PUNNY_PLAYER_TELEPORT));
}

void CPlayerClone::SetRaffAnimName()
{
	m_mapAnimName.clear();

	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_IDLE, RAFF_PLAYER_RIGHT_IDLE));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_WALK, RAFF_PLAYER_RIGHT_WALK));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_ATTACK, RAFF_PLAYER_RIGHT_ATTACK));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_RUN, RAFF_PLAYER_RIGHT_RUN));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_DEATH, RAFF_PLAYER_RIGHT_DEATH));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_DASH, RAFF_PLAYER_RIGHT_DASH));
	m_mapAnimName.insert(std::make_pair(PLAYER_RIGHT_HIT, RAFF_PLAYER_RIGHT_HIT));

	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_IDLE, RAFF_PLAYER_LEFT_IDLE));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_WALK, RAFF_PLAYER_LEFT_WALK));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_ATTACK, RAFF_PLAYER_LEFT_ATTACK));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_RUN, RAFF_PLAYER_LEFT_RUN));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_DEATH, RAFF_PLAYER_LEFT_DEATH));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_DASH, RAFF_PLAYER_LEFT_DASH));
	m_mapAnimName.insert(std::make_pair(PLAYER_LEFT_HIT, RAFF_PLAYER_LEFT_HIT));

	m_mapAnimName.insert(std::make_pair(PLAYER_TELEPORT, RAFF_PLAYER_TELEPORT));
}

void CPlayerClone::AddAssAnimName()
{
	AddAnimation(ASS_PLAYER_RIGHT_IDLE, true, 2.f);
	AddAnimation(ASS_PLAYER_RIGHT_WALK, true, 1.f);
	AddAnimation(ASS_PLAYER_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(ASS_PLAYER_RIGHT_RUN, true, 0.6f);
	AddAnimation(ASS_PLAYER_RIGHT_DEATH, false, 0.1f);
	AddAnimation(ASS_PLAYER_RIGHT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(ASS_PLAYER_RIGHT_HIT, true, 0.6f);

	AddAnimation(ASS_PLAYER_LEFT_IDLE, true, 2.f);
	AddAnimation(ASS_PLAYER_LEFT_WALK, true, 1.f);
	AddAnimation(ASS_PLAYER_LEFT_ATTACK, false, 0.1f);
	AddAnimation(ASS_PLAYER_LEFT_RUN, true, 0.6f);
	AddAnimation(ASS_PLAYER_LEFT_DEATH, false, 0.1f);
	AddAnimation(ASS_PLAYER_LEFT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(ASS_PLAYER_LEFT_HIT, true, 0.6f);

	AddAnimation(ASS_PLAYER_TELEPORT, false, 0.3f);
}

void CPlayerClone::AddJimmyAnimName()
{
	AddAnimation(JIMMY_PLAYER_RIGHT_IDLE, true, 2.f);
	AddAnimation(JIMMY_PLAYER_RIGHT_WALK, true, 1.f);
	AddAnimation(JIMMY_PLAYER_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(JIMMY_PLAYER_RIGHT_RUN, true, 0.6f);
	AddAnimation(JIMMY_PLAYER_RIGHT_DEATH, false, 0.1f);
	AddAnimation(JIMMY_PLAYER_RIGHT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(JIMMY_PLAYER_RIGHT_HIT, true, 0.6f);

	AddAnimation(JIMMY_PLAYER_LEFT_IDLE, true, 2.f);
	AddAnimation(JIMMY_PLAYER_LEFT_WALK, true, 1.f);
	AddAnimation(JIMMY_PLAYER_LEFT_ATTACK, false, 0.1f);
	AddAnimation(JIMMY_PLAYER_LEFT_RUN, true, 0.6f);
	AddAnimation(JIMMY_PLAYER_LEFT_DEATH, false, 0.1f);
	AddAnimation(JIMMY_PLAYER_LEFT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(JIMMY_PLAYER_LEFT_HIT, true, 0.6f);

	AddAnimation(JIMMY_PLAYER_TELEPORT, false, 0.3f);
}

void CPlayerClone::AddBiuAnimName()
{
	AddAnimation(BIU_PLAYER_RIGHT_IDLE, true, 2.f);
	AddAnimation(BIU_PLAYER_RIGHT_WALK, true, 1.f);
	AddAnimation(BIU_PLAYER_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(BIU_PLAYER_RIGHT_RUN, true, 0.6f);
	AddAnimation(BIU_PLAYER_RIGHT_DEATH, false, 0.1f);
	AddAnimation(BIU_PLAYER_RIGHT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(BIU_PLAYER_RIGHT_HIT, true, 0.6f);

	AddAnimation(BIU_PLAYER_LEFT_IDLE, true, 2.f);
	AddAnimation(BIU_PLAYER_LEFT_WALK, true, 1.f);
	AddAnimation(BIU_PLAYER_LEFT_ATTACK, false, 0.1f);
	AddAnimation(BIU_PLAYER_LEFT_RUN, true, 0.6f);
	AddAnimation(BIU_PLAYER_LEFT_DEATH, false, 0.1f);
	AddAnimation(BIU_PLAYER_LEFT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(BIU_PLAYER_LEFT_HIT, true, 0.6f);

	AddAnimation(BIU_PLAYER_TELEPORT, false, 0.3f);
}

void CPlayerClone::AddPinkyAnimName()
{
	AddAnimation(PINKY_PLAYER_RIGHT_IDLE, true, 2.f);
	AddAnimation(PINKY_PLAYER_RIGHT_WALK, true, 1.f);
	AddAnimation(PINKY_PLAYER_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(PINKY_PLAYER_RIGHT_RUN, true, 0.6f);
	AddAnimation(PINKY_PLAYER_RIGHT_DEATH, false, 0.1f);
	AddAnimation(PINKY_PLAYER_RIGHT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(PINKY_PLAYER_RIGHT_HIT, true, 0.6f);

	AddAnimation(PINKY_PLAYER_LEFT_IDLE, true, 2.f);
	AddAnimation(PINKY_PLAYER_LEFT_WALK, true, 1.f);
	AddAnimation(PINKY_PLAYER_LEFT_ATTACK, false, 0.1f);
	AddAnimation(PINKY_PLAYER_LEFT_RUN, true, 0.6f);
	AddAnimation(PINKY_PLAYER_LEFT_DEATH, false, 0.1f);
	AddAnimation(PINKY_PLAYER_LEFT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(PINKY_PLAYER_LEFT_HIT, true, 0.6f);

	AddAnimation(PINKY_PLAYER_TELEPORT, false, 0.3f);
}

void CPlayerClone::AddPunnyAnimName()
{
	AddAnimation(PUNNY_PLAYER_RIGHT_IDLE, true, 2.f);
	AddAnimation(PUNNY_PLAYER_RIGHT_WALK, true, 1.f);
	AddAnimation(PUNNY_PLAYER_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(PUNNY_PLAYER_RIGHT_RUN, true, 0.6f);
	AddAnimation(PUNNY_PLAYER_RIGHT_DEATH, false, 0.1f);
	AddAnimation(PUNNY_PLAYER_RIGHT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(PUNNY_PLAYER_RIGHT_HIT, true, 0.6f);

	AddAnimation(PUNNY_PLAYER_LEFT_IDLE, true, 2.f);
	AddAnimation(PUNNY_PLAYER_LEFT_WALK, true, 1.f);
	AddAnimation(PUNNY_PLAYER_LEFT_ATTACK, false, 0.1f);
	AddAnimation(PUNNY_PLAYER_LEFT_RUN, true, 0.6f);
	AddAnimation(PUNNY_PLAYER_LEFT_DEATH, false, 0.1f);
	AddAnimation(PUNNY_PLAYER_LEFT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(PUNNY_PLAYER_LEFT_HIT, true, 0.6f);

	AddAnimation(PUNNY_PLAYER_TELEPORT, false, 0.3f);
}

void CPlayerClone::AddRaffAnimName()
{
	AddAnimation(RAFF_PLAYER_RIGHT_IDLE, true, 2.f);
	AddAnimation(RAFF_PLAYER_RIGHT_WALK, true, 1.f);
	AddAnimation(RAFF_PLAYER_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(RAFF_PLAYER_RIGHT_RUN, true, 0.6f);
	AddAnimation(RAFF_PLAYER_RIGHT_DEATH, false, 0.1f);
	AddAnimation(RAFF_PLAYER_RIGHT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(RAFF_PLAYER_RIGHT_HIT, true, 0.6f);

	AddAnimation(RAFF_PLAYER_LEFT_IDLE, true, 2.f);
	AddAnimation(RAFF_PLAYER_LEFT_WALK, true, 1.f);
	AddAnimation(RAFF_PLAYER_LEFT_ATTACK, false, 0.1f);
	AddAnimation(RAFF_PLAYER_LEFT_RUN, true, 0.6f);
	AddAnimation(RAFF_PLAYER_LEFT_DEATH, false, 0.1f);
	AddAnimation(RAFF_PLAYER_LEFT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(RAFF_PLAYER_LEFT_HIT, true, 0.6f);

	AddAnimation(RAFF_PLAYER_TELEPORT, false, 0.3f);
}

void CPlayerClone::SetNotifyFunctions()
{
	// Notify Functions
// Attack
	std::string AnimName = m_mapAnimName.find(PLAYER_RIGHT_ATTACK)->second;
	AddAnimationNotify<CPlayerClone>(AnimName, 2, this, &CPlayerClone::Fire);
	AnimName = m_mapAnimName.find(PLAYER_LEFT_ATTACK)->second;
	AddAnimationNotify<CPlayerClone>(AnimName, 2, this, &CPlayerClone::Fire);
	AnimName = m_mapAnimName.find(PLAYER_RIGHT_ATTACK)->second;
	SetAnimationEndNotify<CPlayerClone>(AnimName, this, &CPlayerClone::AttackEnd);
	AnimName = m_mapAnimName.find(PLAYER_LEFT_ATTACK)->second;
	SetAnimationEndNotify<CPlayerClone>(AnimName, this, &CPlayerClone::AttackEnd);

	// Death
	AnimName = m_mapAnimName.find(PLAYER_LEFT_DEATH)->second;
	AddAnimationNotify<CPlayerClone>(AnimName, 11, this, &CPlayerClone::Destroy);
	SetAnimationEndNotify<CPlayerClone>(AnimName, this, &CPlayerClone::Destroy);
	AnimName = m_mapAnimName.find(PLAYER_RIGHT_DEATH)->second;
	AddAnimationNotify<CPlayerClone>(AnimName, 11, this, &CPlayerClone::Destroy);
	SetAnimationEndNotify<CPlayerClone>(AnimName, this, &CPlayerClone::Destroy);
}

void CPlayerClone::SetCharName()
{
	m_NameWidget = CreateWidgetComponent(NAMEWIDGET_COMPONENET);
	CUIText* NameText = m_NameWidget->CreateWidget<CUIText>("NameText");
	switch (m_CharType)
	{
	case EChar_Type::Ass:
		NameText->SetText(TEXT("Ass"));
		break;
	case EChar_Type::Biu:
		NameText->SetText(TEXT("Biu"));
		break;
	case EChar_Type::Jimmy:
		NameText->SetText(TEXT("Jimmy"));
		break;
	case EChar_Type::Pinky:
		NameText->SetText(TEXT("Pinky"));
		break;
	case EChar_Type::Punny:
		NameText->SetText(TEXT("Punny"));
		break;
	case EChar_Type::Raff:
		NameText->SetText(TEXT("Raff"));
		break;
	}

	NameText->SetTextColor(255, 0, 0);
	m_NameWidget->SetPos(-25.f, -140.f);
}