#pragma once
#include "UIWindow.h"
#include "NumberWidget.h"
class CUIHome :
    public CUIWindow
{
	friend class CScene;

private:
	CUIHome();
	virtual ~CUIHome();
private:
// HP 
	class   CNumberWidget* m_HpCostHunderedWidget;
	class   CNumberWidget* m_HpCostTenWidget;
	class   CNumberWidget* m_HpCostOneWidget;
	int     m_HpCost;
public:
	void SetHpCostHundredWidget(int Number)
	{
		m_HpCostHunderedWidget->SetNumber(Number);
	}
	void SetHpCostHundredRenderEnable(bool State)
	{
		m_HpCostHunderedWidget->SetRenderEnable(State);
	}
	void SetHpCostTenWidget(int Number)
	{
		m_HpCostTenWidget->SetNumber(Number);
	}
	void SetHpCostTenRenderEnable(bool State)
	{
		m_HpCostTenWidget->SetRenderEnable(State);
	}
	void SetHpCostOneWidget(int Number)
	{
		m_HpCostOneWidget->SetNumber(Number);
	}
	void SetHpCostOneRenderEnable(bool State)
	{
		m_HpCostOneWidget->SetRenderEnable(State);
	}
private:
	// HP 
	class   CNumberWidget* m_MpCostHunderedWidget;
	class   CNumberWidget* m_MpCostTenWidget;
	class   CNumberWidget* m_MpCostOneWidget;
	int     m_MpCost;
public:
	void SetMpCostHundredWidget(int Number)
	{
		m_MpCostHunderedWidget->SetNumber(Number);
	}
	void SetMpCostHundredRenderEnable(bool State)
	{
		m_MpCostHunderedWidget->SetRenderEnable(State);
	}
	void SetMpCostTenWidget(int Number)
	{
		m_MpCostTenWidget->SetNumber(Number);
	}
	void SetMpCostTenRenderEnable(bool State)
	{
		m_MpCostTenWidget->SetRenderEnable(State);
	}
	void SetMpCostOneWidget(int Number)
	{
		m_MpCostOneWidget->SetNumber(Number);
	}
	void SetMpCostOneRenderEnable(bool State)
	{
		m_MpCostOneWidget->SetRenderEnable(State);
	}
private:
	// Shield
	class   CNumberWidget* m_ShieldCostHunderedWidget;
	class   CNumberWidget* m_ShieldCostTenWidget;
	class   CNumberWidget* m_ShieldCostOneWidget;
	int     m_ShieldCost;
public:
	void SetShieldCostHundredWidget(int Number)
	{
		m_ShieldCostHunderedWidget->SetNumber(Number);
	}
	void SetShieldCostHundredRenderEnable(bool State)
	{
		m_ShieldCostHunderedWidget->SetRenderEnable(State);
	}
	void SetShieldCostTenWidget(int Number)
	{
		m_ShieldCostTenWidget->SetNumber(Number);
	}
	void SetShieldCostTenRenderEnable(bool State)
	{
		m_ShieldCostTenWidget->SetRenderEnable(State);
	}
	void SetShieldCostOneWidget(int Number)
	{
		m_ShieldCostOneWidget->SetNumber(Number);
	}
	void SetShieldCostOneRenderEnable(bool State)
	{
		m_ShieldCostOneWidget->SetRenderEnable(State);
	}
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	void HpCostInit(std::vector<std::wstring>& vecNumberFileName);
	void MpCostInit(std::vector<std::wstring>& vecNumberFileName);
	void ShieldCostInit(std::vector<std::wstring>& vecNumberFileName);
};

