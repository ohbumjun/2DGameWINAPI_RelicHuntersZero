// Gamae
#include "../GameManager.h"

// Scene
#include "MainScene.h"
#include "SceneResource.h"
#include "SceneManager.h"
#include "HomeScene.h"
// Object
#include "../MonsterInfo.h"
#include "../Object/Player.h"
#include "../Object/Bullet.h"
#include "../Object/Monster.h"
#include "../Object/EffectHit.h"
#include "../Object/TeleportMouse.h"
#include "../Object/StageDoor.h"
#include "../Object/Potion.h"
#include "../Object/HPPotion.h"
// UI
#include "Camera.h"
#include "../UI/UIMain.h"
#include "../UI/UICharacterStateHUD.h"
#include "../UI/UIImage.h"

CMainScene::CMainScene()
{
}

CMainScene::~CMainScene()
{
}

bool CMainScene::Init()
{
	LoadSound();
	LoadAnimationSequence();
	GetCamera()->SetWorldResolution(2000.f, 2000.f);

	// Effect 
	CEffectHit* EffectPrototype = CreatePrototype<CEffectHit>("HitEffect");

	// Teleport
	CTeleportMouse* TeleportMousePrototype = CreatePrototype<CTeleportMouse>("TeleportMouse");
	CBullet* PlayerBullet = CreatePrototype<CBullet>("PlayerBullet");
	CBullet* SlowMotionAttackBullet = CreatePrototype<CBullet>("SkillSlowMotionAttackBullet");

	CCollider* Collider = PlayerBullet->FindCollider("Body");
	if (Collider)
		Collider->SetCollisionProfile("PlayerAttack");

	CBullet* MonsterBullet = CreatePrototype<CBullet>("MonsterBullet");
	Collider = MonsterBullet->FindCollider("Body");
	if (Collider)
		Collider->SetCollisionProfile("MonsterAttack");

	// Player
	CPlayer* Player = CreateObject<CPlayer>("Player");
	Player->SetCharacterInfo(60, 10, CPlayer::GetPlayerStaticHP(), CPlayer::GetPlayerStaticMP(),
		1, 100, 100, 100, NORMAL_ATTACK_DISTANCE);

	SetPlayer(Player);
	GetCamera()->SetTarget(Player);
	GetCamera()->SetTargetPivot(0.5f, 0.5f);

	// Monsters
	Vector2 WorldResolution = m_Camera->GetWorldResolution();
	CMonster* Monster = CreateObject<CMonster>("Monster", Vector2(300.f + rand() % 700, rand() % 100));
	Monster->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, NORMAL_MONSTER_HP_MAX,
		NORMAL_MONSTER_MP_MAX, 1, 100, 100, 100, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
	Monster->SetMoveSpeed(NORMAL_MONSTER_MOVE_SPEED);
	Monster->SetPlayer(Player);

	CMonster* Monster2 = CreateObject<CMonster>("Monster", Vector2(300.f + rand() % 700, rand() % 100));
	Monster2->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, NORMAL_MONSTER_HP_MAX,
		NORMAL_MONSTER_MP_MAX, 1, 100, 100, 100, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
	Monster2->SetMoveSpeed(NORMAL_MONSTER_MOVE_SPEED);
	Monster2->SetPlayer(Player);

	// Stage Door
	CStageDoor* StageDoor_One = CreateObject<CStageDoor>("StageDoor",
		Vector2(300.f + rand() % 700, 30.f + rand() % 100),
		Vector2(50.f, 50.f));
	StageDoor_One->SetDoorStageType(EDoorStage_Type::Stage_Home);

	// Windows
	CUIMain* MainWindow = CreateUIWindow<CUIMain>("MainWindow");
	CUICharacterStateHUD* StateWindow = CreateUIWindow<CUICharacterStateHUD>("CharacterStateHUD");

	// Potion
	Resolution	RS = CGameManager::GetInst()->GetResolution();
	CHPPotion* HPPotion  = CreatePrototype<CHPPotion>("HPPotion");
	CHPPotion* HPPotion1 = CreateObject<CHPPotion>("HPPotion1","HPPotion");

	HPPotion1->SetTexture("HPPotion1", TEXT("Potion/HPPotion.bmp"));
	HPPotion1->SetPos(Vector2(300.f + rand() % 700, 30.f + rand() % 100));
	HPPotion1->SetTextureColorKey(255, 0, 255);

	return true;
}

