#pragma once
#include "UIWidget.h"

class CUIAnimation :
    public CUIWidget
{
public:
	CUIAnimation();
	CUIAnimation(const CUIAnimation& widget);
	virtual ~CUIAnimation();

// Animation
protected:
	class CAnimation* m_Animation;
	Vector2		m_Pos;
	Vector2		m_Pivot;
	Vector2		m_Offset;
public:
	void CreateAnimation();
	void AddAnimation(const std::string& SequenceName,
		bool Loop = true, float PlayTime = 1.f, float PlayScale = 1.f,
		bool Reverse = false);
	void SetAnimationPlayTime(const std::string& Name, float PlayTime);
	void SetAnimationPlayScale(const std::string& Name, float PlayScale);
	void SetCurrentAnimation(const std::string& Name);
	void ChangeAnimation(const std::string& Name);
	bool CheckCurrentAnimation(const std::string& Name);
	void SetAnimationReverse(const std::string& Name, bool Reverse);
	void SetAnimationLoop(const std::string& Name, bool Loop);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CUIAnimation* Clone();
};

