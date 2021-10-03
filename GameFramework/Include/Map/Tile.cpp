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

		// World 상 위치 x --> 현재 창 내의 위치 
		m_Texture->Render(hDC,
			m_Pos-CameraPos,
			m_StartFrame,
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

		CCamera* Camera = CSceneManager::GetInst()->GetScene()->GetCamera();
		Vector2 CameraPos = Camera->GetPos();

		RECT rc;
		rc.left   = (LONG)m_Pos.x - (LONG)CameraPos.x;
		rc.top    = (LONG)m_Pos.y - (LONG)CameraPos.y;
		rc.right  = (LONG)m_Pos.x + (LONG)m_Size.x - (LONG)CameraPos.x;
		rc.bottom = (LONG)m_Pos.y + (LONG)m_Size.y - (LONG)CameraPos.y;

		FrameRect(hDC,&rc,Brush);

	}
}

void CTile::Save(FILE* pFile)
{
	fwrite(&m_Pos, sizeof(Vector2), 1, pFile);
	fwrite(&m_Size, sizeof(Vector2), 1, pFile);

	fwrite(&m_IndexX, sizeof(int), 1, pFile);
	fwrite(&m_IndexY, sizeof(int), 1, pFile);
	fwrite(&m_Index, sizeof(int), 1, pFile);

	fwrite(&m_StartFrame, sizeof(Vector2), 1, pFile);
	fwrite(&m_EndFrame, sizeof(Vector2), 1, pFile);
	fwrite(&m_Option, sizeof(ETileOption), 1, pFile);

	if (m_Texture)
	{
		// Texture 존재 여부를 저장한다 
		bool Tex = true;
		fwrite(&Tex, sizeof(bool), 1, pFile);
		m_Texture->Save(pFile);
	}
	else
	{
		bool Tex = false;
		fwrite(&Tex, sizeof(bool), 1, pFile);
	}
}

void CTile::Load(FILE* pFile)
{
	fread(&m_Pos, sizeof(Vector2), 1, pFile);
	fread(&m_Size, sizeof(Vector2), 1, pFile);

	fread(&m_IndexX, sizeof(int), 1, pFile);
	fread(&m_IndexY, sizeof(int), 1, pFile);
	fread(&m_Index, sizeof(int), 1, pFile);

	fread(&m_StartFrame, sizeof(Vector2), 1, pFile);
	fread(&m_EndFrame, sizeof(Vector2), 1, pFile);
	fread(&m_Option, sizeof(ETileOption), 1, pFile);

	bool Tex = true;
	fread(&Tex, sizeof(bool), 1, pFile);
	if (Tex)
	{
		m_Texture = CTexture::LoadStatic(pFile, m_Scene);
	}

}
