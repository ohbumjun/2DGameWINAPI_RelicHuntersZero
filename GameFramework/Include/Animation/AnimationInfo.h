#pragma once

#include "../Resource/AnimationSequence.h"

struct AnimationNotify
{
	int		Frame;
	bool	Call;
	std::function<void()>	Function;

	AnimationNotify()	:
		Frame(0),
		Call(false)
	{
	}
};

struct AnimationInfo
{
	CSharedPtr<CAnimationSequence> Sequence;
	int		Frame;	// ���� �ִϸ��̼��� �� ������°�� ���� �ִ����� �����Ѵ�.
	float	Time;	// ���� �ð�
	float	FrameTime;	// 1�������� ���ʸ��� �귯�� �ϴ���.
	float	PlayTime;	// ����� �ѹ� ���� �ʿ��� �ð�.
	float	PlayScale;	// ��� ����
	bool	Loop;
	bool	Reverse;
	std::function<void()>	EndFunction;
	std::vector<AnimationNotify*>	vecNotify;

	AnimationInfo()	:
		Frame(0),
		Time(0.f),
		FrameTime(0.f),
		PlayTime(0.f),
		PlayScale(1.f),
		Loop(false),
		Reverse(false)
	{
	}

	~AnimationInfo()
	{
		size_t	Size = vecNotify.size();

		for (size_t i = 0; i < Size; ++i)
		{
			SAFE_DELETE(vecNotify[i]);
		}
	}
};
