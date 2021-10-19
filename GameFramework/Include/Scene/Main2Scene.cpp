// Gamae
#include "../GameManager.h"

// Scene
#include "Main2Scene.h"
#include "SceneResource.h"
#include "SceneManager.h"
#include "HomeScene.h"
// Object
#include "../MonsterInfo.h"
#include "../Object/Player.h"
#include "../Object/Bullet.h"
#include "../Object/Monster.h"
#include "../Object/DuckMonster.h"
#include "../Object/TurtleMonster.h"
#include "../Object/EffectHit.h"
#include "../Object/EffectDash.h"
#include "../Object/TeleportMouse.h"
#include "../Object/KamiKazeMonster.h"
#include "../Object/KamiKazeCage.h"
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

CMain2Scene::CMain2Scene()
{
}

CMain2Scene::~CMain2Scene()
{
}

bool CMain2Scene::Init()
{
	LoadSound();
	LoadAnimationSequence();
	SetBasicProtoTypes();
	SetBasicUIs();


	// Monster
	CKamiKazeCage* KamiKaze = CreateObject<CKamiKazeCage>("KamiKazeCage", MONSTER_KAMIKAZECAGE_PROTO, 
		Vector2(300.f,300.f));

	CGun* PistolLightGun = CreateObject<CGun>(GUN_PISTOL_LIGHT, GUN_PISTOL_LIGHT_PROTO);
	PistolLightGun = CreateObject<CGun>(GUN_PISTOL_LIGHT, GUN_PISTOL_LIGHT_PROTO);
	CTurtleMonster* TurtleMonster = CreateObject<CTurtleMonster>("Monster2", MONSTER_TURTLE2_PROTO, 
		Vector2(300.f + rand() % 700, 500.f + (float)(rand() % 100)));
	TurtleMonster->Equip(PistolLightGun);


	CStageDoor* StageDoor = CreateObject<CStageDoor>("StageDoorHome",
		Vector2(600.f, 300.f),
		Vector2(50.f, 50.f));
	StageDoor->SetDoorStageType(EDoorStage_Type::Stage_Home);

	StageDoor = CreateObject<CStageDoor>("StageDoor3",
		Vector2(3500.f, 2200.f),
		Vector2(50.f, 50.f));
	StageDoor->SetDoorStageType(EDoorStage_Type::Stage_Three);

	// Player
	CPlayer* Player = CreatePlayer("Player", Vector2(600.f, 330.f));
	SetPlayer(Player);

	// Camera
	GetCamera()->SetTarget(Player);
	GetCamera()->SetTargetPivot(0.5f, 0.5f);

	// Windows
	CUIMain* MainWindow = CreateUIWindow<CUIMain>("MainWindow");

	// ScrollMap Size 1500.f, 1200.f
	// Scroll Map : Sky
	float	ScrollWidth = 1500.f - GetCamera()->GetResolution().x;
	float	ScrollHeight = 1200.f - GetCamera()->GetResolution().y;

	float	TileScrollMapWidth = GetCamera()->GetWorldResolution().x - GetCamera()->GetResolution().x;
	float	TileScrollMapHeight = GetCamera()->GetWorldResolution().y - GetCamera()->GetResolution().y;

	/*


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
	*/

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

	// Tile Map
	CTileMap* TileMap = CreateMap<CTileMap>("TileMap");
	TileMap->LoadFile("Stage2.map");
	TileMap->SetZOrder(-1);

	// Monster
	SetMonsterOnTileMap<
		CTurtleMonster,
		CTurtleMonster,
		CTurtleMonster>
		(MONSTER_TURTLE1_PROTO, MONSTER_TURTLE2_PROTO, MONSTER_TURTLE3_PROTO);

	return true;
}

void CMain2Scene::LoadAnimationSequence()
{
	SetPlayerAnimation();
	SetItemsAnimation();
	SetBulletsAnimation();
	SetCollideAnimation();
	SetMouseAnimation();
	SetPlayerAnimation();
	SetShieldAnimation();

	SetLevel2MonsterAnimation();
}

void CMain2Scene::LoadSound()
{
	// GetSceneResource()->LoadSound("BGM", true, "StartBGM", "MainBgm.mp3");
	// GetSceneResource()->SoundPlay("StartBGM");
	// GetSceneResource()->SetVolume("BGM", 1);

	GetSceneResource()->LoadSound("Effect", false, "Fire", "Fire1.wav");
	GetSceneResource()->SetVolume("Effect", 1);

	GetSceneResource()->LoadSound("Effect", false, "TextSound", "water-step-01.ogg");

	// Player --> run, dash
	GetSceneResource()->LoadSound("Player", false, "Run", "snow-step-1.ogg");
	GetSceneResource()->LoadSound("Player", false, "Dash", "snow-step-2.ogg");
}

void CMain2Scene::GoBackToWaitingScene()
{
	CSceneManager::GetInst()->CreateScene<CHomeScene>();
}
