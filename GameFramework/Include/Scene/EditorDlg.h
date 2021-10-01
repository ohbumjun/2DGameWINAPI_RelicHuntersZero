#pragma once

#include"../GameInfo.h"
#include "../Resource/Texture.h"

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
	bool m_Open;
	// �ڱⰡ �����ִ� scene�� ������� �˰� �ְ� �ϱ�
	class CEditorScene* m_Scene;

	// Button, ListBos �� �ϳ��ϳ��� �� window �̴�
	// ��, �������� window handle�� ���´�
	// ����, �ش� window handle���� ����
	// dialog �� ������ ��Ҹ� constrol�� �� �ִ�.
	HWND  m_TextureListBox;
	int   m_SelectTextureListIndex;
	TCHAR m_SelectTextureListText[256];

	// ������ Texture ����
	CSharedPtr<CTexture> m_SelectTileTexture;

	// ComboBox�� �ڵ�
	// Dialog ���� �� ��ҵ鵵 Window !
	HWND m_EditModeCombo;

	// ���� � Edit�� ���� Mode �� ���õǾ� �ִ���
	ETileEditMode m_TileEditMode;

public :
	ETileEditMode GetTileEditMode() const
	{
		return m_TileEditMode;
	}

public :
	bool IsOpen() const
	{
		return m_Open;
	}
	RECT GetRect() const 
	{
		RECT rc;
		// Dialog ũ�⸸ŭ�� RECT �� ���´� 
		GetWindowRect(m_hDlg, &rc);
		return rc;
	}
	void SetScene(class CEditorScene* Scene)
	{
		m_Scene = Scene;
	}

public :
	// �� �༮�� window
	// �޼����� ó���ؾ� �Ѵ� 
	// ���� message procedure�� �ʿ��ϴ� 
	bool Init(int ID);
	void Show();
	void CreateMap();
	void LoadTileTexture();
	void SelectTexture();
	// ListBox���� ������ ���� �����ص� ���̴�
	void SelectList();
	
private :
	// WindowProc is a user-defined callback function 
	// that processes messages sent to a window
	// This function is specified when an application
	// registers its window class
	// and can be named anything ( not necessarily WindowProc )
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

