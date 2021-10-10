#include "../Timer.h"
#include "../Input.h"
#include "../GameManager.h"
// Object
#include "Player.h"
#include "EffectHit.h"
#include "EffectDash.h"
#include "TeleportMouse.h"
#include "Potion.h"
#include "DamageFont.h"
// Scene
#include "../Scene/Scene.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/SceneCollision.h"
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
					 m_SkillDestoryAllAttackTime(0.f),
					 m_LaserBulletObj(nullptr),
					 m_PlayerDeath(false),
					m_HPBarWidget(nullptr),
					m_MPBarWidget(nullptr),
					m_NameWidget(nullptr),
					m_SteminaBarWidget(nullptr)
			
{
	m_ObjType = EObject_Type::Player;
}

CPlayer::CPlayer(const CPlayer &obj) : CCharacter(obj)
{
	m_SkillSlowMotionAttackTime = obj.m_SkillSlowMotionAttackTime;
	m_SkillSlowMotionAttackEnable = false;
	m_TeleportEnable = false;
	m_PlayerDeath = false;
	m_TargetEnable = obj.m_TargetEnable;
	m_RunEnable = false;
	m_DashEnable = false;
	m_DashTime = 0.f;
	m_TelePortTime = 0.f;
	m_TeleportObj = obj.m_TeleportObj;
	m_TeleportPos = obj.m_TeleportPos;
	m_DeathAnimationTime = 0.f;
	m_SkillDestoryAllAttackEnable = false;
	m_SkillDestoryAllAttackTime = 0.f;

	// GameObj 에서, 해당 목록으로 복사되어 들어온다 
	auto iter = m_WidgetComponentList.begin();
	auto iterEnd = m_WidgetComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == HPWIDGET_COMPONENET)
		{
			m_HPBarWidget = (*iter);
		}
		if ((*iter)->GetName() == MPWIDGET_COMPONENET)
		{
			m_MPBarWidget = (*iter);
		}
		if ((*iter)->GetName() == NAMEWIDGET_COMPONENET)
		{
			m_NameWidget = (*iter);
		}
		if ((*iter)->GetName() == STEMINAMEWIDGET_COMPONENET)
		{
			m_SteminaBarWidget = (*iter);
		}
	}
}

CPlayer::~CPlayer()
{
	DeleteTeleportObj();
	m_HPBarWidget = nullptr;
	m_MPBarWidget = nullptr;
	m_NameWidget = nullptr;
}

