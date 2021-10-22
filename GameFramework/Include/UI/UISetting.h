#pragma once
#include "UIWindow.h"
class CUISetting :
    public CUIWindow
{
	friend class CScene;

private:
	CUISetting();
	virtual ~CUISetting();
// Btns 
private :
	class CUIImage* m_LTBtn;
	class CUIText* m_LTText;
	class CUIImage* m_RTBtn;
	class CUIText* m_RTText;
	void  SetTextImages();
// Texts
private :
// Volume
private :
	class CUIText*  m_MVText;
	class CUIImage* m_MVImage;
	class CButton*  m_MVLeftBtn;
	class CButton*  m_MVRightBtn;
	void SetMainVolumeElements();
private :
	class CUIText*  m_BVText;
	class CUIImage* m_BVImage;
	class CButton*  m_BVLeftBtn;
	class CButton*  m_BVRightBtn;
	void SetBackgroundVolumeElements();
private :
	class CUIText*  m_EVText;
	class CUIImage* m_EVImage;
	class CButton*  m_EVLeftBtn;
	class CButton*  m_EVRightBtn;
	void SetEffectVolumeElements();
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
private:
	void StartClick();
	void EditorClick();
	void SettingClick();
	void ExitClick();
};

