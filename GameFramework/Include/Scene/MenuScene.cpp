
#include "MenuScene.h"
#include "SceneResource.h"
#include "../Object/EffectHit.h"
#include "Camera.h"
#include "../UI/UIMenu.h"
#include "../GameManager.h"

CMenuScene::CMenuScene()
{
}

CMenuScene::~CMenuScene()
{
}

bool CMenuScene::Init()
{
	LoadSound();

	LoadAnimationSequence();

	CUIMenu* MenuWindow = CreateUIWindow<CUIMenu>("MenuWindow");

	return true;
}

void CMenuScene::LoadAnimationSequence()
{
}

void CMenuScene::LoadSound()
{
	float MasterVol = CGameManager::GetInst()->GetMasterVolume();
	int BGVol = CGameManager::GetInst()->GetBGVolume();
	int EffectVol = CGameManager::GetInst()->GetEffectVolume();

	GetSceneResource()->LoadSound("BGM", true, "MenuBGM", "Menu1Bgm.wav");
	GetSceneResource()->SoundPlay("MenuBGM");
	GetSceneResource()->SetVolume("BGM", (int)(MasterVol * BGVol));

	GetSceneResource()->LoadSound("UI", false, "ButtonMouseOn", "UI_Hover.wav");
	// GetSceneResource()->LoadSound("UI", false, "ButtonClick", "TeemoMenuClicck.mp3");
	GetSceneResource()->SetVolume("UI", (int)(MasterVol * EffectVol));
}
