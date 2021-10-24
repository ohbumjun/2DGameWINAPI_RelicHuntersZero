#pragma once
#include "UIAnimation.h"
#include "../Resource/Sound.h"
class CUIBtnAnimation :
    public CUIAnimation
{
public :
	CUIBtnAnimation();
	virtual ~CUIBtnAnimation();
private :
	EChar_Type m_CharType;
public :
	void SetCharType(EChar_Type CharType)
	{
		m_CharType = CharType;
	}
private :
    EButton_State			m_ButtonState;
    std::function<void()>	m_ButtonClickCallback;
	std::function<void()>	m_ButtonMouseOnCallback;
    CSharedPtr<CSound>		m_ButtonSound[2];
public :
	void SetButtonState(EButton_State State);
private:
	bool m_ClickAnimEnable;
	bool m_HoverAnimEnable;
	std::string m_ClickAnimation;
	std::string m_HoverAnimation;
	std::string m_DefaultAnimation;
private :
	bool m_CardSelected;
public :
	void SetCardSelected(bool State);
	bool GetCardSelected() const { return m_CardSelected; }
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
public:
	void SetMouseOnSound(const std::string& Name);
	void SetClickSound(const std::string& Name);
protected:
	virtual void CollisionMouseHoveredCallback(float DeltaTime);
	virtual void CollisionMouseReleaseCallback(float DeltaTime);
public:
	EButton_State GetButtonState() const
	{
		return m_ButtonState;
	}
public :
	void SetDefaultAnimation(const std::string& Name);
	void SetClickAnimation(const std::string& Name);
	void SetMouseOnAnimation(const std::string& Name);
};

