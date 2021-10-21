#include "UIIntroLogo.h"
#include "../Scene/Camera.h"
#include "../Scene/Scene.h"

CUIIntroLogo::CUIIntroLogo()
{
}

CUIIntroLogo::~CUIIntroLogo()
{
}

bool CUIIntroLogo::Init()
{
	if (!CUIAnimation::Init()) return false;

	CreateAnimation();
	AddAnimation(SCENE_INTRO_LOGO_AFTR, false, 5.f);
	AddAnimation(SCENE_INTRO_LOGO_PREV, false, 5.f);
	SetCurrentAnimation(SCENE_INTRO_LOGO_PREV);

	SetAnimationEndNotify<CUIIntroLogo>(SCENE_INTRO_LOGO_PREV, this, &CUIIntroLogo::ChangeToNextAnimation);

	// SetPos
	CCamera* Camera = m_Scene->GetCamera();
	Vector2 Resolution = Camera->GetResolution();
	m_Pos = Vector2(Resolution.x * 0.1f, Resolution.y * 0.3f);

	return true;
}

void CUIIntroLogo::Update(float DeltaTime)
{
	CUIAnimation::Update(DeltaTime);
}

void CUIIntroLogo::PostUpdate(float DeltaTime)
{
	CUIAnimation::PostUpdate(DeltaTime);
}

void CUIIntroLogo::Render(HDC hDC)
{
	CUIAnimation::Render(hDC);
}

void CUIIntroLogo::ChangeToNextAnimation()
{
	ChangeAnimation(SCENE_INTRO_LOGO_AFTR);
}
