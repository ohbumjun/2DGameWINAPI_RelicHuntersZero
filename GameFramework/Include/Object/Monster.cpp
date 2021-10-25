#include "Monster.h"
#include "Bullet.h"
#include "Coin.h"
#include "../GameManager.h"
#include "EffectSurprise.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"
#include "../Scene/SceneResource.h"
#include "../Collision/ColliderBox.h"
#include "../UI/ProgressBar.h"
#include "Player.h"

CMonster::CMonster() : m_Count(0),
					   m_RandomMoveTime(MONSTER_TARGET_POS_LIMIT_TIME),
					   m_AI(EMonsterAI::Idle),
					   m_DashDistance((int)NORMAL_MONSTER_DASH_DISTANCE),
					   m_AttackDistance((int)NORMAL_MONSTER_ATTACK_DISTANCE),
					   m_TraceSurprise(false),
						m_SurpriseStopTime(0.0f),
						m_TypeChanged(false),
						m_MonsterType(EMonster_Type::Duck1)
{
	m_Dir.x           = (float)(rand() % 2);
	m_Dir.y           = (float)(rand() % 2);
	m_ObjType         = EObject_Type::Monster;
}

CMonster::CMonster(const CMonster &obj) : CCharacter(obj)
{
	m_TypeChanged = false;
	m_ObjType = EObject_Type::Monster;
	m_Dir = obj.m_Dir;
	m_DashDistance = obj.m_DashDistance;
	m_AttackDistance = obj.m_AttackDistance;
	m_Count = obj.m_Count;
	m_RandomMoveTime = MONSTER_TARGET_POS_LIMIT_TIME;
	m_MonsterType = obj.m_MonsterType;
	m_TraceSurprise = false;

	//this
	m_AI = EMonsterAI::Idle;
	// Name
	m_mapAnimName.clear();
	{
		auto iter    = obj.m_mapAnimName.begin();
		auto iterEnd = obj.m_mapAnimName.end();
		for (; iter != iterEnd; ++iter)
		{
			m_mapAnimName.insert(std::make_pair(iter->first, iter->second));
		}
	}
	// Widget
	{
		auto iter = m_WidgetComponentList.begin();
		auto iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == HPWIDGET_COMPONENET)
				m_HPBarWidget = (*iter);
			if ((*iter)->GetName() == MPWIDGET_COMPONENET)
				m_MPBarWidget = (*iter);
		}
	}
}

CMonster::~CMonster()
{
}

void CMonster::Start()
{
	CCharacter::Start();
}

bool CMonster::Init()
{
	if (!CCharacter::Init())
		return false;

	SetPivot(0.5f, 1.f);
	CreateAnimation();

	CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(82.f, 73.f);
	Body->SetOffset(0.f, -39.5f);
	Body->SetCollisionProfile("Monster");

	// HPBar
	m_HPBarWidget = CreateWidgetComponent("HPBarWidget");
	CProgressBar* HPBar = m_HPBarWidget->CreateWidget<CProgressBar>("HPBar");
	HPBar->SetTexture("WorldHPBar", TEXT("CharacterHPBar.bmp"));
	m_HPBarWidget->SetPos(-25.f, -95.f);


	// MPBar
	m_MPBarWidget = CreateWidgetComponent("MPBarWidget");
	CProgressBar* MPBar = m_MPBarWidget->CreateWidget<CProgressBar>("MPBar");
	MPBar->SetTexture("WorldMPBar", TEXT("CharacterMPBar.bmp"));
	m_MPBarWidget->SetPos(-25.f, -85.f);

	return true;
}

