#include "../Timer.h"
#include "../Input.h"
#include "../GameManager.h"
// Object
#include "Player.h"
#include "PlayerClone.h"
#include "EffectHit.h"
#include "EffectText.h"
#include "EffectShield.h"
#include "EffectGrenade.h"
#include "EffectShieldStart.h"
#include "EffectDash.h"
#include "EffectDoorAbove.h"
#include "Coin.h"
#include "Npc.h"
#include "EffectReload.h"
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
#include "../UI/UIGunStateHUD.h"
#include "../UI/ProgressBar.h"
#include "../UI/UIText.h"
#include "../UI/UIPause.h"
#include "../UI/UIGameOver.h"

EChar_Type CPlayer::m_CharType = EChar_Type::Ass;
CharacterInfo CPlayer::m_SelectedCharacterInfo = {};

// Static
CPlayer::CPlayer() : m_RunEnable(false),
					 m_DashEnable(false),
					 m_TargetEnable(false),
					 m_DashTime(0.f),
					 m_TeleportEnable(false),
					 m_TelePortTime(0.f),
					 m_TeleportObj{},
					 m_TeleportPos(Vector2(0.f, 0.f)),
					 m_DeathAnimationTime(0.f),
					m_MonsterCollideTime(0.f),
					 m_UIPause(nullptr),
					 m_PlayerDeath(false),
					m_HPBarWidget(nullptr),
					m_MPBarWidget(nullptr),
					m_NameWidget(nullptr),
					m_SteminaBarWidget(nullptr),
					m_HpPotionInv(0),
					m_MpPotionInv(0),
					m_ShieldInv(0),
					m_SkillTime(0.f),
					m_SkillTimeMax(20.f),
					m_MonsterKilled(0),
					m_SkillEnable(false),
					m_DeathWidgetCreate(false),
					m_WallCollision(false),
					m_State1End(false),
					m_State2End(false)
			
{

	m_ObjType = EObject_Type::Player;
	m_TimeScale = 1.f;
	m_CharacterInfo = m_SelectedCharacterInfo;
	m_MoveSpeed = m_SelectedCharacterInfo.MoveSpeed;
	m_RunSpeed = m_SelectedCharacterInfo.MoveSpeed * 1.5f;
	m_NormalSpeed = m_SelectedCharacterInfo.MoveSpeed;
	m_DashSpeed = m_SelectedCharacterInfo.MoveSpeed * 2.5f;
}

CPlayer::CPlayer(const CPlayer &obj) : CCharacter(obj)
{
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
	
	m_CharacterInfo.Attack = m_SelectedCharacterInfo.Attack;
	m_CharacterInfo.Armor  = m_SelectedCharacterInfo.Armor;
	m_MoveSpeed = m_SelectedCharacterInfo.MoveSpeed;
	m_RunSpeed = m_SelectedCharacterInfo.MoveSpeed * 1.5f;
	m_NormalSpeed = m_SelectedCharacterInfo.MoveSpeed;
	m_DashSpeed = m_SelectedCharacterInfo.MoveSpeed * 2.5f;

	m_HpPotionInv = obj.m_HpPotionInv;
	m_MpPotionInv = obj.m_MpPotionInv;
	m_ShieldInv   = obj.m_ShieldInv;

	m_SkillTime    = 0.f;
	m_SkillTimeMax = 20.f;
	m_SkillEnable  = false;

	m_MonsterKilled = obj.m_MonsterKilled;

	m_WallCollision = false;

	if (m_CurrentGun)
	{
		if (m_CurrentGun->GetGunType() == EGun_Type::Pistol)
		{
			m_FireTimeMax = FIREMAX_TIME;
			m_CharacterInfo.Attack += m_CurrentGun->GetGunDamage();
		}
	}

	m_UIPause = nullptr;
	m_DeathWidgetCreate = false;

	m_State1End = obj.m_State1End;
	m_State2End = obj.m_State2End;
	
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
	// 충돌체 지워주기 
	m_ColliderList.clear();
}

CPlayer::~CPlayer()
{
}

void CPlayer::UpdateHpPotionInv(CUICharacterStateHUD* const State)
{
	// MPBar , HPBar
	int FullT = m_HpPotionInv / 10, FullO = m_HpPotionInv % 10;

	if (FullT != 0)
		State->SetHpInvTenWidget(FullT);
	else
		State->SetHpInvTenRenderEnable(false);
	if (FullT != 0 || FullO != 0)
		State->SetHpInvOneWidget(FullO);
	else
		State->SetHpInvOneRenderEnable(false);
}

void CPlayer::UpdateMpPotionInv(CUICharacterStateHUD* const State)
{
	// MPBar , HPBar
	int FullT = m_MpPotionInv / 10, FullO = m_MpPotionInv % 10;

	if (FullT != 0)
		State->SetMpInvTenWidget(FullT);
	else
		State->SetMpInvTenRenderEnable(false);
	if (FullT != 0 || FullO != 0)
		State->SetMpInvOneWidget(FullO);
	else
		State->SetMpInvOneRenderEnable(false);
}

void CPlayer::UpdateShieldInv(CUICharacterStateHUD* const State)
{
	// MPBar , HPBar
	int FullT = m_ShieldInv / 10, FullO = m_ShieldInv % 10;

	if (FullT != 0)
		State->SetShieldInvTenWidget(FullT);
	else
		State->SetShieldInvTenRenderEnable(false);
	if (FullT != 0 || FullO != 0)
		State->SetShieldInvOneWidget(FullO);
	else
		State->SetShieldInvOneRenderEnable(false);
}

void CPlayer::UseHpPotionInv(float DeltaTime)
{
	CUICharacterStateHUD* State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");
	if (m_HpPotionInv > 0)
	{
		m_HpPotionInv -= 1;
		m_CharacterInfo.HP += 2000;
		if(m_CharacterInfo.HP > m_CharacterInfo.HPMax)
			m_CharacterInfo.HP = m_CharacterInfo.HPMax;
		UpdateHpPotionInv(State);
		CEffectShieldStart* ShieldStart = m_Scene->CreateObject<CEffectShieldStart>(
			"ShieldStart",
			SHIELD_START_PROTO, Vector2(m_Pos.x, m_Pos.y - m_Size.y * 0.5f));
		ShieldStart->SetOwner(this);
		ShieldStart->SetLifeTime(0.2f);

		m_Scene->GetSceneResource()->SoundPlay("ItemUse");
	}
}

