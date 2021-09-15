
#include "Player.h"
#include "../Scene/Scene.h"
#include "../Input.h"
#include "../GameManager.h"
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderSphere.h"
#include "../UI/UICharacterStateHUD.h"
#include "EffectHit.h"
#include"../Scene/SceneResource.h"
#include "../Scene/Scene.h"

CPlayer::CPlayer()	:
	m_Skill1Enable(false),
	m_Skill1Time(0.f)
{
}

CPlayer::CPlayer(const CPlayer& obj)	:
	CCharacter(obj)
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

	CInput::GetInst()->SetCallback<CPlayer>("MoveUp", KeyState_Push,
		this, &CPlayer::MoveUp);

	CInput::GetInst()->SetCallback<CPlayer>("MoveDown", KeyState_Push,
		this, &CPlayer::MoveDown);

	CInput::GetInst()->SetCallback<CPlayer>("MoveLeft", KeyState_Push,
		this, &CPlayer::MoveLeft);

	CInput::GetInst()->SetCallback<CPlayer>("MoveRight", KeyState_Push,
		this, &CPlayer::MoveRight);

	CInput::GetInst()->SetCallback<CPlayer>("Fire", KeyState_Down,
		this, &CPlayer::BulletFire);

	CInput::GetInst()->SetCallback<CPlayer>("Pause", KeyState_Down,
		this, &CPlayer::Pause);
	CInput::GetInst()->SetCallback<CPlayer>("Resume", KeyState_Down,
		this, &CPlayer::Resume);

	CInput::GetInst()->SetCallback<CPlayer>("Skill1", KeyState_Down,
		this, &CPlayer::Skill1);

	// Run
	CInput::GetInst()->SetCallback<CPlayer>("RunUp", KeyState_Push,
		this, &CPlayer::RunUp);

	CInput::GetInst()->SetCallback<CPlayer>("RunDown", KeyState_Push,
		this, &CPlayer::RunDown);

	CInput::GetInst()->SetCallback<CPlayer>("RunLeft", KeyState_Push,
		this, &CPlayer::RunLeft);

	CInput::GetInst()->SetCallback<CPlayer>("RunRight", KeyState_Push,
		this, &CPlayer::RunRight);
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
	AddAnimation("LucidNunNaRightRun", false, 0.5f);

	AddAnimation("LucidNunNaLeftWalk", true, 0.6f);
	AddAnimation("LucidNunNaLeftRun", false, 0.5f);

	AddAnimationNotify<CPlayer>("LucidNunNaRightAttack", 2, this, &CPlayer::Fire);
	SetAnimationEndNotify<CPlayer>("LucidNunNaRightAttack", this, &CPlayer::AttackEnd);

	AddAnimationNotify<CPlayer>("LucidNunNaRightSkill1", 2, this, &CPlayer::Skill1Enable);
	SetAnimationEndNotify<CPlayer>("LucidNunNaRightSkill1", this, &CPlayer::Skill1End);

	/*CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(80.f, 75.f);
	Body->SetOffset(0.f, -37.5f);*/

	CColliderSphere* Head = AddCollider<CColliderSphere>("Head");
	//Head->SetExtent(40.f, 30.f);
	Head->SetRadius(20.f);
	Head->SetOffset(0.f, -60.f);
	Head->SetCollisionProfile("Player");

	CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(80.f, 45.f);
	Body->SetOffset(0.f, -22.5f);
	Body->SetCollisionProfile("Player");

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

			/*auto	iter = m_Skill1BulletList.begin();
			auto	iterEnd = m_Skill1BulletList.end();

			for (; iter != iterEnd; ++iter)
			{
				(*iter)->SetTimeScale(1.f);
			}

			m_Skill1BulletList.clear();*/
		}
	}

	if (!m_RunEnable)
	{
		if (m_CharacterInfo.Stemina <= m_CharacterInfo.SteminaMax)
			m_CharacterInfo.Stemina += DeltaTime;
	}

	// Run
	if (m_RunEnable)
	{
		// 스테미나가 줄어드는 속도가 4배 빠르다
		if(m_CharacterInfo.Stemina >= 0)
			m_CharacterInfo.Stemina -= DeltaTime;
		if (m_CharacterInfo.Stemina <= 0 || m_RunTime <= 0)
		{
			// 스테미나가 다 줄어들면 중단 //
			RunEnd();
		}
	}

	// Stemina Bar Update
	CUICharacterStateHUD* State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");
	if (State)
		State->SetSteminaPercent(m_CharacterInfo.Stemina / (float)m_CharacterInfo.SteminaMax);

	if (CheckCurrentAnimation("LucidNunNaRightAttack"))
		SetOffset(0.f, 20.f);

	else
		SetOffset(0.f, 0.f);
}

