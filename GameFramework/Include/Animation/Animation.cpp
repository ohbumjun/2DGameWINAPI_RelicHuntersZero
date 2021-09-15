
#include "Animation.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CAnimation::CAnimation()	:
	m_CurrentAnimation(nullptr)
{
}

CAnimation::CAnimation(const CAnimation& Anim)
{
	// Anim은 const 객체이다. Anim이 가지고 있는 stl을 사용하려 할때는
	// iterator 들을 const_iterator 를 사용해야 한다.
	std::unordered_map<std::string, AnimationInfo*>::const_iterator	iter = Anim.m_mapAnimation.begin();
	std::unordered_map<std::string, AnimationInfo*>::const_iterator	iterEnd = Anim.m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		AnimationInfo* Info = new AnimationInfo;

		Info->Sequence = iter->second->Sequence;
		Info->FrameTime = iter->second->FrameTime;
		Info->PlayTime = iter->second->PlayTime;
		Info->PlayScale = iter->second->PlayScale;
		Info->Loop = iter->second->Loop;
		Info->Reverse = iter->second->Reverse;

		if (Anim.m_CurrentAnimation->Sequence->GetName() == Info->Sequence->GetName())
			m_CurrentAnimation = Info;

		m_mapAnimation.insert(std::make_pair(iter->first, Info));
	}
}

CAnimation::~CAnimation()
{
	auto	iter = m_mapAnimation.begin();
	auto	iterEnd = m_mapAnimation.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

void CAnimation::AddAnimation(const std::string& SequenceName, 
	bool Loop, float PlayTime, float PlayScale, bool Reverse)
{
	CAnimationSequence* Sequence = m_Scene->GetSceneResource()->FindAnimationSequence(SequenceName);

	if (!Sequence)
		return;

	AnimationInfo* Info = new AnimationInfo;

	Info->Sequence = Sequence;
	Info->Loop = Loop;
	Info->PlayTime = PlayTime;
	Info->PlayScale = PlayScale;
	Info->Reverse = Reverse;
	Info->FrameTime = PlayTime / Sequence->GetFrameCount();

	// 처음 추가되는 애니메이션일 경우 이 애니메이션을 기본 애니메이션으로
	// 설정한다.
	if (m_mapAnimation.empty())
		m_CurrentAnimation = Info;

	m_mapAnimation.insert(std::make_pair(SequenceName, Info));
}

void CAnimation::SetPlayTime(const std::string& Name, float PlayTime)
{
	AnimationInfo* Info = FindInfo(Name);

	if (!Info)
		return;

	Info->PlayTime = PlayTime;
}

void CAnimation::SetPlayScale(const std::string& Name, float PlayScale)
{
	AnimationInfo* Info = FindInfo(Name);

	if (!Info)
		return;

	Info->PlayScale = PlayScale;
}

void CAnimation::SetCurrentAnimation(const std::string& Name)
{
	m_CurrentAnimation = FindInfo(Name);

	m_CurrentAnimation->Frame = 0;
	m_CurrentAnimation->Time = 0.f;

	size_t	Size = m_CurrentAnimation->vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_CurrentAnimation->vecNotify[i]->Call = false;
	}
}

void CAnimation::ChangeAnimation(const std::string& Name)
{
	if (m_CurrentAnimation->Sequence->GetName() == Name)
		return;

	m_CurrentAnimation->Frame = 0;
	m_CurrentAnimation->Time = 0.f;

	size_t	Size = m_CurrentAnimation->vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_CurrentAnimation->vecNotify[i]->Call = false;
	}

	m_CurrentAnimation = FindInfo(Name);
	m_CurrentAnimation->Frame = 0;
	m_CurrentAnimation->Time = 0.f;
}

bool CAnimation::CheckCurrentAnimation(const std::string& Name)
{
	return m_CurrentAnimation->Sequence->GetName() == Name;
}

void CAnimation::SetReverse(const std::string& Name, bool Reverse)
{
	AnimationInfo* Info = FindInfo(Name);

	if (!Info)
		return;

	Info->Reverse = Reverse;
}

void CAnimation::SetLoop(const std::string& Name, bool Loop)
{
	AnimationInfo* Info = FindInfo(Name);

	if (!Info)
		return;

	Info->Loop = Loop;
}

void CAnimation::Update(float DeltaTime)
{
	// 애니메이션 시간을 증가시켜준다.
	m_CurrentAnimation->Time += DeltaTime * m_CurrentAnimation->PlayScale;

	bool	AnimEnd = false;

	// 증가된 시간이 1프레임당 흘러야 할 시간을 지났다면 프레임을 증가시켜줘야 한다.
	if (m_CurrentAnimation->Time >= m_CurrentAnimation->FrameTime)
	{
		// 1프레임당 흘러야 할 시간을 빼준다.
		m_CurrentAnimation->Time -= m_CurrentAnimation->FrameTime;

		if (m_CurrentAnimation->Reverse)
		{
			--m_CurrentAnimation->Frame;

			if (m_CurrentAnimation->Frame < 0)
				AnimEnd = true;
		}

		else
		{
			++m_CurrentAnimation->Frame;

			if (m_CurrentAnimation->Frame == m_CurrentAnimation->Sequence->GetFrameCount())
				AnimEnd = true;
		}
	}

	size_t	Size = m_CurrentAnimation->vecNotify.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (!m_CurrentAnimation->vecNotify[i]->Call &&
			m_CurrentAnimation->vecNotify[i]->Frame == m_CurrentAnimation->Frame)
		{
			m_CurrentAnimation->vecNotify[i]->Call = true;

			m_CurrentAnimation->vecNotify[i]->Function();
		}
	}

	if (AnimEnd)
	{
		if (m_CurrentAnimation->Loop)
		{
			if (m_CurrentAnimation->Reverse)
				m_CurrentAnimation->Frame = m_CurrentAnimation->Sequence->GetFrameCount() - 1;

			else
				m_CurrentAnimation->Frame = 0;
		}

		else
		{
			if (m_CurrentAnimation->Reverse)
				m_CurrentAnimation->Frame = 0;

			else
				m_CurrentAnimation->Frame = m_CurrentAnimation->Sequence->GetFrameCount() - 1;
		}

		// 모션이 끝났다면 EndFunction이 있을 경우 호출한다.
		if (m_CurrentAnimation->EndFunction)
			m_CurrentAnimation->EndFunction();

		if (m_CurrentAnimation->Loop)
		{
			size_t	Size = m_CurrentAnimation->vecNotify.size();

			for (size_t i = 0; i < Size; ++i)
			{
				m_CurrentAnimation->vecNotify[i]->Call = false;
			}
		}
	}
}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
}

AnimationInfo* CAnimation::FindInfo(const std::string& Name)
{
	auto	iter = m_mapAnimation.find(Name);

	if (iter == m_mapAnimation.end())
		return nullptr;

	return iter->second;
}