void CPlayer::UseMpPotionInv(float DeltaTime)
{
	CUICharacterStateHUD* State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");
	if (m_MpPotionInv > 0)
	{
		m_MpPotionInv -= 1;
		m_CharacterInfo.MP += 100;
		if (m_CharacterInfo.MP > m_CharacterInfo.MPMax)
			m_CharacterInfo.MP = m_CharacterInfo.MPMax;
		UpdateMpPotionInv(State);
		CEffectShieldStart* ShieldStart = m_Scene->CreateObject<CEffectShieldStart>(
			"ShieldStart",
			SHIELD_START_PROTO, Vector2(m_Pos.x, m_Pos.y - m_Size.y * 0.5f));
		ShieldStart->SetOwner(this);
		ShieldStart->SetLifeTime(0.2f);

		m_Scene->GetSceneResource()->SoundPlay("ItemUse");
	}
}

void CPlayer::UseShieldInv(float DeltaTime)
{
	CUICharacterStateHUD* State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");
	if (m_ShieldInv > 0 )
	{
		m_ShieldInv -= 1;
		m_ShieldEnable = true;
		m_ShieldTime = m_ShieldTimeMax;
		UpdateShieldInv(State); 

		CEffectShieldStart* ShieldStart = m_Scene->CreateObject<CEffectShieldStart>(
			"ShieldStart",
			SHIELD_START_PROTO, Vector2(m_Pos.x,m_Pos.y-m_Size.y*0.5f));
		ShieldStart->SetOwner(this);
		ShieldStart->SetLifeTime(1.f);

		m_Scene->GetSceneResource()->SoundPlay("ItemUse");
	}
}
void CPlayer::ShieldUpdate(float DeltaTime)
{
	if (m_ShieldTime > 0.f && m_ShieldEnable)
	{
		m_ShieldTime -= DeltaTime;
		if (m_ShieldTime <= 0.f)
		{
			m_ShieldEnable = false;
		}
		// Character State
		CUICharacterStateHUD* State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");
		State->SetShieldPercent(m_ShieldTime / (float)m_ShieldTimeMax);
	}
}
void CPlayer::Start()
{
	CCharacter::Start();

	SetAnimName();

	// Change Animation To Idle
	ChangeIdleAnimation();

	// Update HUD
	CUICharacterStateHUD* State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");
	UpdateHpPotionInv(State);
	UpdateMpPotionInv(State);
	UpdateShieldInv(State);

	CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(60.f, 65.f);
	Body->SetOffset(0.f, -42.5f);
	Body->SetCollisionProfile("Player");
	
	// Item
	CInput::GetInst()->SetCallback<CPlayer>("GetItem", KeyState_Down,
											this, &CPlayer::AcquireItem);
	CInput::GetInst()->SetCallback<CPlayer>("BuyItem", KeyState_Down,
		this, &CPlayer::BuyItem);

	// Pause, Resume
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
	CInput::GetInst()->SetCallback<CPlayer>("Reload", KeyState_Down,
		this, &CPlayer::ReloadGun);

	// 1) Slow Motion
	CInput::GetInst()->SetCallback<CPlayer>("ActivateSkill", KeyState_Down,
											this, &CPlayer::ActivateSkills);
	// 2) Destroy All
	// CInput::GetInst()->SetCallback<CPlayer>("SkillDestoryAll", KeyState_Down,
	//										this, &CPlayer::SkillDestroyAllAttack);

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
	
	// Inv
	CInput::GetInst()->SetCallback<CPlayer>("UseHpPotion", KeyState_Down,
		this, &CPlayer::UseHpPotionInv);
	CInput::GetInst()->SetCallback<CPlayer>("UseMpPotion", KeyState_Down,
		this, &CPlayer::UseMpPotionInv);
	CInput::GetInst()->SetCallback<CPlayer>("UseShield", KeyState_Down,
		this, &CPlayer::UseShieldInv);

}

void CPlayer::SetNotifyFunctions()
{
	// Teleport
	std::string AnimName = m_mapAnimName.find(PLAYER_TELEPORT)->second;
	SetAnimationEndNotify<CPlayer>(AnimName, this, &CPlayer::ChangeMoveAnimation);

	// Attack
	AnimName = m_mapAnimName.find(PLAYER_RIGHT_ATTACK)->second;
	AddAnimationNotify<CPlayer>(AnimName, 2, this, &CPlayer::FireTarget);
	AnimName = m_mapAnimName.find(PLAYER_LEFT_ATTACK)->second;
	AddAnimationNotify<CPlayer>(AnimName, 2, this, &CPlayer::FireTarget);
	AnimName = m_mapAnimName.find(PLAYER_RIGHT_ATTACK)->second;
	SetAnimationEndNotify<CPlayer>(AnimName, this, &CPlayer::AttackEnd);
	AnimName = m_mapAnimName.find(PLAYER_LEFT_ATTACK)->second;
	SetAnimationEndNotify<CPlayer>(AnimName, this, &CPlayer::AttackEnd);

	// Death
	AnimName = m_mapAnimName.find(PLAYER_LEFT_DEATH)->second;
	// AddAnimationNotify<CPlayer>(AnimName, 11, this, &CPlayer::CharacterDestroy);
	SetAnimationEndNotify<CPlayer>(AnimName, this, &CPlayer::CharacterDestroy);
	AnimName = m_mapAnimName.find(PLAYER_RIGHT_DEATH)->second;
	// AddAnimationNotify<CPlayer>(AnimName, 11, this, &CPlayer::CharacterDestroy);
	SetAnimationEndNotify<CPlayer>(AnimName, this, &CPlayer::CharacterDestroy);
}

