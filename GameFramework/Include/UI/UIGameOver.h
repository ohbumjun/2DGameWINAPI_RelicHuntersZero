#pragma once
#include "UIWindow.h"
class CUIGameOver :
    public CUIWindow
{
public:
    CUIGameOver();
    virtual ~CUIGameOver();
private :
    CSharedPtr<class CButton> m_MenuBtn;
    CSharedPtr<class CUIText> m_MenuTxt;
    CSharedPtr<class CButton> m_AgainBtn;
    CSharedPtr<class CUIText> m_AgainTxt;
public :
    virtual bool Init();
    virtual void Update(float DeltaTime);
public :
    void SetGameOverWidgets();
public :
    void MenuClick();
    void AgainClick();
};

