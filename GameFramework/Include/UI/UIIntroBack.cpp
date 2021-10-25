#include "UIIntroBack.h"
#include "../Scene/Camera.h"
#include "../Scene/Scene.h"
#include "../Input.h"

CUIIntroBack::CUIIntroBack()
{
	m_ButtonState = EButton_State::Normal;
	m_IntroStartTime = 0.f;
	m_IntroStartTimeMax = 4.f;
}

CUIIntroBack::~CUIIntroBack()
{
}

bool CUIIntroBack::Init()
{
	if (!CUIAnimation::Init()) return false;

	CreateAnimation();
	AddAnimation(SCENE_INTRO, true, 4.f);

	// SetPos
	CCamera* Camera = m_Scene->GetCamera();
	// Vector2 Resolution = Camera->GetResolution();
	// m_Pos = Vector2(Resolution.x * 0.1f, Resolution.y * 0.3f);

	return true;
}

void CUIIntroBack::Update(float DeltaTime)
{
	CUIAnimation::Update(DeltaTime);
	if (m_IntroStartTime <= m_IntroStartTimeMax)
	{
		m_IntroStartTime += DeltaTime;
	}
	if (m_IntroStartTime >= m_IntroStartTimeMax)
	{
		if (m_MouseHovered)
		{
			if (CInput::GetInst()->GetMouseDown())
				m_ButtonState = EButton_State::Click;
			else if (m_ButtonState == EButton_State::Click &&
				CInput::GetInst()->GetMouseUp())
			{
				// 버튼 기능 동작.
				if (m_ButtonClickCallback)
					m_ButtonClickCallback();
				m_ButtonState = EButton_State::MouseOn;
			}
			else if (m_ButtonState == EButton_State::Click &&
				CInput::GetInst()->GetMousePush())
				m_ButtonState = EButton_State::Click;
		}
	}
}

void CUIIntroBack::PostUpdate(float DeltaTime)
{
	CUIAnimation::PostUpdate(DeltaTime);
}

void CUIIntroBack::Render(HDC hDC)
{
	CUIAnimation::Render(hDC);
}
