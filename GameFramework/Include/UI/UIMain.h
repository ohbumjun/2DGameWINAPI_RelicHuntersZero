#pragma once
#include "UIWindow.h"
#include "NumberWidget.h"
class CUIMain :
    public CUIWindow
{
	friend class CScene;

private:
	CUIMain();
	virtual ~CUIMain();

private:
	// Main Bar Text 
	class CUIText* m_Text;
	float	m_TextTime;

	int		m_OutputText;
	int		m_OutputIndex;
private:
	// Time 
	float   m_Time;
	int     m_Minute;
	class   CNumberWidget* m_NumberWidget;
	class   CNumberWidget* m_Number1Widget;
	class   CNumberWidget* m_MinuteWidget;

private :
	// Door
	class CStageDoor* m_DoorToStageOne;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
};

