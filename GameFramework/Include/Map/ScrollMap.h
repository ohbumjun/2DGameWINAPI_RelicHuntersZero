#pragma once

#include "MapBase.h"
#include "../Resource/Texture.h"

class CScrollMap :
	public CMapBase
{
	friend class CScene;

protected:
	CScrollMap();
	virtual ~CScrollMap();

protected:
	CSharedPtr<CTexture>	m_ScrollTexture;
	Vector2		m_TextureSize;
	Vector2		m_ScrollRatio;
	bool		m_Loop;

public:
	void SetScrollRatio(float x, float y)
	{
		m_ScrollRatio = Vector2(x, y);
	}

	void SetLoop(bool Loop)
	{
		m_Loop = Loop;
	}

public:
	void SetTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTextureFullPath(const std::string& Name,
		const TCHAR* FullPath);
	void SetTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTexture(const std::string& Name);
	void SetTextureColorKey(const unsigned char r, const unsigned char g,
		const unsigned char b);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render(HDC hDC);
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