void CPlayer::Start()
{
	CCharacter::Start();

	// Item
	CInput::GetInst()->SetCallback<CPlayer>("GetItem", KeyState_Down,
											this, &CPlayer::AcquireItem);
	CInput::GetInst()->SetCallback<CPlayer>("Pause", KeyState_Down,
											this, &CPlayer::Pause);
	CInput::GetInst()->SetCallback<CPlayer>("Resume", KeyState_Down,
											this, &CPlayer::Resume);

	// Gun 
	CInput::GetInst()->SetCallback<CPlayer>("ChangeGunToLight", KeyState_Down,
		this, &CPlayer::ChangeGunToLight);
	CInput::GetInst()->SetCallback<CPlayer>("ChangeGunToMedium", KeyState_Down,
		this, &CPlayer::ChangeGunToMedium);
	CInput::GetInst()->SetCallback<CPlayer>("ChangeGunToHeavy", KeyState_Down,
		this, &CPlayer::ChangeGunToHeavy);

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

	// Jump
	CInput::GetInst()->SetCallback<CPlayer>("Jump", KeyState_Push,
		this, &CPlayer::JumpKey);
	
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
	CInput::GetInst()->SetCallback<CPlayer>("DashUp", KeyState_Push,
		this, &CPlayer::DashUp);
	CInput::GetInst()->SetCallback<CPlayer>("DashDown", KeyState_Push,
		this, &CPlayer::DashDown);
	CInput::GetInst()->SetCallback<CPlayer>("DashLeft", KeyState_Push,
		this, &CPlayer::DashLeft);
	CInput::GetInst()->SetCallback<CPlayer>("DashRight", KeyState_Push,
		this, &CPlayer::DashRight);

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
	SetAnimationEndNotify<CPlayer>(PLAYER_TELEPORT, this, &CPlayer::ChangeMoveAnimation);

	// Attack
	AddAnimationNotify<CPlayer>(PLAYER_RIGHT_ATTACK, 2, this, &CPlayer::FireTarget);
	AddAnimationNotify<CPlayer>(PLAYER_LEFT_ATTACK, 2, this, &CPlayer::FireTarget);
	SetAnimationEndNotify<CPlayer>(PLAYER_RIGHT_ATTACK, this, &CPlayer::AttackEnd);
	SetAnimationEndNotify<CPlayer>(PLAYER_LEFT_ATTACK, this, &CPlayer::AttackEnd);

	// Death
	AddAnimationNotify<CPlayer>(PLAYER_LEFT_DEATH, 11, this, &CPlayer::Destroy);
	SetAnimationEndNotify<CPlayer>(PLAYER_LEFT_DEATH , this, &CPlayer::Destroy);
	AddAnimationNotify<CPlayer>(PLAYER_RIGHT_DEATH, 11, this, &CPlayer::Destroy);
	SetAnimationEndNotify<CPlayer>(PLAYER_RIGHT_DEATH, this, &CPlayer::Destroy);

	// Skill
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
	AddAnimation(PLAYER_RIGHT_IDLE,true, 2.f);
	AddAnimation(PLAYER_RIGHT_WALK, true, 1.f);
	AddAnimation(PLAYER_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(PLAYER_RIGHT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(PLAYER_RIGHT_RUN, true, 0.6f);

	// Left
	AddAnimation(PLAYER_LEFT_IDLE, true, 2.f);
	AddAnimation(PLAYER_LEFT_WALK, true, 1.f);
	AddAnimation(PLAYER_LEFT_ATTACK, false, 0.1f);
	AddAnimation(PLAYER_LEFT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(PLAYER_LEFT_RUN, true, 0.6f);

	// Skill
	AddAnimation("SkillSlowMotionAttack", false, 0.5f);
	AddAnimation("SkillDestoryAll", false, 0.5f);

	// Target
	AddAnimation("LucidNunNaTargetAttack", false, 0.6f);

	// Stun
	AddAnimation(PLAYER_LEFT_DEATH, false, 0.1f);
	AddAnimation(PLAYER_RIGHT_DEATH, false, 0.1f);

	// Stun
	AddAnimation(PLAYER_LEFT_HIT, true, 0.6f);
	AddAnimation(PLAYER_RIGHT_HIT, true, 0.6f);

	// Teleport
	AddAnimation(PLAYER_TELEPORT, false, 0.3f);

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
	m_HPBarWidget = CreateWidgetComponent(HPWIDGET_COMPONENET);
	CProgressBar *HPBar = m_HPBarWidget->CreateWidget<CProgressBar>("HPBar");
	HPBar->SetTexture("WorldHPBar", TEXT("CharacterHPBar.bmp"));
	m_HPBarWidget->SetPos(-25.f, -105.f);
	
	// MPBar
	m_MPBarWidget = CreateWidgetComponent(MPWIDGET_COMPONENET);
	CProgressBar *MPBar = m_MPBarWidget->CreateWidget<CProgressBar>("MPBar");
	MPBar->SetTexture("WorldMPBar", TEXT("CharacterMPBar.bmp"));
	m_MPBarWidget->SetPos(-25.f, -95.f);

	// SteminaBar
	m_SteminaBarWidget = CreateWidgetComponent(STEMINAMEWIDGET_COMPONENET);
	CProgressBar* SteminaBar = m_SteminaBarWidget->CreateWidget<CProgressBar>("SteminaBar");
	SteminaBar->SetTexture("WorldSteminaBar", TEXT("CharacterSteminaBar.bmp"));
	m_SteminaBarWidget->SetPos(-25.f, -85.f);


	// Name
	m_NameWidget = CreateWidgetComponent(NAMEWIDGET_COMPONENET);
	CUIText *NameText = m_NameWidget->CreateWidget<CUIText>("NameText");
	NameText->SetText(TEXT("Lucid"));
	NameText->SetTextColor(255, 0, 0);
	m_NameWidget->SetPos(-25.f, -125.f);

	// 수업용 : 물리 적용
	// SetGravityAccel();
	/*
	SetPhysicsSimulate(true);
	SetJumpVelocity(50.f);
	SetSideWallCheck(true);
	*/

	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CCharacter::Update(DeltaTime);

	// if (GetAsyncKeyState(VK_F1) & 0x8000)
	// SetAttackSpeed(0.5f);
	// if (m_DeathAnimationTime > 0.f) return;

	// Death
	if (m_CharacterInfo.HP <= 0 )
	{
		ChangeDeathAnimation();
		return;
	}

	MoveWithinWorldResolution();

	// Collide Monster 
	CGameObject *CollideMonster = MonsterCollisionCheck();
	if (CollideMonster)
		CollideMonsterBody(CollideMonster);

	// Skill Slow Motion
	if (m_SkillSlowMotionAttackEnable)
		SkillSlowMotionUpdate(DeltaTime);

	// Hp, Mp, Stemina
	AbilityUpdate(DeltaTime);

	// Run
	if (m_RunEnable)
		RunUpdate(DeltaTime);

	// Dash
	if (m_DashEnable)
		DashUpdate(DeltaTime);

	// Teleport
	if (m_TeleportEnable)
		TeleportUpdate(DeltaTime);

	// MPBar , HPBar
	CUICharacterStateHUD *State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");
	if (State)
	{
		// Ability 
		State->SetMPPercent(m_CharacterInfo.MP / (float)m_CharacterInfo.MPMax);
		State->SetHPPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);
		State->SetSteminaPercent(m_CharacterInfo.Stemina / (float)m_CharacterInfo.SteminaMax);

		// Gun
		CGun* Gun = nullptr;
		// Light 
		if (m_GunEquipment[EGunClass::Light])
		{
			Gun = m_GunEquipment[EGunClass::Light];
			State->SetLightGunPercent(Gun->GetGunLeftBullet() / (float)Gun->GetGunFullBullet());
		}
		else
			State->SetLightGunPercent(0.f);
		// Medium
		if (m_GunEquipment[EGunClass::Medium])
		{
			Gun = m_GunEquipment[EGunClass::Medium];
			State->SetMediumGunPercent(Gun->GetGunLeftBullet() / (float)Gun->GetGunFullBullet());
		}
		else
			State->SetMediumGunPercent(0.f);
		// Heavy
		if (m_GunEquipment[EGunClass::Heavy])
		{
			Gun = m_GunEquipment[EGunClass::Heavy];
			State->SetHeavyGunPercent(Gun->GetGunLeftBullet() / (float)Gun->GetGunFullBullet());
		}
		else
			State->SetHeavyGunPercent(0.f);
	}
	
	CProgressBar *MPBar = (CProgressBar *)m_MPBarWidget->GetWidget();
	MPBar->SetPercent(m_CharacterInfo.MP / (float)m_CharacterInfo.MPMax);

	CProgressBar *HPBar = (CProgressBar *)m_HPBarWidget->GetWidget();
	HPBar->SetPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);

	CProgressBar* SteminaBar = (CProgressBar*)m_SteminaBarWidget->GetWidget();
	SteminaBar->SetPercent(m_CharacterInfo.Stemina / (float)m_CharacterInfo.SteminaMax);

	// Dir Set toward Mouse Pos
	if (CheckCurrentAnimation(PLAYER_RIGHT_IDLE) || CheckCurrentAnimation(PLAYER_LEFT_IDLE))
		ChangeDirToMouse();

	
}

void CPlayer::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);
	if ((CheckCurrentAnimation(PLAYER_RIGHT_WALK) ||
		CheckCurrentAnimation(PLAYER_RIGHT_RUN) || 
		CheckCurrentAnimation(PLAYER_RIGHT_DASH)) &&
		m_Velocity.Length() == 0.f)
	{
		ChangeAnimation(PLAYER_RIGHT_IDLE);
	}
	if ((CheckCurrentAnimation(PLAYER_LEFT_WALK) ||
		CheckCurrentAnimation(PLAYER_LEFT_RUN) ||
		CheckCurrentAnimation(PLAYER_LEFT_DASH)) &&
		m_Velocity.Length() == 0.f)
	{
		ChangeAnimation(PLAYER_LEFT_IDLE);
	}

}

