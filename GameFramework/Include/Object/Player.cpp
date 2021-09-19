
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

CPlayer::CPlayer() : 
	m_Skill1Enable(false),
	m_Skill1Time(0.f),
	m_RunEnable(false),
	m_DashEnable(false),
	m_DashTime(0.f),
	m_TeleportEnable(false),
	m_TelePortTime(0.f),
	m_TeleportObj{},
	m_TeleportPos(Vector2(0.f,0.f))
{
	m_ObjType = EObject_Type::Character;
}

CPlayer::CPlayer(const CPlayer &obj) : CCharacter(obj)
{
	m_Skill1Time = obj.m_Skill1Time;
	m_Skill1Enable = false;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Start()
{
	CCharacter::Start();

	CInput::GetInst()->SetCallback<CPlayer>("Fire", KeyState_Down,
		this, &CPlayer::BulletFire);
	CInput::GetInst()->SetCallback<CPlayer>("Pause", KeyState_Down,
		this, &CPlayer::Pause);
	CInput::GetInst()->SetCallback<CPlayer>("Resume", KeyState_Down,
		this, &CPlayer::Resume);
	CInput::GetInst()->SetCallback<CPlayer>("Skill1", KeyState_Down,
		this, &CPlayer::Skill1);

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
}

bool CPlayer::Init()
{
	if (!CCharacter::Init())
		return false;

	SetPivot(0.5f, 1.f);

	//SetTexture("Teemo", TEXT("teemo.bmp"));
	CreateAnimation();
	AddAnimation("LucidNunNaRightIdle");
	AddAnimation("LucidNunNaRightWalk", true, 0.6f);
	AddAnimation("LucidNunNaRightAttack", false, 0.5f);
	AddAnimation("LucidNunNaRightSkill1", false, 0.5f);
	AddAnimation("LucidNunNaRightRun", true, 0.6f);

	AddAnimation("LucidNunNaLeftIdle");
	AddAnimation("LucidNunNaLeftWalk", true, 0.6f);
	AddAnimation("LucidNunNaLeftRun", true, 0.6f);

	// Stun
	AddAnimation("LucidNunNaStun", true, 0.6f);

	AddAnimation("TeleportMouseDisplay", true, 0.6f);


	AddAnimationNotify<CPlayer>("LucidNunNaRightAttack", 2, this, &CPlayer::Fire);
	SetAnimationEndNotify<CPlayer>("LucidNunNaRightAttack", this, &CPlayer::AttackEnd);

	AddAnimationNotify<CPlayer>("LucidNunNaRightSkill1", 2, this, &CPlayer::Skill1Enable);
	SetAnimationEndNotify<CPlayer>("LucidNunNaRightSkill1", this, &CPlayer::Skill1End);

	/*CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(80.f, 75.f);
	Body->SetOffset(0.f, -37.5f);*/

	CColliderSphere *Head = AddCollider<CColliderSphere>("Head");
	Head->SetRadius(20.f);
	Head->SetOffset(0.f, -60.f);
	Head->SetCollisionProfile("Player");

	CColliderBox *Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(80.f, 45.f);
	Body->SetOffset(0.f, -22.5f);
	Body->SetCollisionProfile("Player");

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

	if (GetAsyncKeyState(VK_F1) & 0x8000)
		SetAttackSpeed(0.5f);

	if (m_Skill1Enable)
	{
		m_Skill1Time += DeltaTime;

		if (m_Skill1Time >= 3.f)
		{
			m_Skill1Enable = false;
			m_Skill1Time = 0.f;

			SetTimeScale(1.f);
			CGameManager::GetInst()->SetTimeScale(1.f);

		}
	}

	if (!m_RunEnable && !m_DashEnable)
	{
		if (m_CharacterInfo.MP <= m_CharacterInfo.MPMax)
			m_CharacterInfo.MP += DeltaTime;
	}

	// Run
	if (m_RunEnable)
	{
		if (m_CharacterInfo.MP >= 0)
			m_CharacterInfo.MP -= 2*DeltaTime;
		if (m_CharacterInfo.MP <= 0)
		{
			RunEnd();
		}
	}

	// Dash
	if (m_DashEnable)
	{
		if (CollisionCheck())
			DashCollide();
		if (m_DashTime >= 0)
			m_DashTime -= DeltaTime;
		if (m_DashTime <= 0)
		{
			DashEnd();
		}
	}

	// Teleport
	/*
		if (m_TeleportEnable)
		{
			m_TelePortTime -= DeltaTime;
			SAFE_DELETE(m_TeleportObj);
		}
	*/
	
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

}

void CPlayer::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);

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
	return Damage;
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
		// �׿� �浹�� ȿ�� �߰��ϱ� 
		// �̷��� �ϸ�, Dash�߿� �浹 ����, Move�� �����
		// < Dash �浹 ���� > 
		// 1) Update --> Dash + CollsionCheck() : DashCollide �Լ� ����
		// 2) ����, Move �󿡼��� Dash + CollisionCheck() �̸�, Move ���� x ( ������ Dash�� ���� Move �� �����ֱ� ���� )
		// 3) (������ Move�� ���� ����) DashCollide() �Լ��� ����, �ش� obj�� �ڷ� �з����� �Ѵ�.
		if (m_DashEnable) return;

	}
	CCharacter::Move(Dir);
}

void CPlayer::Move(const Vector2& Dir, float Speed)
{
	if (CollisionCheck())
	{
		// �׿� �浹�� ȿ�� �߰��ϱ� 
		if (m_DashEnable) return;
	}
	CCharacter::Move(Dir, Speed);
}

