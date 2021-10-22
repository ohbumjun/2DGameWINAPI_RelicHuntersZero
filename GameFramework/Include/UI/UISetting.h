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
	class CUIImage* m_RTBtn;
// Texts
private :
	class CUIText* m_LTText;
	class CUIText* m_RTText;
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
private:
	void StartClick();
	void EditorClick();
	void SettingClick();
	void ExitClick();
};

