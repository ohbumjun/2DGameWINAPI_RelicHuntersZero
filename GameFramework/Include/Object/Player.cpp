
#include "Player.h"
#include "../Scene/Scene.h"
#include "../Input.h"
#include "../GameManager.h"
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderSphere.h"
#include "../UI/UICharacterStateHUD.h"
#include "../UI/ProgressBar.h"
#include "../UI/UIText.h"
#include "EffectHit.h"
#include "TeleportMouse.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Camera.h"
#include "DamageFont.h"
#include "../Timer.h"

CPlayer::CPlayer() : 
	m_SkillSlowMotionAttackEnable(false),
	m_SkillSlowMotionAttackTime(0.f),
	m_RunEnable(false),
	m_DashEnable(false),
	m_DashTime(0.f),
	m_TeleportEnable(false),
	m_TelePortTime(0.f),
	m_TeleportObj{},
	m_TeleportPos(Vector2(0.f,0.f)),
	m_DeathAnimationTime(0.f),
	m_SkillDestoryAllAttackEnable(false),
	m_SkillDestoryAllAttackTime(0.f)
{
	m_ObjType = EObject_Type::Player;
}

CPlayer::CPlayer(const CPlayer &obj) : CCharacter(obj)
{
	m_SkillSlowMotionAttackTime = obj.m_SkillSlowMotionAttackTime;
	m_SkillSlowMotionAttackEnable = false;
	m_TeleportEnable = false;
	m_RunEnable = false;
	m_DashEnable = false;
	m_DashTime = 0.f;
	m_TelePortTime = 0.f;
	m_TeleportObj = obj.m_TeleportObj;
	m_TeleportPos = obj.m_TeleportPos;
	m_DeathAnimationTime = 0.f;
	m_SkillDestoryAllAttackEnable = false;
	m_SkillDestoryAllAttackTime = 0.f;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Start()
{
	CCharacter::Start();

	// Fire, Pause, Resume
	CInput::GetInst()->SetCallback<CPlayer>("Fire", KeyState_Push,
		this, &CPlayer::BulletFire);
	CInput::GetInst()->SetCallback<CPlayer>("Pause", KeyState_Down,
		this, &CPlayer::Pause);
	CInput::GetInst()->SetCallback<CPlayer>("Resume", KeyState_Down,
		this, &CPlayer::Resume);

	// 1) Slow Motion
	CInput::GetInst()->SetCallback<CPlayer>("SkillSlowMotionAttack", KeyState_Down,
		this, &CPlayer::SkillSlowMotionAttack);
	// 2) Destroy All
	CInput::GetInst()->SetCallback<CPlayer>("SkillDestoryAll", KeyState_Down,
		this, &CPlayer::SkillDestroyAllAttack);

	// Move
	CInput::GetInst()->SetCallback<CPlayer>("MoveUp", KeyState_Push,
											this, &CPlayer::MoveUp);
	CInput::GetInst()->SetCallback<CPlayer>("MoveDown", KeyState_Push,
											this, &CPlayer::MoveDown);
	CInput::GetInst()->SetCallback<CPlayer>("MoveLeft", KeyState_Push,
											this, &CPlayer::MoveLeft);
	CInput::GetInst()->SetCallback<CPlayer>("MoveRight", KeyState_Push,
											this, &CPlayer::MoveRight);

	// Run
	CInput::GetInst()->SetCallback<CPlayer>("RunUp", KeyState_Push,
											this, &CPlayer::RunUp);
	CInput::GetInst()->SetCallback<CPlayer>("RunDown", KeyState_Push,
											this, &CPlayer::RunDown);
	CInput::GetInst()->SetCallback<CPlayer>("RunLeft", KeyState_Push,
											this, &CPlayer::RunLeft);
	CInput::GetInst()->SetCallback<CPlayer>("RunRight", KeyState_Push,
											this, &CPlayer::RunRight);

	// Dash
	CInput::GetInst()->SetCallback<CPlayer>("Dash", KeyState_Down,
		this, &CPlayer::Dash);

	// Teleport
	CInput::GetInst()->SetCallback<CPlayer>("Teleport", KeyState_Down,
		this, &CPlayer::Teleport);
	CInput::GetInst()->SetCallback<CPlayer>("TeleportPositionSet", KeyState_Down,
		this, &CPlayer::SetTeleportPos);

	// Target
	CInput::GetInst()->SetCallback<CPlayer>("TargetPos", KeyState_Push,
		this, &CPlayer::SetTargetPos);
	CInput::GetInst()->SetCallback<CPlayer>("TargetFire", KeyState_Down,
		this, &CPlayer::BulletFireTarget);
	
}

bool CPlayer::Init()
{
	if (!CCharacter::Init())
		return false;

	SetPivot(0.5f, 1.f);

	// Animation ---
	CreateAnimation();

	// Right 
	AddAnimation("LucidNunNaRightIdle");
	AddAnimation("LucidNunNaRightWalk", true, 1.f);
	AddAnimation("LucidNunNaRightAttack", false, 0.2f);
	AddAnimation("LucidNunNaRightRun", true, 0.6f);

	// Left
	AddAnimation("LucidNunNaLeftIdle");
	AddAnimation("LucidNunNaLeftWalk", true, 1.f);
	AddAnimation("LucidNunNaLeftRun", true, 0.6f);

	// Skill
	AddAnimation("SkillSlowMotionAttack", false, 0.5f);
	AddAnimation("SkillDestoryAll", false, 0.5f);

	// Target
	AddAnimation("LucidNunNaTargetAttack", false, 0.6f);

	// Stun
	AddAnimation("LucidNunNaRightDeath",false, DEATH_TIME);
	AddAnimation("LucidNunNaLeftDeath",false,DEATH_TIME);

	// Stun
	AddAnimation("LucidNunNaStun", true, 0.6f);

	// Teleport
	AddAnimation("LucidNunNaTeleport", false, 0.3f);
	SetAnimationEndNotify<CPlayer>("LucidNunNaTeleport", this, &CPlayer::ChangeMoveAnimation);

	AddAnimationNotify<CPlayer>("LucidNunNaRightAttack", 2, this, &CPlayer::Fire);
	SetAnimationEndNotify<CPlayer>("LucidNunNaRightAttack", this, &CPlayer::AttackEnd);

	SetAnimationEndNotify<CPlayer>("LucidNunNaLeftDeath", this, &CPlayer::Destroy);
	SetAnimationEndNotify<CPlayer>("LucidNunNaRightDeath", this, &CPlayer::Destroy);

	AddAnimationNotify<CPlayer>("LucidNunNaTargetAttack", 2, this, &CPlayer::FireTarget);
	SetAnimationEndNotify<CPlayer>("LucidNunNaTargetAttack", this, &CPlayer::AttackEnd);

	AddAnimationNotify<CPlayer>("SkillSlowMotionAttack", 2, this, &CPlayer::SkillSlowMotionAttackEnable);
	SetAnimationEndNotify<CPlayer>("SkillSlowMotionAttack", this, &CPlayer::SkillSlowMotionAttackEnd);

	AddAnimationNotify<CPlayer>("SkillDestoryAll", 2, this, &CPlayer::SkillDestoryAllAttackEnable);
	SetAnimationEndNotify<CPlayer>("SkillDestoryAll", this, &CPlayer::SkillDestroyAllAttackEnd);
	
	// Collider ---
	CColliderSphere *Head = AddCollider<CColliderSphere>("Head");
	Head->SetRadius(20.f);
	Head->SetOffset(0.f, -60.f);
	Head->SetCollisionProfile("Player");

	CColliderBox *Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(80.f, 45.f);
	Body->SetOffset(0.f, -22.5f);
	Body->SetCollisionProfile("Player");

	// Widget --- 
	// HPBar 
	m_HPBarWidget = CreateWidgetComponent("HPBarWidget");
	CProgressBar *HPBar = m_HPBarWidget->CreateWidget<CProgressBar>("HPBar");
	HPBar->SetTexture("WorldHPBar", TEXT("CharacterHPBar.bmp"));
	m_HPBarWidget->SetPos(-25.f, -95.f);

	// MPBar
	m_MPBarWidget = CreateWidgetComponent("MPBarWidget");
	CProgressBar* MPBar = m_MPBarWidget->CreateWidget<CProgressBar>("MPBar");
	MPBar->SetTexture("WorldMPBar", TEXT("CharacterMPBar.bmp"));
	m_MPBarWidget->SetPos(-25.f, -85.f);

	CWidgetComponent *NameWidget = CreateWidgetComponent("NameWidget");

	CUIText *NameText = NameWidget->CreateWidget<CUIText>("NameText");

	NameText->SetText(TEXT("Lucid"));
	NameText->SetTextColor(255, 0, 0);

	NameWidget->SetPos(-25.f, -115.f);

	// Info ---
	// MP,HP Setting
	m_CharacterInfo.MP = 5;
	m_CharacterInfo.MPMax = 5;

	m_CharacterInfo.HP = 1000;
	m_CharacterInfo.HPMax = 1000;

	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CCharacter::Update(DeltaTime);

	// if (GetAsyncKeyState(VK_F1) & 0x8000)
		// SetAttackSpeed(0.5f);
	// if (m_DeathAnimationTime > 0.f) return;

	// 범위 벗어남 여부 파악
	MoveWithinWorldResolution();

	// 몬스터와의 충돌 여부 파악
	int MonsterDamage = MonsterCollisionCheck();
	if (MonsterDamage !=- 1)
	{
		// Damage Font 
		CDamageFont* DamageFont = m_Scene->CreateObject<CDamageFont>("DamageFont", m_Pos);
		MonsterDamage -= m_CharacterInfo.Armor;
		if (MonsterDamage <= 0) MonsterDamage = 0;
		DamageFont->SetDamageNumber(MonsterDamage);
		SetDamage((float)MonsterDamage);

		// 튕겨져 나가기 
		CollideBounceBack();
	}

	if (m_SkillSlowMotionAttackEnable)
	{
		m_SkillSlowMotionAttackTime += DeltaTime * m_TimeScale;

		if (m_SkillSlowMotionAttackTime >= SLOW_MOTION_ATTACK_TIME )
		{
			// 시간 되돌리기 
			SetTimeScale(1.f);
			CGameManager::GetInst()->SetTimeScale(1.f);
			m_SkillSlowMotionAttackEnable = false;
			m_SkillSlowMotionAttackTime = 0.f;

			
			// Damage Font Test 출력  
			// CDamageFont* DamageFont = m_Scene->CreateObject<CDamageFont>("DamageFont", m_Pos);
			// DamageFont->SetDamageNumber(DeltaTime * 100000);

		}
	}

	// MP 자동 충전 
	if (m_CharacterInfo.MP <= m_CharacterInfo.MPMax)
		m_CharacterInfo.MP +=  DeltaTime;

	// Run
	if (m_RunEnable)
	{
		if (m_CharacterInfo.MP >= 0)
			m_CharacterInfo.MP -= 3 * DeltaTime;
		if (m_CharacterInfo.MP <= 0)
		{
			RunEnd();
		}
	}

	// Dash
	if (m_DashEnable)
	{
		if (CollisionCheck())
			CollideBounceBack();
		if (m_DashTime >= 0)
			m_DashTime -= DeltaTime;
		if (m_DashTime <= 0)
		{
			DashEnd();
		}
	}

	// Teleport
	if (m_TeleportEnable)
	{
		m_TelePortTime -= DeltaTime;
		if(m_TelePortTime <= 0.f)
			DeleteTeleportObj();
	}
	
	// MPBar
	CUICharacterStateHUD* State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");
	if (State)
		State->SetMPPercent(m_CharacterInfo.MP / (float)m_CharacterInfo.MPMax);
	CProgressBar* MPBar = (CProgressBar*)m_MPBarWidget->GetWidget();
	MPBar->SetPercent(m_CharacterInfo.MP / (float)m_CharacterInfo.MPMax);

	// Character Offset
	if (CheckCurrentAnimation("LucidNunNaRightAttack"))
		SetOffset(0.f, 20.f);
	else
		SetOffset(0.f, 0.f);

	// 가만히 있을 때 Mouse Pos에 따른, 방향 조정( Animation 조정 )
	if (CheckCurrentAnimation("LucidNunNaRightIdle") || CheckCurrentAnimation("LucidNunNaLeftIdle"))
	{
		Vector2 MousePos = CInput::GetInst()->GetMousePos();
		Vector2 MousePlayerPosDiff = m_Pos - MousePos;
		// 방향 setting
		float	Angle = GetAngle(m_Pos, MousePos);
		SetDir(Angle);
		// Animation Change
		if (MousePlayerPosDiff.x >= 0) // Player가 마우스보다 오른쪽에 있음 --> 왼쪽을 보게 하기
			ChangeAnimation("LucidNunNaLeftIdle");
		else				 // Player가 마우스보다 왼쪽에 있음 --> 오른쪽을 보게 하기
			ChangeAnimation("LucidNunNaRightIdle");
	}
}

void CPlayer::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);

	// Walk 이후 pos
	if (CheckCurrentAnimation("LucidNunNaRightWalk") &&
		m_Velocity.Length() == 0.f)
	{
		ChangeAnimation("LucidNunNaRightIdle");
	}
	if (CheckCurrentAnimation("LucidNunNaLeftWalk") &&
		m_Velocity.Length() == 0.f)
	{
		ChangeAnimation("LucidNunNaLeftIdle");
	}

	// Run 이후 pos
	if (CheckCurrentAnimation("LucidNunNaRightRun") &&
		m_Velocity.Length() == 0.f)
	{
		RunEnd();
		ChangeAnimation("LucidNunNaRightIdle");
	}
	if (CheckCurrentAnimation("LucidNunNaLeftRun") &&
		m_Velocity.Length() == 0.f)
	{
		RunEnd();
		ChangeAnimation("LucidNunNaLeftIdle");
	}
}

