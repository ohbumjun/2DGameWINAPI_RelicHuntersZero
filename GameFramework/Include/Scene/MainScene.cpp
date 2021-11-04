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
	CStageDoor *StageDoor = CreateObject<CStageDoor>("StageDoorHome",
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
	StageDoor->SetLimitPlayerPass(false);
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

	// GetSceneResource()->LoadSound("BGM", true, "StartBGM", "HomeBgm.wav");
	GetSceneResource()->LoadSound("BGM", true, "NormalBGM", "NormalBgm.wav");
	// GetSceneResource()->LoadSound("BGM", true, "BossBGM", "BossBgm.wav");
	GetSceneResource()->SoundPlay("NormalBGM");
	GetSceneResource()->SetVolume("BGM", (int)(MasterVol * BGVol));

	GetSceneResource()->LoadSound("Effect", false, "GreNade", "GreNade.mp3");
	GetSceneResource()->LoadSound("Effect", false, "Hit", "BulletHit.wav");
	// GetSceneResource()->LoadSound("Effect", false, "MonsterDestroy", "MonsterDestroy.wav");
	GetSceneResource()->LoadSound("Effect", false, "MonsterDestroy", "Death.wav");
	GetSceneResource()->LoadSound("Effect", false, "PistolFire", "Pistol.mp3");
	GetSceneResource()->LoadSound("Effect", false, "ShotGunFire", "ShotGun.wav");
	GetSceneResource()->LoadSound("Effect", false, "TextSound", "water-step-01.ogg");
	GetSceneResource()->SetVolume("Effect", (int)(MasterVol * EffectVol));

	// Player --> run, dash
	GetSceneResource()->LoadSound("Player", false, "Run", "snow-step-1.ogg");
	GetSceneResource()->LoadSound("Player", false, "Dash", "snow-step-2.ogg");
	GetSceneResource()->LoadSound("Player", false, "Equip", "Equip.wav");
	GetSceneResource()->LoadSound("Player", false, "Teleport", "Teleport.wav");
	GetSceneResource()->LoadSound("Player", false, "ItemUse", "ItemUse.wav");
	GetSceneResource()->LoadSound("Player", false, "Buy", "BuyItem.wav");
	GetSceneResource()->LoadSound("Player", false, "Reload", "Reload.wav");
	GetSceneResource()->LoadSound("Player", false, "Skill", "Skill.wav");
	GetSceneResource()->SetVolume("Player", (int)(MasterVol * EffectVol));
}

void CMainScene::GoBackToWaitingScene()
{
	CSceneManager::GetInst()->CreateScene<CHomeScene>();
}
