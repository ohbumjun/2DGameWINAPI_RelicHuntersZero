
#include "NumberWidget.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "UIWindow.h"

CNumberWidget::CNumberWidget()	:
	m_Number(0)
{
}

CNumberWidget::CNumberWidget(const CNumberWidget& widget) :
	CUIImage(widget)
{
	m_Number = widget.m_Number;
}

CNumberWidget::~CNumberWidget()
{
}

bool CNumberWidget::Init()
{
	return true;
}

void CNumberWidget::Update(float DeltaTime)
{
	int	Number = m_Number;

	std::stack<int>	NumberStack;

	// 1234 % 10 = 4    1234 / 10 = 123
	while (Number > 0)
	{
		NumberStack.push(Number % 10);
		Number /= 10;
	}

	m_vecNumber.clear();

	while (!NumberStack.empty())
	{
		m_vecNumber.push_back(NumberStack.top());
		NumberStack.pop();
	}
}

void CNumberWidget::PostUpdate(float DeltaTime)
{
}

void CNumberWidget::Collision(float DeltaTime)
{
}

void CNumberWidget::Render(HDC hDC)
{
	if (m_vecNumber.empty())
	{
		// 비어있으면 0을 출력한다.
		m_FrameIndex = 0;
		CUIImage::Render(hDC);
	}

	else
	{
		// 비어있지 않다면 값이 있다는 것이다.
		size_t	Size = m_vecNumber.size();

		for (size_t i = 0; i < Size; ++i)
		{
		}
	}
}

void CNumberWidget::Render(const Vector2& Pos, HDC hDC)
{
	CUIImage::Render(Pos, hDC);
}

CNumberWidget* CNumberWidget::Clone()
{
	return new CNumberWidget(*this);
}