void CPlayer::Collision(float DeltaTime)
{
	CCharacter::Collision(DeltaTime);
}

void CPlayer::Render(HDC hDC)
{
	CCharacter::Render(hDC);
}

CPlayer *CPlayer::Clone()
{
	return new CPlayer(*this);
}

float CPlayer::SetDamage(float Damage)
{
	Damage = CCharacter::SetDamage(Damage);
	CUICharacterStateHUD *State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");
	if (State)
		State->SetHPPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);
	CProgressBar *HPBar = (CProgressBar *)m_HPBarWidget->GetWidget();
	HPBar->SetPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);

	if (m_CharacterInfo.HP <= 0)
	{
		// CharacterDestroy();
		Destroy();
		return -1.f;
	}
	return Damage;
}

void CPlayer::ChangeIdleAnimation()
{
	if (m_StunEnable) return;
	// 왼쪽 
	if (m_Dir.x < 0.f) ChangeAnimation("LucidNunNaLeftIdle");
	// 오른쪽 
	else ChangeAnimation("LucidNunNaRightIdle");
}

void CPlayer::MoveUp(float DeltaTime)
{
	RunEnd();
	Move(Vector2(0.f, -1.f));
	ChangeMoveAnimation();
}

void CPlayer::MoveDown(float DeltaTime)
{
	RunEnd();
	Move(Vector2(0.f, 1.f));
	ChangeMoveAnimation();
}

