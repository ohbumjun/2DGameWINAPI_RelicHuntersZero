#include "HomeScene.h"
#include "SceneResource.h"
#include "SceneManager.h"
#include "../Object/Player.h"
#include "../Object/LaserObject.h"
#include "../Object/Npc.h"
#include "../Scene/Camera.h"
#include "../Object/Gun.h"
#include "../Object/Coin.h"
#include "../Object/Monster.h"
#include "../Object/StageDoor.h"
#include "../Object/EffectHit.h"
#include "../Object/MPPotion.h"
#include "../Object/HPPotion.h"
#include "../UI/UICharacterStateHUD.h"
#include "../UI/UICommon.h"
#include "../UI/UIHome.h"
#include "../Map/TileMap.h"

CHomeScene::CHomeScene()
{
}

CHomeScene::~CHomeScene()
{
}

bool CHomeScene::Init()
{
	// Sound 
	LoadSound();
	// Animation
	LoadAnimationSequence();
	// Protos
	SetBasicProtoTypes();
	// UI
	SetBasicUIs();
	CUIHome* HomeWindow = CreateUIWindow<CUIHome>("HomeUI");
	// Guns
	SetBasicObjectGuns();
	// Objects
	CPlayer* Player = CreatePlayer("Player", Vector2(1900.f, 530.f));
	SetPlayer(Player);

	// Camera
	GetCamera()->SetTarget(Player);
	GetCamera()->SetTargetPivot(0.5f, 0.5f);

	// Stage Door
	CStageDoor* StageDoor = CreateObject<CStageDoor>("StageDoor", 
		Vector2(1300.f, 2100.f),
		Vector2(50.f,50.f));
	StageDoor->SetDoorStageType(EDoorStage_Type::Stage_One);
	StageDoor = CreateObject<CStageDoor>("StageDoor",
		Vector2(1950.f, 2100.f),
		Vector2(50.f, 50.f));
	StageDoor->SetDoorStageType(EDoorStage_Type::Stage_Two);
	StageDoor = CreateObject<CStageDoor>("StageDoor",
		Vector2(2600.f, 2100.f),
		Vector2(50.f, 50.f));
	// StageDoor_One->SetDoorStageType(EDoorStage_Type::Stage_One);
	StageDoor->SetDoorStageType(EDoorStage_Type::Stage_Three);

	// Potion
	CMPPotion* MPPotion1 = CreateObject<CMPPotion>(POTION_MP_PROTO, POTION_MP_PROTO);
	MPPotion1->SetPos(Vector2(300.f, 230.f));

	CHPPotion* HPPotion1 = CreateObject<CHPPotion>(POTION_HP_PROTO, POTION_HP_PROTO);
	HPPotion1->SetPos(Vector2(200.f, 210.f));

	// Coin 
	CCoin* Coin = CreateObject<CCoin>("MonsterCoin", COIN_PROTO, Vector2(500.f,500.f));

	// Npc
	CNpc* NpcMP     = CreateObject<CNpc>(NPC_IDLE,(Vector2(600.f, 1000.f)));
	CNpc* NpcHP     = CreateObject<CNpc>(NPC_IDLE,(Vector2(600.f, 1250.f)));
	NpcHP->SetNpcType(ENpc_Type::Hp);
	CNpc* NpcShield = CreateObject<CNpc>(NPC_IDLE,(Vector2(600.f, 1500.f)));
	NpcShield->SetNpcType(ENpc_Type::Shield);
	NpcShield->SetCost(300.f);

	// Map
	CTileMap* TileMap = CreateMap<CTileMap>("TileMap");
	TileMap->LoadFile("StageHome.map");
	// TileMap->SetTileWorldResolution();

	return true;
}

void CHomeScene::LoadAnimationSequence()
{
	SetPlayerAnimation();
	SetBulletsAnimation();
	SetCollideAnimation();
	SetItemsAnimation();
	SetMouseAnimation();
	SetSkillAnimation();
	SetShieldAnimation();
	SetNpcAnimation();

	// Map
	SetLevel1MonsterAnimation();
}

void CHomeScene::LoadSound()
{
	GetSceneResource()->LoadSound("BGM", true, "StartBGM", "MainBgm.mp3");
	GetSceneResource()->SoundPlay("StartBGM");
	GetSceneResource()->SetVolume("BGM", 1);

	GetSceneResource()->LoadSound("Effect", false, "Fire", "Fire1.wav");
	GetSceneResource()->SetVolume("Effect", 1);

	GetSceneResource()->LoadSound("Effect", false, "TextSound", "water-step-01.ogg");

	// Player --> run, dash
	GetSceneResource()->LoadSound("Player", false, "Run", "snow-step-1.ogg");
	GetSceneResource()->LoadSound("Player", false, "Dash", "snow-step-2.ogg");
}
