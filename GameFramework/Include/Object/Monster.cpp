
#include "Monster.h"
#include "Bullet.h"
#include "EffectSurprise.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"
#include "../Scene/SceneResource.h"
#include "../Collision/ColliderBox.h"
#include "../UI/ProgressBar.h"
#include "Player.h"

CMonster::CMonster() : m_FireTime(0.f),
					   m_FireTimeMax(1.f),
					   m_Count(0),
					   m_RandomMoveTime(MONSTER_TARGET_POS_LIMIT_TIME),
					   m_mapAnimName{},
					   m_AI(EMonsterAI::Idle),
					   m_DashDistance(NORMAL_MONSTER_DASH_DISTANCE),
					   m_AttackDistance(NORMAL_MONSTER_ATTACK_DISTANCE),
					   m_TraceSurprise(false),
						m_SurpriseStopTime(0.0f)
{
	m_Dir.x           = (float)(rand() % 2);
	m_Dir.y           = (float)(rand() % 2);
	m_ObjType         = EObject_Type::Monster;
	m_MonsterType     = EMonster_Type::Duck1;
}

CMonster::CMonster(const CMonster &obj) : CCharacter(obj)
{
	m_ObjType = EObject_Type::Monster;
	m_Dir = obj.m_Dir;
	m_DashDistance = obj.m_DashDistance;
	m_AttackDistance = obj.m_AttackDistance;
	m_FireTime = obj.m_FireTime;
	m_FireTimeMax = obj.m_FireTimeMax;
	m_Count = obj.m_Count;
	m_RandomMoveTime = MONSTER_TARGET_POS_LIMIT_TIME;
	m_MonsterType = obj.m_MonsterType;
	m_TraceSurprise = false;

	auto iter = m_WidgetComponentList.begin();
	auto iterEnd = m_WidgetComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == HPWIDGET_COMPONENET)
			m_HPBarWidget = (*iter);
		if ((*iter)->GetName() == MPWIDGET_COMPONENET)
			m_MPBarWidget = (*iter);
	}

	// m_mapAnimNames
	m_mapAnimName.clear();
	{
		auto iter    = obj.m_mapAnimName.begin();
		auto iterEnd = obj.m_mapAnimName.end();
		for (; iter != iterEnd; ++iter)
		{
			m_mapAnimName[iter->first] = iter->second;
		}
	}
}

CMonster::~CMonster()
{
	m_HPBarWidget = nullptr;
	m_MPBarWidget = nullptr;
	m_mapAnimName.clear();
}

void CMonster::Start()
{
	CCharacter::Start();
	
	SetAnimationEndNotify<CMonster>(MONSTER_DUCK1_LEFT_DEATH, this, &CMonster::CharacterDestroy);
	SetAnimationEndNotify<CMonster>(MONSTER_DUCK1_RIGHT_DEATH, this, &CMonster::CharacterDestroy);
}

bool CMonster::Init()
{
	if (!CCharacter::Init())
		return false;

	SetPivot(0.5f, 1.f);
	CreateAnimation();
	SetAnimation();
	SetAnimNames();
	
	CColliderBox *Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(82.f, 73.f);
	Body->SetOffset(0.f, -36.5f);
	Body->SetCollisionProfile("Monster");

	// HPBar
	m_HPBarWidget = CreateWidgetComponent("HPBarWidget");
	CProgressBar *HPBar = m_HPBarWidget->CreateWidget<CProgressBar>("HPBar");
	HPBar->SetTexture("WorldHPBar", TEXT("CharacterHPBar.bmp"));
	m_HPBarWidget->SetPos(-25.f, -95.f);

	// MPBar
	m_MPBarWidget = CreateWidgetComponent("MPBarWidget");
	CProgressBar *MPBar = m_MPBarWidget->CreateWidget<CProgressBar>("MPBar");
	MPBar->SetTexture("WorldMPBar", TEXT("CharacterMPBar.bmp"));
	m_MPBarWidget->SetPos(-25.f, -85.f);

	return true;
}

