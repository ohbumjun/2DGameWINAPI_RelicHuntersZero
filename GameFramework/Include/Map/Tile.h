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
	class CScene* m_Scene;
	Vector2	m_Pos;
	Vector2	m_Size;
	int		m_IndexX;
	int		m_IndexY;
	int		m_Index;
	CSharedPtr<CTexture>	m_Texture;
	ETileOption	m_Option;
	Vector2	m_StartFrame;
	Vector2	m_EndFrame;
	bool	m_SideCollision;

public:
	void SetSideCollision(bool SideCollision)
	{
		m_SideCollision = SideCollision;
	}

	void SetStartFrame(const Vector2& Frame)
	{
		m_StartFrame = Frame;
	}

	void SetEndFrame(const Vector2& Frame)
	{
		m_EndFrame = Frame;
	}

public:
	bool GetSideCollision()	const
	{
		return m_SideCollision;
	}

	ETileOption GetTileOption()	const
	{
		return m_Option;
	}

	Vector2 GetPos()	const
	{
		return m_Pos;
	}

	Vector2 GetSize()	const
	{
		return m_Size;
	}

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

		m_EndFrame = Size;
	}

	void SetTileTexture(CTexture* Texture)
	{
		m_Texture = Texture;
	}

	void SetTileOption(ETileOption Option)
	{
		m_Option = Option;
	}

public:
	bool Init();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	void PrevRender();
	void Render(HDC hDC);
	void Save(FILE* pFile);
	void Load(FILE* pFile);
};

