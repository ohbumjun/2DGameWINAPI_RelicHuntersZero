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
#include "../Object/EffectDash.h"
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
	SetBasicProtoTypes();
	
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
	
	CMPPotion *MPPotion1 = CreateObject<CMPPotion>("MPPotion1", "MPPotion");
	MPPotion1->SetTexture("MPPotion1", TEXT("Potion/MPPotion.bmp"));
	MPPotion1->SetPos(Vector2(300.f, 230.f));

	CHPPotion *HPPotion1 = CreateObject<CHPPotion>("HPPotion1", "HPPotion");
	HPPotion1->SetTexture("HPPotion1", TEXT("Potion/HPPotion.bmp"));
	HPPotion1->SetPos(Vector2(200.f, 210.f));
	HPPotion1->SetTextureColorKey(255, 0, 255);

	
	// Tile Map
	CTileMap *TileMap = CreateMap<CTileMap>("TileMap");
	TileMap->LoadFile("MainMap.map");
	TileMap->SetZOrder(1);

	// WorldResolution is set to TileMap
	// if you want the other World Resolution Size instead of TileMap
	// Then, you have to set world resolution at the end  
	// or after TileMap
	GetCamera()->SetWorldResolution(2000.f, 2000.f);

	// ScrollMap Size 1500.f, 1200.f
	// Scroll Map : Sky
	float	ScrollWidth = 1500.f - GetCamera()->GetResolution().x;
	float	ScrollHeight = 1200.f - GetCamera()->GetResolution().y;

	float	TileScrollMapWidth  = GetCamera()->GetWorldResolution().x - GetCamera()->GetResolution().x;
	float	TileScrollMapHeight = GetCamera()->GetWorldResolution().y - GetCamera()->GetResolution().y;

	CScrollMap* Map = CreateMap<CScrollMap>("ScrollMap");
	Map->SetSize(1280.f, 720.f);
	Map->SetTexture("Sky", TEXT("Sky.bmp")); // Sky.bmp : 1500, 1200
	Map->SetScrollRatio(ScrollWidth / TileScrollMapWidth, ScrollHeight / TileScrollMapHeight);
	Map->SetZOrder(0);

	// Scroll Map : Mountain
	Map = CreateMap<CScrollMap>("ScrollMap");
	ScrollWidth = 2048.f - GetCamera()->GetResolution().x;  // Mountain.bmp : 2048 * 2048
	ScrollHeight = 2048.f - GetCamera()->GetResolution().y;

	Map->SetSize(1280.f, 720.f);
	Map->SetTexture("Mountain", TEXT("Mountain.bmp"));
	Map->SetScrollRatio(ScrollWidth / TileScrollMapWidth, ScrollHeight / TileScrollMapHeight);
	Map->SetZOrder(1);
	Map->SetTextureColorKey(255, 0, 255);

	/*
	Scroll Map Infinite Loop

	GetCamera()->SetWorldResolution(300000.f, 120000.f);
	// ScrollMap Size 1500.f, 1200.f
	// Scroll Map : Sky
	CScrollMap* Map = CreateMap<CScrollMap>("ScrollMap");

	float	ScrollWidth = 1500.f - GetCamera()->GetResolution().x;
	float	ScrollHeight = 1200.f - GetCamera()->GetResolution().y;

	float	TileMapWidth = 3000.f - GetCamera()->GetResolution().x;
	float	TileMapHeight = 1200.f - GetCamera()->GetResolution().y;

	Map->SetSize(1280.f, 720.f);
	Map->SetTexture("ScrollBack", TEXT("Sky.bmp"));
	Map->SetLoop(true);
	Map->SetZOrder(0);

	*/

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
