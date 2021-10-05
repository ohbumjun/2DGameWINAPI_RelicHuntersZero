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
#include "../Object/MPPotion.h"
// UI
#include "Camera.h"
#include "../UI/UIMain.h"
#include "../UI/UICharacterStateHUD.h"
#include "../UI/UIImage.h"
// Map
#include "../Map/ScrollMap.h"
#include "../Map/TileMap.h"

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

	// Effect
	CEffectHit *EffectPrototype = CreatePrototype<CEffectHit>("HitEffect");

	// Teleport
	CTeleportMouse *TeleportMousePrototype = CreatePrototype<CTeleportMouse>("TeleportMouse");

	// Bullet 
	CBullet* PlayerBullet = CreatePrototype<CBullet>("PlayerBullet");
	PlayerBullet->SetCollisionProfile("PlayerAttack");

	/*
	CBullet* PlayerLaserBullet = CreatePrototype<CBullet>("PlayerLaserProto");
	PlayerLaserBullet->SetCollisionProfile("PlayerLaser");
	PlayerLaserBullet->SetMoveSpeed(LASER_SPEED);
	PlayerLaserBullet->SetDistance(LASER_DISTANCE);
	*/

	CBullet* SlowMotionAttackBullet = CreatePrototype<CBullet>("SkillSlowMotionAttackBullet");
	SlowMotionAttackBullet->SetCollisionProfile("PlayerAttack");

	// Monster
	CMonster *MonsterPrototype = CreatePrototype<CMonster>("MonsterProto");

	// Collider
	CCollider *Collider = PlayerBullet->FindCollider("Body");
	if (Collider)
		Collider->SetCollisionProfile("PlayerAttack");
	CBullet *MonsterBullet = CreatePrototype<CBullet>("MonsterBullet");
	Collider = MonsterBullet->FindCollider("Body");
	if (Collider)
		Collider->SetCollisionProfile("MonsterAttack");

	// Player
	CPlayer *Player = CreatePlayer("Player", Vector2(230.f, 230.f));
	SetPlayer(Player);

	// Camera
	GetCamera()->SetTarget(Player);
	GetCamera()->SetTargetPivot(0.5f, 0.5f);

	// Monster
	/*
	Vector2 WorldResolution = m_Camera->GetWorldResolution();
	CMonster *Monster = CreateObject<CMonster>("Monster", "MonsterProto", Vector2(300.f + rand() % 700, (float)(rand() % 100)));
	Monster->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, NORMAL_MONSTER_HP_MAX,
							  NORMAL_MONSTER_MP_MAX, 1, 100, 100, 100, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
	Monster->SetMoveSpeed(NORMAL_MONSTER_MOVE_SPEED);

	CMonster* Monster2 = CreateObject<CMonster>("Monster", Vector2(300.f + rand() % 700, (float)(rand() % 100)));
	Monster2->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, NORMAL_MONSTER_HP_MAX,
		NORMAL_MONSTER_MP_MAX, 1, 100, 100, 100, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
	Monster2->SetMoveSpeed(NORMAL_MONSTER_MOVE_SPEED);
	*/

	// Stage Door
	CStageDoor *StageDoor_One = CreateObject<CStageDoor>("StageDoor",
														 Vector2(300.f + rand() % 700, 30.f + rand() % 100),
														 Vector2(50.f, 50.f));
	StageDoor_One->SetDoorStageType(EDoorStage_Type::Stage_One);

	// Windows
	CUIMain *MainWindow = CreateUIWindow<CUIMain>("MainWindow");
	CUICharacterStateHUD *StateWindow = CreateUIWindow<CUICharacterStateHUD>("CharacterStateHUD");

	// Potion
	CMPPotion *MPPotionPrototype = CreatePrototype<CMPPotion>("MPPotion");
	CMPPotion *MPPotion1 = CreateObject<CMPPotion>("MPPotion1", "MPPotion");

	MPPotion1->SetTexture("MPPotion1", TEXT("Potion/MPPotion.bmp"));
	MPPotion1->SetPos(Vector2(300.f, 230.f));

	CHPPotion *HPPotionPrototype = CreatePrototype<CHPPotion>("HPPotion");
	CHPPotion *HPPotion1 = CreateObject<CHPPotion>("HPPotion1", "HPPotion");

	HPPotion1->SetTexture("HPPotion1", TEXT("Potion/HPPotion.bmp"));
	HPPotion1->SetPos(Vector2(200.f, 210.f));
	HPPotion1->SetTextureColorKey(255, 0, 255);

	// Scroll ���� ����
	GetCamera()->SetWorldResolution(2000.f, 2000.f);
	float ScrollWidth = 1500.f - GetCamera()->GetResolution().x; // ScrollMap ũ�Ⱑ 1500.f, 1200.f
	float ScrollHeight = 1200.f - GetCamera()->GetResolution().y;

	float TileMapWidth = GetCamera()->GetWorldResolution().x - GetCamera()->GetResolution().x;
	float TileMapHeight = GetCamera()->GetWorldResolution().y - GetCamera()->GetResolution().y;

	// Scroll Map
	CScrollMap *Map = CreateMap<CScrollMap>("ScrollMap");
	Map->SetSize(1280.f, 720.f);
	Map->SetTexture("ScrollBack", TEXT("Stage1.bmp"));

	// Scroll ������, ScrollMap, TileMap ������ �����Ѵ�
	Map->SetScrollRatio(ScrollWidth / TileMapWidth, ScrollHeight / TileMapHeight);
	Map->SetZOrder(0);
	Map->SetTextureColorKey(255, 0, 255);

	// Tile Map
	CTileMap *TileMap = CreateMap<CTileMap>("TileMap");
	TileMap->LoadFile("MainMap.map");
	TileMap->SetZOrder(1);

	// ���ϴ� ũ���� World Resolution��,
	// TileMap ���� ���� ����� �Ѵ�
	// �� �׷���,���ּ� ������ TileMap ũ�⿡ ���缭
	// World Resolution�� ������� ���̴�
	GetCamera()->SetWorldResolution(2000.f, 2000.f);

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

	GetSceneResource()->CreateAnimationSequence("LucidNunNaRightIdle",
		"LucidNunNaRightIdle", TEXT("images/Character/jimmy/sample.bmp"));
	GetSceneResource()->SetTextureColorKey("LucidNunNaRightIdle",255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData("LucidNunNaRightIdle",
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Player ---
	/*
	std::vector<std::wstring> vecFileName;
	for (int i = 0; i <= 11; ++i)
	{
		TCHAR FileName[MAX_PATH] = {};
		wsprintf(FileName, TEXT("images/Character/jimmy/E_spr_jimmy_idle_%d.bmp"), i);
		vecFileName.push_back(FileName);
	}
	GetSceneResource()->CreateAnimationSequence("LucidNunNaRightIdle",
												"LucidNunNaRightIdle", vecFileName);
	GetSceneResource()->SetTextureColorKey("LucidNunNaRightIdle",
										   255, 0, 255);
	for (int i = 0; i < 11; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaRightIdle",
												  0.f, 0.f, 114.f, 94.f);
	}
	CTexture *Texture = CResourceManager::GetInst()->FindTexture("MouseDefault");

	for (int i = 0; i <= 10; ++i)
	{
		Texture->SetColorKey(255, 0, 255, i);
	}

	MouseDefault->SetTexture(Texture);

	for (int i = 0; i <= 10; ++i)
	{
		MouseDefault->AddFrameData(Vector2(0.f, 0.f), Vector2(32.f, 31.f));
	}

	m_vecMouseImage.push_back(MouseDefault);
	*/

	GetSceneResource()->CreateAnimationSequence("LucidNunNaRightWalk",
												"LucidNunNaRightWalk", TEXT("Player/Right/awalk.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaRightWalk",
										   255, 0, 255);

	for (int i = 0; i < 4; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaRightWalk",
												  i * 85.f, 0.f, 85.f, 75.f);
	}

	// Right Attack
	GetSceneResource()->CreateAnimationSequence("LucidNunNaRightAttack",
												"LucidNunNaRightAttack", TEXT("Player/Right/aswing.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaRightAttack",
										   255, 0, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaRightAttack",
												  i * 176.f, 0.f, 176.f, 89.f);
	}

	// Left Attack
	// Left Attack
	GetSceneResource()->CreateAnimationSequence("LucidNunNaLeftAttack",
												"LucidNunNaLeftAttack", TEXT("Player/Left/aswing_left.bmp"));

	GetSceneResource()->SetTextureColorKey("LucidNunNaLeftAttack",
										   255, 0, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaLeftAttack",
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

	// �ڷ���Ʈ ĳ����
	/* 1��° ����
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

	// 3��° ����
	GetSceneResource()->CreateAnimationSequence("LucidNunNaTeleport",
												"LucidNunNaTeleport", TEXT("Player/teleport2.bmp"));

	// �ӽ������� SetTexture Ű ���� ( ���ȭ����� �Ͼ�� )attack
	// GetSceneResource()->SetTextureColorKey("LucidNunNaTeleport",
	// 255, 0, 255);

	for (int i = 0; i < 11; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("LucidNunNaTeleport",
												  i * 57.3f, 0.f, 57.3f, 78.f);
	}

	// ���� ���� �⺻ �ִϸ��̼�
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

	// �׿� �ִϸ��̼� ---
	// �浹 ȿ�� �ִϸ��̼�
	GetSceneResource()->CreateAnimationSequence("HitRight",
												"HitRight", TEXT("Hit2.bmp"));
	GetSceneResource()->SetTextureColorKey("HitRight",
										   255, 0, 255);

	for (int i = 0; i < 6; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("HitRight",
												  i * 178.f, 0.f, 178.f, 164.f);
	}

	// �Ѿ�
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

	// �ڸ���Ʈ ���콺
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