void CPlayer::ChangeMoveAnimation()
{
	if (m_StunEnable) return;
	// ���� 
	if (m_Dir.x == -1.f) ChangeAnimation("LucidNunNaLeftWalk");
	// ������ 
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
	// ���� 
	if (m_Dir.x == -1.f) ChangeAnimation("LucidNunNaLeftRun");
	// ������ 
	else ChangeAnimation("LucidNunNaRightRun");
}

void CPlayer::Dash(float DelatTime)
{
	if (m_DashEnable || m_CharacterInfo.MP < 0.5 * m_CharacterInfo.MPMax) return;

	// Dash Time ���� 
	m_DashTime = DASH_TIME;
	m_DashEnable = true;

	// speed ���� 
	SetMoveSpeed(DASH_SPEED);

	// MP ����
	if (m_CharacterInfo.MP >= 0.5 * m_CharacterInfo.MPMax)
		m_CharacterInfo.MP -= 0.5 * m_CharacterInfo.MPMax;

	// Effect ȿ��
	CEffectHit* Hit = m_Scene->CreateObject<CEffectHit>("HitEffect", "HitEffect",
		m_Pos, Vector2(178.f, 164.f));

	// Sound ȿ��
	m_Scene->GetSceneResource()->SoundPlay("Dash");
}

void CPlayer::DashEnd()
{
	if (!m_DashEnable) return;
	m_DashEnable = false;
	SetMoveSpeed(NORMAL_SPEED);
}

void CPlayer::DashCollide()
{
	// ���� ����� ���( � �浹ü�� �浹�ϴ� �ڷ� �з����� ) + �ش� collider�� mouse type�� �ƴϾ�� �Ѵ�
	// ����� �浹 ���� Ȯ��
	// ���߿� üũ�ؾ� �Ѵ�. 
	// �浹�� ����� ��������, ��ֹ�����, ��� 
	// ������ �켱 �̷��� �ܼ��ϰ� ��������.
	// �̵� ���� �ݴ�� �̵���Ű��
	Vector2 OppDir = Vector2(-m_Dir.x, -m_Dir.y);
	OppDir.Normalize();
	SetDir(OppDir);

	DashEnd();
	// �ڱ� ũ�⸸ŭ bounce back
	Move(m_Dir * m_Size );
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

void CPlayer::Skill1(float DeltaTime)
{
	ChangeAnimation("LucidNunNaRightSkill1");
}

// ���� : Bullet�� ���, Collision�� ����� �ʿ䰡 ����
// �ֳ��ϸ� �浹�ϴ� ���� Bullet�� �ڱ� ȥ�� �ٷ� ����� ������ �����̴� 
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

Vector2 CPlayer::GetColliderPos()
{
	// ���� �浹ü�� ���ٸ�, �����ػ� ���� ��ġ�� ���� ( ���⿡�� � collider�� ���� ���̹Ƿ� ) 
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
	// ���⿡ ���� �ٲ��ֱ� ( ���� ���콺 ��ġ�� ���� ���� ������ ���� )
	ChangeAnimation("LucidNunNaRightIdle");
}


void CPlayer::CollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	/*
	bool Result = CollisionCheck();
	if (Result && m_DashEnable)
	{
		m_Pos = GetColliderPos();
		m_Pos -= Vector2(GetSize());
	}
	*/
}

void CPlayer::Teleport(float DeltaTime)
{
	if (!m_TeleportEnable || m_CharacterInfo.MP <= 0.9 * m_CharacterInfo.MPMax) return;

	// Animation �����ϱ� 
	

	// �̵��ϱ�
	m_Pos = m_TeleportPos;

	// Animation �ǵ����α�

	// m_TeleportEnable
	m_TeleportEnable = false;

	// MP 90% ����
	if (m_CharacterInfo.MP >= 0.9 * m_CharacterInfo.MPMax)
		m_CharacterInfo.MP -= 0.9 * m_CharacterInfo.MPMax;
	
	// TeleportMouse Cursor Animation �����ֱ�
	if (m_TeleportObj)
		delete m_TeleportObj;
}

void CPlayer::SetTeleportPos(float DeltaTime)
{
	if(m_CharacterInfo.MP <= 0.9 * m_CharacterInfo.MPMax) return;

	// Teleport �غ� 
	m_TeleportEnable = true;

	// Ŭ���ϴ� ����, ��ǥ���� ���콺 ��ġ �������� 
	POINT ptMouse;
	HWND hwnd = CGameManager::GetInst()->GetWindowHandle();
	GetCursorPos(&ptMouse);
	ScreenToClient(hwnd, &ptMouse);
	m_TeleportPos = Vector2((float)ptMouse.x, (float)ptMouse.y);

	// ȭ��� Teleport ��ġ �ִϸ��̼� �׸���
	// m_TeleportObj�� �ο��� obj�� ���� ���� �Ҵ�( �׷��� ������ �޸� leak )
	if (!m_TeleportObj)
	{
		m_TeleportObj = m_Scene->CreateObject<CTeleportMouse>("TeleportMouse", "TeleportMouse",
		m_TeleportPos);
	}

	// Update �Լ�����, Ŀ���� ������, Teleport �����ָ�
	// m_TeleportObj �� Animation�� ��� �������� �� �ִ�
	// ����, ���� �ð��� ������ �����ֱ� ���� m_TeleportTime�� �����Ѵ�
	m_TelePortTime = TELEPORT_MOUSE_DISPLAY_TIME;

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
}

void CPlayer::Skill1End()
{
	ChangeAnimation("LucidNunNaRightIdle");
}

void CPlayer::Skill1Enable()
{
	CGameManager::GetInst()->SetTimeScale(0.01f);
	SetTimeScale(100.f);
	m_Skill1Enable = true;
	//m_Skill1Time = 0.f;
}
