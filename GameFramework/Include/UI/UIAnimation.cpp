#include "UIAnimation.h"
#include "../Animation/Animation.h"

CUIAnimation::CUIAnimation() :
	m_Animation(nullptr), 
	m_Pivot(Vector2(0.f,0.f)),
	m_Offset(Vector2(0.f,0.f))
{
	
}

CUIAnimation::CUIAnimation(const CUIAnimation& widget)
{
	m_Pos = widget.m_Pos;
	m_Pivot = widget.m_Pivot;
	m_Offset = widget.m_Offset;
}

CUIAnimation::~CUIAnimation()
{
	SAFE_DELETE(m_Animation);
}

void CUIAnimation::CreateAnimation()
{
	if (!m_Animation)
	{
		m_Animation = new CAnimation;

		m_Animation->m_WidgetOwner = this;
		m_Animation->m_Scene = m_Scene;
		m_Animation->m_CurrentAnimation = nullptr;
	}
}

void CUIAnimation::AddAnimation(const std::string& SequenceName, bool Loop, float PlayTime, float PlayScale, bool Reverse)
{
	if (!m_Animation)
	{
		m_Animation = new CAnimation;

		m_Animation->m_WidgetOwner = this;
		m_Animation->m_Scene = m_Scene;
	}

	m_Animation->AddAnimation(SequenceName, Loop, PlayTime, PlayScale, Reverse);
}

void CUIAnimation::SetAnimationPlayTime(const std::string& Name, float PlayTime)
{
	if (!m_Animation)
		return;

	m_Animation->SetPlayTime(Name, PlayTime);
}

void CUIAnimation::SetAnimationPlayScale(const std::string& Name, float PlayScale)
{
	if (!m_Animation)
		return;

	m_Animation->SetPlayScale(Name, PlayScale);
}

void CUIAnimation::SetCurrentAnimation(const std::string& Name)
{
	if (!m_Animation)
		return;

	m_Animation->SetCurrentAnimation(Name);
}

void CUIAnimation::ChangeAnimation(const std::string& Name)
{
	if (!m_Animation)
		return;

	m_Animation->ChangeAnimation(Name);
}

bool CUIAnimation::CheckCurrentAnimation(const std::string& Name)
{
	return m_Animation->CheckCurrentAnimation(Name);
}

void CUIAnimation::SetAnimationReverse(const std::string& Name, bool Reverse)
{
	if (!m_Animation)
		return;

	m_Animation->SetReverse(Name, Reverse);
}

void CUIAnimation::SetAnimationLoop(const std::string& Name, bool Loop)
{
	if (!m_Animation)
		return;

	m_Animation->SetLoop(Name, Loop);
}

bool CUIAnimation::Init()
{
    return true;
}

void CUIAnimation::Update(float DeltaTime)
{
	if (m_Animation)
		m_Animation->Update(DeltaTime);
}

void CUIAnimation::PostUpdate(float DeltaTime)
{
	if (m_Animation)
	{
		if (m_Animation->m_CurrentAnimation)
		{
			AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;
			const AnimationFrameData& FrameData = AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);
			m_Size = FrameData.Size;
		}
	}
}

void CUIAnimation::Render(HDC hDC)
{
	if (m_Animation && m_Animation->m_CurrentAnimation)
	{
		AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;
		const AnimationFrameData& FrameData = AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);
		Vector2	LT = m_Pos - m_Pivot * FrameData.Size + m_Offset;

		if (AnimInfo->Sequence->GetTextureType() == ETexture_Type::Atlas)
		{
			AnimInfo->Sequence->GetTexture()->Render(hDC, LT,
				FrameData.StartPos, FrameData.Size);
		}
		else
		{
			AnimInfo->Sequence->GetTexture()->Render(hDC, LT,
				FrameData.StartPos, FrameData.Size, AnimInfo->Frame);
		}
	}
}

CUIAnimation* CUIAnimation::Clone()
{
	return new CUIAnimation(*this);
}

void CUIAnimation::AnimationDestroy()
{
	Destroy();
}
