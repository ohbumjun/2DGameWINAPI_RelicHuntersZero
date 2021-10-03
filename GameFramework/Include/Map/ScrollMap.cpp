#include"ScrollMap.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Camera.h"

CScrollMap::CScrollMap() :
	m_ScrollRatio(Vector2(1.f,1.f))
{
}

CScrollMap::~CScrollMap()
{
}

bool CScrollMap::SetTexture(const std::string& Name, 
	const TCHAR* FileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);
	SetTexture(Name);
	return true;
}

bool CScrollMap::SetTextureFullPath(const std::string& Name, 
	const TCHAR* FullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);
	SetTexture(Name);
	return true;
}

bool CScrollMap::SetTexture(const std::string& Name, 
	const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName ,PathName);
	SetTexture(Name);
	return true;
}

bool CScrollMap::SetTexture(const std::string& Name)
{
	m_ScrollTexture = m_Scene->GetSceneResource()->FindTexture(Name);
	return true;
}

void CScrollMap::Start()
{
	CMapBase::Start();
}

bool CScrollMap::Init()
{
	if (!CMapBase::Init()) return false;
	return true;
}

void CScrollMap::Update(float DeltaTime)
{
	CMapBase::Update(DeltaTime);
}

void CScrollMap::PostUpdate(float DeltaTime)
{
	CMapBase::PostUpdate(DeltaTime);
}

void CScrollMap::PrevRender()
{
}

void CScrollMap::Render(HDC hDC)
{
	CMapBase::Render(hDC);
	if (m_ScrollTexture)
	{
		CCamera* Camera     = m_Scene->GetCamera();
		Vector2 Resolution  = Camera->GetResolution();
		Vector2 ImagePos    = Camera->GetPos() * m_ScrollRatio;
		unsigned int Width  = m_ScrollTexture->GetWidth();
		unsigned int Height = m_ScrollTexture->GetHeight();
		
		// 범위를 넘어가지 않게 만들어준다 
		ImagePos.x = ImagePos.x < 0.f ? 0.f : ImagePos.x;
		ImagePos.x = ImagePos.x + Resolution.x > Width ? Width - Resolution.x : ImagePos.x;
		ImagePos.y = ImagePos.y < 0.f ? 0.f : ImagePos.y;
		ImagePos.y = ImagePos.y + Resolution.y > Height? Height - Resolution.y :ImagePos.y;
		
		m_ScrollTexture->Render(hDC, m_Pos,
			ImagePos, m_Size);
	}
}

void CScrollMap::Save(FILE* File)
{
}

void CScrollMap::Load(FILE* File)
{
}
