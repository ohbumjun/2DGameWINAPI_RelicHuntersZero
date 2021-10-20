#include "UISceneStart.h"

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
	AddAnimation(STAGE1_START,false,2.f);
	AddAnimation(STAGE2_START,false,2.f);
	AddAnimation(STAGEBOSS_START,false,2.f);

	SetAnimationEndNotify<CUISceneStart>(STAGE1_START, this, &CUISceneStart::AnimationDestroy);
	SetAnimationEndNotify<CUISceneStart>(STAGE2_START, this, &CUISceneStart::AnimationDestroy);
	SetAnimationEndNotify<CUISceneStart>(STAGEBOSS_START, this, &CUISceneStart::AnimationDestroy);
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
