#pragma once

#include "UIWidget.h"
#include "../Resource/Texture.h"

class CProgressBar :
    public CUIWidget
{
public:
	CProgressBar();
	CProgressBar(const CProgressBar& widget);
	virtual ~CProgressBar();

protected:
	CSharedPtr<CTexture>	m_Texture;
	float		m_Percent;

public:
	void SetPercent(float Percent)
	{
		m_Percent = Percent;

		if (m_Percent < 0.f)
			m_Percent = 0.f;

		else if (m_Percent > 1.f)
			m_Percent = 1.f;
	}

	void SetTexture(CTexture* Texture)
	{
		m_Texture = Texture;

		if (m_Texture)
		{
			m_Size.x = (float)m_Texture->GetWidth();
			m_Size.y = (float)m_Texture->GetHeight();
		}
	}

	void SetTexture(const std::string& Name);
	void SetTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	void SetTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTextureColorKey(unsigned char r,
		unsigned char g, unsigned char b, int Index = 0);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& Pos, HDC hDC);
	virtual CProgressBar* Clone();
};

