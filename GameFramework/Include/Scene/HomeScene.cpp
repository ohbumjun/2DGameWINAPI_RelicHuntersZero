#include "HomeScene.h"
#include "SceneResource.h"
#include "../Object/Player.h"
#include "../Scene/Camera.h"
#include "../Object/StageDoor.h"

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
	// Objects
	/*
	CPlayer* Player = CreateObject<CPlayer>("Player");
	Player->SetCharacterInfo(60, 10, NORMAL_MONSTER_HP_MAX, NORMAL_MONSTER_MP_MAX,
		1, 100, 100, 100, NORMAL_ATTACK_DISTANCE);

	SetPlayer(Player);
	GetCamera()->SetTarget(Player);
	GetCamera()->SetTargetPivot(0.5f, 0.5f);

	*/
	Vector2 WorldResolution = m_Camera->GetWorldResolution();
	CStageDoor* StageDoor_One = CreateObject<CStageDoor>("StageDoor", 
		Vector2(300.f + rand() % 700, rand() % 100),
		Vector2(50.f,50.f));
	StageDoor_One->SetDoorStageType(EDoorStage_Type::Stage_One);

	return true;
}

void CHomeScene::LoadAnimationSequence()
{
	GetSceneResource()->CreateAnimationSequence("StageDoor",
		"StageDoor", TEXT("DoorToNextStage.bmp"));
	GetSceneResource()->SetTextureColorKey("StageDoor",
		255, 0, 255);

	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 5; j++)
		{
			GetSceneResource()->AddAnimationFrameData("StageDoor",
				j * 192.f, i *192.f, 192.f, 192.f);
		}
	}
}

void CHomeScene::LoadSound()
{
	GetSceneResource()->LoadSound("BGM", true, "StartBGM", "MainBgm.mp3");
	GetSceneResource()->SoundPlay("StartBGM");
	GetSceneResource()->SetVolume("StartBGM", 1);
}

// DoorToNextStage