void CPlayer::Collision(float DeltaTime)
{
	CCharacter::Collision(DeltaTime);
}

void CPlayer::Render(HDC hDC)
{
	CCharacter::Render(hDC);

	Vector2 CameraPos = m_Scene->GetCamera()->GetPos();
	Vector2 ScreenPlayerPos = m_Pos - CameraPos;

	if (m_TargetEnable)
	{
		HPEN Pen = CGameManager::GetInst()->GetRedPen();
		HGDIOBJ PrevPen = SelectObject(hDC, Pen);
		
		HBRUSH Brush = CGameManager::GetInst()->GetRedBrush();
		HGDIOBJ PrevBrush = SelectObject(hDC, Brush);

		Vector2 CameraPos = m_Scene->GetCamera()->GetPos();
		Vector2 ScreenPlayerPos = m_Pos - CameraPos;
		Vector2 ScreenTargetPos = m_TargetPos - CameraPos;

		// LaserObj 가 충돌을 일으켰다면, LaserObj의 위치로 세팅
		// 그렇지 않다면, MousePos로 세팅 
		/*
		bool LaserCollide = m_LaserBulletObj->IsCollisionCheck();
		if (LaserCollide) // 특정 물체와 충돌했다면 
		{
			ScreenTargetPos = m_LaserBulletObj->GetPos() - CameraPos;
			m_LaserBulletObj->Destroy();
		}
		*/

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

	return Damage;
}

void CPlayer::AbilityUpdate(float DeltaTime)
{
	m_CharacterInfo.Stemina += 0.8f * DeltaTime;
	if (m_CharacterInfo.Stemina >= m_CharacterInfo.SteminaMax)
		m_CharacterInfo.Stemina = m_CharacterInfo.SteminaMax;

	if (m_CharacterInfo.HP <= m_CharacterInfo.HPMax)
		m_CharacterInfo.HP = m_CharacterInfo.HPMax;

	if (m_CharacterInfo.MP <= m_CharacterInfo.MPMax)
		m_CharacterInfo.MP += DeltaTime;
}


void CPlayer::ChangeGunToLight(float DeltaTime)
{
	if (m_GunEquipment[EGunClass::Light])
		m_CurrentGun = m_GunEquipment[EGunClass::Light];

	// Chnage Equipment 
	// CGun* ExistingGun = m_GunEquipment[GunClass];
	// m_GunEquipment[GunClass] = Gun;
	// Set Current Gun
	// m_CurrentGun = m_GunEquipment[GunClass];

	// Set Owner, Pos 
	// Gun->SetOwner(this);
	// 아래의 코드가 반드시 있어야 한다... 왜지 ?
	// Gun->SetPos(m_Pos);
	// Gun->SetOffset(m_Size.x * 0.1, -m_Size.y * 0.3);
	// Gun->SetPos(m_Size.x * 0.1, -m_Size.y * 0.3);

}

void CPlayer::ChangeGunToMedium(float DeltaTime)
{
	if (m_GunEquipment[EGunClass::Medium])
		m_CurrentGun = m_GunEquipment[EGunClass::Medium];
}

void CPlayer::ChangeGunToHeavy(float DeltaTime)
{
	if (m_GunEquipment[EGunClass::Heavy])
		m_CurrentGun = m_GunEquipment[EGunClass::Heavy];
}

void CPlayer::ChangeIdleAnimation()
{
	CCharacter::ChangeIdleAnimation();
	if (m_Dir.x < 0.f)
		ChangeAnimation(PLAYER_LEFT_IDLE);
	else
		ChangeAnimation(PLAYER_RIGHT_IDLE);
}

void CPlayer::ChangeDeathAnimation()
{
	if (m_Dir.x < 0)
		ChangeAnimation(PLAYER_LEFT_DEATH);
	else 
		ChangeAnimation(PLAYER_RIGHT_DEATH);
}

void CPlayer::ChangeRunAnimation()
{
	CCharacter::ChangeRunAnimation();
	if (m_Dir.x == -1.f)
		ChangeAnimation(PLAYER_LEFT_RUN);
	else
		ChangeAnimation(PLAYER_RIGHT_RUN);
}

void CPlayer::ChangeDashAnimation()
{
	if (m_HitEnable) return;
	if (m_Dir.x < 0.f)
		ChangeAnimation(PLAYER_LEFT_DASH);
	else
		ChangeAnimation(PLAYER_RIGHT_DASH);
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
		if (m_DashEnable)
			return;
	}
	CCharacter::Move(Dir, Speed);
}

