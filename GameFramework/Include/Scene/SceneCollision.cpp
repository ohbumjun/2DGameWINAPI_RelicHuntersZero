
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

	// ���� ���콺�� UI�� �浹ó���Ѵ�.
	int	WidgetCount = 0;

	// ��� �����찡 ������ �ִ� ��ü ������ ���� ���Ѵ�.
	size_t	WindowCount = m_vecUIWindow.size();
	for (size_t i = 0; i < WindowCount; ++i)
	{
		WidgetCount += m_vecUIWindow[i]->GetWidgetCount();
	}

	std::vector<CUIWidget*>	vecWidget;
	vecWidget.resize(WidgetCount);

	int	WidgetOffset = 0;

	// �̹� ���õ� Widget�� �ִٸ� ���� �������ش� 
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

	// ���콺�� UI�� �浹�� �����Ѵ�.
	// ���� �ϳ��� �浹�� �ȴٸ� ���̻� �浹�� �� �ʿ䰡 ����.
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

	// ���콺�� ���忡 ��ġ�� ��ü���� �浹�� �����Ѵ�.
	// ��, ������ UI�� �浹�� �Ǿ��ٸ� �浹�� ���Ѵ�.
	if (!EnableCollision)
	{
		if (m_MouseHoveredWidget)
		{
			m_MouseHoveredWidget->CollisionMouseReleaseCallback(DeltaTime);
			m_MouseHoveredWidget = nullptr;
		}

		if (m_SelectWidget)
			m_SelectWidget = nullptr;

		// ������� �浹ü�� ���콺���� �浹ó���� �Ѵ�.
		// �浹ü���� ȭ�鿡 ������ �浹ü�鸸 �ɷ����� �浹ü�� �ٴ� ��������
		// ������ �����ش�.
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

				// �� �浹ü�� ���� ������Ʈ�� �ҼӵǾ� �ִٸ� �浹�� ���Ѵ�.
				if (Src->GetOwner() == Dest->GetOwner())
					continue;

				CollisionProfile* SrcProfile = Src->GetProfile();
				CollisionProfile* DestProfile = Dest->GetProfile();

				// ���� ���濡 ���� CollisionState�� ���´�.
				ECollision_State	SrcState = SrcProfile->vecState[DestProfile->Channel];
				ECollision_State	DestState = DestProfile->vecState[SrcProfile->Channel];

				if (SrcState == ECollision_State::Ignore || // ��ȭ���� &&(��������)
					DestState == ECollision_State::Ignore)
					continue;

				if (Src->Collision(Dest))
				{
					// ������ �浹�ǰ� �־������� �Ǵ��Ѵ�.
					if (!Src->CheckCollisionList(Dest))
					{
						// ���� ������ �浹��Ͽ� �����Ѵ�.
						Src->AddCollisionList(Dest);
						Dest->AddCollisionList(Src);

						Src->CallCollisionBegin(Dest, DeltaTime);
						Dest->CallCollisionBegin(Src, DeltaTime);
					}
				}

				// �浹�� �ȵǾ��� ��� ������ �浹�ϴ� ��ü������ �Ǵ��Ѵ�.
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

bool CSceneCollision::CollisionLaser(Vector2 LaserPos)
{
	CGameObject* Player   = m_Scene->GetPlayer();
	Vector2 PlayerPos     = Player->GetPos();
	size_t	Size = m_vecCollider.size();
	if (Size > 1)
	{
		for (size_t i = 0; i < Size - 1; ++i)
		{
			CCollider* Src = m_vecCollider[i];
			// Player ��ü Ȥ�� Player�� �� Bullet ���� �浹ü�� �Ÿ��� 
			if (Src->GetOwner() == Player) continue;
			if (Src->IsCollisionWithLaser(LaserPos)) return true;
		}
	}
	return false;
}

int CSceneCollision::SortY(const void* Src, const void* Dest)
{
	CCollider* SrcObj = *(CCollider**)Src;
	CCollider* DestObj = *(CCollider**)Dest;

	// Bottom ���� ���Ѵ�.
	float	SrcY = SrcObj->GetBottom();
	float	DestY = DestObj->GetBottom();

	if (SrcY < DestY)
		return -1;

	else if (SrcY > DestY)
		return 1;

	return 0;
}
