
#include "StartScene.h"
#include "SceneResource.h"
#include "../Object/EffectHit.h"
#include "Camera.h"
#include "../UI/UIStart.h"
#include "../GameManager.h"

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
	float MasterVol = CGameManager::GetInst()->GetMasterVolume();
	int BGVol = CGameManager::GetInst()->GetBGVolume();
	int EffectVol = CGameManager::GetInst()->GetEffectVolume();

	GetSceneResource()->LoadSound("BGM", true, "StartBGM", "MainBgm.mp3");
	GetSceneResource()->SoundPlay("StartBGM");
	GetSceneResource()->SetVolume("BGM", (int)(MasterVol * BGVol));

	GetSceneResource()->LoadSound("UI", false, "ButtonMouseOn", "TeemoSmile.mp3");
	GetSceneResource()->LoadSound("UI", false, "ButtonClick", "TeemoStartClicck.mp3");
	GetSceneResource()->SetVolume("UI", (int)(MasterVol * EffectVol));
}
