#include "../Timer.h"
#include "../Input.h"
#include "../GameManager.h"
// Object
#include "Player.h"
#include "EffectHit.h"
#include "TeleportMouse.h"
#include "Potion.h"
#include "DamageFont.h"
// Scene
#include "../Scene/Scene.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Camera.h"
// Collision
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderSphere.h"
// UI
#include "../UI/UICharacterStateHUD.h"
#include "../UI/ProgressBar.h"
#include "../UI/UIText.h"

// Static
CPlayer::CPlayer() : m_SkillSlowMotionAttackEnable(false),
					 m_SkillSlowMotionAttackTime(0.f),
					 m_RunEnable(false),
					 m_DashEnable(false),
					 m_TargetEnable(false),
					 m_DashTime(0.f),
					 m_TeleportEnable(false),
					 m_TelePortTime(0.f),
					 m_TeleportObj{},
					 m_TeleportPos(Vector2(0.f, 0.f)),
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

	auto iter = m_WidgetComponentList.begin();
	auto iterEnd = m_WidgetComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == PLAYER_HPWIDGET_COMPONENET)
			m_HPBarWidget = (*iter);
		if ((*iter)->GetName() == PLAYER_MPWIDGET_COMPONENET)
			m_MPBarWidget = (*iter);
		if ((*iter)->GetName() == PLAYER_NAMEWIDGET_COMPONENET)
			m_NameWidget = (*iter);
	}
}

CPlayer::~CPlayer()
{
	SAFE_DELETE(m_TeleportObj);
}

void CPlayer::Start()
{
	CCharacter::Start();

	// Item
	CInput::GetInst()->SetCallback<CPlayer>("GetItem", KeyState_Down,
											this, &CPlayer::AcquireItem);

	// Fire, Pause, Resume
	// CInput::GetInst()->SetCallback<CPlayer>("Fire", KeyState_Push,
	// this, &CPlayer::BulletFire);
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
	CInput::GetInst()->SetCallback<CPlayer>("MouseRButton", KeyState_Push,
											this, &CPlayer::SetTargetPos);
	CInput::GetInst()->SetCallback<CPlayer>("MouseRButton", KeyState_Up,
											this, &CPlayer::RemoveTargetPos);
	CInput::GetInst()->SetCallback<CPlayer>("MouseLButton", KeyState_Push,
											this, &CPlayer::BulletFireTarget);
	CInput::GetInst()->SetCallback<CPlayer>("MouseLButton", KeyState_Up,
											this, &CPlayer::RemoveTargetPos);
}

