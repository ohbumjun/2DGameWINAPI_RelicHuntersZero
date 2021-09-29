#pragma once

// CRef�� ������� �ʴ� ����
// ��� �� �ٽ� �����پ��ų� ���� �ʱ� �����̴�
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

