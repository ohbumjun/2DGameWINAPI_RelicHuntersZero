#pragma once

#include "../GameInfo.h"
#include "../Resource/Texture.h"

class CTile
{
	friend class CTileMap;

private:
	CTile();
	~CTile();

private:
	Vector2	m_Pos;
	Vector2	m_Size;
	int		m_IndexX;
	int		m_IndexY;
	int		m_Index;
	CSharedPtr<CTexture>	m_Texture;

public:
	void SetTileInfo(const Vector2& Pos, const Vector2& Size,
		int IndexX, int IndexY, int Index, CTexture* Texture)
	{
		m_Pos = Pos;
		m_Size = Size;
		m_IndexX = IndexX;
		m_IndexY = IndexY;
		m_Index = Index;
		m_Texture = Texture;
	}

	void SetTileTexture(CTexture* Texture)
	{
		m_Texture = Texture;
	}

public:
	bool Init();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void PrevRender();
	void Render(HDC hDC);
};

