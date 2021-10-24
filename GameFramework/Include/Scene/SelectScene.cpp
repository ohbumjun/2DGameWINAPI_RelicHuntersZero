
#include "SelectScene.h"
#include "SceneResource.h"
#include "../Object/EffectHit.h"
#include "Camera.h"
#include "../UI/UISelect.h"
#include "../GameManager.h"

CSelectScene::CSelectScene()
{
}

CSelectScene::~CSelectScene()
{
}

bool CSelectScene::Init()
{
	LoadSound();

	LoadAnimationSequence();

	CUISelect* SelectWindow = CreateUIWindow<CUISelect>("MenuWindow");

	return true;
}

void CSelectScene::LoadAnimationSequence()
{
	SetCharacterCardUIs();
}

void CSelectScene::LoadSound()
{
	float MasterVol = CGameManager::GetInst()->GetMasterVolume();
	int BGVol = CGameManager::GetInst()->GetBGVolume();
	int EffectVol = CGameManager::GetInst()->GetEffectVolume();

	GetSceneResource()->LoadSound("BGM", true, "MenuBGM", "MainBgm.mp3");
	GetSceneResource()->SoundPlay("MenuBGM");
	GetSceneResource()->SetVolume("BGM", (int)(MasterVol * BGVol));

	GetSceneResource()->LoadSound("UI", false, "ButtonMouseOn", "TeemoSmile.mp3");
	GetSceneResource()->LoadSound("UI", false, "ButtonClick", "TeemoMenuClicck.mp3");
	GetSceneResource()->SetVolume("UI", (int)(MasterVol * EffectVol));
}
