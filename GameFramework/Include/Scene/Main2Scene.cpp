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
#include "../UI/UICommon.h"
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
	KamiKaze = CreateObject<CKamiKazeCage>("KamiKazeCage", MONSTER_KAMIKAZECAGE_PROTO,
		Vector2(500.f, 1200.f));
	KamiKaze = CreateObject<CKamiKazeCage>("KamiKazeCage", MONSTER_KAMIKAZECAGE_PROTO,
		Vector2(1500.f, 300.f));
	KamiKaze = CreateObject<CKamiKazeCage>("KamiKazeCage", MONSTER_KAMIKAZECAGE_PROTO,
		Vector2(2500.f, 1300.f));

	CGun* PistolLightGun = CreateObject<CGun>(GUN_PISTOL_LIGHT, GUN_PISTOL_LIGHT_PROTO);
	PistolLightGun = CreateObject<CGun>(GUN_PISTOL_LIGHT, GUN_PISTOL_LIGHT_PROTO);
	CTurtleMonster* TurtleMonster = CreateObject<CTurtleMonster>("Monster2", MONSTER_TURTLE2_PROTO, 
		Vector2(300.f + rand() % 700, 500.f + (float)(rand() % 100)));
	TurtleMonster->Equip(PistolLightGun);


	CStageDoor* StageDoor = CreateObject<CStageDoor>("StageDoorHome",
		Vector2(600.f, 300.f),
		Vector2(50.f, 50.f));
	StageDoor->SetDoorStageType(EDoorStage_Type::Stage_Home);
	CWidgetComponent* DoorNameWidget = StageDoor->CreateWidgetComponent(NAMEWIDGET_COMPONENET);
	DoorNameWidget->SetPos(-60.f, 20.f);
	CUIText* NameText = DoorNameWidget->CreateWidget<CUIText>("StageDoorHomeText");
	NameText->SetTextColor(255, 255, 255);
	NameText->SetText(TEXT("STAGE HOME"));

	StageDoor = CreateObject<CStageDoor>("StageDoor3",
		Vector2(3500.f, 2200.f),
		Vector2(50.f, 50.f));
	StageDoor->SetDoorStageType(EDoorStage_Type::Stage_Three);
	StageDoor->SetLimitPlayerPass(false);
	DoorNameWidget = StageDoor->CreateWidgetComponent(NAMEWIDGET_COMPONENET);
	DoorNameWidget->SetPos(-40.f, 20.f);
	NameText = DoorNameWidget->CreateWidget<CUIText>("StageDoorThreeText");
	NameText->SetTextColor(255, 255, 255);
	NameText->SetText(TEXT("STAGE 3"));

	// Player
	CPlayer* Player = CreatePlayer("Player", Vector2(600.f, 330.f));
	SetPlayer(Player);

	// Camera
	GetCamera()->SetTarget(Player);
	GetCamera()->SetTargetPivot(0.5f, 0.5f);

	// Windows
	CUICommon* CommonWindow = CreateUIWindow<CUICommon>("CommonUI");
	CommonWindow->SetSceneStage(ESceneStage::Two);
	// CUIMain* MainWindow = CreateUIWindow<CUIMain>("MainWindow");

	// Tile Map
	CTileMap* TileMap = CreateMap<CTileMap>("TileMap");
	TileMap->LoadFile("Stage2.map");

	// Monster
	SetMonsterOnTileMap<
		CTurtleMonster,
		CTurtleMonster,
		CTurtleMonster>
		(MONSTER_TURTLE1_PROTO, MONSTER_TURTLE2_PROTO, MONSTER_TURTLE3_PROTO);

	// 
	float	ScrollWidth = 1500.f - GetCamera()->GetResolution().x;
	float	ScrollHeight = 1200.f - GetCamera()->GetResolution().y;

	float	TileScrollMapWidth = GetCamera()->GetWorldResolution().x - GetCamera()->GetResolution().x;
	float	TileScrollMapHeight = GetCamera()->GetWorldResolution().y - GetCamera()->GetResolution().y;

	CScrollMap* Map = CreateMap<CScrollMap>("ScrollMap");
	Map->SetSize(2000.f,1000.f);
	Map->SetTexture("Sky", TEXT("Stage2Back.bmp"));
	Map->SetScrollRatio(ScrollWidth / TileScrollMapWidth, ScrollHeight / TileScrollMapHeight);
	Map->SetZOrder(-1);

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
	SetSceneStartAnimation();
	SetGrenadeAnimation();
	SetBossAnimation();

	SetLevel2MonsterAnimation();
}

void CMain2Scene::LoadSound()
{
	float MasterVol = CGameManager::GetInst()->GetMasterVolume();
	int BGVol = CGameManager::GetInst()->GetBGVolume();
	int EffectVol = CGameManager::GetInst()->GetEffectVolume();

	// GetSceneResource()->LoadSound("BGM", true, "StartBGM", "MainBgm.mp3");
	// GetSceneResource()->SoundPlay("StartBGM");
	// GetSceneResource()->SetVolume("BGM", MasterVol * BGVol);

	GetSceneResource()->LoadSound("Effect", false, "Fire", "Fire1.wav");
	GetSceneResource()->LoadSound("Effect", false, "TextSound", "water-step-01.ogg");
	GetSceneResource()->SetVolume("Effect", (int)(MasterVol * EffectVol));

	// Player --> run, dash
	GetSceneResource()->LoadSound("Player", false, "Run", "snow-step-1.ogg");
	GetSceneResource()->LoadSound("Player", false, "Dash", "snow-step-2.ogg");
	GetSceneResource()->SetVolume("Player", (int)(MasterVol * EffectVol));
}

void CMain2Scene::GoBackToWaitingScene()
{
	CSceneManager::GetInst()->CreateScene<CHomeScene>();
}
