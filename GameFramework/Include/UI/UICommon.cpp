#include "UICommon.h"
#include "../GameManager.h"
#include "../Object/StageDoor.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"

CUICommon::CUICommon(): StartSceneWidget(nullptr)
{
}

CUICommon::~CUICommon()
{
}

void CUICommon::SetSceneStage(ESceneStage Stage)
{
	StartSceneWidget->SetSceneStage(Stage);
}

bool CUICommon::Init()
{

	StartSceneWidget = CreateWidget<CUISceneStart>("StartSceneUI");
	return true;
}
