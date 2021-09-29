#pragma once

#include "MapBase.h"
class CTileMap : public CMapBase
{
	
	friend class CScene;

protected :
	// 여러개의 Tile을 가질 수 있도록 세팅한다
	CTileMap();
	virtual ~CTileMap();
public :
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render(HDC hDC);
};

