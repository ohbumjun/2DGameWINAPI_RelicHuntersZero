
#include "StartScene.h"
#include "SceneResource.h"
#include "../Object/EffectHit.h"
#include "Camera.h"
#include "../UI/UIStart.h"

CStartScene::CStartScene()
{
}

CStartScene::~CStartScene()
{
}

bool CStartScene::Init()
{
	LoadSound();

	LoadAnimationSequence();

	CUIStart* StartWindow = CreateUIWindow<CUIStart>("StartWindow");

	return true;
}

void CStartScene::LoadAnimationSequence()
{
}

void CStartScene::LoadSound()
{
	GetSceneResource()->LoadSound("BGM", true, "StartBGM", "MainBgm.mp3");
	GetSceneResource()->SoundPlay("StartBGM");

	GetSceneResource()->LoadSound("UI", false, "ButtonMouseOn", "TeemoSmile.mp3");
	GetSceneResource()->LoadSound("UI", false, "ButtonClick", "TeemoStartClicck.mp3");

	GetSceneResource()->SetVolume("UI", 30);
}
