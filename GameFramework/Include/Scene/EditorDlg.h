#pragma once

#include"../GameInfo.h"

class CEditorDlg
{
public :
	CEditorDlg();
	~CEditorDlg();
private :
	// �굵 window �̱� ������ window �Ѱ��� ���
	// �־�� �Ѵ� 
	HWND m_hDlg;
	int  m_ID;

public :
	// �� �༮�� window
	// �޼����� ó���ؾ� �Ѵ� 
	// ���� message procedure�� �ʿ��ϴ� 
	bool Init(int ID);
	void Show();
	
private :
	// WindowProc is a user-defined callback function 
	// that processes messages sent to a window
	// This function is specified when an application
	// registers its window class
	// and can be named anything ( not necessarily WindowProc )
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

