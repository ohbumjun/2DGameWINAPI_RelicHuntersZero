#pragma once
#include "UIWindow.h"
class CUIMenu :
    public CUIWindow
{
	friend class CScene;

private:
	CUIMenu();
	virtual ~CUIMenu();
private :
	bool m_Toggles[4];
	std::vector<CSharedPtr<class CButton>> m_Buttons;
	std::vector<CSharedPtr<class CUIText>> m_Texts;
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
private :
	void StartClick();
	void EditorClick();
	void SettingClick();
	void ExitClick();
};

