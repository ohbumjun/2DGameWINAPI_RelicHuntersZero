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

void CScrollMap::SetTextureColorKey(const unsigned char r, const unsigned char g, const unsigned char b)
{
	if (m_ScrollTexture)
		m_ScrollTexture->SetColorKey(r, g, b);
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
	CMapBase::PrevRender();
}

void CScrollMap::Render(HDC hDC)
{
	CMapBase::Render(hDC);
	if (m_ScrollTexture)
	{
		CCamera* Camera     = m_Scene->GetCamera();
		Vector2 Resolution  = Camera->GetResolution();
		// 예를 들어, 윈도우 해상도가 2000
		// 이미지가, 1000
		// scroll ratio가 0.5
		// 카메라가 윈도우 해상도 상으로, 20을 움직일때
		// 이미지상에서의 움직임은 10이 될 것이다
		// 즉, 실제 움직임보다, 배경은 0.5 배 로 움직이는 것처럼 보일 것이다 
		Vector2 ImagePos    = Camera->GetPos() * m_ScrollRatio;
		unsigned int Width  = m_ScrollTexture->GetWidth();
		unsigned int Height = m_ScrollTexture->GetHeight();
		
		// 범위를 넘어가지 않게 만들어준다 
		ImagePos.x = ImagePos.x < 0.f ? 0.f : ImagePos.x;
		ImagePos.x = ImagePos.x + Resolution.x > Width ? Width - Resolution.x : ImagePos.x;
		ImagePos.y = ImagePos.y < 0.f ? 0.f : ImagePos.y;
		ImagePos.y = ImagePos.y + Resolution.y > Height? Height - Resolution.y :ImagePos.y;
		
		m_ScrollTexture->Render(hDC, m_Pos, ImagePos, m_Size);
	}
}

void CScrollMap::Save(FILE* File)
{
}

void CScrollMap::Load(FILE* File)
{
}
