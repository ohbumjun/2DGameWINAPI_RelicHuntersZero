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
#include "../Object/DuckMonster.h"
#include "../Object/KamikazeMonster.h"
#include "../Object/WallObject.h"
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
#include "../UI/UICommon.h"
#include "../UI/UICharacterStateHUD.h"
#include "../UI/UIImage.h"
#include "../UI/UISceneStart.h"
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
	SetBasicUIs();

	// Player
	CPlayer* Player = CreatePlayer("Player", Vector2(350.f, 320.f));
	SetPlayer(Player);

	// Camera
	GetCamera()->SetTarget(Player);
	GetCamera()->SetTargetPivot(0.5f, 0.5f);
	
	// Tile Map
	CTileMap* TileMap = CreateMap<CTileMap>("TileMap");
	TileMap->LoadFile("Stage1.map");

	// Monster
	SetMonsterOnTileMap<
		CDuckMonster, 
		CDuckMonster, 
		CDuckMonster>
		(MONSTER_DUCK1_PROTO, MONSTER_DUCK2_PROTO, MONSTER_DUCK3_PROTO);
	
	// Stage Door
	CStageDoor *StageDoor = CreateObject<CStageDoor>("StageDoor1",
														 Vector2(200.f,300.f),
														 Vector2(50.f, 50.f));
	StageDoor->SetDoorStageType(EDoorStage_Type::Stage_Home);
	CWidgetComponent* DoorNameWidget = StageDoor->CreateWidgetComponent(NAMEWIDGET_COMPONENET);
	DoorNameWidget->SetPos(-60.f, 20.f);
	CUIText* NameText = DoorNameWidget->CreateWidget<CUIText>("StageDoorHomeText");
	NameText->SetTextColor(255, 255, 255);
	NameText->SetText(TEXT("STAGE HOME"));

	StageDoor = CreateObject<CStageDoor>("StageDoor2",
		Vector2(3500.f, 2200.f),
		Vector2(50.f, 50.f));
	StageDoor->SetDoorStageType(EDoorStage_Type::Stage_Two);
	DoorNameWidget = StageDoor->CreateWidgetComponent(NAMEWIDGET_COMPONENET);
	DoorNameWidget->SetPos(-40.f, 20.f);
	NameText = DoorNameWidget->CreateWidget<CUIText>("StageDoorThreeText");
	NameText->SetTextColor(255, 255, 255);
	NameText->SetText(TEXT("STAGE 2"));

	// Windows
	CUIMain *MainWindow = CreateUIWindow<CUIMain>("MainWindow");
	CUICommon* CommonWindow = CreateUIWindow<CUICommon>("CommonUI");
	CommonWindow->SetSceneStage(ESceneStage::One);


	// Scroll Map
	float	ScrollWidth = 1500.f - GetCamera()->GetResolution().x;
	float	ScrollHeight = 1200.f - GetCamera()->GetResolution().y;

	float	TileScrollMapWidth = GetCamera()->GetWorldResolution().x - GetCamera()->GetResolution().x;
	float	TileScrollMapHeight = GetCamera()->GetWorldResolution().y - GetCamera()->GetResolution().y;

	CScrollMap* Map = CreateMap<CScrollMap>("ScrollMap");
	Map->SetSize(2000.f, 1000.f);
	Map->SetTexture("Sky", TEXT("Stage1Back.bmp")); 
	Map->SetScrollRatio(ScrollWidth / TileScrollMapWidth, ScrollHeight / TileScrollMapHeight);
	Map->SetZOrder(-1);

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
	SetShieldAnimation();
	SetSceneStartAnimation();
	SetGrenadeAnimation();
	SetBossAnimation();

	SetLevel1MonsterAnimation();
	SetKamikaze2MonsterAnimation();
}

void CMainScene::LoadSound()
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

void CMainScene::GoBackToWaitingScene()
{
	CSceneManager::GetInst()->CreateScene<CHomeScene>();
}