bool CPlayer::Init()
{
	if (!CCharacter::Init())
		return false;

	SetPivot(0.5f, 1.f);

	// Animation ---
	CreateAnimation();
	AddAssAnimName();
	AddJimmyAnimName();
	AddBiuAnimName();
	AddPinkyAnimName();
	AddPunnyAnimName();
	AddRaffAnimName();

	// Shield
	AddAnimation(SHIELD_START, false, 0.5f);

	// Skill
	AddAnimation("SkillSlowMotionAttack", false, 0.5f);
	AddAnimation("SkillDestoryAll", false, 0.5f);

	// Target
	AddAnimation("LucidNunNaTargetAttack", false, 0.6f);

	// AnimName + NotifyFunctions
	SetAnimName();
	SetNotifyFunctions();

	// Set Initial Animation As Right Idle
	std::string Anim = m_mapAnimName.find(PLAYER_RIGHT_IDLE)->second;
	ChangeAnimation(Anim);

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
	SetCharName();


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
	
	// Wall Move
	m_WallCollision = PreventWallMove();
	if (m_WallCollision)
	{
		if (m_DashEnable)
			CollideBounceBack(Vector2(-m_Dir.x, -m_Dir.y));
		else // Run or Move
			SetMoveSpeed(200.f);
	}
	MoveWithinWorldResolution();
	
	// Death
	if (m_CharacterInfo.HP < 0 )
	{
		m_PlayerDeath = true;
		m_ColliderList.clear();
		ChangeDeathAnimation();
		return;
	}
	
	// Collide Monster 
	CGameObject *CollideMonster = MonsterCollisionCheck();

	if (CollideMonster) CollideMonsterBody(CollideMonster);

	// Hp, Mp, Stemina
	AbilityUpdate(DeltaTime);
	
	// Run
	if (m_RunEnable) RunUpdate(DeltaTime);

	// Dash
	if (m_DashEnable) DashUpdate(DeltaTime);

	// Teleport
	if (m_TeleportEnable) TeleportUpdate(DeltaTime);

	// MPBar , HPBar
	CUICharacterStateHUD *State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");
	if (State) AbilityStateUIUpdate(State);
	CProgressBar *MPBar = (CProgressBar *)m_MPBarWidget->GetWidget();
	MPBar->SetPercent(m_CharacterInfo.MP / (float)m_CharacterInfo.MPMax);

	CProgressBar *HPBar = (CProgressBar *)m_HPBarWidget->GetWidget();
	HPBar->SetPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);

	CProgressBar* SteminaBar = (CProgressBar*)m_SteminaBarWidget->GetWidget();
	SteminaBar->SetPercent(m_CharacterInfo.Stemina / (float)m_CharacterInfo.SteminaMax);

	// Gold 
	CurGoldNumUpdate(State);

	// Gun 
	CUIGunStateHUD *GunState = m_Scene->FindUIWindow<CUIGunStateHUD>("GunStateHUD");
	if (GunState) GunStateUIUpdate(GunState);

	// Dir Set toward Mouse Pos
	std::string RIdleAnim = m_mapAnimName.find(PLAYER_RIGHT_IDLE)->second;
	std::string LIdleAnim = m_mapAnimName.find(PLAYER_LEFT_IDLE)->second;
	if (CheckCurrentAnimation(RIdleAnim) || CheckCurrentAnimation(LIdleAnim))
		ChangeDirToMouse();

	// Gun Bullet Update
	GunCurBulletNumUpdate();

	// Update m_MonsterCollideTime

	if (m_MonsterCollideTime >= 0.f) m_MonsterCollideTime -= DeltaTime;
	// Shield Update
	ShieldUpdate(DeltaTime);
	// Skill Update
	SkillTimeUpdate(DeltaTime);
	// Coin
	AutoAcquireCoin(DeltaTime);
	// Gun
	GunTimeUpdate(DeltaTime);
}

void CPlayer::PostUpdate(float DeltaTime)
{
	std::string RIdleAnim = m_mapAnimName.find(PLAYER_RIGHT_IDLE)->second;
	std::string RWalkAnim = m_mapAnimName.find(PLAYER_RIGHT_WALK)->second;
	std::string RRunAnim  = m_mapAnimName.find(PLAYER_RIGHT_RUN)->second;
	std::string RDashAnim = m_mapAnimName.find(PLAYER_RIGHT_DASH)->second;
	CCharacter::PostUpdate(DeltaTime);
	if ((CheckCurrentAnimation(RWalkAnim) ||
		CheckCurrentAnimation(RRunAnim) ||
		CheckCurrentAnimation(RDashAnim)) &&
		m_Velocity.Length() == 0.f)
	{
		ChangeAnimation(RIdleAnim);
	}

	std::string LIdleAnim = m_mapAnimName.find(PLAYER_LEFT_IDLE)->second;
	std::string LWalkAnim = m_mapAnimName.find(PLAYER_LEFT_WALK)->second;
	std::string LRunAnim  = m_mapAnimName.find(PLAYER_LEFT_RUN)->second;
	std::string LDashAnim = m_mapAnimName.find(PLAYER_LEFT_DASH)->second;
	if ((CheckCurrentAnimation(LWalkAnim) ||
		CheckCurrentAnimation(LRunAnim) ||
		CheckCurrentAnimation(LDashAnim)) &&
		m_Velocity.Length() == 0.f)
	{
		ChangeAnimation(LIdleAnim);
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

int CPlayer::SetDamage(int Damage)
{
	Damage = (int)CCharacter::SetDamage(Damage);
	CUICharacterStateHUD *State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");
	if (State)
		State->SetHPPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);
	CProgressBar *HPBar = (CProgressBar *)m_HPBarWidget->GetWidget();
	HPBar->SetPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);

	return Damage;
}

void CPlayer::AbilityUpdate(float DeltaTime)
{
	m_CharacterInfo.Stemina += 1.5f * DeltaTime;
	if (m_CharacterInfo.Stemina >= m_CharacterInfo.SteminaMax)
		m_CharacterInfo.Stemina = m_CharacterInfo.SteminaMax;

}

void CPlayer::AbilityStateUIUpdate(CUICharacterStateHUD* State)
{
	State->SetMPPercent(m_CharacterInfo.MP / (float)m_CharacterInfo.MPMax);
	State->SetHPPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);
	State->SetSteminaPercent(m_CharacterInfo.Stemina / (float)m_CharacterInfo.SteminaMax);
}


void CPlayer::GunTimeUpdate(float DeltaTime)
{
	if(m_FireTime < m_FireTimeMax)
		m_FireTime += DeltaTime;
}

void CPlayer::ChangeGunToLight(float DeltaTime)
{
	if (m_GunEquipment[EGunClass::Light])
	{
		if(m_CurrentGun) m_CurrentGun->Destroy();
		m_CurrentGun = m_GunEquipment[EGunClass::Light];

		m_FireTime    = m_CurrentGun->GetFireTime();
		m_FireTimeMax = m_CurrentGun->GetFireTimeMax();
		if (m_CurrentGun->GetGunType() == EGun_Type::Pistol) 
			m_FireTimeMax = FIREMAX_TIME;
	}
}

void CPlayer::ChangeGunToMedium(float DeltaTime)
{
	if (m_GunEquipment[EGunClass::Medium])
	{
		if(m_CurrentGun) m_CurrentGun->Destroy();
		m_CurrentGun = m_GunEquipment[EGunClass::Medium];

		m_FireTime = m_CurrentGun->GetFireTime();
		m_FireTimeMax = m_CurrentGun->GetFireTimeMax();
		if (m_CurrentGun->GetGunType() == EGun_Type::Pistol) 
			m_FireTimeMax = FIREMAX_TIME;
	}
}

void CPlayer::ChangeGunToHeavy(float DeltaTime)
{
	if (m_GunEquipment[EGunClass::Heavy])
	{
		if(m_CurrentGun) m_CurrentGun->Destroy();
		m_CurrentGun = m_GunEquipment[EGunClass::Heavy];

		m_FireTime = m_CurrentGun->GetFireTime();
		m_FireTimeMax = m_CurrentGun->GetFireTimeMax();
		if (m_CurrentGun->GetGunType() == EGun_Type::Pistol) 
			m_FireTimeMax = FIREMAX_TIME;
	}
}

