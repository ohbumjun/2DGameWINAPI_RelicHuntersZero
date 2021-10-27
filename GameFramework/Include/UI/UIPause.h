#pragma once
#include "UIWindow.h"
class CUIPause :
    public CUIWindow
{
	friend class CScene;
public:
	CUIPause();
	virtual ~CUIPause();
private:
	bool m_Toggles[4];
	std::vector<CSharedPtr<class CButton>> m_Buttons;
	std::vector<CSharedPtr<class CUIText>> m_Texts;
private :
	class CPlayer* m_Player;
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
public :
	void SetPlayer(class CPlayer* Player) { m_Player = Player; }
private:
	void ContinueClick();
	void SelectClick();
	void MainMenuClick();
	void ExitClick();
};