void CPlayer::ChangeMoveAnimation()
{
	if (m_HitEnable) return;
	CCharacter::ChangeMoveAnimation();
	if (m_Dir.x < 0.f)
		ChangeAnimation(PLAYER_LEFT_WALK);
	else
		ChangeAnimation(PLAYER_RIGHT_WALK);
}

void CPlayer::JumpKey(float DeltaTime)
{
	Jump();
}

void CPlayer::RunLeft(float DeltaTime)
{
	Move(Vector2(-1.f, 0.f));
	RunStart();
}

void CPlayer::RunRight(float DeltaTime)
{
	Move(Vector2(1.f, 0.f));
	RunStart();
}

void CPlayer::RunUp(float DeltaTime)
{
	Move(Vector2(0.f, -1.f));
	RunStart();
}

void CPlayer::RunDown(float DeltaTime)
{
	Move(Vector2(0.f, 1.f));
	RunStart();
}

void CPlayer::RunUpdate(float DeltaTime)
{
	if (m_CharacterInfo.Stemina > 0)
		m_CharacterInfo.Stemina -= 3 * DeltaTime;
	if (m_CharacterInfo.Stemina <= 0)
		RunEnd();
}

void CPlayer::RunStart()
{
	if (m_CharacterInfo.Stemina <= 0.2 * m_CharacterInfo.SteminaMax || m_RunEnable) return;
	m_RunEnable = true;

	Vector2 PlayerBtm;
	float PlayerBtmy = m_Pos.y + (1.f - m_Pivot.y) * m_Size.y + m_Offset.y - m_Size.y / 2;
	if(m_Dir.x < 0 ) // 왼쪽 
		PlayerBtm = Vector2(m_Pos.x + (m_Pivot.x * m_Size.x),PlayerBtmy);
	else // 오른쪽 
		PlayerBtm = Vector2(m_Pos.x - (m_Pivot.x * m_Size.x),PlayerBtmy);


	CEffectDash *Hit = m_Scene->CreateObject<CEffectDash>(
		DASH_EFFECT,DASH_EFFECT,
		PlayerBtm,
		Vector2(5.f,5.f));

	m_Scene->GetSceneResource()->SoundPlay("Run");
	SetMoveSpeed(FAST_SPEED);

	// Animation
	ChangeRunAnimation();
}

