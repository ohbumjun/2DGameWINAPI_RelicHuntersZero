// Gamae
#include "../GameManager.h"
// Scene
#include "Main3Scene.h"
#include "SceneResource.h"
#include "SceneManager.h"
#include "HomeScene.h"
// Object
#include "../MonsterInfo.h"
#include "../Object/Player.h"
#include "../Object/Bullet.h"
#include "../Object/Monster.h"
#include "../Object/BossMonster.h"
#include "../Object/BossEgg.h"
#include "../Object/EffectHit.h"
#include "../Object/EffectDash.h"
#include "../Object/KamiKazeMonster.h"
#include "../Object/KamiKazeCage.h"
#include "../Object/StageDoor.h"
#include "../Object/HPPotion.h"
#include "../Object/MPPotion.h"
// UI
#include "Camera.h"
#include "../UI/UIMain.h"
#include "../UI/UICommon.h"
#include "../UI/UICharacterStateHUD.h"
#include "../UI/UIBossStateHUD.h"
#include "../UI/UIImage.h"
// Map
#include "../Map/ScrollMap.h"
#include "../Map/TileMap.h"

CMain3Scene::CMain3Scene()
{
}

CMain3Scene::~CMain3Scene()
{
}

bool CMain3Scene::Init()
{
	LoadSound();
	LoadAnimationSequence();
	SetBasicProtoTypes();
	SetBasicUIs();

	// Monster
	CStageDoor* StageDoor = CreateObject<CStageDoor>("StageDoorHome",
		Vector2(600.f, 300.f),
		Vector2(50.f, 50.f));
	StageDoor->SetDoorStageType(EDoorStage_Type::Stage_Home);
	CWidgetComponent* DoorNameWidget = StageDoor->CreateWidgetComponent(NAMEWIDGET_COMPONENET);
	DoorNameWidget->SetPos(-60.f, 20.f);
	CUIText* NameText = DoorNameWidget->CreateWidget<CUIText>("StageDoorThreeText");
	NameText->SetTextColor(255, 255, 255);
	NameText->SetText(TEXT("STAGE HOME"));


	// Player
	CPlayer* Player = CreatePlayer("Player", Vector2(600.f, 330.f));
	SetPlayer(Player);

	// Camera
	GetCamera()->SetTarget(Player);
	GetCamera()->SetTargetPivot(0.5f, 0.5f);

	// Windows
	CUIMain* MainWindow = CreateUIWindow<CUIMain>("MainWindow");
	CUICommon* CommonWindow = CreateUIWindow<CUICommon>("CommonUI");
	CommonWindow->SetSceneStage(ESceneStage::Boss);

	// Tile Map
	CTileMap* TileMap = CreateMap<CTileMap>("TileMap");
	TileMap->LoadFile("Stage3.map");

	// Monster
	// SetMonsterOnTileMap<CTurtleMonster>(MONSTER_TURTLE1_PROTO, MONSTER_TURTLE2_PROTO, MONSTER_TURTLE3_PROTO);
	CBossEgg* BossEgg = CreateObject<CBossEgg>("BossEgg",
		Vector2(1700.f, 1500.f));

	// Scroll Map
	float	ScrollWidth = 1500.f - GetCamera()->GetResolution().x;
	float	ScrollHeight = 1200.f - GetCamera()->GetResolution().y;

	float	TileScrollMapWidth = GetCamera()->GetWorldResolution().x - GetCamera()->GetResolution().x;
	float	TileScrollMapHeight = GetCamera()->GetWorldResolution().y - GetCamera()->GetResolution().y;

	CScrollMap* Map = CreateMap<CScrollMap>("BackGroundMap");
	Map->SetSize(2000.f, 1000.f);
	Map->SetTexture("Sky", TEXT("Stage3Back.bmp")); 
	Map->SetScrollRatio(ScrollWidth / TileScrollMapWidth, ScrollHeight / TileScrollMapHeight);
	Map->SetZOrder(-1);

	// UI
	CUIBossStateHUD* BossStateWindow = CreateUIWindow<CUIBossStateHUD>("BossStateHUD");

	
	return true;
}

void CMain3Scene::LoadAnimationSequence()
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

	SetDuck1MonsterAnimation();
	SetDuck2MonsterAnimation();
	SetDuck3MonsterAnimation();
	SetTurtle1MonsterAnimation();
	SetTurtle2MonsterAnimation();
	SetTurtle3MonsterAnimation();

	SetLevel3MonsterAnimation();
}

void CMain3Scene::LoadSound()
{
	float MasterVol = CGameManager::GetInst()->GetMasterVolume();
	int BGVol = CGameManager::GetInst()->GetBGVolume();
	int EffectVol = CGameManager::GetInst()->GetEffectVolume();

	// GetSceneResource()->LoadSound("BGM", true, "StartBGM", "HomeBgm.wav");
	// GetSceneResource()->LoadSound("BGM", true, "NormalBGM", "NormalBgm.wav");
	GetSceneResource()->LoadSound("BGM", true, "BossBGM", "BossBgm.wav");
	GetSceneResource()->SoundPlay("BossBGM");
	GetSceneResource()->SetVolume("BGM", (int)(MasterVol * BGVol));

	GetSceneResource()->LoadSound("Effect", false, "GreNade", "GreNade.mp3");
	GetSceneResource()->LoadSound("Effect", false, "Hit", "BulletHit.wav");
	GetSceneResource()->LoadSound("Effect", false, "MonsterDestroy", "Death.wav");
	GetSceneResource()->LoadSound("Effect", false, "PistolFire", "Pistol.mp3");
	GetSceneResource()->LoadSound("Effect", false, "ShotGunFire", "ShotGun.wav");
	GetSceneResource()->LoadSound("Effect", false, "GameEnd", "GameEnd.wav");
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

void CMain3Scene::GoBackToWaitingScene()
{
	CSceneManager::GetInst()->CreateScene<CHomeScene>();
}