void CPlayer::SetNotifyFunctions()
{
	// Teleport
	SetAnimationEndNotify<CPlayer>("LucidNunNaTeleport", this, &CPlayer::ChangeMoveAnimation);

	// Attack
	AddAnimationNotify<CPlayer>("LucidNunNaRightAttack", 2, this, &CPlayer::FireTarget);
	AddAnimationNotify<CPlayer>("LucidNunNaLeftAttack", 2, this, &CPlayer::FireTarget);
	SetAnimationEndNotify<CPlayer>("LucidNunNaRightAttack", this, &CPlayer::AttackEnd);
	SetAnimationEndNotify<CPlayer>("LucidNunNaLeftAttack", this, &CPlayer::AttackEnd);

	// Death
	SetAnimationEndNotify<CPlayer>("LucidNunNaLeftDeath", this, &CPlayer::Destroy);
	SetAnimationEndNotify<CPlayer>("LucidNunNaRightDeath", this, &CPlayer::Destroy);

	AddAnimationNotify<CPlayer>("LucidNunNaTargetAttack", 2, this, &CPlayer::FireTarget);
	SetAnimationEndNotify<CPlayer>("LucidNunNaTargetAttack", this, &CPlayer::AttackEnd);

	AddAnimationNotify<CPlayer>("SkillSlowMotionAttack", 2, this, &CPlayer::SkillSlowMotionAttackEnable);
	SetAnimationEndNotify<CPlayer>("SkillSlowMotionAttack", this, &CPlayer::SkillSlowMotionAttackEnd);

	AddAnimationNotify<CPlayer>("SkillDestoryAll", 2, this, &CPlayer::SkillDestoryAllAttackEnable);
	SetAnimationEndNotify<CPlayer>("SkillDestoryAll", this, &CPlayer::SkillDestroyAllAttackEnd);
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
	AddAnimation("LucidNunNaRightAttack", false, 0.1f);
	AddAnimation("LucidNunNaRightRun", true, 0.6f);

	// Left
	AddAnimation("LucidNunNaLeftIdle");
	AddAnimation("LucidNunNaLeftWalk", true, 1.f);
	AddAnimation("LucidNunNaLeftAttack", false, 0.1f);
	AddAnimation("LucidNunNaLeftRun", true, 0.6f);

	// Skill
	AddAnimation("SkillSlowMotionAttack", false, 0.5f);
	AddAnimation("SkillDestoryAll", false, 0.5f);

	// Target
	AddAnimation("LucidNunNaTargetAttack", false, 0.6f);

	// Stun
	AddAnimation("LucidNunNaRightDeath", false, DEATH_TIME);
	AddAnimation("LucidNunNaLeftDeath", false, DEATH_TIME);

	// Stun
	AddAnimation("LucidNunNaStun", true, 0.6f);

	// Teleport
	AddAnimation("LucidNunNaTeleport", false, 0.3f);

	// NotifyFunctions
	SetNotifyFunctions();

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
	m_HPBarWidget = CreateWidgetComponent(PLAYER_HPWIDGET_COMPONENET);
	CProgressBar *HPBar = m_HPBarWidget->CreateWidget<CProgressBar>("HPBar");
	HPBar->SetTexture("WorldHPBar", TEXT("CharacterHPBar.bmp"));
	m_HPBarWidget->SetPos(-25.f, -95.f);
	// this
	// MPBar
	m_MPBarWidget = CreateWidgetComponent(PLAYER_MPWIDGET_COMPONENET);
	CProgressBar *MPBar = m_MPBarWidget->CreateWidget<CProgressBar>("MPBar");
	MPBar->SetTexture("WorldMPBar", TEXT("CharacterMPBar.bmp"));
	m_MPBarWidget->SetPos(-25.f, -85.f);

	auto iter = m_WidgetComponentList.begin();

	m_NameWidget = CreateWidgetComponent(PLAYER_NAMEWIDGET_COMPONENET);

	CUIText *NameText = m_NameWidget->CreateWidget<CUIText>("NameText");

	NameText->SetText(TEXT("Lucid"));
	NameText->SetTextColor(255, 0, 0);

	m_NameWidget->SetPos(-25.f, -115.f);

	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CCharacter::Update(DeltaTime);

	// if (GetAsyncKeyState(VK_F1) & 0x8000)
	// SetAttackSpeed(0.5f);
	// if (m_DeathAnimationTime > 0.f) return;

	// ���� ��� ���� �ľ�
	MoveWithinWorldResolution();

	// ���Ϳ��� �浹 ���� �ľ�
	CGameObject *CollideMonster = MonsterCollisionCheck();
	if (CollideMonster)
	{
		float MonsterDamage = CollideMonster->GetAttack();
		// Damage Font
		CDamageFont *DamageFont = m_Scene->CreateObject<CDamageFont>("DamageFont", m_Pos);
		MonsterDamage -= m_CharacterInfo.Armor;
		if (MonsterDamage <= 0)
			MonsterDamage = 0;
		DamageFont->SetDamageNumber(MonsterDamage);
		SetDamage((float)MonsterDamage);

		// ƨ���� ������
		Vector2 MonsterDir = CollideMonster->GetDir();
		CollideBounceBack(Vector2(MonsterDir.x, MonsterDir.y));
	}

	if (m_SkillSlowMotionAttackEnable)
	{
		m_SkillSlowMotionAttackTime += DeltaTime * m_TimeScale;

		if (m_SkillSlowMotionAttackTime >= SLOW_MOTION_ATTACK_TIME)
		{
			// �ð� �ǵ�����
			SetTimeScale(1.f);
			CGameManager::GetInst()->SetTimeScale(1.f);
			m_SkillSlowMotionAttackEnable = false;
			m_SkillSlowMotionAttackTime = 0.f;

			// Damage Font Test ���
			// CDamageFont* DamageFont = m_Scene->CreateObject<CDamageFont>("DamageFont", m_Pos);
			// DamageFont->SetDamageNumber(DeltaTime * 10000000);
		}
	}

	// MP �ڵ� ����
	if (m_CharacterInfo.MP <= m_CharacterInfo.MPMax)
		m_CharacterInfo.MP += DeltaTime;

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
		if (ObstacleCollisionCheck())
			CollideBounceBack(Vector2(-m_Dir.x, -m_Dir.y));
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
		if (m_TelePortTime <= 0.f)
			DeleteTeleportObj();
	}

	// MPBar , HPBar
	CUICharacterStateHUD *State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");
	if (State)
	{
		State->SetMPPercent(m_CharacterInfo.MP / (float)m_CharacterInfo.MPMax);
		State->SetHPPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);
	}
	// this
	CProgressBar *MPBar = (CProgressBar *)m_MPBarWidget->GetWidget();
	MPBar->SetPercent(m_CharacterInfo.MP / (float)m_CharacterInfo.MPMax);

	CProgressBar *HPBar = (CProgressBar *)m_HPBarWidget->GetWidget();
	HPBar->SetPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);

	// Character Offset
	if (CheckCurrentAnimation("LucidNunNaRightAttack") || CheckCurrentAnimation("LucidNunNaLeftAttack"))
		SetOffset(0.f, 20.f);
	else
		SetOffset(0.f, 0.f);

	// ������ ���� �� Mouse Pos�� ����, ���� ����( Animation ���� )
	if (CheckCurrentAnimation("LucidNunNaRightIdle") || CheckCurrentAnimation("LucidNunNaLeftIdle"))
	{
		Vector2 MousePos = CInput::GetInst()->GetMousePos();
		Vector2 MousePlayerPosDiff = m_Pos - MousePos;
		// ���� setting
		float Angle = GetAngle(m_Pos, MousePos);
		SetDir(Angle);
		// Animation Change
		if (MousePlayerPosDiff.x >= 0) // Player�� ���콺���� �����ʿ� ���� --> ������ ���� �ϱ�
			ChangeAnimation("LucidNunNaLeftIdle");
		else // Player�� ���콺���� ���ʿ� ���� --> �������� ���� �ϱ�
			ChangeAnimation("LucidNunNaRightIdle");
	}
}