void CPlayer::RunEnd()
{
	if (!m_RunEnable) return;
	m_RunEnable = false;
	SetMoveSpeed(NORMAL_SPEED);

	if (CheckCurrentAnimation(PLAYER_RIGHT_RUN))
	{
		ChangeAnimation(PLAYER_RIGHT_WALK);
	}
	if (CheckCurrentAnimation(PLAYER_LEFT_RUN))
	{
		ChangeAnimation(PLAYER_LEFT_WALK);
	}
}

void CPlayer::DashStart()
{
	if (m_DashEnable) return;
	if (m_DashEnable || m_CharacterInfo.Stemina < 0.5 * m_CharacterInfo.SteminaMax) return;

	// Dash Time 
	m_DashTime   = DASH_TIME;
	m_DashEnable = true;

	// Speed 
	SetMoveSpeed(DASH_SPEED);

	// Stemina
	if (m_CharacterInfo.Stemina >= 0.5f * m_CharacterInfo.SteminaMax)
		m_CharacterInfo.Stemina -= 0.5f * m_CharacterInfo.SteminaMax;

	// Sound 
	m_Scene->GetSceneResource()->SoundPlay("Dash");

	// Animation
	ChangeDashAnimation();
}

void CPlayer::DashEnd()
{
	if (!m_DashEnable) return;
	m_DashEnable = false;
	SetMoveSpeed(NORMAL_SPEED);

	if (CheckCurrentAnimation(PLAYER_RIGHT_DASH))
	{
		ChangeAnimation(PLAYER_RIGHT_WALK);
	}
	if (CheckCurrentAnimation(PLAYER_LEFT_DASH))
	{
		ChangeAnimation(PLAYER_LEFT_WALK);
	}
}

void CPlayer::DashUpdate(float DeltaTime)
{
	if (ObstacleCollisionCheck())
		CollideBounceBack(Vector2(-m_Dir.x, -m_Dir.y));
	if (m_DashTime >= 0)
		m_DashTime -= DeltaTime;
	if (m_DashTime <= 0)
		DashEnd();
}

void CPlayer::DashLeft(float DeltaTime)
{
	Move(Vector2(-1.f,0.f));
	DashStart();
}

void CPlayer::DashRight(float DeltaTime)
{
	Move(Vector2(1.f,0.f));
	DashStart();
}

void CPlayer::DashUp(float DeltaTime)
{
	Move(Vector2(0.f, -1.f));
	DashStart();
}