void CPlayer::MoveLeft(float DeltaTime)
{
	RunEnd();
	Move(Vector2(-1.f, 0.f));
	ChangeMoveAnimation();
}

void CPlayer::MoveRight(float DeltaTime)
{
	RunEnd();
	Move(Vector2(1.f, 0.f));
	ChangeMoveAnimation();
}

void CPlayer::Move(const Vector2& Dir)
{
	if (CollisionCheck())
	{
		// 그외 충돌시 효과 추가하기 
		// 이렇게 하면, Dash중에 충돌 날시, Move를 멈춘다
		// < Dash 충돌 원리 > 
		// 1) Update --> Dash + CollsionCheck() : CollideBounceBack 함수 실행
		// 2) 한편, Move 상에서는 Dash + CollisionCheck() 이면, Move 진행 x ( 이전의 Dash를 통한 Move 를 막아주기 위해 )
		// 3) (이전의 Move는 멈춘 상태) CollideBounceBack() 함수를 통해, 해당 obj를 뒤로 밀려나게 한다.
		if (m_DashEnable) return;
		
		// 우선 충돌하면 무조건 
		// return;
	}
	CCharacter::Move(Dir);
}

void CPlayer::Move(const Vector2& Dir, float Speed)
{
	if (CollisionCheck())
	{
		// 그외 충돌시 효과 추가하기 
		if (m_DashEnable) return;
	}
	CCharacter::Move(Dir, Speed);
}

