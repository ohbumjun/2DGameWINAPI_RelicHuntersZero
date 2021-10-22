
#include "SettingScene.h"
#include "SceneResource.h"
#include "../Object/EffectHit.h"
#include "Camera.h"
#include "../UI/UISetting.h"
#include "../GameManager.h"

CSettingScene::CSettingScene()
{
}

CSettingScene::~CSettingScene()
{
}

bool CSettingScene::Init()
{
	LoadSound();

	LoadAnimationSequence();

	CUISetting* SettingWindow = CreateUIWindow<CUISetting>("SettingWindow");

	return true;
}

bool CSettingScene::Update(float DeltaTime)
{
	CScene::Update(DeltaTime);

	float MasterVol = CGameManager::GetInst()->GetMasterVolume();
	int BGVol = CGameManager::GetInst()->GetBGVolume();
	int EffectVol = CGameManager::GetInst()->GetEffectVolume();
	GetSceneResource()->SetVolume("BGM", (int)(MasterVol * BGVol));
	GetSceneResource()->SetVolume("UI", (int)(MasterVol * EffectVol));

	return true;
}

void CSettingScene::LoadAnimationSequence()
{
}

void CSettingScene::LoadSound()
{
	float MasterVol = CGameManager::GetInst()->GetMasterVolume();
	int BGVol = CGameManager::GetInst()->GetBGVolume();
	int EffectVol = CGameManager::GetInst()->GetEffectVolume();

	GetSceneResource()->LoadSound("BGM", true, "SettingBGM", "MainBgm.mp3");
	GetSceneResource()->SoundPlay("SettingBGM");
	GetSceneResource()->SetVolume("BGM", (int)(MasterVol * BGVol));

	GetSceneResource()->LoadSound("UI", false, "ButtonMouseOn", "TeemoSmile.mp3");
	GetSceneResource()->LoadSound("UI", false, "ButtonClick", "TeemoSettingClicck.mp3");
	GetSceneResource()->SetVolume("UI", (int)(MasterVol * EffectVol));
}
