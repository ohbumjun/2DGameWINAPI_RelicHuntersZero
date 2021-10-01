#pragma once

#include "MapBase.h"
#include "../Resource/Texture.h"
#include "Tile.h"

class CTileMap : public CMapBase
{
	
	friend class CScene;

protected :
	// 여러개의 Tile을 가질 수 있도록 세팅한다
	CTileMap();
	virtual ~CTileMap();
protected :
	std::vector<CTile*> m_vecTile;
	CSharedPtr<CTexture> m_TileTexture;
	int m_TileCountX;
	int m_TileCountY;
	Vector2 m_TileSize;

	int m_StartX;
	int m_StartY;
	int m_EndX;
	int m_EndY;

public :
	bool CreateTile(int CountX, int CountY,
		const Vector2& TileSize);

	// 전체에 원하는 Texture 세팅 
	bool SetTileTexture(CTexture* Texture);

	// 내가 원하는 특정 idx 에만 
	bool SetTileTexture(CTexture* Texture,
		int IndexX, int IndexY);
	void ChangeTileOption(const Vector2& Pos,
		ETileOption Option);
	void SetTileFrame(
		const Vector2& Pos,
		const Vector2& Start, const Vector2& End);
	CTile* GetTile(const Vector2& Pos);
public :
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render(HDC hDC);
};

