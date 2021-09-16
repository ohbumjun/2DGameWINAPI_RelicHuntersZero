
#include "UIImage.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "UIWindow.h"

CUIImage::CUIImage()
{
}

CUIImage::CUIImage(const CUIImage& widget)	:
	CUIWidget(widget)
{
	m_Texture = widget.m_Texture;
}

CUIImage::~CUIImage()
{
}

void CUIImage::SetTexture(const std::string& Name)
{
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CUIImage::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CUIImage::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CUIImage::SetTexture(const std::string& Name,
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

void CUIImage::SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
	if (m_Texture)
		m_Texture->SetColorKey(r, g, b, Index);
}

bool CUIImage::Init()
{
	return true;
}

void CUIImage::Update(float DeltaTime)
{
}

void CUIImage::PostUpdate(float DeltaTime)
{
}

void CUIImage::Collision(float DeltaTime)
{
}

void CUIImage::Render(HDC hDC)
{
	if (m_Texture)
	{
		Vector2	Pos = m_Pos + m_Owner->GetPos();

		// 이미지를 이용해서 출력한다.
		m_Texture->Render(hDC, Pos, Vector2(0.f, 0.f), m_Size);
	}
}

void CUIImage::Render(const Vector2& Pos, HDC hDC)
{
	if (m_Texture)
	{
		// 이미지를 이용해서 출력한다.
		m_Texture->Render(hDC, Pos, Vector2(0.f, 0.f), m_Size);
	}
}

CUIImage* CUIImage::Clone()
{
	return new CUIImage(*this);
}
