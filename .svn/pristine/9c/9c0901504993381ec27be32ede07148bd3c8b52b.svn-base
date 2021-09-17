#pragma once
#include "UIWindow.h"
class CUIMain :
    public CUIWindow
{
	friend class CScene;

private:
	CUIMain();
	virtual ~CUIMain();

private:
	class CUIText* m_Text;
	float	m_TextTime;

	int		m_OutputText;
	int		m_OutputIndex;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
};