void CPlayer::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);
	// Walk ���� pos
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

	// Run ���� pos
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

	if (m_TargetEnable)
	{
		HPEN Pen = CGameManager::GetInst()->GetRedPen();
		HGDIOBJ PrevPen = SelectObject(hDC, Pen);

		HBRUSH Brush = CGameManager::GetInst()->GetRedBrush();
		HGDIOBJ PrevBrush = SelectObject(hDC, Brush);

		Vector2 CameraPos = m_Scene->GetCamera()->GetPos();
		Vector2 ScreenPlayerPos = m_Pos - CameraPos; 
		Vector2 ScreenTargetPos = m_TargetPos - CameraPos;
		MoveToEx(hDC, (int)ScreenPlayerPos.x, (int)ScreenPlayerPos.y, nullptr);
		LineTo(hDC, (int)ScreenTargetPos.x, (int)ScreenTargetPos.y);
		Ellipse(hDC, (int)(ScreenTargetPos.x - 5), (int)(ScreenTargetPos.y - 5),
				(int)(ScreenTargetPos.x + 5), (int)(ScreenTargetPos.y + 5)); //  L, T, R, B

		SelectObject(hDC, PrevPen);
		SelectObject(hDC, PrevBrush);
	}
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
	if (m_StunEnable)
		return;
	// ����
	if (m_Dir.x < 0.f)
		ChangeAnimation("LucidNunNaLeftIdle");
	// ������
	else
		ChangeAnimation("LucidNunNaRightIdle");
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

void CPlayer::Move(const Vector2 &Dir)
{
	if (ObstacleCollisionCheck())
	{
		if (m_DashEnable)
			return;
	}
	CCharacter::Move(Dir);
}

void CPlayer::Move(const Vector2 &Dir, float Speed)
{
	if (ObstacleCollisionCheck())
	{
		// �׿� �浹�� ȿ�� �߰��ϱ�
		if (m_DashEnable)
			return;
	}
	CCharacter::Move(Dir, Speed);
}

void CPlayer::ChangeMoveAnimation()
{
	if (m_StunEnable)
		return;
	// ����
	if (m_Dir.x < 0.f)
		ChangeAnimation("LucidNunNaLeftWalk");
	// ������
	else
		ChangeAnimation("LucidNunNaRightWalk");
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
	if (m_CharacterInfo.MP <= 0.2 * m_CharacterInfo.MPMax || m_RunEnable)
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
	if (m_StunEnable)
		return;
	// ����
	if (m_Dir.x == -1.f)
		ChangeAnimation("LucidNunNaLeftRun");
	// ������
	else
		ChangeAnimation("LucidNunNaRightRun");
}