void CMonster::Update(float DeltaTime)
{
	// this
	CCharacter::Update(DeltaTime);
	// Monster Move 
	float DirLength = m_Dir.Length();

	if(DirLength != 0.f)
		m_Dir.Normalize();

	// m_Dir.Normalize();
	Move(m_Dir);

	// Wall Move
	bool WallCollision = PreventWallMove();
	if (WallCollision) SetRandomTargetDir();

	CGameObject *Player = m_Scene->GetPlayer();
	Vector2 PlayerPos   = Player->GetPos();
	float DistToPlayer  = Distance(m_Pos,PlayerPos);
	if (DistToPlayer <= m_DashDistance)
	{
		// Suprise  
		if (!m_TraceSurprise)
		{
			Vector2 LT = m_Pos - m_Pivot * m_Size + m_Offset;
			Vector2 RT = Vector2(LT.x + m_Size.x * 0.8f, LT.y + m_Size.y * 0.4f);
			CEffectSurprise* Surprise = m_Scene->CreateObject<CEffectSurprise>(SURPRISE_EFFECT, EFFECT_SURPRISE_PROTO,
				RT, Vector2(10.f, 10.f));
			m_Scene->GetSceneResource()->SoundPlay("Fire");
			m_TraceSurprise = true;
		}
		if (DistToPlayer < m_AttackDistance)
			m_AI = EMonsterAI::Attack;
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
		if (m_Dir.x == 0.f && m_Dir.y == 0.f)
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
		CGameObject* Player = m_Scene->GetPlayer();
		Vector2 PlayerPos = Player->GetPos();
		AITrace(DeltaTime, PlayerPos);
	}
		break;
	case EMonsterAI::Attack:
	{
		CGameObject* Player = m_Scene->GetPlayer();
		Vector2 PlayerPos = Player->GetPos();
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

float CMonster::SetDamage(float Damage)
{
	Damage = CCharacter::SetDamage(Damage);
	CProgressBar *HPBar = (CProgressBar *)m_HPBarWidget->GetWidget();
	HPBar->SetPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);
	return Damage;
}

void CMonster::CharacterDestroy()
{
	CCharacter::CharacterDestroy();
	CreateCoin();
}

CCoin* CMonster::CreateCoin()
{
	CCoin* Coin = m_Scene->CreateObject<CCoin>("MonsterCoin", COIN_PROTO, m_Pos);
	Coin->SetInitPos(m_Pos);
	Coin->SetGold(m_CharacterInfo.Gold);
	return Coin;
}

bool CMonster::PlayerCollisionCheck()
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->IsCollisionWithPlayer()) return true;
	}
	return false;
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
		m_CurrentGun->MonsterFire(Player->GetPos(),(float)m_CharacterInfo.Attack);
	}
}

void CMonster::Move(const Vector2& Dir)
{
	CCharacter::Move(Dir);
}

void CMonster::Move(const Vector2& Dir, float Speed)
{
	CCharacter::Move(Dir, Speed);
}

void CMonster::ChangeIdleAnimation()
{
	CCharacter::ChangeIdleAnimation();
}
void CMonster::ChangeMoveAnimation()
{
	CCharacter::ChangeMoveAnimation();
}

void CMonster::ChangeRunAnimation()
{
	CCharacter::ChangeRunAnimation();
}

void CMonster::ChangeDeathAnimation()
{
	CCharacter::ChangeDeathAnimation();
}

void CMonster::SetRandomTargetDir()
{
	m_MoveTargetPos = SetRandomTargetPos();
	float Angle = GetAngle(m_Pos, m_MoveTargetPos);
	SetDir(Angle);
	m_RandomMoveTime = MONSTER_TARGET_POS_LIMIT_TIME;
}

Vector2 CMonster::SetRandomTargetPos()
{
	Vector2 WorldResolution = m_Scene->GetCamera()->GetWorldResolution();
	float x = (float)(rand() % (int)WorldResolution.x);
	float y = (float)(rand() % (int)WorldResolution.y);
	m_MoveTargetPos = Vector2(x, y);
	return m_MoveTargetPos;
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
	ChangeMoveAnimation();
}

void CMonster::AITrace(float DeltaTime, Vector2 PlayerPos)
{
	m_MoveSpeed = NORMAL_MONSTER_MOVE_SPEED;
	ChangeRunAnimation();
	float Angle = GetAngle(m_Pos, PlayerPos);
	SetDir(Angle);
	m_RandomMoveTime = MONSTER_TARGET_POS_LIMIT_TIME;
}

void CMonster::AIAttack(float DeltaTime, Vector2 PlayerPos)
{
	float Angle = GetAngle(m_Pos, PlayerPos);
	SetDir(Angle);

	m_MoveSpeed = 0.f;

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
	m_ColliderList.clear();
	ChangeDeathAnimation();
}

void CMonster::AIHit(float DeltaTime)
{
	// HitMove();
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
	if (m_CharacterInfo.HP <= 0)
	{
		m_MoveSpeed = 0.f;
		m_ColliderList.clear();
	}
}


