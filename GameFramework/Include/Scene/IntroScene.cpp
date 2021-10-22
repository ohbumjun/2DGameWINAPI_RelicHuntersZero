
#include "IntroScene.h"
#include "SceneResource.h"
#include "../Object/EffectHit.h"
#include "Camera.h"
#include "../UI/UIIntro.h"
#include "../GameManager.h"

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
	float MasterVol = CGameManager::GetInst()->GetMasterVolume();
	int BGVol = CGameManager::GetInst()->GetBGVolume();
	int EffectVol = CGameManager::GetInst()->GetEffectVolume();

	GetSceneResource()->LoadSound("BGM", true, "IntroBGM", "MainBgm.mp3");
	GetSceneResource()->SoundPlay("IntroBGM");
	GetSceneResource()->SetVolume("BGM", (int)(MasterVol * BGVol));

	GetSceneResource()->LoadSound("UI", false, "ButtonMouseOn", "TeemoSmile.mp3");
	GetSceneResource()->LoadSound("UI", false, "ButtonClick", "TeemoIntroClicck.mp3");
	GetSceneResource()->SetVolume("UI", (int)(MasterVol * EffectVol));
}