void CPlayer::ChangeMoveAnimation()
{
	if (m_StunEnable) return;
	// 왼쪽 
	if (m_Dir.x < 0.f) ChangeAnimation("LucidNunNaLeftWalk");
	// 오른쪽 
	else ChangeAnimation("LucidNunNaRightWalk");
}

void CPlayer::RunLeft(float DeltaTime)
{
	Move(Vector2(-1.f, 0.f));
	RunStart();
	ChangeRunAnimation();
}

void CPlayer::RunRight(float DeltaTime)
{
	Move(Vector2(1.f, 0.f));
	RunStart();
	ChangeRunAnimation();
}

void CPlayer::RunUp(float DeltaTime)
{
	Move(Vector2(0.f, -1.f));
	RunStart();
	ChangeRunAnimation();
}

void CPlayer::RunDown(float DeltaTime)
{
	Move(Vector2(0.f, 1.f));
	RunStart();
	ChangeRunAnimation();
}

void CPlayer::RunStart()
{
	if (m_CharacterInfo.MP <= 0.2 * m_CharacterInfo.MPMax || m_RunEnable )
		return;
	m_RunEnable = true;
	CEffectHit *Hit = m_Scene->CreateObject<CEffectHit>("HitEffect", "HitEffect",
														m_Pos, Vector2(178.f, 164.f));
	m_Scene->GetSceneResource()->SoundPlay("Run");
	SetMoveSpeed(FAST_SPEED);
}

