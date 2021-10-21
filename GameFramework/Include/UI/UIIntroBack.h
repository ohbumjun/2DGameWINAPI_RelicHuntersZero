#pragma once
#include "UIWidget.h"
#include "UIAnimation.h"
class CUIIntroBack :
    public CUIAnimation
{
public:
	CUIIntroBack();
	virtual ~CUIIntroBack();
private :
	EButton_State			m_ButtonState;
	std::function<void()>	m_ButtonClickCallback;
	float m_IntroStartTime;
	float m_IntroStartTimeMax;
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC);
public:
	template <typename T>
	void SetClickCallback(T* Obj, void(T::* Func)())
	{
		m_ButtonClickCallback = std::bind(Func, Obj);
	}
};

