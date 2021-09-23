#include "UIHome.h"
#include "../GameManager.h"
#include "../Object/StageDoor.h"

CUIHome::CUIHome()
{
}

CUIHome::~CUIHome()
{
}

bool CUIHome::Init()
{
	Resolution	RS = CGameManager::GetInst()->GetResolution();

	// CStageDoor *m_DoorToStageOne = CreateObject<CStageDoor>("DoorToStageOne");
	// m_DoorToStageOne->SetDoorStageType(EDoorStage_Type::Stage_One);
	

	return true;
}

void CUIHome::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
	
}
