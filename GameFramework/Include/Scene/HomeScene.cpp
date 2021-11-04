#include "HomeScene.h"
#include "SceneResource.h"
#include "SceneManager.h"
#include "../Object/Player.h"
#include "../Object/PlayerClone.h"
#include "../Object/LaserObject.h"
#include "../Object/Npc.h"
#include "../Scene/Camera.h"
#include "../Object/Gun.h"
#include "../Object/Coin.h"
#include "../Object/EffectBigWindow.h"
#include "../Object/Monster.h"
#include "../Object/StageDoor.h"
#include "../Object/EffectHit.h"
#include "../Object/MPPotion.h"
#include "../Object/HPPotion.h"
#include "../UI/UICharacterStateHUD.h"
#include "../UI/UICommon.h"
#include "../UI/UIHome.h"
#include "../UI/WidgetComponent.h"
#include "../Map/TileMap.h"
#include "../GameManager.h"

CHomeScene::CHomeScene()
{
}

CHomeScene::~CHomeScene()
{
	auto iter    = PlayerCloneLists.begin();
	auto iterEnd = PlayerCloneLists.end();
	for (; iter != iterEnd; ++iter)
		(*iter)->Destroy();
	PlayerCloneLists.clear();
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
	// CPlayer* Player = CreateObject<CPlayer>("Player", Vector2(1900.f, 530.f));
	CPlayer* Player = CreatePlayer("Player", Vector2(1900.f, 590.f));
	SetPlayer(Player);

	// Camera
	GetCamera()->SetTarget(Player);
	GetCamera()->SetTargetPivot(0.5f, 0.5f);

	// Big Window
	CEffectBigWindow* BigWindow = CreateObject<CEffectBigWindow>("BigWindow",
		Vector2(1650.f, 1100.f),
		Vector2(50.f, 50.f));
	BigWindow = CreateObject<CEffectBigWindow>("BigWindow2",
		Vector2(2150.f, 1100.f),
		Vector2(50.f, 50.f));

	// Characters
	CharacterInfo DefaultCharInfo;
	CPlayerClone* PlayerClone = CreateObject<CPlayerClone>(
		"PlayerClone1",
		Vector2(1500.f, 1200.f));
	PlayerClone->SetCharType(EChar_Type::Ass);
	PlayerClone->SetAnimName();
	PlayerClone->SetCharacterInfo(DefaultCharInfo);
	PlayerClone->SetMoveEnable(false);

	PlayerClone = (CPlayerClone*)CreateObject<CPlayerClone>(
		"PlayerClone2",
		Vector2(1650.f, 1200.f));
	PlayerClone->SetCharType(EChar_Type::Biu);
	PlayerClone->SetAnimName();
	PlayerClone->SetCharacterInfo(DefaultCharInfo);
	PlayerClone->SetMoveEnable(false);

	PlayerClone = (CPlayerClone*)CreateObject<CPlayerClone>(
		"PlayerClone3",
		Vector2(1800.f, 1200.f));
	PlayerClone->SetCharType(EChar_Type::Jimmy);
	PlayerClone->SetAnimName();
	PlayerClone->SetCharacterInfo(DefaultCharInfo);
	PlayerClone->SetMoveEnable(false);

	PlayerClone = (CPlayerClone*)CreateObject<CPlayerClone>(
		"PlayerClone4",
		Vector2(1950.f, 1200.f));
	PlayerClone->SetCharType(EChar_Type::Pinky);
	PlayerClone->SetAnimName();
	PlayerClone->SetCharacterInfo(DefaultCharInfo);
	PlayerClone->SetMoveEnable(false);

	PlayerClone = (CPlayerClone*)CreateObject<CPlayerClone>(
		"PlayerClone5",
		Vector2(2100.f, 1200.f));
	PlayerClone->SetCharType(EChar_Type::Punny);
	PlayerClone->SetAnimName();
	PlayerClone->SetCharacterInfo(DefaultCharInfo);
	PlayerClone->SetMoveEnable(false);

	PlayerClone = (CPlayerClone*)CreateObject<CPlayerClone>(
		"PlayerClone6",
		Vector2(2250.f, 1200.f));
	PlayerClone->SetCharType(EChar_Type::Raff);
	PlayerClone->SetAnimName();
	PlayerClone->SetCharacterInfo(DefaultCharInfo);
	PlayerClone->SetMoveEnable(false);

	// Stage Door
	CStageDoor* StageDoor1 = CreateObject<CStageDoor>("StageDoorOne", 
		Vector2(1300.f, 2100.f),
		Vector2(50.f,50.f));
	StageDoor1->SetDoorStageType(EDoorStage_Type::Stage_One);
	CWidgetComponent* DoorNameWidget = StageDoor1->CreateWidgetComponent(NAMEWIDGET_COMPONENET);
	DoorNameWidget->SetPos(-40.f, 20.f);
	CUIText* NameText = DoorNameWidget->CreateWidget<CUIText>("StageDoorOneText");
	NameText->SetTextColor(255, 255, 255);
	NameText->SetText(TEXT("STAGE 1"));

	CStageDoor* StageDoor2 = CreateObject<CStageDoor>("StageDoorTwo",
		Vector2(1950.f, 2100.f),
		Vector2(50.f, 50.f));
	StageDoor2->SetDoorStageType(EDoorStage_Type::Stage_Two);
	DoorNameWidget = StageDoor2->CreateWidgetComponent(NAMEWIDGET_COMPONENET);
	DoorNameWidget->SetPos(-40.f, 20.f);
	NameText = DoorNameWidget->CreateWidget<CUIText>("StageDoorTwoText");
	NameText->SetTextColor(255, 255, 255);
	NameText->SetText(TEXT("STAGE 2"));

	CStageDoor* StageDoor3 = CreateObject<CStageDoor>("StageDoorThree",
		Vector2(2600.f, 2100.f),
		Vector2(50.f, 50.f));
	StageDoor3->SetDoorStageType(EDoorStage_Type::Stage_Three);
	DoorNameWidget = StageDoor3->CreateWidgetComponent(NAMEWIDGET_COMPONENET);
	DoorNameWidget->SetPos(-40.f, 20.f);
	NameText = DoorNameWidget->CreateWidget<CUIText>("StageDoorThreeText");
	NameText->SetTextColor(255, 255, 255);
	NameText->SetText(TEXT("STAGE 3"));

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
	NpcShield->SetCost(60);

	// Map
	CTileMap* TileMap = CreateMap<CTileMap>("TileMap");
	TileMap->LoadFile("StageHome.map");

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
	SetGrenadeAnimation();
}

void CHomeScene::LoadSound()
{
	float MasterVol     = CGameManager::GetInst()->GetMasterVolume();
	int BGVol         = CGameManager::GetInst()->GetBGVolume();
	int EffectVol     = CGameManager::GetInst()->GetEffectVolume();

	GetSceneResource()->LoadSound("BGM", true, "StartBGM", "HomeBgm.wav");
	// GetSceneResource()->LoadSound("BGM", true, "NormalBGM", "NormalBgm.wav");
	// GetSceneResource()->LoadSound("BGM", true, "BossBGM", "BossBgm.wav");
	GetSceneResource()->SoundPlay("StartBGM");
	GetSceneResource()->SetVolume("BGM", (int)(MasterVol * BGVol));

	GetSceneResource()->LoadSound("Effect", false, "GreNade", "GreNade.mp3");
	GetSceneResource()->LoadSound("Effect", false, "Hit", "BulletHit.wav");
	GetSceneResource()->LoadSound("Effect", false, "MonsterDestroy", "MonsterDestroy.wav");
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
