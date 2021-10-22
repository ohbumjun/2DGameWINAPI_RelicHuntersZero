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

    return true;
}

void CUIHome::Update(float DeltaTime)
{
    CUIWindow::Update(DeltaTime);
}
