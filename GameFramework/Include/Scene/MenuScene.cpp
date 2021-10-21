
#include "MenuScene.h"
#include "SceneResource.h"
#include "../Object/EffectHit.h"
#include "Camera.h"
#include "../UI/UIStart.h"

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

	CUIStart* MenuWindow = CreateUIWindow<CUIStart>("MenuWindow");

	return true;
}

void CMenuScene::LoadAnimationSequence()
{
}

void CMenuScene::LoadSound()
{
	GetSceneResource()->LoadSound("BGM", true, "MenuBGM", "MainBgm.mp3");
	GetSceneResource()->SoundPlay("MenuBGM");
	GetSceneResource()->SetVolume("BGM", 10);

	GetSceneResource()->LoadSound("UI", false, "ButtonMouseOn", "TeemoSmile.mp3");
	GetSceneResource()->LoadSound("UI", false, "ButtonClick", "TeemoMenuClicck.mp3");

	GetSceneResource()->SetVolume("UI", 3);
}
