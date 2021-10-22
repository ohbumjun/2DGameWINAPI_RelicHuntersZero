
#include "SettingScene.h"
#include "SceneResource.h"
#include "../Object/EffectHit.h"
#include "Camera.h"
#include "../UI/UISetting.h"

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

void CSettingScene::LoadAnimationSequence()
{
}

void CSettingScene::LoadSound()
{
	GetSceneResource()->LoadSound("BGM", true, "SettingBGM", "MainBgm.mp3");
	GetSceneResource()->SoundPlay("SettingBGM");
	GetSceneResource()->SetVolume("BGM", 10);

	GetSceneResource()->LoadSound("UI", false, "ButtonMouseOn", "TeemoSmile.mp3");
	GetSceneResource()->LoadSound("UI", false, "ButtonClick", "TeemoSettingClicck.mp3");

	GetSceneResource()->SetVolume("UI", 3);
}
