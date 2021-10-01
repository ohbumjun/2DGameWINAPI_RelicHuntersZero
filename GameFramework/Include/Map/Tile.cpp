#include "Tile.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"
#include "../Scene/SceneManager.h"
#include "../GameManager.h"

CTile::CTile() :
	m_Index(0),
	m_IndexX(0),
	m_IndexY(0),
	m_Option(ETileOption::Normal)
{
}

CTile::~CTile()
{
}

bool CTile::Init()
{
	return true;
}

void CTile::Update(float DeltaTime)
{
}

void CTile::PostUpdate(float DeltaTime)
{
}

void CTile::PrevRender()
{
}

void CTile::Render(HDC hDC)
{
	if (m_Texture)
	{
		CCamera* Camera = CSceneManager::GetInst()->GetScene()->GetCamera();
		Vector2 CameraPos = Camera->GetPos();

		// World »ó À§Ä¡ 
		m_Texture->Render(hDC,
			m_Pos-CameraPos,
			Vector2(0.f,0.f),
			m_Size);
	}

	if (CGameManager::GetInst()->IsEditorMode())
	{
		HBRUSH Brush = 0;

		switch (m_Option)
		{
		case ETileOption::Normal:
			Brush = CGameManager::GetInst()->GetGreenBrush();
			break;
		case ETileOption::Wall:
			Brush = CGameManager::GetInst()->GetRedBrush();
			break;
		case ETileOption::Slow:
			Brush = CGameManager::GetInst()->GetYellowBrush();
			break;

	}

		RECT rc;
		rc.left   = (LONG)m_Pos.x;
		rc.top    = (LONG)m_Pos.y;
		rc.right  = (LONG)m_Pos.x + (LONG)m_Size.x;
		rc.bottom = (LONG)m_Pos.x + (LONG)m_Size.y;

		FrameRect(hDC,&rc,Brush);

	}
}
