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
	Vector2 WorldResolution = m_Camera->GetWorldResolution();
	CMonster *Monster = CreateObject<CMonster>("Monster", "MonsterProto", Vector2(300.f + rand() % 700, (float)(rand() % 100)));
	Monster->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, NORMAL_MONSTER_HP_MAX,
							  NORMAL_MONSTER_MP_MAX, 1, 100, 100, 100, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
	Monster->SetMoveSpeed(NORMAL_MONSTER_MOVE_SPEED);

	/*
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

	GetCamera()->SetWorldResolution(2000.f, 2000.f);

	return true;
}

void CMainScene::LoadAnimationSequence()
{
	SetPlayerAnimation();
	SetItemsAnimation();
	SetBulletsAnimation();
	SetCollideAnimation();
	SetMouseAnimation();
	SetPlayerAnimation();

	SetLevel1MonsterAnimation();
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
