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

	float   m_Time;
	int     m_Minute;
	class   CNumberWidget* m_NumberWidget;
	class   CNumberWidget* m_Number1Widget;
	class   CNumberWidget* m_MinuteWidget;


public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
};

