#include "EditorDlg.h"
#include "../GameManager.h"

CEditorDlg::CEditorDlg() :
	m_ID(0),
	m_hDlg(0),
	m_Open(false)
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


	// ���̾�α׸� ������ �ϱ�
	ShowWindow(m_hDlg, SW_SHOW);
	
	return true;
}

void CEditorDlg::Show()
{
	if (m_Open)
	{
		m_Open = false;
		ShowWindow(m_hDlg, SW_HIDE);
	}
	else
	{
		m_Open = true;
		ShowWindow(m_hDlg, SW_SHOW);
	}
}


LRESULT CEditorDlg::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE :
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY : // �����츦 �����ٸ� �ƿ� �����Ű��
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return 0;
}
