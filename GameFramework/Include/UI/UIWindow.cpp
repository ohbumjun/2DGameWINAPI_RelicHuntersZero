
#include "UIWindow.h"
#include "UIWidget.h"

CUIWindow::CUIWindow()	:
	m_Visibility(true),
	m_WidgetArray(nullptr),
	m_Scene(nullptr),
	m_ZOrder(0),
	m_WidgetCount(0),
	m_WidgetCapacity(4)
{
	m_WidgetArray = new CUIWidget * [m_WidgetCapacity];
}

CUIWindow::~CUIWindow()
{
	for (int i = 0; i < m_WidgetCount; ++i)
	{
		SAFE_RELEASE(m_WidgetArray[i]);
	}

	SAFE_DELETE_ARRAY(m_WidgetArray);
}

bool CUIWindow::Init()
{
	return true;
}

void CUIWindow::Update(float DeltaTime)
{
	for (int i = 0; i < m_WidgetCount;)
	{
		if (!m_WidgetArray[i]->IsActive())
		{
			SAFE_RELEASE(m_WidgetArray[i]);

			for (int j = i; j < m_WidgetCount - 1; ++j)
			{
				m_WidgetArray[j] = m_WidgetArray[j + 1];
			}
<<<<<<< HEAD

			--m_WidgetCount;

=======
			--m_WidgetCount;
>>>>>>> 69991dbf9dd9167eeb97dc5c1ce2636caa31afcd
			continue;
		}

		else if (!m_WidgetArray[i]->GetVisibility())
		{
			++i;
			continue;
		}

		m_WidgetArray[i]->Update(DeltaTime);
		++i;
	}
}

void CUIWindow::PostUpdate(float DeltaTime)
{
	for (int i = 0; i < m_WidgetCount;)
	{
		if (!m_WidgetArray[i]->IsActive())
		{
			SAFE_RELEASE(m_WidgetArray[i]);

			for (int j = i; j < m_WidgetCount - 1; ++j)
			{
				m_WidgetArray[j] = m_WidgetArray[j + 1];
			}

			--m_WidgetCount;

			continue;
		}

		else if (!m_WidgetArray[i]->GetVisibility())
		{
			++i;
			continue;
		}

		m_WidgetArray[i]->PostUpdate(DeltaTime);
		++i;
	}
}

void CUIWindow::Collision(float DeltaTime)
{
	// ZOrder�� ���� Widget�� �����Ѵ�.
	if (m_WidgetCount >= 2)
	{
		qsort(m_WidgetArray, (size_t)m_WidgetCount, sizeof(CUIWidget*),
			CUIWindow::SortZOrder);
	}


	/*for (int i = 0; i < m_WidgetCount;)
	{
		if (!m_WidgetArray[i]->IsActive())
		{
			SAFE_RELEASE(m_WidgetArray[i]);

			for (int j = i; j < m_WidgetCount - 1; ++j)
			{
				m_WidgetArray[j] = m_WidgetArray[j + 1];
			}

			--m_WidgetCount;

			continue;
		}

		m_WidgetArray[i]->Update(DeltaTime);
		++i;
	}*/
}

void CUIWindow::Render(HDC hDC)
{
	for (int i = 0; i < m_WidgetCount;)
	{
		if (!m_WidgetArray[i]->IsActive())
		{
			SAFE_RELEASE(m_WidgetArray[i]);

			for (int j = i; j < m_WidgetCount - 1; ++j)
			{
				m_WidgetArray[j] = m_WidgetArray[j + 1];
			}

			--m_WidgetCount;

			continue;
		}

		++i;
	}

	for (int i = m_WidgetCount - 1; i >= 0; --i)
	{
		if (!m_WidgetArray[i]->GetVisibility())
			continue;

		m_WidgetArray[i]->Render(hDC);
	}
}

int CUIWindow::SortZOrder(const void* Src, const void* Dest)
{
	CUIWidget* SrcObj = *(CUIWidget**)Src;
	CUIWidget* DestObj = *(CUIWidget**)Dest;

	// Bottom ���� ���Ѵ�.
	int	SrcZ = SrcObj->GetZOrder();
	int	DestZ = DestObj->GetZOrder();

	if (SrcZ > DestZ)
		return -1;

	else if (SrcZ < DestZ)
		return 1;

	return 0;
}
