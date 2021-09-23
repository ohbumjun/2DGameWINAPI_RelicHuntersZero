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
#include "../Object/EffectHit.h"
#include "../Object/TeleportMouse.h"
// UI
#include "Camera.h"
#include "../UI/UIMain.h"
#include "../UI/UICharacterStateHUD.h"

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
	GetCamera()->SetWorldResolution(2000.f, 2000.f);

	// Monster Bullet 세팅 
	CBullet *MonsterBullet = CreatePrototype<CBullet>("MonsterBullet");
	CCollider* Collider = MonsterBullet->FindCollider("Body");
	if (Collider)
		Collider->SetCollisionProfile("MonsterAttack");

	// Player 세팅 
	CPlayer *Player = CreateObject<CPlayer>("Player");
	Player->SetCharacterInfo(60, 10, NORMAL_MONSTER_HP_MAX, NORMAL_MONSTER_MP_MAX, 
		1, 100, 100, 100, NORMAL_ATTACK_DISTANCE);
	SetPlayer(Player);
	GetCamera()->SetTarget(Player);
	GetCamera()->SetTargetPivot(0.5f, 0.5f);

	// 몬스터 본체 
	Vector2 WorldResolution = m_Camera->GetWorldResolution();
	CMonster* Monster = CreateObject<CMonster>("Monster", Vector2(300.f + rand() % 700, rand() % 100));
	Monster->SetCharacterInfo(NORMAL_MONSTER_ATTACK,NORMAL_MONSTER_ARMOR, NORMAL_MONSTER_HP_MAX, 
		NORMAL_MONSTER_MP_MAX, 1, 100, 100, 100, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
	Monster->SetMoveSpeed(NORMAL_MONSTER_MOVE_SPEED);
	Monster->SetPlayer(Player);

	CMonster* Monster2 = CreateObject<CMonster>("Monster", Vector2(300.f + rand() % 700, rand() % 100));
	Monster2->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, NORMAL_MONSTER_HP_MAX,
		NORMAL_MONSTER_MP_MAX, 1, 100, 100, 100, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
	Monster2->SetMoveSpeed(NORMAL_MONSTER_MOVE_SPEED);
	Monster2->SetPlayer(Player);
	
	// 윈도우
	CUIMain *MainWindow = CreateUIWindow<CUIMain>("MainWindow");
	CUICharacterStateHUD *StateWindow = CreateUIWindow<CUICharacterStateHUD>("CharacterStateHUD");

	return true;
}

void CMainScene::LoadAnimationSequence()
{
	

}

void CMainScene::LoadSound()
{
	GetSceneResource()->LoadSound("BGM", true, "MainBGM", "MainBgm.mp3");
	GetSceneResource()->SoundPlay("MainBGM");
	GetSceneResource()->SetVolume("MainBGM", 1);
}

void CMainScene::GoBackToWaitingScene()
{
	CSceneManager::GetInst()->CreateScene<CHomeScene>();
}