void CPlayer::RunEnd()
{
	if (!m_RunEnable)
		return;
	m_RunEnable = false;
	SetMoveSpeed(NORMAL_SPEED);

	if (CheckCurrentAnimation("LucidNunNaRightRun"))
	{
		ChangeAnimation("LucidNunNaRightWalk");
	}
	if (CheckCurrentAnimation("LucidNunNaLeftRun"))
	{
		ChangeAnimation("LucidNunNaLeftWalk");
	}
}

void CPlayer::ChangeRunAnimation()
{
	if (m_StunEnable) return;
	// 왼쪽 
	if (m_Dir.x == -1.f) ChangeAnimation("LucidNunNaLeftRun");
	// 오른쪽 
	else ChangeAnimation("LucidNunNaRightRun");
}

void CPlayer::Dash(float DelatTime)
{
	if (m_DashEnable || m_CharacterInfo.MP < 0.5 * m_CharacterInfo.MPMax) return;

	// Dash Time 세팅 
	m_DashTime = DASH_TIME;
	m_DashEnable = true;

	// speed 조정 
	SetMoveSpeed(DASH_SPEED);

	// MP 감소
	if (m_CharacterInfo.MP >= 0.5f * m_CharacterInfo.MPMax)
		m_CharacterInfo.MP -= 0.5f * m_CharacterInfo.MPMax;

	// Effect 효과
	CEffectHit* Hit = m_Scene->CreateObject<CEffectHit>("HitEffect", "HitEffect",
		m_Pos, Vector2(178.f, 164.f));

	// Sound 효과
	m_Scene->GetSceneResource()->SoundPlay("Dash");
}

void CPlayer::DashEnd()
{
	if (!m_DashEnable) return;
	m_DashEnable = false;
	SetMoveSpeed(NORMAL_SPEED);
}

void CPlayer::CollideBounceBack()
{
	// 벽에 대시한 경우( 어떤 충돌체와 충돌하던 뒤로 밀려난다 ) + 해당 collider가 mouse type이 아니어야 한다
	// 대시중 충돌 여부 확인
	// 나중에 체크해야 한다. 
	// 충돌한 대상이 몬스터인지, 장애물인지, 등등 
	// 지금은 우선 이렇게 단순하게 세팅하자.
	// 이동 방향 반대로 이동시키기
	Vector2 OppDir = Vector2(-m_Dir.x, -m_Dir.y);
	OppDir.Normalize();
	CGameObject* Player = m_Scene->GetPlayer();
	SetStunDir(OppDir);
	DashEnd();

	// 자기 크기만큼 bounce back
	Stun();
}


void CPlayer::BulletFire(float DeltaTime)
{
	ChangeAnimation("LucidNunNaRightAttack");
}

void CPlayer::Pause(float DeltaTime)
{
	CGameManager::GetInst()->SetTimeScale(0.f);
}

void CPlayer::Resume(float DeltaTime)
{
	CGameManager::GetInst()->SetTimeScale(1.f);
}

void CPlayer::SkillSlowMotionAttack(float DeltaTime)
{
	if (m_CharacterInfo.MP <= 0.95 * m_CharacterInfo.MPMax) return;
	ChangeAnimation("SkillSlowMotionAttack");
}

void CPlayer::SkillSlowMotionAttackEnd()
{
	ChangeIdleAnimation();
}

