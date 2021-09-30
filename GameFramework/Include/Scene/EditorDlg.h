#pragma once

#include"../GameInfo.h"

class CEditorDlg
{
public :
	CEditorDlg();
	~CEditorDlg();
private :
	// 얘도 window 이기 때문에 window 한개를 들고
	// 있어야 한다 
	HWND m_hDlg;
	int  m_ID;

public :
	// 이 녀석도 window
	// 메세지를 처리해야 한다 
	// 따라서 message procedure가 필요하다 
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

