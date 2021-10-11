
#include "NumberWidget.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "UIWindow.h"

CNumberWidget::CNumberWidget()	:
	m_Number(1),
	m_RenderEnable(false)
{
}

CNumberWidget::CNumberWidget(const CNumberWidget& widget) :
	CUIImage(widget)
{
	m_Number = widget.m_Number;
	m_RenderEnable = widget.m_RenderEnable;
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
	CUIImage::Update(DeltaTime);
	int	Number = m_Number;

	std::stack<int>	NumberStack;

	// 1234 % 10 = 4    1234 / 10 = 123
	while (Number > 0)
	{
		NumberStack.push(Number % 10);
		Number /= 10;
	}
	
	m_vecNumber.clear();

	// input : 1234
	// NumberStack : 4321
	// m_vecNumber : 1234
	while (!NumberStack.empty())
	{
		m_vecNumber.push_back(NumberStack.top());
		NumberStack.pop();
	}
}

void CNumberWidget::PostUpdate(float DeltaTime)
{
	CUIImage::PostUpdate(DeltaTime);
}

void CNumberWidget::Collision(float DeltaTime)
{
	CUIImage::Collision(DeltaTime);
}

void CNumberWidget::Render(HDC hDC)
{
	if (!m_RenderEnable) return;
	if (m_vecNumber.empty())
	{
		// ��������� 0�� ����Ѵ�.
		m_FrameIndex = 0;
		CUIImage::Render(hDC);
	}
	else
	{
		// ������� �ʴٸ� ���� �ִٴ� ���̴�.
		size_t	Size = m_vecNumber.size();
		for (size_t i = 0; i < Size; ++i)
		{
			if (m_Texture->GetTextureType() == ETexture_Type::Frame)
				m_FrameIndex = m_vecNumber[i];
			else
				m_FrameIndex = 0;
			// ó���� OffSet 0���� �������� ���� Render�� �Ѿ �� �ְ� ���ֱ� 
			// ex) 1 --> 2 --> 3 : �̷������� ���������� �Ѿ�鼭 !
			m_Offset.x = i * m_Size.x;
			CUIImage::Render(hDC);
		}
	}
}

void CNumberWidget::Render(const Vector2& Pos, HDC hDC)
{
	if (!m_RenderEnable) return;
	if (m_vecNumber.empty())
	{
		// ��������� 0�� ����Ѵ�.
		m_FrameIndex = 0;
		CUIImage::Render(Pos, hDC);
	}
	else
	{
		// ������� �ʴٸ� ���� �ִٴ� ���̴�.
		size_t	Size = m_vecNumber.size();

		for (size_t i = 0; i < Size; ++i)
		{
			if (m_Texture->GetTextureType() == ETexture_Type::Frame)
				m_FrameIndex = m_vecNumber[i];
			else
				m_FrameIndex = 0;

			// ó���� OffSet 0���� �������� ���� Render�� �Ѿ �� �ְ� ���ֱ� 
			// ex) 1 --> 2 --> 3 : �̷������� ���������� �Ѿ�鼭 !
			m_Offset.x = i * m_Size.x;

			CUIImage::Render(Pos, hDC);
		}
	}
}

CNumberWidget* CNumberWidget::Clone()

{
	return new CNumberWidget(*this);
}