void CPlayer::SkillSlowMotionAttackEnable()
{
	// Slow Motion
	CGameManager::GetInst()->SetTimeScale(0.01f);
	SetTimeScale(100.f);
	m_SkillSlowMotionAttackEnable = true;

	// MP Decrease
	m_CharacterInfo.MP = 0.f;


	for (float f = 0.0; f < 2 * M_PI; f += M_PI / 9.0f) // 9.0 으로 나눈다는 것은 20씩 증가시킨다 --> 18개
	{
		CSharedPtr<CBullet> Bullet = m_Scene->CreateObject<CBullet>("Bullet",
			"SkillSlowMotionAttackBullet",
			// 중점 + 반지름 길이 * 함수
			Vector2((m_Pos.x - m_Offset.x) + m_Size.Length() * cos(f) 
				, (m_Pos.y - m_Offset.y) + m_Size.Length() * sin(f)),
			Vector2(m_Size.x,m_Size.y));
		Bullet->SetObjectType(EObject_Type::Bullet);

		// Bullet 충돌체 : PlayerAttack 으로 처리하기 
		CCollider* BulletBody = Bullet->FindCollider("Body");
		BulletBody->SetCollisionProfile("PlayerAttack");

		CGameObject* ClosestMonster = FindClosestTarget(Bullet->GetPos());
		if (ClosestMonster)
		{
			float AngleBtwBulletMonster = GetAngle(Bullet->GetPos(), ClosestMonster->GetPos());
			Bullet->SetDir(AngleBtwBulletMonster);
		}
		else
		{
			Bullet->SetDir(m_Dir);
		}
		Bullet->SetBulletDamage((float)m_CharacterInfo.Attack);
		Bullet->SetTimeScale(m_TimeScale);
	}
}

void CPlayer::SkillDestroyAllAttack(float DeltaTime)
{
	ChangeAnimation("SkillDestoryAll");
}

void CPlayer::SkillDestroyAllAttackEnd()
{
	ChangeIdleAnimation();
}

void CPlayer::SkillDestoryAllAttackEnable()
{
	// DestroyAll
	// monster로부터 오는 모든 bullet 및 공격체들을 사라지게 한다. 
	// 동시에 사라지는 animation도 적용한다. 
	m_Scene->DestroyAllAttackObjects();

	// 시야 내에 있는 모든 game obj 들에게 20%의 데미지를 입힌다.

}

CGameObject* CPlayer::FindClosestTarget(Vector2 PlayerPos)
{
	return m_Scene->FindClosestMonsterToPlayer(PlayerPos);
}

// 참고 : Bullet의 경우, Collision을 고려할 필요가 없다
// 왜냐하면 충돌하는 순간 Bullet은 자기 혼자 바로 사라져 버리기 때문이다 
bool CPlayer::CollisionCheck()
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsCollisionListEmpty())
		{
			return true;
		}
	}
	return false;
}

int CPlayer::MonsterCollisionCheck()
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		int MonsterDamage = (*iter)->IsCollisionWithMonster();
		if (MonsterDamage != -1)
		{
			return MonsterDamage;
		}
	}
	return -1;
}

Vector2 CPlayer::GetColliderPos()
{
	// 만약 충돌체가 없다면, 월드해상도 끝의 위치를 리턴 ( 여기에는 어떤 collider도 없을 것이므로 ) 
	Vector2 m_Resolution = m_Scene->GetCamera()->GetWorldResolution();
	if(!CollisionCheck()) return m_Resolution;

	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsCollisionListEmpty())
		{
			return (*iter)->GetOwner()->GetPos();
		}
	}
	return m_Resolution;

}

void CPlayer::Stun()
{
	CCharacter::Stun();
	ChangeAnimation("LucidNunNaStun");
}

void CPlayer::StunEnd()
{
	CCharacter::StunEnd();
	// 방향에 따라서 바꿔주기 ( 차후 마우스 위치에 따른 방향 수정후 적용 )
	ChangeAnimation("LucidNunNaRightIdle");
}


void CPlayer::CollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime)
{

}

