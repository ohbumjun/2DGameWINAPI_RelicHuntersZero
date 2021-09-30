#include "EditorDlg.h"
#include "../GameManager.h"

CEditorDlg::CEditorDlg() :
	m_ID(0),
	m_hDlg(0)
{
}

CEditorDlg::~CEditorDlg()
{
	// ���̾�αװ� �����־��ٸ� �����ش� 
	if (m_hDlg)
		DestroyWindow(m_hDlg);
}

bool CEditorDlg::Init(int ID)
{
	// Modal : �θ�������� ���߰�, ������ ���̾�α׸� ���۵Ǵ� ���
	// Modeless : �θ�������� ���̾�α� ��� ���۵Ǵ� ���
	m_hDlg = CreateDialog(
		
		CGameManager::GetInst()->GetWindowInstance(),
		// �̸�
		MAKEINTRESOURCE(ID),
		// ParentWindow
		CGameManager::GetInst()->GetWindowHandle(),
		// Func 
		CEditorDlg::WndProc

	); // Modeless ���̾�α׸� ������ش� 

	/*
	> ������ instance ( hInstance )
	- �����찡 �������ִ� ������, ������ �ü������ ����Ǵ� 
	- "���α׷�"���� �����ϱ� ���� id ���� �ǹ��Ѵ� 
	> ������ handle (hwnd )
	- ������ �ü������ 
	- "������"���� �����ϱ� ���� id ��

	"���α׷�"�� "������"�� ���̴� �����ϱ� ?
	������ ���α׷��ֿ���, �ϳ��� ���α׷�����
	���� ��������� ���� �� �ִ�. 
	���α׷��� ������ �ڵ� �߿���, ��ǥ���� ���� 
	�θ� �������� �ڵ�

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