void CMainScene::LoadAnimationSequence()
{
	// Stage Door
	// Door ---
	GetSceneResource()->CreateAnimationSequence("StageDoor",
		"StageDoor", TEXT("DoorToNextStage.bmp"));
	GetSceneResource()->SetTextureColorKey("StageDoor",
		255, 0, 255);

	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 5; j++)
		{
			GetSceneResource()->AddAnimationFrameData("StageDoor",
				j * 192.f, i * 192.f, 192.f, 192.f);
		}
	}

	// Player ---
	GetSceneResource()->CreateAnimationSequence("LucidNunNaRightIdle",
		"LucidNunNaRightIdle", TEXT("Player/Right/astand.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaRightIdle",
		255, 0, 255);

	for (int i = 0; i < 6; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaRightIdle",
			i * 82.f, 0.f, 82.f, 73.f);
	}

	GetSceneResource()->CreateAnimationSequence("LucidNunNaRightWalk",
		"LucidNunNaRightWalk", TEXT("Player/Right/awalk.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaRightWalk",
		255, 0, 255);

	for (int i = 0; i < 4; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaRightWalk",
			i * 85.f, 0.f, 85.f, 75.f);
	}

	GetSceneResource()->CreateAnimationSequence("LucidNunNaRightAttack",
		"LucidNunNaRightAttack", TEXT("Player/Right/aswing.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaRightAttack",
		255, 0, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaRightAttack",
			i * 176.f, 0.f, 176.f, 89.f);
	}

	// Slow Motion Skill
	GetSceneResource()->CreateAnimationSequence("SkillSlowMotionAttack",
		"SkillSlowMotionAttack", TEXT("Player/Right/ashoot1.bmp"));
	GetSceneResource()->SetTextureColorKey("SkillSlowMotionAttack",
		255, 0, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("SkillSlowMotionAttack",
			i * 70.f, 0.f, 70.f, 81.f);
	}

	// DestoryAll
	GetSceneResource()->CreateAnimationSequence("SkillDestoryAll",
		"SkillDestoryAll", TEXT("Player/Right/ashoot1.bmp"));
	GetSceneResource()->SetTextureColorKey("SkillDestoryAll",
		255, 0, 255);
	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("SkillDestoryAll",
			i * 70.f, 0.f, 70.f, 81.f);
	}

	// 텔레포트 캐릭터
	/* 1번째 버전
		GetSceneResource()->CreateAnimationSequence("LucidNunNaTeleport",
			"LucidNunNaTeleport", TEXT("Player/teleport.bmp"));

		GetSceneResource()->SetTextureColorKey("LucidNunNaTeleport",
			255, 0, 255);

		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				GetSceneResource()->AddAnimationFrameData("LucidNunNaTeleport",
					j * 240.f, i * 320.f, 240.f, 320.f);
			}
		}
	*/

	// 3번째 버전
	GetSceneResource()->CreateAnimationSequence("LucidNunNaTeleport",
		"LucidNunNaTeleport", TEXT("Player/teleport2.bmp"));

	// 임시적으로 SetTexture 키 방지 ( 배경화면색도 하얀색 )attack
	// GetSceneResource()->SetTextureColorKey("LucidNunNaTeleport",
		// 255, 0, 255);

	for (int i = 0; i < 11; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaTeleport",
			i * 57.3f, 0.f, 57.3f, 78.f);
	}


	// 왼쪽 방향 기본 애니메이션 
	GetSceneResource()->CreateAnimationSequence("LucidNunNaLeftIdle",
		"LucidNunNaLeftIdle", TEXT("Player/Left/astand_left.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaLeftIdle",
		255, 0, 255);

	for (int i = 0; i < 6; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaLeftIdle",
			i * 82.f, 0.f, 82.f, 73.f);
	}

	// Left  Walk
	GetSceneResource()->CreateAnimationSequence("LucidNunNaLeftWalk",
		"LucidNunNaLeftWalk", TEXT("Player/Left/awalk_left.bmp"));
	GetSceneResource()->SetTextureColorKey("LucidNunNaLeftWalk",
		255, 0, 255);

	for (int i = 0; i < 4; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaLeftWalk",
			i * 85.f, 0.f, 85.f, 75.f);
	}

	// Run
	GetSceneResource()->CreateAnimationSequence("LucidNunNaLeftRun",
		"LucidNunNaLeftRun", TEXT("Player/Left/alert_left.bmp"));
	GetSceneResource()->SetTextureColorKey("LucidNunNaLeftRun",
		255, 0, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaLeftRun",
			i * 59.f, 0.f, 59.f, 87.f);
	}

	// right run
	GetSceneResource()->CreateAnimationSequence("LucidNunNaRightRun",
		"LucidNunNaRightRun", TEXT("Player/Right/alert.bmp"));
	GetSceneResource()->SetTextureColorKey("LucidNunNaRightRun",
		255, 0, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaRightRun",
			i * 59.f, 0.f, 59.f, 87.f);
	}

	// Stun
	GetSceneResource()->CreateAnimationSequence("LucidNunNaStun",
		"LucidNunNaStun", TEXT("Player.bmp"));
	GetSceneResource()->SetTextureColorKey("LucidNunNaStun",
		255, 0, 255);
	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaStun",
			i * 32.f, 0.f, 32.f, 32.f);
	}

	// Target Attack 
	GetSceneResource()->CreateAnimationSequence("LucidNunNaTargetAttack",
		"LucidNunNaTargetAttack", TEXT("Player/TargetAttack.bmp"));
	GetSceneResource()->SetTextureColorKey("LucidNunNaTargetAttack",
		255, 0, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaTargetAttack",
			i * 192.f, 0.f, 192.f, 192.f);
	}

	// Destory
	GetSceneResource()->CreateAnimationSequence("LucidNunNaLeftDeath",
		"LucidNunNaLeftDeath", TEXT("Player/Left/dead.bmp"));
	GetSceneResource()->SetTextureColorKey("LucidNunNaLeftDeath",
		255, 0, 255);
	for (int i = 0; i < 1; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaLeftDeath",
			i * 59.f, 0.f, 59.f, 68.f);
	}

	GetSceneResource()->CreateAnimationSequence("LucidNunNaRightDeath",
		"LucidNunNaRightDeath", TEXT("Player/Right/Right_dead.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaRightDeath",
		255, 0, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaRightDeath",
			i * 59.f, 0.f, 59.f, 68.f);
	}

	// Door ---
	GetSceneResource()->CreateAnimationSequence("StageDoor",
		"StageDoor", TEXT("DoorToNextStage.bmp"));
	GetSceneResource()->SetTextureColorKey("StageDoor",
		255, 0, 255);

	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 5; j++)
		{
			GetSceneResource()->AddAnimationFrameData("StageDoor",
				j * 192.f, i * 192.f, 192.f, 192.f);
		}
	}

	// 그외 애니메이션 ---
	// 충돌 효과 애니메이션 
	GetSceneResource()->CreateAnimationSequence("HitRight",
		"HitRight", TEXT("Hit2.bmp"));
	GetSceneResource()->SetTextureColorKey("HitRight",
		255, 0, 255);

	for (int i = 0; i < 6; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("HitRight",
			i * 178.f, 0.f, 178.f, 164.f);
	}

	// 총알
	GetSceneResource()->CreateAnimationSequence("Bullet",
		"Bullet", TEXT("Smoke.bmp"));
	GetSceneResource()->SetTextureColorKey("Bullet",
		255, 0, 255);
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			GetSceneResource()->AddAnimationFrameData("Bullet",
				j * 92.f, i * 92.f, 92.f, 92.f);
		}
	}

	// 텔리포트 마우스 
	GetSceneResource()->CreateAnimationSequence("TeleportMouseDisplay",
		"TeleportMouseDisplay", TEXT("Mouse/h.bmp"));
	GetSceneResource()->SetTextureColorKey("TeleportMouseDisplay", 255, 0, 255);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			GetSceneResource()->AddAnimationFrameData("TeleportMouseDisplay",
				j * 61.f, i * 61.f, 61.f, 61.f);
		}
	}

}

void CMainScene::LoadSound()
{
	GetSceneResource()->LoadSound("BGM", true, "StartBGM", "MainBgm.mp3");
	// GetSceneResource()->SoundPlay("StartBGM");
	GetSceneResource()->SetVolume("BGM", 1);

	GetSceneResource()->LoadSound("Effect", false, "Fire", "Fire1.wav");
	GetSceneResource()->SetVolume("Effect", 1);

	GetSceneResource()->LoadSound("Effect", false, "TextSound", "water-step-01.ogg");

	// Player --> run, dash
	GetSceneResource()->LoadSound("Player", false, "Run", "snow-step-1.ogg");
	GetSceneResource()->LoadSound("Player", false, "Dash", "snow-step-2.ogg");
}

void CMainScene::GoBackToWaitingScene()
{
	CSceneManager::GetInst()->CreateScene<CHomeScene>();
}