void CPlayer::Teleport(float DeltaTime)
{
	if (!m_TeleportEnable || m_CharacterInfo.MP <= 0.9 * m_CharacterInfo.MPMax) return;

	// Animation 적용하기 
	ChangeAnimation("LucidNunNaTeleport");

	// 이동하기
	m_Pos = m_TeleportPos;

	// Animation 되돌려두기
	// ChangeMoveAnimation();

	// m_TeleportEnable
	m_TeleportEnable = false;

	// MP 90% 감소
	if (m_CharacterInfo.MP >= 0.9f * m_CharacterInfo.MPMax)
		m_CharacterInfo.MP -= 0.9f * m_CharacterInfo.MPMax;
	
	// TeleportMouse Cursor Animation 지워주기
	DeleteTeleportObj();
}

void CPlayer::SetTeleportPos(float DeltaTime)
{
	if(m_CharacterInfo.MP <= 0.9 * m_CharacterInfo.MPMax) return;

	// Teleport 준비 
	m_TeleportEnable = true;

	// 클릭하는 순간, 좌표상의 마우스 위치 가져오기 
	Vector2 MousePos = CInput::GetInst()->GetMousePos();
	Vector2 CameraPos = m_Scene->GetCamera()->GetPos();
	m_TeleportPos = Vector2((float)(MousePos.x+ CameraPos.x), (float)(MousePos.y+ CameraPos.y));

	// 화면상에 Teleport 위치 애니메이션 그리기
	 // 이전에 그린 teleport animation 지우기 (메모리 leak 방지 )
	DeleteTeleportObj();
	// 새로 그리기 
	m_TeleportObj = m_Scene->CreateObject<CTeleportMouse>("TeleportMouse", "TeleportMouse",
	m_TeleportPos);

	// Update 함수에서, 커서만 누르고, Teleport 안해주면
	// m_TeleportObj 에 Animation이 계속 남아있을 수 있다
	// 따라서, 일정 시간이 지나면 지워주기 위해 m_TeleportTime을 세팅한다
	m_TelePortTime = TELEPORT_MOUSE_DISPLAY_TIME;
}

void CPlayer::DeleteTeleportObj()
{
	if (m_TeleportObj)
		m_TeleportObj->Destroy();
}

void CPlayer::AttackEnd()
{
	ChangeAnimation("LucidNunNaRightIdle");
}

void CPlayer::Fire()
{
	CSharedPtr<CBullet> Bullet = m_Scene->CreateObject<CBullet>("Bullet",
																"PlayerBullet",
																Vector2(m_Pos + Vector2(75.f, 0.f)),
																Vector2(50.f, 50.f));
	Bullet->SetBulletDamage((float)m_CharacterInfo.Attack);
	Bullet->SetObjectType(EObject_Type::Bullet);
}

void CPlayer::SetTargetPos(float DeltaTime)
{
	// m_TargetPos에 세팅 
	POINT ptMouse;
	HWND hwnd = CGameManager::GetInst()->GetWindowHandle();
	Vector2 MousePos = CInput::GetInst()->GetMousePos();
	Vector2 CameraPos = m_Scene->GetCamera()->GetPos();
	m_TargetPos = Vector2((float)(MousePos.x + CameraPos.x), (float)(MousePos.y + CameraPos.y));

}

void CPlayer::FireTarget() 
{
	if (m_CharacterInfo.MP <= 0.2f * m_CharacterInfo.MPMax) return;
	m_CharacterInfo.MP -= 0.2f * m_CharacterInfo.MPMax;

	CSharedPtr<CBullet> Bullet = m_Scene->CreateObject<CBullet>("Bullet",
		"PlayerBullet",
		Vector2(m_Pos + Vector2(75.f, 0.f)),
		Vector2(50.f, 50.f));
	Bullet->SetObjectType(EObject_Type::Bullet);
	float	Angle = GetAngle(Bullet->GetPos(), m_TargetPos);

	Bullet->SetDir(Angle);
	Bullet->SetBulletDamage((float)m_CharacterInfo.Attack);
}

void CPlayer::BulletFireTarget(float DeltaTime)
{
	if (m_CharacterInfo.MP <= 0.2 * m_CharacterInfo.MPMax) return;
	ChangeAnimation("LucidNunNaTargetAttack");
}

void CPlayer::CharacterDestroy()
{
	m_DeathAnimationTime = DEATH_TIME;
	// 왼쪽 
	if (m_Dir.x <= 0.f) 
		ChangeAnimation("LucidNunNaLeftDeath");
	// 오른쪽 
	else ChangeAnimation("LucidNunNaRightDeath");
}