void CMonster::Update(float DeltaTime)
{
	CCharacter::Update(DeltaTime);

	// Monster 이동 
	m_Pos += m_Dir * m_MoveSpeed * DeltaTime;

	CGameObject *Player = m_Scene->GetPlayer();
	Vector2 PlayerPos   = Player->GetPos();
	float DistToPlayer  = Distance(m_Pos,PlayerPos);

	if (DistToPlayer <= m_DashDistance)
	{
		// 느낌표 
		if (!m_TraceSurprise)
		{
			Vector2 LT = m_Pos - m_Pivot * m_Size + m_Offset;
			Vector2 RT = Vector2(LT.x + m_Size.x * 0.8, LT.y + m_Size.y * 0.4);
			CEffectSurprise* Surprise = m_Scene->CreateObject<CEffectSurprise>(SURPRISE_EFFECT, EFFECT_SURPRISE_PROTO,
				RT, Vector2(10.f, 10.f));

			// 나중에 sound 바꾸기 
			m_Scene->GetSceneResource()->SoundPlay("Fire");

			m_TraceSurprise = true;
		}

		if (DistToPlayer < m_AttackDistance)
			m_AI = EMonsterAI::Attack;
		// 추적 
		else
			m_AI = EMonsterAI::Trace;
	}
	else
	{
		m_TraceSurprise = false;
		m_RandomMoveTime -= DeltaTime;
		if (m_RandomMoveTime <= 0.f)
		{
			SetRandomTargetDir();
			m_RandomMoveTime = MONSTER_TARGET_POS_LIMIT_TIME;
		}
		if (m_Velocity.Length() == 0.f)
			m_AI = EMonsterAI::Idle;
		else
			m_AI = EMonsterAI::Walk;
	}
	if (m_CharacterInfo.HP <= 0)
	{
		m_AI = EMonsterAI::Death;
	}
	if (m_HitEnable)
	{
		m_AI = EMonsterAI::Hit;
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
		AITrace(DeltaTime, PlayerPos);
	}
		break;
	case EMonsterAI::Attack:
	{
		AIAttack(DeltaTime, PlayerPos);
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

void CMonster::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);
}

void CMonster::Collision(float DeltaTime)
{
	CCharacter::Collision(DeltaTime);
}

void CMonster::Render(HDC hDC)
{
	CCharacter::Render(hDC);
}

CMonster *CMonster::Clone()
{
	return new CMonster(*this);
}

float CMonster::SetDamage(float Damage)
{
	Damage = CCharacter::SetDamage(Damage);
	CProgressBar *HPBar = (CProgressBar *)m_HPBarWidget->GetWidget();
	HPBar->SetPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);
	return Damage;
}

void CMonster::AttackEnd()
{
	// m_MoveSpeed = NORMAL_MONSTER_MOVE_SPEED;
}

void CMonster::Fire()
{
	CGameObject* Player = m_Scene->GetPlayer();
	if (m_CurrentGun)
	{
		// Fire
		m_CurrentGun->MonsterFire(Player->GetPos(),m_CharacterInfo.Attack);
	}
}

void CMonster::Move(const Vector2& Dir)
{
	if (ObstacleCollisionCheck())
	{
	}
	CCharacter::Move(Dir);
}

void CMonster::Move(const Vector2& Dir, float Speed)
{
	if (ObstacleCollisionCheck())
	{
	}
	CCharacter::Move(Dir, Speed);
}

