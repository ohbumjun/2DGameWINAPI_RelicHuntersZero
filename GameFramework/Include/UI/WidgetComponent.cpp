
#include "WidgetComponent.h"
#include "../Object/GameObject.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"

CWidgetComponent::CWidgetComponent()	:
	m_Visibility(true),
	m_Owner(nullptr),
	m_Scene(nullptr),
	m_Widget(nullptr)
{
}

CWidgetComponent::CWidgetComponent(const CWidgetComponent& widget)
{
	m_Pos = widget.m_Pos;
	m_Visibility = widget.m_Visibility;
	m_Widget = widget.m_Widget->Clone();
}

CWidgetComponent::~CWidgetComponent()
{
}

bool CWidgetComponent::Init()
{
	return true;
}

void CWidgetComponent::Update(float DeltaTime)
{
	if (m_Widget)
	{
		m_Widget->Update(DeltaTime);
	}
}

void CWidgetComponent::PostUpdate(float DeltaTime)
{
	if (m_Widget)
	{
		m_Widget->PostUpdate(DeltaTime);
	}
}

void CWidgetComponent::Collision(float DeltaTime)
{
	if (m_Widget)
		m_Widget->Collision(DeltaTime);
}

void CWidgetComponent::Render(HDC hDC)
{
	if (m_Widget)
	{
		Vector2	RenderPos = m_Pos + m_Owner->GetPos() - m_Scene->GetCamera()->GetPos();
		m_Widget->Render(RenderPos, hDC);
	}
}

CWidgetComponent* CWidgetComponent::Clone()
{
	return new CWidgetComponent(*this);
}
