
#include "SceneCollision.h"
#include "../Collision/Collider.h"
#include "../UI/UIWindow.h"
#include "../UI/UIWidget.h"
#include "../Input.h"
#include "Scene.h"
#include "Camera.h"

CSceneCollision::CSceneCollision()
{
	m_vecCollider.reserve(500);
	m_vecUIWindow.reserve(10);
	m_SelectWidget = nullptr;
	m_MouseHoveredWidget = nullptr;
	m_MouseCollision = nullptr;
}

CSceneCollision::~CSceneCollision()
{
}

void CSceneCollision::AddCollider(CCollider* Collider)
{
	m_vecCollider.push_back(Collider);
}

void CSceneCollision::AddUIWindow(CUIWindow* Window)
{
	m_vecUIWindow.push_back(Window);
}

void CSceneCollision::CollisionMouse(float DeltaTime)
{
	Vector2	MousePos = CInput::GetInst()->GetMousePos();
	Vector2	MouseWorldPos = MousePos + m_Scene->GetCamera()->GetPos();

	// 먼저 마우스와 UI를 충돌처리한다.
	int	WidgetCount = 0;

	// 모든 윈도우가 가지고 있는 전체 위젯의 수를 구한다.
	size_t	WindowCount = m_vecUIWindow.size();
	for (size_t i = 0; i < WindowCount; ++i)
	{
		WidgetCount += m_vecUIWindow[i]->GetWidgetCount();
	}

	std::vector<CUIWidget*>	vecWidget;
	vecWidget.resize(WidgetCount);

	int	WidgetOffset = 0;

	if (m_SelectWidget)
	{
		vecWidget[WidgetOffset] = m_SelectWidget;
		++WidgetOffset;
	}

	for (size_t i = 0; i < WindowCount; ++i)
	{
		int	Count = m_vecUIWindow[i]->GetWidgetCount();

		for (int j = 0; j < Count; ++j)
		{
			CUIWidget* Widget = m_vecUIWindow[i]->GetWidget(j);

			if (Widget != m_SelectWidget)
			{
				vecWidget[WidgetOffset] = Widget;
				++WidgetOffset;
			}
		}
	}

	// 마우스와 UI의 충돌을 시작한다.
	// 만약 하나라도 충돌이 된다면 더이상 충돌을 할 필요가 없다.
	bool	EnableCollision = false;

	for (int i = 0; i < WidgetCount; ++i)
	{
		if (vecWidget[i]->CollisionMouse(MousePos, DeltaTime))
		{
			if (m_MouseHoveredWidget && m_MouseHoveredWidget != vecWidget[i])
			{
				m_MouseHoveredWidget->CollisionMouseReleaseCallback(DeltaTime);
			}

			if (m_MouseCollision)
			{
				m_MouseCollision->SetMouseCollision(false);
				m_MouseCollision = nullptr;
			}

			m_MouseHoveredWidget = vecWidget[i];

			EnableCollision = true;

			break;
		}
	}

	// 마우스와 월드에 배치된 물체와의 충돌을 진행한다.
	// 단, 위에서 UI와 충돌이 되었다면 충돌을 안한다.
	if (!EnableCollision)
	{
		if (m_MouseHoveredWidget)
		{
			m_MouseHoveredWidget->CollisionMouseReleaseCallback(DeltaTime);
			m_MouseHoveredWidget = nullptr;
		}

		if (m_SelectWidget)
			m_SelectWidget = nullptr;

		// 월드상의 충돌체와 마우스와의 충돌처리를 한다.
		// 충돌체들을 화면에 나오는 충돌체들만 걸러내고 충돌체의 바닥 기준으로
		// 정렬을 시켜준다.
		size_t	Size = m_vecCollider.size();

		if (Size > 1)
		{
			qsort(&m_vecCollider[0], (size_t)Size, sizeof(CCollider*),
				CSceneCollision::SortY);

			bool	MouseCollision = false;

			for (size_t i = 0; i < Size; ++i)
			{
				if (m_vecCollider[i]->CollisionMouse(MouseWorldPos))
				{
					if (m_MouseCollision)
						m_MouseCollision->SetMouseCollision(false);

					m_MouseCollision = m_vecCollider[i];
					m_MouseCollision->SetMouseCollision(true);

					MouseCollision = true;
				}
			}

			if (!MouseCollision)
			{
				if (m_MouseCollision)
				{
					m_MouseCollision->SetMouseCollision(false);
					m_MouseCollision = nullptr;
				}
			}
		}
	}
}

void CSceneCollision::Collision(float DeltaTime)
{
	size_t	Size = m_vecCollider.size();

	if (Size > 1)
	{
		for (size_t i = 0; i < Size - 1; ++i)
		{
			CCollider* Src = m_vecCollider[i];

			for (size_t j = i + 1; j < Size; ++j)
			{
				CCollider* Dest = m_vecCollider[j];

				// 두 충돌체가 같은 오브젝트에 소속되어 있다면 충돌을 안한다.
				if (Src->GetOwner() == Dest->GetOwner())
					continue;

				CollisionProfile* SrcProfile = Src->GetProfile();
				CollisionProfile* DestProfile = Dest->GetProfile();

				// 서로 상대방에 대한 CollisionState를 얻어온다.
				ECollision_State	SrcState = SrcProfile->vecState[DestProfile->Channel];
				ECollision_State	DestState = DestProfile->vecState[SrcProfile->Channel];

				if (SrcState == ECollision_State::Ignore || // 평화주의 &&(폭력주의)
					DestState == ECollision_State::Ignore)
					continue;

				if (Src->Collision(Dest))
				{
					// 이전에 충돌되고 있었는지를 판단한다.
					if (!Src->CheckCollisionList(Dest))
					{
						// 서로 상대방을 충돌목록에 지정한다.
						Src->AddCollisionList(Dest);
						Dest->AddCollisionList(Src);

						Src->CallCollisionBegin(Dest, DeltaTime);
						Dest->CallCollisionBegin(Src, DeltaTime);
					}
				}

				// 충돌이 안되었을 경우 이전에 충돌하던 물체인지를 판단한다.
				else if (Src->CheckCollisionList(Dest))
				{
					Src->DeleteCollisionList(Dest);
					Dest->DeleteCollisionList(Src);

					Src->CallCollisionEnd(Dest, DeltaTime);
					Dest->CallCollisionEnd(Src, DeltaTime);
				}
			}
		}
	}




	m_vecCollider.clear();
	m_vecUIWindow.clear();
}

int CSceneCollision::SortY(const void* Src, const void* Dest)
{
	CCollider* SrcObj = *(CCollider**)Src;
	CCollider* DestObj = *(CCollider**)Dest;

	// Bottom 값을 구한다.
	float	SrcY = SrcObj->GetBottom();
	float	DestY = DestObj->GetBottom();

	if (SrcY < DestY)
		return -1;

	else if (SrcY > DestY)
		return 1;

	return 0;
}