void CPlayer::Dash(float DelatTime)
{
	if (m_DashEnable || m_CharacterInfo.MP < 0.5 * m_CharacterInfo.MPMax)
		return;

	// Dash Time ����
	m_DashTime = DASH_TIME;
	m_DashEnable = true;

	// speed ����
	SetMoveSpeed(DASH_SPEED);

	// MP ����
	if (m_CharacterInfo.MP >= 0.5f * m_CharacterInfo.MPMax)
		m_CharacterInfo.MP -= 0.5f * m_CharacterInfo.MPMax;

	// Effect ȿ��
	CEffectHit *Hit = m_Scene->CreateObject<CEffectHit>("HitEffect", "HitEffect",
														m_Pos, Vector2(178.f, 164.f));

	// Sound ȿ��
	m_Scene->GetSceneResource()->SoundPlay("Dash");
}

void CPlayer::DashEnd()
{
	if (!m_DashEnable)
		return;
	m_DashEnable = false;
	SetMoveSpeed(NORMAL_SPEED);
}

void CPlayer::CollideBounceBack(Vector2 Dir)
{
	Vector2 OppDir = Dir;
	OppDir.Normalize();
	SetStunDir(OppDir);
	DashEnd();

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
	if (m_CharacterInfo.MP <= 0.95 * m_CharacterInfo.MPMax)
		return;
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

	for (float f = 0.0f; f < 2 * M_PI; f += M_PI / 9.0f) // 9.0 ���� �����ٴ� ���� 20�� ������Ų�� --> 18��
	{
		CSharedPtr<CBullet> Bullet = m_Scene->CreateObject<CBullet>("Bullet",
																	"SkillSlowMotionAttackBullet",
																	// ���� + ������ ���� * �Լ�
																	Vector2((m_Pos.x - m_Offset.x) + m_Size.Length() * cos(f), (m_Pos.y - m_Offset.y) + m_Size.Length() * sin(f)),
																	Vector2(m_Size.x, m_Size.y));
		Bullet->SetObjectType(EObject_Type::Bullet);

		// Bullet �浹ü : PlayerAttack ���� ó���ϱ�
		CCollider *BulletBody = Bullet->FindCollider("Body");
		BulletBody->SetCollisionProfile("PlayerAttack");

		CGameObject *ClosestMonster = FindClosestTarget(Bullet->GetPos());
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
	m_Scene->DestroyAllAttackObjects();

}

CGameObject *CPlayer::FindClosestTarget(Vector2 PlayerPos)
{
	return m_Scene->FindClosestMonsterToPlayer(PlayerPos);
}

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

bool CPlayer::ObstacleCollisionCheck() const
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->DidCollideWithObstacles())
			return true;
	}

	return false;
}

CGameObject *CPlayer::MonsterCollisionCheck()
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		CGameObject *Monster = (*iter)->IsCollisionWithMonster();
		if (Monster)
		{
			return Monster;
		}
	}
	return nullptr;
}

Vector2 CPlayer::GetColliderPos()
{
	// ���� �浹ü�� ���ٸ�, �����ػ� ���� ��ġ�� ���� ( ���⿡�� � collider�� ���� ���̹Ƿ� )
	Vector2 m_Resolution = m_Scene->GetCamera()->GetWorldResolution();
	if (!CollisionCheck())
		return m_Resolution;

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
	// ���⿡ ���� �ٲ��ֱ� ( ���� ���콺 ��ġ�� ���� ���� ������ ���� )
	ChangeAnimation("LucidNunNaRightIdle");
}

void CPlayer::CollisionBegin(CCollider *Src, CCollider *Dest, float DeltaTime)
{
}

void CPlayer::Teleport(float DeltaTime)
{
	if (!m_TeleportEnable || m_CharacterInfo.MP <= 0.9 * m_CharacterInfo.MPMax)
		return;

	// Animation �����ϱ�
	ChangeAnimation("LucidNunNaTeleport");

	// �̵��ϱ�
	m_Pos = m_TeleportPos;

	// Animation �ǵ����α�
	// ChangeMoveAnimation();

	// m_TeleportEnable
	m_TeleportEnable = false;

	// MP 90% ����
	if (m_CharacterInfo.MP >= 0.9f * m_CharacterInfo.MPMax)
		m_CharacterInfo.MP -= 0.9f * m_CharacterInfo.MPMax;

	// TeleportMouse Cursor Animation �����ֱ�
	DeleteTeleportObj();
}

