#pragma once

// CRef를 사용하지 않는 이유
// 어디서 또 다시 가져다쓰거나 하지 않기 때문이다
#include "../GameInfo.h"
class CMapBase
{
	friend class CScene;
protected: 
	CMapBase();
	virtual ~CMapBase();
public :
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render(HDC hDC);
};