void CPlayer::ReloadGun(float DelatTime)
{
	if (m_CurrentGun)
	{
		CEffectReload* ReloadAnim = m_Scene->CreateObject<CEffectReload>(
			"ReloadEffect",
			RELOAD_PROTO,
			Vector2(m_Pos.x, m_Pos.y + m_Size.y * 0.1f), Vector2(10.f, 10.f));
		ReloadAnim->SetOwner(this);

		m_Scene->GetSceneResource()->SoundPlay("Reload");
	}
}

void CPlayer::GunStateUIUpdate(CUIGunStateHUD* State)
{
	// 1) Gun Bar Setting  ---
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

	// 2) Gun Image Setting  ---
	if (m_CurrentGun)
		State->SetCurrentGunImage(m_CurrentGun->GetRightTextureName());

	bool SetThirdGun = false;
	for (int i = 0; i < EGunClass::End; i++)
	{
		if (!m_GunEquipment[i]) continue;
		if (m_GunEquipment[i] == m_CurrentGun) continue;
		if (!SetThirdGun)
		{
			State->SetSecondGunImage(m_GunEquipment[i]->GetRightTextureName());
			SetThirdGun = true;
		}
		else
			State->SetThirdGunImage(m_GunEquipment[i]->GetRightTextureName());
	}
}

void CPlayer::GunCurBulletNumUpdate()
{
	if (m_CurrentGun)
	{
		// MPBar , HPBar
		CUICharacterStateHUD* State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");
		int FullBullet = m_CurrentGun->GetGunFullBullet();
		int FullH = FullBullet / 100, FullT = (FullBullet % 100) / 10, FullO = FullBullet % 10;
		if (FullH != 0)
			State->SetFullBulletHundredWidget(FullH);
		else
			State->SetFullBulletHundredRenderEnable(false);
		if (FullH != 0 || FullT != 0)
			State->SetFullBulletTenWidget(FullT);
		else
			State->SetFullBulletTenRenderEnable(false);
		if (FullH != 0 || FullT != 0 || FullO != 0)
			State->SetFullBulletOneWidget(FullO);
		else
			State->SetFullBulletOneRenderEnable(false);

		int CurBullet = m_CurrentGun->GetGunLeftBullet();
		int CurH = CurBullet / 100, CurT = (CurBullet % 100) / 10, CurO = CurBullet % 10;
		if (CurH != 0)
			State->SetBulletHundredWidget(CurH);
		else
			State->SetBulletHundredRenderEnable(false);
		if (CurH != 0 || CurT != 0)
			State->SetBulletTenWidget(CurT);
		else
			State->SetBulletTenRenderEnable(false);
		if (CurH != 0 || CurT != 0 || CurO != 0)
			State->SetBulletOneWidget(CurO);
		else
			State->SetBulletOneRenderEnable(false);
	}
}

void CPlayer::ChangeIdleAnimation()
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

void CPlayer::ChangeDeathAnimation()
{
	if (!m_PlayerDeath)
	{
		ChangeIdleAnimation();
		return;
	}
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

void CPlayer::ChangeRunAnimation()
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

void CPlayer::ChangeDashAnimation()
{
	if (m_HitEnable) return;
	if (m_Dir.x < 0.f)
	{
		Vector2 DashPos = Vector2(m_Pos.x + m_Size.x * 0.2f,m_Pos.y - m_Size.y * 0.7f);
		CEffectDash* EffectDash = m_Scene->CreateObject<CEffectDash>(
			"PlayerDashEffect",
			EFFECT_DASH_PROTO, DashPos);
		std::string Anim = m_mapAnimName.find(PLAYER_LEFT_DASH)->second;
		ChangeAnimation(Anim);
	}
	else
	{
		Vector2 DashPos = Vector2(m_Pos.x - m_Size.x * 0.2f, m_Pos.y - m_Size.y * 0.7f);
		CEffectDash* EffectDash = m_Scene->CreateObject<CEffectDash>(
			"PlayerDashEffect",
			EFFECT_DASH_PROTO, DashPos);
		std::string Anim = m_mapAnimName.find(PLAYER_RIGHT_DASH)->second;
		ChangeAnimation(Anim);
	}
}

void CPlayer::CurGoldNumUpdate(class CUICharacterStateHUD* State)
{
	if (m_CharacterInfo.Gold >= 1000)
		m_CharacterInfo.Gold = 999;
	int Gold = m_CharacterInfo.Gold;
	int FullH = Gold / 100, FullT = (Gold % 100) / 10, FullO = Gold % 10;
	if (FullH != 0)
		State->SetGoldHundredWidget(FullH);
	else
		State->SetGoldHundredRenderEnable(false);
	if (FullH != 0 || FullT != 0)
		State->SetGoldTenWidget(FullT);
	else
		State->SetGoldTenRenderEnable(false);
	if (FullH != 0 || FullT != 0 || FullO != 0)
		State->SetGoldOneWidget(FullO);
	else
		State->SetGoldOneRenderEnable(false);
}

void CPlayer::MoveUp(float DeltaTime)
{
	RunEnd();
	if(!m_WallCollision) SetMoveSpeed(m_NormalSpeed);
	Move(Vector2(0.f, -1.f));
	ChangeMoveAnimation();
}

void CPlayer::MoveDown(float DeltaTime)
{
	RunEnd();
	if(!m_WallCollision) SetMoveSpeed(m_NormalSpeed);
	Move(Vector2(0.f, 1.f));
	ChangeMoveAnimation();
}

void CPlayer::MoveLeft(float DeltaTime)
{
	RunEnd();
	if(!m_WallCollision) SetMoveSpeed(m_NormalSpeed);
	Move(Vector2(-1.f, 0.f));
	ChangeMoveAnimation();
}

void CPlayer::MoveRight(float DeltaTime)
{
	RunEnd();
	if(!m_WallCollision) SetMoveSpeed(m_NormalSpeed);
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
	if (m_PlayerDeath) return;
	CCharacter::Move(Dir);
}

void CPlayer::Move(const Vector2 &Dir, float Speed)
{
	if (ObstacleCollisionCheck())
	{
		if (m_DashEnable)
			return;
	}
	if(m_PlayerDeath) return;
	CCharacter::Move(Dir, Speed);
}

void CPlayer::HitMove()
{
	if (m_PlayerDeath) return;
	CCharacter::HitMove();
}

void CPlayer::ChangeMoveAnimation()
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
	if(m_Dir.x < 0 ) // Left
		PlayerBtm = Vector2(m_Pos.x + (m_Pivot.x * m_Size.x),PlayerBtmy);
	else // Right
		PlayerBtm = Vector2(m_Pos.x - (m_Pivot.x * m_Size.x),PlayerBtmy);


	CEffectDash *Hit = m_Scene->CreateObject<CEffectDash>(
		DASH_EFFECT,DASH_EFFECT,
		PlayerBtm,
		Vector2(5.f,5.f));

	m_Scene->GetSceneResource()->SoundPlay("Run");
	SetMoveSpeed(m_RunSpeed);

	// Animation
	ChangeRunAnimation();
}

