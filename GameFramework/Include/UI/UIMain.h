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
	// Gun 
	class   CNumberWidget* m_BulletHunderedWidget;
	class   CNumberWidget* m_BulletTenWidget;
	class   CNumberWidget* m_BulletOneWidget;
public :
	void SetBulletHundredWidget(int Number)
	{
		m_BulletHunderedWidget->SetNumber(Number);
	}
	void SetBulletHundredRenderEnable(bool State)
	{
		m_BulletHunderedWidget->SetRenderEnable(State);
	}

	void SetBulletTenWidget(int Number)
	{
		m_BulletTenWidget->SetNumber(Number);
	}
	void SetBulletTenRenderEnable(bool State)
	{
		m_BulletTenWidget->SetRenderEnable(State);
	}

	void SetBulletOneWidget(int Number)
	{
		m_BulletOneWidget->SetNumber(Number);
	}
	void SetBulletOneRenderEnable(bool State)
	{
		m_BulletOneWidget->SetRenderEnable(State);
	}
private :
	class   CNumberWidget* m_FullBulletHunderedWidget;
	class   CNumberWidget* m_FullBulletTenWidget;
	class   CNumberWidget* m_FullBulletOneWidget;
public :
	void SetFullBulletHundredWidget(int Number)
	{
		m_FullBulletHunderedWidget->SetNumber(Number);
	}
	void SetFullBulletHundredRenderEnable(bool State)
	{
		m_FullBulletHunderedWidget->SetRenderEnable(State);
	}

	void SetFullBulletTenWidget(int Number)
	{
		m_FullBulletTenWidget->SetNumber(Number);
	}
	void SetFullBulletTenRenderEnable(bool State)
	{
		m_FullBulletTenWidget->SetRenderEnable(State);
	}

	void SetFullBulletOneWidget(int Number)
	{
		m_FullBulletOneWidget->SetNumber(Number);
	}
	void SetFullBulletOneRenderEnable(bool State)
	{
		m_FullBulletOneWidget->SetRenderEnable(State);
	}
private :
	// Door
	class CStageDoor* m_DoorToStageOne;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
};

