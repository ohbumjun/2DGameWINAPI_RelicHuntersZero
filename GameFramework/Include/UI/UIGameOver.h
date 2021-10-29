#pragma once
#include "UIWindow.h"
class CUIGameOver :
    public CUIWindow
{
public:
    CUIGameOver();
    virtual ~CUIGameOver();
private :
    CSharedPtr<class CUIImage> m_UpperImg;
    CSharedPtr<class CUIImage> m_DownImg;
    CSharedPtr<class CButton> m_MenuBtn;
    CSharedPtr<class CUIText> m_MenuTxt;
    CSharedPtr<class CButton> m_AgainBtn;
    CSharedPtr<class CUIText> m_AgainTxt;
public :
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void Render(HDC hdc);
public :
    void SetGameOverWidgets();
public :
    void MenuClick();
    void AgainClick();
};