void CPlayer::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);

	// 현재 애니메이션이 Walk 상태에서 속도가 0이 되었다면 이전까지 움직이다가
	// 지금 멈췄다는 것이다.
	if (CheckCurrentAnimation("LucidNunNaRightWalk") &&
		m_Velocity.Length() == 0.f)
	{
		ChangeAnimation("LucidNunNaRightIdle");
	}

	// Run
		// 달리기의 경우, 끝나면 RunEnd도 호출 
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

CPlayer* CPlayer::Clone()
{
	return new CPlayer(*this);
}

float CPlayer::SetDamage(float Damage)
{
	Damage = CCharacter::SetDamage(Damage);

	CUICharacterStateHUD* State = m_Scene->FindUIWindow<CUICharacterStateHUD>("CharacterStateHUD");

	if (State)
		State->SetHPPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);

	return Damage;
}

void CPlayer::MoveUp(float DeltaTime)
{
	//m_Pos.y -= 200.f * DeltaTime;
	RunEnd();
	Move(Vector2(0.f, -1.f));
	ChangeAnimation("LucidNunNaRightWalk");
}

void CPlayer::MoveDown(float DeltaTime)
{
	RunEnd();
	Move(Vector2(0.f, 1.f));
	ChangeAnimation("LucidNunNaRightWalk");
}

void CPlayer::MoveLeft(float DeltaTime)
{
	RunEnd();
	Move(Vector2(-1.f, 0.f));
	ChangeAnimation("LucidNunNaLeftWalk");
}

void CPlayer::MoveRight(float DeltaTime)
{
	RunEnd();
	Move(Vector2(1.f, 0.f));
	ChangeAnimation("LucidNunNaRightWalk");
}

void CPlayer::BulletFire(float DeltaTime)
{
	ChangeAnimation("LucidNunNaRightAttack");	

	/*if (m_Skill1Enable)
	{
		Bullet->SetTimeScale(0.f);
		m_Skill1BulletList.push_back(Bullet);
	}*/
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
}

void CPlayer::RunRight(float DeltaTime)
{
	Move(Vector2(1.f, 0.f));
	RunStart(true);
}

void CPlayer::RunLeft(float DeltaTime)
{
	Move(Vector2(-1.f, 0.f));
	RunStart(false);
}

void CPlayer::RunUp(float DeltaTime)
{
	Move(Vector2(0.f, -1.f));
	RunStart(true);
}

void CPlayer::RunDown(float DeltaTime)
{
	Move(Vector2(0.f, 1.f));
	RunStart(false);
}

void CPlayer::RunStart(bool IsRight)
{
	if (m_CharacterInfo.Stemina <= 0.2 * m_CharacterInfo.SteminaMax || m_RunEnable) return;
	m_Scene->GetSceneResource()->SoundPlay("Run");
	m_RunEnable = true;

	// Effect 주기 
	CEffectHit* Hit = m_Scene->CreateObject<CEffectHit>("HitEffect", "HitEffect",
		m_Pos, Vector2(178.f, 164.f));

	// 속도 세팅 
	m_MoveSpeed = m_SpeedInfo.Fast;

	// 애니메이션 전환 ( Run으로 수정해야 한다 ) 
	IsRight ? ChangeAnimation("LucidNunNaRightRun") : ChangeAnimation("LucidNunNaLeftRun");
}

// 달리기 끝 함수 -------------------------------------------------------
void CPlayer::RunEnd()
{
	if (!m_RunEnable) return;
	m_RunEnable = false;
	m_RunTime = 1;
	m_MoveSpeed = m_SpeedInfo.Normal;

	// 애니메이션 수정 
	if (CheckCurrentAnimation("LucidNunNaRightRun"))
	{
		ChangeAnimation("LucidNunNaRightWalk");
	}
	if (CheckCurrentAnimation("LucidNunNaLeftRun"))
	{
		ChangeAnimation("LucidNunNaLeftWalk");
	}
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
