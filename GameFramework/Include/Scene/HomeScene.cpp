#include "HomeScene.h"
#include "SceneResource.h"
#include "SceneManager.h"
#include "../Object/Player.h"
#include "../Object/LaserObject.h"
#include "../Scene/Camera.h"
#include "../Object/Gun.h"
#include "../Object/StageDoor.h"
#include "../Object/EffectHit.h"
#include "../Object/MPPotion.h"
#include "../Object/HPPotion.h"
#include "../UI/UICharacterStateHUD.h"

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
	// Guns
	SetBasicObjectGuns();
	// Objects
	CPlayer* Player = CreatePlayer("Player", Vector2(230.f, 230.f));
	SetPlayer(Player);


	Vector2 WorldResolution = m_Camera->GetWorldResolution();
	// Stage Door
	CStageDoor* StageDoor_One = CreateObject<CStageDoor>("StageDoor", 
		Vector2(300.f + rand() % 700, 30.f + rand() % 100),
		Vector2(50.f,50.f));
	StageDoor_One->SetDoorStageType(EDoorStage_Type::Stage_Home);

	// Potion
	CMPPotion* MPPotion1 = CreateObject<CMPPotion>(POTION_MP_PROTO, POTION_MP_PROTO);
	MPPotion1->SetPos(Vector2(300.f, 230.f));

	CHPPotion* HPPotion1 = CreateObject<CHPPotion>(POTION_HP_PROTO, POTION_HP_PROTO);
	HPPotion1->SetPos(Vector2(200.f, 210.f));


	// Window
	CUICharacterStateHUD* StateWindow = CreateUIWindow<CUICharacterStateHUD>("CharacterStateHUD");

	// Gun
	// Start ==
	

	return true;
}

void CHomeScene::LoadAnimationSequence()
{
	// Player ---
	SetPlayerAnimation();
	SetBulletsAnimation();
	SetCollideAnimation();
	SetItemsAnimation();
	SetMouseAnimation();
	SetSkillAnimation();

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