void CMonster::ChangeIdleAnimation()
{
	CCharacter::ChangeIdleAnimation();
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

void CMonster::ChangeWalkAnimation()
{
	CCharacter::ChangeMoveAnimation();
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

void CMonster::ChangeTraceAnimation()
{
	CCharacter::ChangeRunAnimation();
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

void CMonster::ChangeDeathAnimation()
{
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

void CMonster::SetRandomTargetDir()
{
	m_MoveTargetPos = SetRandomTargetPos();
	float Angle = GetAngle(m_Pos, m_MoveTargetPos);
	SetDir(Angle);
}

Vector2 CMonster::SetRandomTargetPos()
{
	Vector2 WorldResolution = m_Scene->GetCamera()->GetWorldResolution();
	float x = (float)(rand() % (int)WorldResolution.x);
	float y = (float)(rand() % (int)WorldResolution.y);
	m_MoveTargetPos = Vector2(x, y);
	return m_MoveTargetPos;
}

void CMonster::SetAnimation()
{
	switch (m_MonsterType)
	{
	case EMonster_Type::Duck1:
		SetDuck1Animation();
		break;
	case EMonster_Type::Duck2:
		AddAnimation(PLAYER_LEFT_IDLE);
		break;
	case EMonster_Type::Duck3:
		AddAnimation(PLAYER_LEFT_IDLE);
		break;
	case EMonster_Type::Turtle1:
		AddAnimation(PLAYER_LEFT_IDLE);
		break;
	case EMonster_Type::Turtle2:
		AddAnimation(PLAYER_LEFT_IDLE);
		break;
	case EMonster_Type::Turtle3:
		AddAnimation(PLAYER_LEFT_IDLE);
		break;
	default:
		break;
	}
}

void CMonster::SetDuck1Animation()
{
	// Right
	AddAnimation(MONSTER_DUCK1_RIGHT_IDLE, true, 2.f);
	AddAnimation(MONSTER_DUCK1_RIGHT_WALK, true, 1.f);
	AddAnimation(MONSTER_DUCK1_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_DUCK1_RIGHT_RUN, true, 0.6f);
	AddAnimation(MONSTER_DUCK1_RIGHT_HIT, false, 1.f);
	AddAnimation(MONSTER_DUCK1_RIGHT_DEATH, false, 0.5f);
	

	// Left
	AddAnimation(MONSTER_DUCK1_LEFT_IDLE, true,2.f);
	AddAnimation(MONSTER_DUCK1_LEFT_WALK, true, 1.f);
	AddAnimation(MONSTER_DUCK1_LEFT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_DUCK1_LEFT_RUN, true, 0.6f);
	AddAnimation(MONSTER_DUCK1_LEFT_HIT, false, 1.f);
	AddAnimation(MONSTER_DUCK1_LEFT_DEATH, false, 0.5f);
	
}

void CMonster::SetAnimNames()
{
	switch (m_MonsterType)
	{
	case EMonster_Type::Duck1:
		SetDuck1AnimName();
		break;
	case EMonster_Type::Duck2:
		SetDuck1AnimName();
		break;
	case EMonster_Type::Duck3:
		SetDuck1AnimName();
		break;
	case EMonster_Type::Turtle1:
		SetDuck1AnimName();
		break;
	case EMonster_Type::Turtle2:
		SetDuck1AnimName();
		break;
	case EMonster_Type::Turtle3:
		SetDuck1AnimName();
		break;
	}
}

void CMonster::SetDuck1AnimName()
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

void CMonster::AIIdle(float DeltaTime)
{
	m_MoveSpeed = NORMAL_MONSTER_MOVE_SPEED;
	m_TraceSurprise = false;
	ChangeIdleAnimation();
}

void CMonster::AIWalk(float DeltaTime)
{
	m_TraceSurprise = false;
	m_MoveSpeed = NORMAL_MONSTER_MOVE_SPEED;
	ChangeWalkAnimation();
}

void CMonster::AITrace(float DeltaTime, Vector2 PlayerPos)
{
	m_MoveSpeed = NORMAL_MONSTER_MOVE_SPEED;
	ChangeTraceAnimation();
	// Player를 쫒아가기
	float Angle = GetAngle(m_Pos, PlayerPos);
	SetDir(Angle);
	m_RandomMoveTime = MONSTER_TARGET_POS_LIMIT_TIME;
}

void CMonster::AIAttack(float DeltaTime, Vector2 PlayerPos)
{
	// 방향 조정  
	float Angle = GetAngle(m_Pos, PlayerPos);
	SetDir(Angle);

	// 멈추기
	m_MoveSpeed = 0.f;

	// Animation 
	ChangeIdleAnimation();

	m_RandomMoveTime = MONSTER_TARGET_POS_LIMIT_TIME;

	m_FireTime += DeltaTime;
	if (m_FireTime >= m_FireTimeMax)
	{
		m_FireTime -= m_FireTimeMax;
		++m_Count;
		Fire();
		AttackEnd();
	}
}

void CMonster::AIDeath(float DeltaTime)
{
	m_MoveSpeed = 0.f;
	ChangeDeathAnimation();
}

void CMonster::AIHit(float DeltaTime)
{
	CCharacter::Hit();
	ChangeHitAnimation();
}

CGun* CMonster::Equip(CGun* Gun)
{
	CGun* EquipedGun = CCharacter::Equip(Gun);
	if (m_CurrentGun)
	{
		CCollider* GunBody = m_CurrentGun->FindCollider("Body");
		GunBody->SetCollisionProfile("MonsterAttack");
	}
	return EquipedGun;
}

void CMonster::ChangeHitAnimation()
{
	CCharacter::ChangeHitAnimation();
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