void CPlayer::DashDown(float DeltaTime)
{
	Move(Vector2(0.f, 1.f));
	DashStart();
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
	if (m_CharacterInfo.Stemina <= 0.95 * m_CharacterInfo.SteminaMax)
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
	if (!m_CurrentGun)
	{
		// 메세지
		return;
	}

	CGameManager::GetInst()->SetTimeScale(0.01f);
	SetTimeScale(100.f);
	m_SkillSlowMotionAttackEnable = true;

	// MP Decrease
	m_CharacterInfo.MP -= m_CharacterInfo.MPMax * 0.5f;

	for (float f = 0.0f; f < 2 * M_PI; f += M_PI / 9.0f) 
	{
		CSharedPtr<CBullet> Bullet = m_Scene->CreateObject<CBullet>("Bullet",
																	"SkillSlowMotionAttackBullet",
																	Vector2((m_Pos.x - m_Offset.x) + m_Size.Length() * cos(f), (m_Pos.y - m_Offset.y) + m_Size.Length() * sin(f)),
																	Vector2(m_Size.x, m_Size.y));

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

void CPlayer::SkillSlowMotionUpdate(float DeltaTime)
{
	m_SkillSlowMotionAttackTime += DeltaTime * m_TimeScale;

	if (m_SkillSlowMotionAttackTime >= SLOW_MOTION_ATTACK_TIME)
	{
		SetTimeScale(1.f);
		CGameManager::GetInst()->SetTimeScale(1.f);
		m_SkillSlowMotionAttackEnable = false;
		m_SkillSlowMotionAttackTime = 0.f;
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


Vector2 CPlayer::GetColliderPos()
{
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

void CPlayer::CollideBounceBack(Vector2 Dir)
{
	CCharacter::CollideBounceBack(Dir);
	DashEnd();
}

void CPlayer::CollideMonsterBody(CGameObject* CollideMonster)
{
	float MonsterDamage = (float)CollideMonster->GetAttack();
	// Damage Font
	CDamageFont* DamageFont = m_Scene->CreateObject<CDamageFont>("DamageFont", m_Pos);
	MonsterDamage -= m_CharacterInfo.Armor;
	if (MonsterDamage <= 0)
		MonsterDamage = 0;
	DamageFont->SetDamageNumber((int)MonsterDamage);
	SetDamage((float)MonsterDamage);

	// Bounc Back
	Vector2 MonsterDir = CollideMonster->GetDir() * m_MoveSpeed;
	CollideBounceBack(MonsterDir);
}

void CPlayer::ChangeHitAnimation()
{
	if (m_Dir.x < 0.f)
		ChangeAnimation(PLAYER_LEFT_HIT);
	else
		ChangeAnimation(PLAYER_RIGHT_HIT);
}

void CPlayer::CollisionBegin(CCollider *Src, CCollider *Dest, float DeltaTime)
{
}

void CPlayer::Teleport(float DeltaTime)
{

	if (!m_TeleportEnable || m_CharacterInfo.MP < 0.2 * m_CharacterInfo.MPMax)
		return;

	// Animation Settings
	ChangeAnimation(PLAYER_TELEPORT);

	m_Pos = m_TeleportPos;

	// m_TeleportEnable
	m_TeleportEnable = false;

	if (m_CharacterInfo.MP >= 0.2f * m_CharacterInfo.MPMax)
		m_CharacterInfo.MP -= 0.2f * m_CharacterInfo.MPMax;

	// TeleportMouse Cursor Animation
	DeleteTeleportObj();
}

void CPlayer::SetTeleportPos(float DeltaTime)
{
	if (m_CharacterInfo.MP < 0.2 * m_CharacterInfo.MPMax)
		return;

	m_TeleportEnable = true;

	Vector2 MousePos = CInput::GetInst()->GetMousePos();
	Vector2 CameraPos = m_Scene->GetCamera()->GetPos();
	m_TeleportPos = Vector2((float)(MousePos.x + CameraPos.x), (float)(MousePos.y + CameraPos.y));

	DeleteTeleportObj();
	m_TeleportObj = m_Scene->CreateObject<CTeleportMouse>(TELEPORT_MOUSE_PROTO, TELEPORT_MOUSE_PROTO,
														  m_TeleportPos);

	m_TelePortTime = TELEPORT_MOUSE_DISPLAY_TIME;
}

void CPlayer::DeleteTeleportObj()
{
	if (m_TeleportObj)
		m_TeleportObj->Destroy();
}

void CPlayer::TeleportUpdate(float DeltaTime)
{
	m_TelePortTime -= DeltaTime;
	if (m_TelePortTime <= 0.f)
		DeleteTeleportObj();
}

void CPlayer::AttackEnd()
{
	if (m_Dir.x > 0)
		ChangeAnimation(PLAYER_RIGHT_IDLE);
	else
		ChangeAnimation(PLAYER_LEFT_IDLE);
}

void CPlayer::RemoveTargetPos(float DeltaTime)
{
	m_TargetEnable = false;
}

void CPlayer::FireTarget()
{
	if (m_CurrentGun)
		m_CurrentGun->PlayerFire(m_TargetPos, (float)m_CharacterInfo.Attack);
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

	m_LaserBulletObj = m_Scene->CreateObject<CLaserObject>("Laser",
		"PlayerLaserProto",
		m_Pos,
		m_Size * Vector2(0.1f, 0.1f));
	m_LaserBulletObj->SetCollisionProfile("PlayerLaser");
	float Angle = GetAngle(m_Pos, MousePos);
	m_LaserBulletObj->SetDir(Angle);
	m_LaserBulletObj->SetDistance(Distance(m_Pos, MousePos));

	*/
	
	m_TargetEnable = true;
	m_TargetPos = Vector2((float)(MousePos.x + CameraPos.x), (float)(MousePos.y + CameraPos.y));
}

void CPlayer::BulletFireTarget(float DeltaTime)
{
	if (!m_CurrentGun)
	{
		// 경고 메세지 
		return;
	}
	Vector2 PlayerDir = m_Dir;

	Vector2 MousePos = CInput::GetInst()->GetMousePos();
	Vector2 CameraPos = m_Scene->GetCamera()->GetPos();
	m_TargetPos = Vector2((float)(MousePos.x + CameraPos.x), (float)(MousePos.y + CameraPos.y));

	if (m_Dir.x > 0)
		ChangeAnimation(PLAYER_RIGHT_ATTACK);
	else
		ChangeAnimation(PLAYER_LEFT_ATTACK);
}

void CPlayer::CharacterDestroy()
{
	m_DeathAnimationTime = DEATH_TIME;
	if (m_Dir.x <= 0.f)
		ChangeAnimation("LucidNunNaLeftDeath");
	else
		ChangeAnimation("LucidNunNaRightDeath");
}

void CPlayer::AcquireItem(float DeltaTime)
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		// Potion
		CPotion *Potion = (*iter)->IsCollisionWithPotion();
		if (Potion)
		{
			EPotion_Type PType = Potion->GetPotionType();
			//if (PType == EPotion_Type::HP)
				//m_CharacterInfo.HP = m_CharacterInfo.HPMax;
			//else
				//m_CharacterInfo.MP = m_CharacterInfo.MPMax;
			// Potion->Destroy();
			break;
			return;
		}

		// Weapon
		CGun* Gun = (*iter)->IsCollisionWithGun();
		if (Gun)
		{
			std::string ProtoTypeName = Gun->GetProtoTypeName();
			// Equip(Gun->Clone());
			// Have to Add Newly Cloned Gun to Scene 
			// So that it can be update within Scene.cpp
			Equip(m_Scene->CreateObject<CGun>(ProtoTypeName,ProtoTypeName));
			break;
		}
	}
}



CGun* CPlayer::Equip(CGun* Gun)
{
	CGun* ExitingGun   = CCharacter::Equip(Gun);
	CCollider* GunBody = m_CurrentGun->FindCollider("Body");
	GunBody->SetCollisionProfile("PlayerAttack");
	return ExitingGun;
}

void CPlayer::ChangeDirToMouse()
{
	Vector2 MousePos = CInput::GetInst()->GetMousePos();
	Vector2 CameraPos = m_Scene->GetCamera()->GetPos();
	Vector2 MousePlayerPosDiff = m_Pos - (MousePos + CameraPos);
	float Angle = GetAngle(m_Pos, MousePos);
	SetDir(Angle);
	// Animation Change
	if (MousePlayerPosDiff.x >= 0)
	{
		ChangeAnimation(PLAYER_LEFT_IDLE);
		m_Dir.x = -1;
	}
	else
	{
		ChangeAnimation(PLAYER_RIGHT_IDLE);
		m_Dir.x = 1;
	}
}

