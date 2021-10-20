#include "UISceneStart.h"
#include "../Scene/Camera.h"
#include "../Scene/Scene.h"

CUISceneStart::CUISceneStart()
{
	m_Stage = ESceneStage::One;
}

CUISceneStart::CUISceneStart(const CUISceneStart& widget) : CUIAnimation(widget)
{
	m_Stage = widget.m_Stage;
}

CUISceneStart::~CUISceneStart()
{
}

void CUISceneStart::SetSceneStage(ESceneStage Stage)
{
	m_Stage = Stage;
	switch (m_Stage)
	{
	case ESceneStage::One:
		SetCurrentAnimation(STAGE1_START);
		break;
	case ESceneStage::Two:
		SetCurrentAnimation(STAGE2_START);
		break;
	case ESceneStage::Boss:
		SetCurrentAnimation(STAGEBOSS_START);
		break;
	}
}

bool CUISceneStart::Init()
{
	if (!CUIAnimation::Init()) return false;

	CreateAnimation();
	AddAnimation(STAGE1_START,false,3.f);
	AddAnimation(STAGE2_START,false,3.f);
	AddAnimation(STAGEBOSS_START,false,3.f);

	SetAnimationEndNotify<CUISceneStart>(STAGE1_START, this, &CUISceneStart::AnimationDestroy);
	SetAnimationEndNotify<CUISceneStart>(STAGE2_START, this, &CUISceneStart::AnimationDestroy);
	SetAnimationEndNotify<CUISceneStart>(STAGEBOSS_START, this, &CUISceneStart::AnimationDestroy);
	
	// SetPos
	CCamera* Camera = m_Scene->GetCamera();
	Vector2 Resolution = Camera->GetResolution();
	m_Pos = Vector2(Resolution.x * 0.1f , Resolution.y * 0.3f);
	
	return true;
}

void CUISceneStart::Update(float DeltaTime)
{
	CUIAnimation::Update(DeltaTime);
}

void CUISceneStart::PostUpdate(float DeltaTime)
{
	CUIAnimation::PostUpdate(DeltaTime);
}

void CUISceneStart::Render(HDC hDC)
{
	CUIAnimation::Render(hDC);
}
