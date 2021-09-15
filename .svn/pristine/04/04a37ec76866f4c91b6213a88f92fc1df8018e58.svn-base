
#include "ProgressBar.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "UIWindow.h"

CProgressBar::CProgressBar()	:
	m_Percent(1.f)
{
}

CProgressBar::CProgressBar(const CProgressBar& widget)
{
	m_Percent = widget.m_Percent;
	m_Texture = widget.m_Texture;
}

CProgressBar::~CProgressBar()
{
}

void CProgressBar::SetTexture(const std::string& Name)
{
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CProgressBar::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CProgressBar::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CProgressBar::SetTexture(const std::string& Name,
	const std::vector<std::wstring>& vecFileName,
	const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CProgressBar::SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
	if (m_Texture)
		m_Texture->SetColorKey(r, g, b, Index);
}

bool CProgressBar::Init()
{
	return true;
}

void CProgressBar::Update(float DeltaTime)
{
}

void CProgressBar::PostUpdate(float DeltaTime)
{
}

void CProgressBar::Collision(float DeltaTime)
{
}

void CProgressBar::Render(HDC hDC)
{
	if (m_Texture)
	{
		Vector2	Pos = m_Pos + m_Owner->GetPos();

		// 이미지를 이용해서 출력한다.
		m_Texture->Render(hDC, Pos, Vector2(0.f, 0.f), m_Size * Vector2(m_Percent, 1.f));
	}
}