void CPlayer::RunEnd()
{
	if (!m_RunEnable) return;
	m_RunEnable = false;
	SetMoveSpeed(m_NormalSpeed);

	std::string RightRunAnim = m_mapAnimName.find(PLAYER_RIGHT_RUN)->second;
	std::string LeftRunAnim  = m_mapAnimName.find(PLAYER_LEFT_RUN)->second;
	if (CheckCurrentAnimation(RightRunAnim))
	{
		std::string Anim = m_mapAnimName.find(PLAYER_RIGHT_WALK)->second;
		ChangeAnimation(Anim);
	}
	if (CheckCurrentAnimation(LeftRunAnim))
	{
		std::string Anim = m_mapAnimName.find(PLAYER_LEFT_WALK)->second;
		ChangeAnimation(Anim);
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
	SetMoveSpeed(m_DashSpeed);
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
	SetMoveSpeed(m_NormalSpeed);

	std::string RightDashAnim = m_mapAnimName.find(PLAYER_RIGHT_DASH)->second;
	std::string LeftDashAnim  = m_mapAnimName.find(PLAYER_LEFT_DASH)->second;
	if (CheckCurrentAnimation(RightDashAnim))
	{
		std::string Anim = m_mapAnimName.find(PLAYER_RIGHT_WALK)->second;
		ChangeAnimation(Anim);
	}
	if (CheckCurrentAnimation(LeftDashAnim))
	{
		std::string Anim = m_mapAnimName.find(PLAYER_LEFT_WALK)->second;
		ChangeAnimation(Anim);
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
	m_Scene->SetPauseUIVisbility(true);
}

void CPlayer::Resume(float DeltaTime)
{
	CGameManager::GetInst()->SetTimeScale(1.f);
}

void CPlayer::SkillMultipleBulletAttack(float DeltaTime)
{
	CGameManager::GetInst()->SetTimeScale(0.01f);
	SetTimeScale(100.f);
	m_SkillTime = SLOW_MOTION_ATTACK_TIME;
	m_SkillEnable = true;
	m_CurrentGun->SkillSlowMotionAttack();
}

void CPlayer::SkillDestroyAllAttack(float DeltaTime)
{
	m_Scene->DestroyAllAttackObjects();
	m_SkillTime = SLOW_MOTION_ATTACK_TIME;
	m_SkillEnable = true;
	CGameManager::GetInst()->SetTimeScale(0.01f);
	SetTimeScale(100.f);
}

void CPlayer::SkillIncAbility()
{
	m_SkillTime = m_SkillTimeMax;
	m_SkillEnable = true;
	// Attack Inc
	m_CharacterInfo.Attack *= 2;
	// Armor Inc
	m_CharacterInfo.Armor *= 2;
	// Speed Inc
	m_MoveSpeed *= 1.5f;

	CEffectShieldStart* ShieldStart = m_Scene->CreateObject<CEffectShieldStart>(
		"ShieldStart",
		SHIELD_START_PROTO, Vector2(m_Pos.x, m_Pos.y - m_Size.y * 0.5f));
	ShieldStart->SetOwner(this);
	ShieldStart->SetLifeTime(m_SkillTimeMax);
}

CGameObject *CPlayer::FindClosestTarget(Vector2 PlayerPos)
{
	return m_Scene->FindClosestMonster(PlayerPos);
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
	CMonster* ColliderMonster = (CMonster*)CollideMonster;
	int MonsterDamage = ColliderMonster->GetAttack();
	// this
	// Damage Font
	if (m_MonsterCollideTime <= 0.f)
	{
		CDamageFont* DamageFont = m_Scene->CreateObject<CDamageFont>("DamageFont", DAMAGEFONT_PROTO, m_Pos);
		MonsterDamage -= m_CharacterInfo.Armor;
		if (MonsterDamage <= 0) MonsterDamage = 0;
		DamageFont->SetDamageNumber((int)MonsterDamage);
		SetDamage((int)MonsterDamage);
	}

	// Bounc Back
	Vector2 MonsterDir = CollideMonster->GetDir() * m_MoveSpeed;
	CollideBounceBack(MonsterDir);

	// Monster Collide Time Setting 
	m_MonsterCollideTime = 0.5f;
}


void CPlayer::ChangeHitAnimation()
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

void CPlayer::CollisionBegin(CCollider *Src, CCollider *Dest, float DeltaTime)
{
}

void CPlayer::Teleport(float DeltaTime)
{

	if (!m_TeleportEnable || m_CharacterInfo.MP < 50 ) return;

	// Animation Settings
	std::string Anim = m_mapAnimName.find(PLAYER_TELEPORT)->second;
	ChangeAnimation(Anim);

	m_Pos = m_TeleportPos;

	// m_TeleportEnable
	m_TeleportEnable = false;

	m_CharacterInfo.MP -= 50;

	// TeleportMouse Cursor Animation
	DeleteTeleportObj();

	m_Scene->GetSceneResource()->SoundPlay("Teleport");
}

void CPlayer::SetTeleportPos(float DeltaTime)
{
	if (m_CharacterInfo.MP < 50) return;

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

void CPlayer::RemoveTargetPos(float DeltaTime)
{
	m_TargetEnable = false;
}

void CPlayer::FireTarget()
{
	if (m_CurrentGun)
	{
		// Set Dir To Mouse
		Vector2 MousePos = CInput::GetInst()->GetMousePos();
		Vector2 CameraPos = m_Scene->GetCamera()->GetPos();
		Vector2 MousePlayerPosDiff = m_Pos - (MousePos + CameraPos);
		float Angle = GetAngle(m_Pos - CameraPos, MousePos);
		SetDir(Angle);

		// Fire
		m_CurrentGun->PlayerFire(m_TargetPos, (float)m_CharacterInfo.Attack);
	}
}

void CPlayer::SetTargetPos(float DeltaTime)
{
	// m_TargetPos�� ����
	Vector2 MousePos = CInput::GetInst()->GetMousePos();
	Vector2 CameraPos = m_Scene->GetCamera()->GetPos();
	Vector2 Resolution = m_Scene->GetCamera()->GetResolution();
	
	m_TargetEnable = true;
	m_TargetPos = Vector2((float)(MousePos.x + CameraPos.x), (float)(MousePos.y + CameraPos.y));
}

void CPlayer::BulletFireTarget(float DeltaTime)
{
	if (!m_CurrentGun) return;
	if (m_FireTime < m_FireTimeMax) return;
	m_FireTime -= m_FireTimeMax;

	// Set Target Pos
	Vector2 PlayerDir = m_Dir;
	Vector2 MousePos = CInput::GetInst()->GetMousePos();
	Vector2 CameraPos = m_Scene->GetCamera()->GetPos();
	m_TargetPos = Vector2((float)(MousePos.x + CameraPos.x), (float)(MousePos.y + CameraPos.y));
	
	// Change Animation
	ChangeIdleAnimation();

	// Set Dir To Mouse
	float Angle = GetAngle(m_Pos - CameraPos, MousePos);
	SetDir(Angle);

	// Fire
	m_CurrentGun->PlayerFire(m_TargetPos, (float)m_CharacterInfo.Attack);
	
	
}

void CPlayer::CharacterDestroy()
{
	// Destroy();
	
	// Game Over Effect
	if (m_DeathWidgetCreate) return;

	CUIGameOver* GameOverUI = m_Scene->FindUIWindow<CUIGameOver>("GameOverUI");
	GameOverUI->SetGameOverWidgets();
	CUICharacterStateHUD* StateWindow = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");
	StateWindow->SetVisibility(false);
	CUIGunStateHUD* GunStateWindow = m_Scene->FindUIWindow<CUIGunStateHUD>("GunStateHUD");
	GunStateWindow->SetVisibility(false);
	m_DeathWidgetCreate = true;
	m_Scene->GetSceneResource()->SoundPlay("GameEnd");

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
			if (PType == EPotion_Type::HP)
				m_CharacterInfo.HP = m_CharacterInfo.HPMax;
			else
				m_CharacterInfo.MP = m_CharacterInfo.MPMax;
			Potion->Destroy();
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
			CGun* GunProto = (CGun*)m_Scene->FindPrototype(ProtoTypeName);
			int GunMonsterKillLimit = GunProto->GetMonsterKillLimit();

			if (m_MonsterKilled >= GunMonsterKillLimit)
			{
				Equip(m_Scene->CreateObject<CGun>(ProtoTypeName,ProtoTypeName));
				break;
			}
			else
			{
				CSharedPtr<CEffectText> NoBulletText = m_Scene->CreateObject<CEffectText>(
					"CEffectText",
					EFFECT_TEXT_PROTO,
					Vector2(m_Pos.x - m_Size.x * 1.7f, m_Pos.y - m_Size.y * 0.3f),
					Vector2(50.f, 10.f));
				NoBulletText->SetText(TEXT("Kill More Monster"));
				NoBulletText->SetTextColor(255, 0, 0);
			}
		}
	}
}

void CPlayer::AutoAcquireCoin(float)
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		// Coin
		CCoin* Coin = (*iter)->IsCollisionWithCoin();
		if (Coin)
		{
			// m_CharacterInfo.Gold += Coin->GetCoinGold();
			Vector2 CenterPos = Vector2(m_Pos.x, m_Pos.y - m_Size.y * 0.5f);
			Coin->SetMoveToPlayer(true, CenterPos);
			break;
		}
	}
}

