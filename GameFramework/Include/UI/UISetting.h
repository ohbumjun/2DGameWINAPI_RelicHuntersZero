#pragma once
#include "UIWindow.h"
#include "NumberWidget.h"
class CUISetting :
    public CUIWindow
{
	friend class CScene;

private:
	CUISetting();
	virtual ~CUISetting();
// Btns 
private :
	class CUIImage* m_LTBackImg;
	class CUIText* m_LTText;
	class CUIImage* m_RTBackImg;
	class CUIText* m_RTText;
	void  SetTextImages();
// Volume
// Main Volume ---
private :
	class CUIText*  m_MVText;
	class CUIImage* m_MVImage;
private:
	class CButton*  m_MVLeftBtn;
	class CButton*  m_MVRightBtn;
public :
	void UpMainVol();
	void DownMainVol();
private :
	class   CNumberWidget* m_MVTenWidget;
	class   CNumberWidget* m_MVOneWidget;
	int m_MVVol;
	void SetMainVolumeElements();
	void MainVolumeUpdate();
public:
	void SetMVTenWidget(int Number)
	{
		m_MVTenWidget->SetNumber(Number);
	}
	void SetMVTenRenderEnable(bool State)
	{
		m_MVTenWidget->SetRenderEnable(State);
	}
	void SetMVOneWidget(int Number)
	{
		m_MVOneWidget->SetNumber(Number);
	}
	void SetMVOneRenderEnable(bool State)
	{
		m_MVOneWidget->SetRenderEnable(State);
	}
// BV Volume ---
private :
	class CUIText*  m_BVText;
	class CUIImage* m_BVImage;
private :
	class CButton*  m_BVLeftBtn;
	class CButton*  m_BVRightBtn;
public:
	void UpBackGroundVol();
	void DownBackGroundVol();
private :
	class   CNumberWidget* m_BVTenWidget;
	class   CNumberWidget* m_BVOneWidget;
	int   m_BGVol;
	void SetBackgroundVolumeElements();
	void BGVolumeUpdate();
public:
	void SetBVTenWidget(int Number)
	{
		m_BVTenWidget->SetNumber(Number);
	}
	void SetBVTenRenderEnable(bool State)
	{
		m_BVTenWidget->SetRenderEnable(State);
	}
	void SetBVOneWidget(int Number)
	{
		m_BVOneWidget->SetNumber(Number);
	}
	void SetBVOneRenderEnable(bool State)
	{
		m_BVOneWidget->SetRenderEnable(State);
	}
// EV Volume ---
private :
	class CUIText*  m_EVText;
	class CUIImage* m_EVImage;
private :
	class CButton*  m_EVLeftBtn;
	class CButton*  m_EVRightBtn;
public:
	void UpEffectVol();
	void DownEffectVol();
private :
	class   CNumberWidget* m_EVTenWidget;
	class   CNumberWidget* m_EVOneWidget;
	int   m_EffectVol;
	void SetEffectVolumeElements();
	void EffectVolumeUpdate();
public:
	void SetEVTenWidget(int Number)
	{
		m_EVTenWidget->SetNumber(Number);
	}
	void SetEVTenRenderEnable(bool State)
	{
		m_EVTenWidget->SetRenderEnable(State);
	}
	void SetEVOneWidget(int Number)
	{
		m_EVOneWidget->SetNumber(Number);
	}
	void SetEVOneRenderEnable(bool State)
	{
		m_EVOneWidget->SetRenderEnable(State);
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
private:
	void StartClick();
	void EditorClick();
	void SettingClick();
	void ExitClick();
};

