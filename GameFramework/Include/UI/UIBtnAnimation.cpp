#include "UIBtnAnimation.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Input.h"


CUIBtnAnimation::CUIBtnAnimation() :
	m_ClickAnimEnable(false),
	m_HoverAnimEnable(false),
	m_CardSelected(false)
{
	m_ButtonState = EButton_State::Normal;
}

CUIBtnAnimation::~CUIBtnAnimation()
{
}

void CUIBtnAnimation::SetButtonState(EButton_State State)
{
	m_ButtonState = State;
}

void CUIBtnAnimation::SetCardSelected(bool State)
{
	m_CardSelected = State;
	m_ButtonState  = EButton_State::Normal;
}

bool CUIBtnAnimation::Init()
{
    if (!CUIAnimation::Init()) return false;
	CreateAnimation();

    return true;
}

void CUIBtnAnimation::Update(float DeltaTime)
{
    CUIAnimation::Update(DeltaTime);
	// Mouse Recognition
	if (m_ButtonState != EButton_State::Disable)
	{
		if (m_MouseHovered )
		{
			if (CInput::GetInst()->GetMouseDown())
				m_ButtonState = EButton_State::Click;
			else if (m_ButtonState == EButton_State::Click && 
				CInput::GetInst()->GetMouseUp())
			{
				// 버튼 기능 동작.
				m_CardSelected = true;
				if(m_ClickAnimEnable)
					ChangeAnimation(m_ClickAnimation);
				if (m_ButtonSound[1])
					m_ButtonSound[1]->Play();
			}

			else if (m_ButtonState == EButton_State::Click &&
				CInput::GetInst()->GetMousePush())
				m_ButtonState = EButton_State::Click;

			else if(!m_CardSelected)
			{
				m_ButtonState = EButton_State::MouseOn;
				if (m_HoverAnimEnable)
					ChangeAnimation(m_HoverAnimation);
			}
		}

		else if(!m_CardSelected)
		{
			m_ButtonState = EButton_State::Normal;
			ChangeAnimation(m_DefaultAnimation);
			m_CardSelected = false;
		}
	}
}

void CUIBtnAnimation::PostUpdate(float DeltaTime)
{
    CUIAnimation::PostUpdate(DeltaTime);
}

void CUIBtnAnimation::Collision(float DeltaTime)
{
    CUIAnimation::Collision(DeltaTime);
}

void CUIBtnAnimation::Render(HDC hDC)
{
    CUIAnimation::Render(hDC);
}

void CUIBtnAnimation::SetMouseOnSound(const std::string& Name)
{
    m_ButtonSound[0] = m_Scene->GetSceneResource()->FindSound(Name);
}

void CUIBtnAnimation::SetClickSound(const std::string& Name)
{
    m_ButtonSound[1] = m_Scene->GetSceneResource()->FindSound(Name);
}

void CUIBtnAnimation::CollisionMouseHoveredCallback(float DeltaTime)
{
	CUIAnimation::CollisionMouseHoveredCallback(DeltaTime);
}

void CUIBtnAnimation::CollisionMouseReleaseCallback(float DeltaTime)
{
	CUIAnimation::CollisionMouseReleaseCallback(DeltaTime);
}

void CUIBtnAnimation::SetDefaultAnimation(const std::string& Name)
{
	m_DefaultAnimation = Name;
	AddAnimation(Name,false);
}

void CUIBtnAnimation::SetClickAnimation(const std::string& Name)
{
	m_ClickAnimation = Name;
	m_ClickAnimEnable = true;
	AddAnimation(Name, false, 0.5f);
}

void CUIBtnAnimation::SetMouseOnAnimation(const std::string& Name)
{
	m_HoverAnimation = Name;
	m_HoverAnimEnable = true;
	AddAnimation(Name, false, 0.5f);
}