void CPlayer::BuyItem(float)
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		// Potion
		CNpc* Npc = (*iter)->IsCollisionWithNpc();
		bool CanBuy = false;
		if (Npc)
		{
			CUICharacterStateHUD* State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");
			ENpc_Type NpcType = Npc->GetNpcType();
			int Cost = Npc->GetCost();
			switch (NpcType)
			{
			case ENpc_Type::Hp:
				if (m_CharacterInfo.Gold >= Cost)
				{
					m_CharacterInfo.Gold -= Cost;
					m_HpPotionInv += 1;
					UpdateHpPotionInv(State);
					CanBuy = true;
				}
				break;
			case ENpc_Type::Mp:
				if (m_CharacterInfo.Gold >= Cost)
				{
					m_CharacterInfo.Gold -= Cost;
					m_MpPotionInv += 1;
					UpdateMpPotionInv(State);
					CanBuy = true;
				}
				break;
			case ENpc_Type::Shield:
				if (m_CharacterInfo.Gold >= Cost)
				{
					m_CharacterInfo.Gold -= Cost;
					m_ShieldInv += 1;
					UpdateShieldInv(State);
					CanBuy = true;
				}
				break;
			}
			if (CanBuy)
			{
				m_Scene->GetSceneResource()->SoundPlay("Buy");
				break;
			}
			else ShowNoGoldSign();
		}
	}
}

void CPlayer::ShowNoGoldSign()
{
	CSharedPtr<CEffectText> NoBulletText = m_Scene->CreateObject<CEffectText>(
		"CEffectText",
		EFFECT_TEXT_PROTO,
		Vector2(m_Pos.x - m_Size.x, m_Pos.y-m_Size.y*0.3f),
		Vector2(50.f, 10.f));
	NoBulletText->SetText(TEXT("No Gold"));
	NoBulletText->SetTextColor(255, 0, 0);
}

CGun* CPlayer::Equip(CGun* Gun)
{
	CGun* ExitingGun   = CCharacter::Equip(Gun);
	CCollider* GunBody = m_CurrentGun->FindCollider("Body");
	GunBody->SetCollisionProfile("PlayerAttack");

	// Inc Attack, Armor
	m_CharacterInfo.Attack = m_SelectedCharacterInfo.Attack + m_CurrentGun->GetGunDamage();

	// FireTime Update
	EGun_Type GunType = m_CurrentGun->GetGunType();
	if (GunType == EGun_Type::Pistol)
		m_FireTimeMax = FIREMAX_TIME;

	m_Scene->GetSceneResource()->SoundPlay("Equip");

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
		std::string Anim = m_mapAnimName.find(PLAYER_LEFT_IDLE)->second;
		ChangeAnimation(Anim);
		m_Dir.x = -1;
	}
	else
	{
		std::string Anim = m_mapAnimName.find(PLAYER_RIGHT_IDLE)->second;
		ChangeAnimation(Anim);
		m_Dir.x = 1;
	}
}

void CPlayer::SetCharName()
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

void CPlayer::SetAnimName()
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
}

void CPlayer::SetAssAnimName()
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

void CPlayer::SetJimmyAnimName()
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

void CPlayer::SetBiuAnimName()
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

void CPlayer::SetPinkyAnimName()
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

void CPlayer::SetPunnyAnimName()
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

void CPlayer::SetRaffAnimName()
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

