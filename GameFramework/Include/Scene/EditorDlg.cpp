#include "EditorDlg.h"
#include "../GameManager.h"

CEditorDlg::CEditorDlg() :
	m_ID(0),
	m_hDlg(0)
{
}

CEditorDlg::~CEditorDlg()
{
	// 다이얼로그가 열려있었다면 지워준다 
	if (m_hDlg)
		DestroyWindow(m_hDlg);
}

bool CEditorDlg::Init(int ID)
{
	// Modal : 부모윈도우는 멈추고, 열리는 다이얼로그만 동작되는 방식
	// Modeless : 부모윈도우와 다이얼로그 모두 동작되는 방식
	m_hDlg = CreateDialog(
		
		CGameManager::GetInst()->GetWindowInstance(),
		// 이름
		MAKEINTRESOURCE(ID),
		// ParentWindow
		CGameManager::GetInst()->GetWindowHandle(),
		// Func 
		CEditorDlg::WndProc

	); // Modeless 다이얼로그를 만들어준다 

	/*
	> 윈도우 instance ( hInstance )
	- 윈도우가 제공해주는 것으로, 윈도우 운영체제에서 실행되는 
	- "프로그램"들을 구별하기 위한 id 값을 의미한다 
	> 윈도우 handle (hwnd )
	- 윈도우 운영체제에서 
	- "윈도우"들을 구별하기 위한 id 값

	"프로그램"과 "윈도우"의 차이는 무엇일까 ?
	윈도우 프로그래밍에서, 하나의 프로그램에는
	많은 윈도우들을 가질 수 있다. 
	프로그램의 윈도우 핸들 중에서, 대표적인 것이 
	부모 윈도우의 핸들

	*/
	
	return true;
}

void CEditorDlg::Show()
{
}

LRESULT CEditorDlg::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}
