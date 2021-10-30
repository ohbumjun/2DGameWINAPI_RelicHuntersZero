
#include "EndScene.h"
#include "SceneResource.h"
#include "../Object/EffectHit.h"
#include "Camera.h"
#include "../UI/UIEnd.h"
#include "../GameManager.h"

CEndScene::CEndScene()
{
}

CEndScene::~CEndScene()
{
}

bool CEndScene::Init()
{
	LoadSound();

	LoadAnimationSequence();

	CUIEnd* EndWindow = CreateUIWindow<CUIEnd>("EndWindow");

	return true;
}

void CEndScene::LoadAnimationSequence()
{
	SetSceneIntroAnimation();
}

void CEndScene::LoadSound()
{
	float MasterVol = CGameManager::GetInst()->GetMasterVolume();
	int BGVol = CGameManager::GetInst()->GetBGVolume();
	int EffectVol = CGameManager::GetInst()->GetEffectVolume();

	GetSceneResource()->LoadSound("BGM", true, "IntroBGM", "MainBgm.mp3");
	GetSceneResource()->SoundPlay("IntroBGM");
	GetSceneResource()->SetVolume("BGM", (int)(MasterVol * BGVol));

}