void CPlayer::SetTeleportPos(float DeltaTime)
{
	if (m_CharacterInfo.MP <= 0.9 * m_CharacterInfo.MPMax)
		return;

	// Teleport �غ�
	m_TeleportEnable = true;

	// Ŭ���ϴ� ����, ��ǥ���� ���콺 ��ġ ��������
	Vector2 MousePos = CInput::GetInst()->GetMousePos();
	Vector2 CameraPos = m_Scene->GetCamera()->GetPos();
	m_TeleportPos = Vector2((float)(MousePos.x + CameraPos.x), (float)(MousePos.y + CameraPos.y));

	// ȭ��� Teleport ��ġ �ִϸ��̼� �׸���
	// ������ �׸� teleport animation ����� (�޸� leak ���� )
	DeleteTeleportObj();
	// ���� �׸���
	m_TeleportObj = m_Scene->CreateObject<CTeleportMouse>("TeleportMouse", "TeleportMouse",
														  m_TeleportPos);

	// Update �Լ�����, Ŀ���� ������, Teleport �����ָ�
	// m_TeleportObj �� Animation�� ��� �������� �� �ִ�
	// ����, ���� �ð��� ������ �����ֱ� ���� m_TeleportTime�� �����Ѵ�
	m_TelePortTime = TELEPORT_MOUSE_DISPLAY_TIME;
}

void CPlayer::DeleteTeleportObj()
{
	if (m_TeleportObj)
		m_TeleportObj->Destroy();
}

void CPlayer::AttackEnd()
{
	if (CheckCurrentAnimation("LucidNunNaRightAttack"))
		ChangeAnimation("LucidNunNaRightIdle");
	else
		ChangeAnimation("LucidNunNaLeftIdle");
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

void CPlayer::RemoveTargetPos(float DeltaTime)
{
	m_TargetEnable = false;
}

void CPlayer::FireTarget()
{
	Vector2 BulletOffset = CheckCurrentAnimation("LucidNunNaRightAttack") ? Vector2(75.f, 0.f) : Vector2(-75.f, 0.f);
	CSharedPtr<CBullet> Bullet = m_Scene->CreateObject<CBullet>("Bullet",
																"PlayerBullet",
																Vector2(m_Pos + BulletOffset),
																Vector2(50.f, 50.f));
	Bullet->SetObjectType(EObject_Type::Bullet);
	float Angle = GetAngle(Bullet->GetPos(), m_TargetPos);

	Bullet->SetDir(Angle);
	Bullet->SetBulletDamage((float)m_CharacterInfo.Attack);
}

void CPlayer::SetTargetPos(float DeltaTime)
{
	// m_TargetPos�� ����
	Vector2 MousePos = CInput::GetInst()->GetMousePos();
	Vector2 CameraPos = m_Scene->GetCamera()->GetPos();
	Vector2 Resolution = m_Scene->GetCamera()->GetResolution();

	/*
	if (MousePos.x < 0.f || MousePos.x > Resolution.x ||
		MousePos.y < 0.f || MousePos.y > Resolution.y)
	{
		// MousePos = m_Pos;
		return;
	}
	*/

	m_TargetEnable = true;
	m_TargetPos = Vector2((float)(MousePos.x + CameraPos.x), (float)(MousePos.y + CameraPos.y));
}

void CPlayer::BulletFireTarget(float DeltaTime)
{
	Vector2 PlayerDir = m_Dir;

	// SetTargetPos(DeltaTime);
	// Laser 표시 없이 Target 만 세팅하기
	Vector2 MousePos = CInput::GetInst()->GetMousePos();
	Vector2 CameraPos = m_Scene->GetCamera()->GetPos();
	m_TargetPos = Vector2((float)(MousePos.x + CameraPos.x), (float)(MousePos.y + CameraPos.y));

	if (m_Dir.x > 0)
		ChangeAnimation("LucidNunNaRightAttack");
	else
		ChangeAnimation("LucidNunNaLeftAttack");
}

void CPlayer::CharacterDestroy()
{
	m_DeathAnimationTime = DEATH_TIME;
	// ����
	if (m_Dir.x <= 0.f)
		ChangeAnimation("LucidNunNaLeftDeath");
	// ������
	else
		ChangeAnimation("LucidNunNaRightDeath");
}

void CPlayer::AcquireItem(float DeltaTime)
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		CPotion *Potion = (*iter)->IsCollisionWithPotion();
		if (Potion)
		{
			EPotion_Type PType = Potion->GetPotionType();
			float PotionAmount = Potion->GetPotionAmount();
			if (PType == EPotion_Type::HP)
				m_CharacterInfo.HP += (int)PotionAmount;
			else
				m_CharacterInfo.MP += PotionAmount;
			Potion->Destroy();
		}
	}
}
