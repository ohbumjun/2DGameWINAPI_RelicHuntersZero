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
	AddAnimation(SCENE_INTRO_LOGO_AFTR, true, 2.f);
	AddAnimation(SCENE_INTRO_LOGO_PREV, false, 7.f);
	SetCurrentAnimation(SCENE_INTRO_LOGO_PREV);

	SetAnimationEndNotify<CUIIntroLogo>(SCENE_INTRO_LOGO_PREV, this, &CUIIntroLogo::ChangeToNextAnimation);

	// SetPos
	CCamera* Camera = m_Scene->GetCamera();
	Vector2 Resolution = Camera->GetResolution();
	m_Pos = Vector2(Resolution.x * 0.3f, Resolution.y * 0.3f);

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
	CCamera* Camera = m_Scene->GetCamera();
	Vector2 Resolution = Camera->GetResolution();
	m_Pos = Vector2(Resolution.x * 0.175f, Resolution.y * 0.2f);
	ChangeAnimation(SCENE_INTRO_LOGO_AFTR);
}
