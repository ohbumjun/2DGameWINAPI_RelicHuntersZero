#include "HomeScene.h"
#include "SceneResource.h"
#include "SceneManager.h"
#include "../Object/Player.h"
#include "../Object/LaserObject.h"
#include "../Scene/Camera.h"
#include "../Object/Gun.h"
#include "../Object/StageDoor.h"
#include "../Object/EffectHit.h"
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

	// Objects
	CPlayer* Player = CreatePlayer("Player", Vector2(230.f, 230.f));
	SetPlayer(Player);


	Vector2 WorldResolution = m_Camera->GetWorldResolution();
	CStageDoor* StageDoor_One = CreateObject<CStageDoor>("StageDoor", 
		Vector2(300.f + rand() % 700, 30.f + rand() % 100),
		Vector2(50.f,50.f));
	StageDoor_One->SetDoorStageType(EDoorStage_Type::Stage_Home);



	// Window
	CUICharacterStateHUD* StateWindow = CreateUIWindow<CUICharacterStateHUD>("CharacterStateHUD");

	// Gun
	// Start ==
	CGun* GunPistolLight = CreateObject<CGun>(GUN_PISTOL_LIGHT);
	GunPistolLight->SetTexture(GUN_PISTOL_LIGHT, TEXT("images/Weapon/Gun/spr_pistol_0.bmp"));
	GunPistolLight->SetPos(200.f,200.f);

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

