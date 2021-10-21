#include "UIIntroBack.h"
#include "../Scene/Camera.h"
#include "../Scene/Scene.h"

CUIIntroBack::CUIIntroBack()
{
}

CUIIntroBack::~CUIIntroBack()
{
}

bool CUIIntroBack::Init()
{
	if (!CUIAnimation::Init()) return false;

	CreateAnimation();
	AddAnimation(SCENE_INTRO, true, 4.f);

	// SetPos
	CCamera* Camera = m_Scene->GetCamera();
	// Vector2 Resolution = Camera->GetResolution();
	// m_Pos = Vector2(Resolution.x * 0.1f, Resolution.y * 0.3f);

	return true;
}

void CUIIntroBack::Update(float DeltaTime)
{
	CUIAnimation::Update(DeltaTime);
}

void CUIIntroBack::PostUpdate(float DeltaTime)
{
	CUIAnimation::PostUpdate(DeltaTime);
}

void CUIIntroBack::Render(HDC hDC)
{
	CUIAnimation::Render(hDC);
}
