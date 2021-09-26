#include "Camera.h"
#include "../Input.h"

CCamera::CCamera()	:
	m_Resolution(1280.f, 720.f),
	m_WorldResolution(1280.f, 720.f)
{
}

CCamera::~CCamera()
{
}

bool CCamera::Init()
{
	return true;
}

void CCamera::Update(float DeltaTime)
{
	if (m_Target)
	{
		if (!m_Target->IsActive())
		{
			m_Target = nullptr;
		}
		else
		{
			Vector2 TargetPos = m_Target->GetPos();
			// MousePos : ���� ��ũ�� �󿡼��� mouse ��ǥ 
			// m_Pos    : ���� ��ũ�� ���� �����, world �󿡼� ���ϴ� ��ǥ
			Vector2	MousePos = CInput::GetInst()->GetMousePos();
			// Vector2	MouseWorldPos = MousePos + m_Scene->GetCamera()->GetPos();
			Vector2	MouseWorldPos = MousePos + m_Pos;

			Vector2 RenderPos = (TargetPos + MouseWorldPos) / 2;
			m_Pos = RenderPos - m_TargetPivot * m_Resolution +
				m_TargetOffset;
		}
	}

	// ī�޶� ���带 ��� �� ������ �����Ѵ�.
	if (m_Pos.x < 0.f)
		m_Pos.x = 0.f;

	else if (m_Pos.x + m_Resolution.x > m_WorldResolution.x)
		m_Pos.x = m_WorldResolution.x - m_Resolution.x;

	if (m_Pos.y < 0.f)
		m_Pos.y = 0.f;

	else if (m_Pos.y + m_Resolution.y > m_WorldResolution.y)
		m_Pos.y = m_WorldResolution.y - m_Resolution.y;
}
