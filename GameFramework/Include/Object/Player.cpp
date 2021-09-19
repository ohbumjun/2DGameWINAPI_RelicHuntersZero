
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
		// 그외 충돌시 효과 추가하기 
		// 이렇게 하면, Dash중에 충돌 날시, Move를 멈춘다
		// < Dash 충돌 원리 > 
		// 1) Update --> Dash + CollsionCheck() : DashCollide 함수 실행
		// 2) 한편, Move 상에서는 Dash + CollisionCheck() 이면, Move 진행 x ( 이전의 Dash를 통한 Move 를 막아주기 위해 )
		// 3) (이전의 Move는 멈춘 상태) DashCollide() 함수를 통해, 해당 obj를 뒤로 밀려나게 한다.
		if (m_DashEnable) return;

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
	if (m_Dir.x == -1.f) ChangeAnimation("LucidNunNaLeftWalk");
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
	if (m_CharacterInfo.MP >= 0.5 * m_CharacterInfo.MPMax)
		m_CharacterInfo.MP -= 0.5 * m_CharacterInfo.MPMax;

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

void CPlayer::DashCollide()
{
	// 벽에 대시한 경우( 어떤 충돌체와 충돌하던 뒤로 밀려난다 ) + 해당 collider가 mouse type이 아니어야 한다
	// 대시중 충돌 여부 확인
	// 나중에 체크해야 한다. 
	// 충돌한 대상이 몬스터인지, 장애물인지, 등등 
	// 지금은 우선 이렇게 단순하게 세팅하자.
	// 이동 방향 반대로 이동시키기
	Vector2 OppDir = Vector2(-m_Dir.x, -m_Dir.y);
	OppDir.Normalize();
	SetDir(OppDir);

	DashEnd();
	// 자기 크기만큼 bounce back
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

	// Animation 적용하기 
	

	// 이동하기
	m_Pos = m_TeleportPos;

	// Animation 되돌려두기

	// m_TeleportEnable
	m_TeleportEnable = false;

	// MP 90% 감소
	if (m_CharacterInfo.MP >= 0.9 * m_CharacterInfo.MPMax)
		m_CharacterInfo.MP -= 0.9 * m_CharacterInfo.MPMax;
	
	// TeleportMouse Cursor Animation 지워주기
	if (m_TeleportObj)
		delete m_TeleportObj;
}

void CPlayer::SetTeleportPos(float DeltaTime)
{
	if(m_CharacterInfo.MP <= 0.9 * m_CharacterInfo.MPMax) return;

	// Teleport 준비 
	m_TeleportEnable = true;

	// 클릭하는 순간, 좌표상의 마우스 위치 가져오기 
	POINT ptMouse;
	HWND hwnd = CGameManager::GetInst()->GetWindowHandle();
	GetCursorPos(&ptMouse);
	ScreenToClient(hwnd, &ptMouse);
	m_TeleportPos = Vector2((float)ptMouse.x, (float)ptMouse.y);

	// 화면상에 Teleport 위치 애니메이션 그리기
	// m_TeleportObj에 부여된 obj가 없을 때만 할당( 그렇지 않으면 메모리 leak )
	if (!m_TeleportObj)
	{
		m_TeleportObj = m_Scene->CreateObject<CTeleportMouse>("TeleportMouse", "TeleportMouse",
		m_TeleportPos);
	}

	// Update 함수에서, 커서만 누르고, Teleport 안해주면
	// m_TeleportObj 에 Animation이 계속 남아있을 수 있다
	// 따라서, 일정 시간이 지나면 지워주기 위해 m_TeleportTime을 세팅한다
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
