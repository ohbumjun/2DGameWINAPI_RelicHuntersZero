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
	SetBasicProtoGuns();
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
	CMPPotion* MPPotion1 = CreateObject<CMPPotion>("MPPotion1", POTION_MP_PROTO);
	MPPotion1->SetPos(Vector2(300.f, 230.f));

	CHPPotion* HPPotion1 = CreateObject<CHPPotion>("HPPotion1", POTION_HP_PROTO);
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

void CHomeScene::SetBasicProtoGuns()
{
	CGun* GunPistolLight = CreateObject<CGun>(GUN_PISTOL_LIGHT);
	GunPistolLight->SetTexture(GUN_PISTOL_LIGHT_R, TEXT(TEXTURE_GUN_PISTOL_LIGHT_R));
	GunPistolLight->SetTexture(GUN_PISTOL_LIGHT_L, TEXT(TEXTURE_GUN_PISTOL_LIGHT_L));
	GunPistolLight->SetPos(200.f, 200.f);
	GunPistolLight->SetTextureColorKey(255.f, 255.f, 255.f);
	GunPistolLight->SetRightTextureName(TEXTURE_GUN_PISTOL_LIGHT_R);
	GunPistolLight->SetLeftTextureName(TEXTURE_GUN_PISTOL_LIGHT_L);

	GunPistolLight = CreateObject<CGun>(GUN_PISTOL_MEDIUM);
	GunPistolLight->SetTexture(GUN_PISTOL_MEDIUM, TEXT(TEXTURE_GUN_PISTOL_MEDIUM_R));
	GunPistolLight->SetPos(200.f, 300.f);
	GunPistolLight->SetGunClass(EGunClass::Medium);
	GunPistolLight->SetGunType(EGun_Type::Medium_Pistol);
	GunPistolLight->SetTextureColorKey(255.f, 255.f, 255.f);
	GunPistolLight->SetLeftTextureName(TEXTURE_GUN_PISTOL_MEDIUM_L);
	GunPistolLight->SetRightTextureName(TEXTURE_GUN_PISTOL_MEDIUM_R);

	GunPistolLight = CreateObject<CGun>(GUN_PISTOL_HEAVY);
	GunPistolLight->SetTexture(GUN_PISTOL_HEAVY, TEXT(TEXTURE_GUN_PISTOL_HEAVY_R));
	GunPistolLight->SetPos(200.f, 400.f);
	GunPistolLight->SetGunClass(EGunClass::Heavy);
	GunPistolLight->SetGunType(EGun_Type::Heavy_Pistol);
	GunPistolLight->SetTextureColorKey(255.f, 255.f, 255.f);
	GunPistolLight->SetLeftTextureName(TEXTURE_GUN_PISTOL_HEAVY_L);
	GunPistolLight->SetRightTextureName(TEXTURE_GUN_PISTOL_HEAVY_R);

	GunPistolLight = CreateObject<CGun>(GUN_SHOTGUN_LIGHT);
	GunPistolLight->SetTexture(GUN_SHOTGUN_LIGHT, TEXT(TEXTURE_GUN_SHOTGUN_LIGHT_R));
	GunPistolLight->SetPos(200.f, 500.f);
	GunPistolLight->SetGunClass(EGunClass::Light);
	GunPistolLight->SetGunType(EGun_Type::Light_ShotGun);
	GunPistolLight->SetTextureColorKey(255.f, 255.f, 255.f);
	GunPistolLight->SetLeftTextureName(TEXTURE_GUN_SHOTGUN_LIGHT_L);
	GunPistolLight->SetRightTextureName(TEXTURE_GUN_SHOTGUN_LIGHT_R);

	GunPistolLight = CreateObject<CGun>(GUN_SHOTGUN_MEDIUM);
	GunPistolLight->SetTexture(GUN_SHOTGUN_MEDIUM, TEXT(TEXTURE_GUN_SHOTGUN_MEDIUM_R));
	GunPistolLight->SetPos(200.f, 600.f);
	GunPistolLight->SetGunClass(EGunClass::Medium);
	GunPistolLight->SetGunType(EGun_Type::Medium_ShotGun);
	GunPistolLight->SetTextureColorKey(255.f, 255.f, 255.f);
	GunPistolLight->SetLeftTextureName(TEXTURE_GUN_SHOTGUN_LIGHT_L);
	GunPistolLight->SetRightTextureName(TEXTURE_GUN_SHOTGUN_LIGHT_R);

	GunPistolLight = CreateObject<CGun>(GUN_SHOTGUN_HEAVY);
	GunPistolLight->SetTexture(GUN_SHOTGUN_HEAVY, TEXT(TEXTURE_GUN_SHOTGUN_HEAVY_R));
	GunPistolLight->SetPos(200.f, 700.f);
	GunPistolLight->SetGunClass(EGunClass::Heavy);
	GunPistolLight->SetGunType(EGun_Type::Heavy_ShotGun);
	GunPistolLight->SetTextureColorKey(255.f, 255.f, 255.f);
	GunPistolLight->SetLeftTextureName(TEXTURE_GUN_SHOTGUN_HEAVY_L);
	GunPistolLight->SetRightTextureName(TEXTURE_GUN_SHOTGUN_HEAVY_R);

}

void CHomeScene::SetBasicObjectGuns()
{
}