void CPlayer::AddAssAnimName()
{
	AddAnimation(ASS_PLAYER_RIGHT_IDLE,true,2.f);
	AddAnimation(ASS_PLAYER_RIGHT_WALK,true,1.f);
	AddAnimation(ASS_PLAYER_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(ASS_PLAYER_RIGHT_RUN, true, 0.6f);
	AddAnimation(ASS_PLAYER_RIGHT_DEATH, false, FIREMAX_TIME);
	AddAnimation(ASS_PLAYER_RIGHT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(ASS_PLAYER_RIGHT_HIT, true, 0.6f);

	AddAnimation(ASS_PLAYER_LEFT_IDLE,true,2.f);
	AddAnimation(ASS_PLAYER_LEFT_WALK,true,1.f);
	AddAnimation(ASS_PLAYER_LEFT_ATTACK,false,0.1f);
	AddAnimation(ASS_PLAYER_LEFT_RUN,true,0.6f);
	AddAnimation(ASS_PLAYER_LEFT_DEATH,false,FIREMAX_TIME);
	AddAnimation(ASS_PLAYER_LEFT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(ASS_PLAYER_LEFT_HIT, true, 0.6f);

	AddAnimation(ASS_PLAYER_TELEPORT, false, 0.3f);
}

void CPlayer::AddJimmyAnimName()
{
	AddAnimation(JIMMY_PLAYER_RIGHT_IDLE, true, 2.f);
	AddAnimation(JIMMY_PLAYER_RIGHT_WALK, true, 1.f);
	AddAnimation(JIMMY_PLAYER_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(JIMMY_PLAYER_RIGHT_RUN, true, 0.6f);
	AddAnimation(JIMMY_PLAYER_RIGHT_DEATH, false, FIREMAX_TIME);
	AddAnimation(JIMMY_PLAYER_RIGHT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(JIMMY_PLAYER_RIGHT_HIT, true, 0.6f);

	AddAnimation(JIMMY_PLAYER_LEFT_IDLE, true, 2.f);
	AddAnimation(JIMMY_PLAYER_LEFT_WALK, true, 1.f);
	AddAnimation(JIMMY_PLAYER_LEFT_ATTACK, false, 0.1f);
	AddAnimation(JIMMY_PLAYER_LEFT_RUN, true, 0.6f);
	AddAnimation(JIMMY_PLAYER_LEFT_DEATH, false, FIREMAX_TIME);
	AddAnimation(JIMMY_PLAYER_LEFT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(JIMMY_PLAYER_LEFT_HIT, true, 0.6f);

	AddAnimation(JIMMY_PLAYER_TELEPORT, false, 0.3f);
}

void CPlayer::AddBiuAnimName()
{
	AddAnimation(BIU_PLAYER_RIGHT_IDLE, true, 2.f);
	AddAnimation(BIU_PLAYER_RIGHT_WALK, true, 1.f);
	AddAnimation(BIU_PLAYER_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(BIU_PLAYER_RIGHT_RUN, true, 0.6f);
	AddAnimation(BIU_PLAYER_RIGHT_DEATH, false, FIREMAX_TIME);
	AddAnimation(BIU_PLAYER_RIGHT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(BIU_PLAYER_RIGHT_HIT, true, 0.6f);

	AddAnimation(BIU_PLAYER_LEFT_IDLE, true, 2.f);
	AddAnimation(BIU_PLAYER_LEFT_WALK, true, 1.f);
	AddAnimation(BIU_PLAYER_LEFT_ATTACK, false, 0.1f);
	AddAnimation(BIU_PLAYER_LEFT_RUN, true, 0.6f);
	AddAnimation(BIU_PLAYER_LEFT_DEATH, false, FIREMAX_TIME);
	AddAnimation(BIU_PLAYER_LEFT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(BIU_PLAYER_LEFT_HIT, true, 0.6f);

	AddAnimation(BIU_PLAYER_TELEPORT, false, 0.3f);
}

void CPlayer::AddPinkyAnimName()
{
	AddAnimation(PINKY_PLAYER_RIGHT_IDLE, true, 2.f);
	AddAnimation(PINKY_PLAYER_RIGHT_WALK, true, 1.f);
	AddAnimation(PINKY_PLAYER_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(PINKY_PLAYER_RIGHT_RUN, true, 0.6f);
	AddAnimation(PINKY_PLAYER_RIGHT_DEATH, false, FIREMAX_TIME);
	AddAnimation(PINKY_PLAYER_RIGHT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(PINKY_PLAYER_RIGHT_HIT, true, 0.6f);

	AddAnimation(PINKY_PLAYER_LEFT_IDLE, true, 2.f);
	AddAnimation(PINKY_PLAYER_LEFT_WALK, true, 1.f);
	AddAnimation(PINKY_PLAYER_LEFT_ATTACK, false, 0.1f);
	AddAnimation(PINKY_PLAYER_LEFT_RUN, true, 0.6f);
	AddAnimation(PINKY_PLAYER_LEFT_DEATH, false, FIREMAX_TIME);
	AddAnimation(PINKY_PLAYER_LEFT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(PINKY_PLAYER_LEFT_HIT, true, 0.6f);

	AddAnimation(PINKY_PLAYER_TELEPORT, false, 0.3f);
}

void CPlayer::AddPunnyAnimName()
{
	AddAnimation(PUNNY_PLAYER_RIGHT_IDLE, true, 2.f);
	AddAnimation(PUNNY_PLAYER_RIGHT_WALK, true, 1.f);
	AddAnimation(PUNNY_PLAYER_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(PUNNY_PLAYER_RIGHT_RUN, true, 0.6f);
	AddAnimation(PUNNY_PLAYER_RIGHT_DEATH, false, FIREMAX_TIME);
	AddAnimation(PUNNY_PLAYER_RIGHT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(PUNNY_PLAYER_RIGHT_HIT, true, 0.6f);

	AddAnimation(PUNNY_PLAYER_LEFT_IDLE, true, 2.f);
	AddAnimation(PUNNY_PLAYER_LEFT_WALK, true, 1.f);
	AddAnimation(PUNNY_PLAYER_LEFT_ATTACK, false, 0.1f);
	AddAnimation(PUNNY_PLAYER_LEFT_RUN, true, 0.6f);
	AddAnimation(PUNNY_PLAYER_LEFT_DEATH, false, FIREMAX_TIME);
	AddAnimation(PUNNY_PLAYER_LEFT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(PUNNY_PLAYER_LEFT_HIT, true, 0.6f);

	AddAnimation(PUNNY_PLAYER_TELEPORT, false, 0.3f);
}

void CPlayer::AddRaffAnimName()
{
	AddAnimation(RAFF_PLAYER_RIGHT_IDLE, true, 2.f);
	AddAnimation(RAFF_PLAYER_RIGHT_WALK, true, 1.f);
	AddAnimation(RAFF_PLAYER_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(RAFF_PLAYER_RIGHT_RUN, true, 0.6f);
	AddAnimation(RAFF_PLAYER_RIGHT_DEATH, false, FIREMAX_TIME);
	AddAnimation(RAFF_PLAYER_RIGHT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(RAFF_PLAYER_RIGHT_HIT, true, 0.6f);

	AddAnimation(RAFF_PLAYER_LEFT_IDLE, true, 2.f);
	AddAnimation(RAFF_PLAYER_LEFT_WALK, true, 1.f);
	AddAnimation(RAFF_PLAYER_LEFT_ATTACK, false, 0.1f);
	AddAnimation(RAFF_PLAYER_LEFT_RUN, true, 0.6f);
	AddAnimation(RAFF_PLAYER_LEFT_DEATH, false, FIREMAX_TIME);
	AddAnimation(RAFF_PLAYER_LEFT_DASH, false, DASH_TIME * 0.5);
	AddAnimation(RAFF_PLAYER_LEFT_HIT, true, 0.6f);

	AddAnimation(RAFF_PLAYER_TELEPORT, false, 0.3f);
}

void CPlayer::SkillTimeUpdate(float DeltaTime)
{
	if (m_SkillTime >= 0.f && m_SkillEnable)
	{
		m_SkillTime -= DeltaTime ;
		if (m_SkillTime <= 0.f)
		{
			m_SkillEnable = false;
			DeActivateSkills(DeltaTime);
		}
	}
}

void CPlayer::ActivateSkills(float DeltaTime)
{
	if (m_SkillEnable) return;
	if (m_CharacterInfo.MP < 100) return;

	// Slow Motion
	if (!m_CurrentGun)
	{
		// 메세지
		CSharedPtr<CEffectText> NoBulletText = m_Scene->CreateObject<CEffectText>(
			"CEffectText",
			EFFECT_TEXT_PROTO,
			Vector2(m_Pos.x - m_Size.x, m_Pos.y - m_Size.y * 0.3f),
			Vector2(50.f, 10.f));
		NoBulletText->SetText(TEXT("Equip Gun"));
		NoBulletText->SetTextColor(255, 0, 0);
		return;
	}

	// Sound
	m_Scene->GetSceneResource()->SoundPlay("Skill");

	// Idle Animation 
	ChangeIdleAnimation();

	// MP Decrease
	m_CharacterInfo.MP -= 100;

	switch (m_CharType)
	{
	case EChar_Type::Ass:
		SkillMakeGrenades(DeltaTime);
		break;
	case EChar_Type::Biu:
		SkillClone(DeltaTime);
		break;
	case EChar_Type::Jimmy:
		SkillMultipleBulletAttack(DeltaTime);
		break;
	case EChar_Type::Pinky:
		SkillSlowMotion(DeltaTime);
		break;
	case EChar_Type::Punny:
		SkillDestroyAllAttack(DeltaTime);
		break;
	case EChar_Type::Raff:
		SkillIncAbility();
		break;
	default:
		break;
	}
}

void CPlayer::DeActivateSkills(float DeltaTime)
{
	switch (m_CharType)
	{
	case EChar_Type::Ass:
		break;
	case EChar_Type::Biu:
		break;
	case EChar_Type::Jimmy:
	{
		SetTimeScale(1.f);
		CGameManager::GetInst()->SetTimeScale(1.f);
	}
		return;
	case EChar_Type::Pinky:
	{
		SetTimeScale(1.f);
		CGameManager::GetInst()->SetTimeScale(1.f);
		m_CharacterInfo.Attack /= 4;
	}
		return;
	case EChar_Type::Punny:
	{
		SetTimeScale(1.f);
		CGameManager::GetInst()->SetTimeScale(1.f);
	}
		break;
	case EChar_Type::Raff:
	{
		m_CharacterInfo.Attack /= 2;
		// Armor Inc
		m_CharacterInfo.Armor /= 2;
		// Speed Inc
		m_MoveSpeed /= 1.5f;
	}
		break;
	}
}

void CPlayer::SkillMakeGrenades(float DeltaTime)
{
	m_SkillTime = SLOW_MOTION_ATTACK_TIME;
	m_SkillEnable = true;

	for (int i = 1; i < 4; i++)
	{
		// 1st Nearby
		for (float f = 0.0f; f < 2 * M_PI; f += M_PI / 12.f)
		{
			CEffectGrenade* EffectGrenade = m_Scene->CreateObject<CEffectGrenade>(
				"GrenadeEffect",
				GRENADE_PROTO,
				Vector2(
					(m_Pos.x - m_Offset.x) + 150.f * i * cos(f),
					(m_Pos.y - m_Size.y * 2.5f - m_Offset.y) + 150.f * i * (float)sin(f))
				);
			EffectGrenade->SetOffset(Vector2(-EffectGrenade->GetSize().x * 0.45f, 0));
			EffectGrenade->SetTexture("Grenade", TEXT("images/Character/ass/ass_explosion_texture.bmp"));
			EffectGrenade->SetTextureColorKey(255, 255, 255);
			EffectGrenade->SetPlayerGrenage(true);
			EffectGrenade->SetDamage(m_CharacterInfo.Attack * 2);
		}
	}
	
}

void CPlayer::SkillClone(float DeltaTime)
{
	for (float f = 0.0f; f < 2 * M_PI; f += M_PI / 2.f)
	{
		CPlayerClone* PlayerClone = (CPlayerClone*)m_Scene->CreateObject<CPlayerClone>(
			"PlayerClone", 
			Vector2(
				(m_Pos.x - m_Offset.x) + m_Size.Length() * 1.5f * cos(f),
				(m_Pos.y - m_Offset.y) + m_Size.Length() * 1.5f * sin(f))
			);
		PlayerClone->SetLifeTime(20.f);
		PlayerClone->SetCharType(m_CharType);
		PlayerClone->SetAnimName();
		PlayerClone->SetCharacterInfo(m_CharacterInfo);
		// Equip Gun
		if (m_CurrentGun)
		{
			CGun* GunClone = m_CurrentGun->Clone();
			PlayerClone->Equip(GunClone);
		}
	}
}

void CPlayer::SkillSlowMotion(float DeltaTime)
{
	CGameManager::GetInst()->SetTimeScale(0.01f);
	SetTimeScale(100.f);
	m_SkillTime = SLOW_MOTION_ATTACK_TIME * 5;
	m_CharacterInfo.Attack *= 4;
	m_SkillEnable = true;
	CEffectShieldStart* ShieldStart = m_Scene->CreateObject<CEffectShieldStart>(
		"ShieldStart",
		SHIELD_START_PROTO, Vector2(m_Pos.x, m_Pos.y - m_Size.y * 0.5f));
	ShieldStart->SetOwner(this);
	ShieldStart->SetLifeTime(m_SkillTime);
	ShieldStart->SetTimeScale(m_TimeScale);
}
