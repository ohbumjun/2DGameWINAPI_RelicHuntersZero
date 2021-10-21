
#include "IntroScene.h"
#include "SceneResource.h"
#include "../Object/EffectHit.h"
#include "Camera.h"
#include "../UI/UIIntro.h"

CIntroScene::CIntroScene()
{
}

CIntroScene::~CIntroScene()
{
}

bool CIntroScene::Init()
{
	LoadSound();

	LoadAnimationSequence();

	CUIIntro* IntroWindow = CreateUIWindow<CUIIntro>("IntroWindow");

	return true;
}

void CIntroScene::LoadAnimationSequence()
{
	SetSceneIntroAnimation();
}

void CIntroScene::LoadSound()
{
	GetSceneResource()->LoadSound("BGM", true, "IntroBGM", "MainBgm.mp3");
	GetSceneResource()->SoundPlay("IntroBGM");
	GetSceneResource()->SetVolume("BGM", 10);

	GetSceneResource()->LoadSound("UI", false, "ButtonMouseOn", "TeemoSmile.mp3");
	GetSceneResource()->LoadSound("UI", false, "ButtonClick", "TeemoIntroClicck.mp3");

	GetSceneResource()->SetVolume("UI", 3);
}
