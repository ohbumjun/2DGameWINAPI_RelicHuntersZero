#pragma once

#include"../GameInfo.h"
#include "../Resource/Texture.h"

struct TileFrameData
{
	Vector2 Start;
	Vector2 End;
};

struct TileTextureFrameData
{
	std::vector<TileFrameData> vecData;
};

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
	// Texture�� �߰��ɶ�����, �갡 �ϳ��� �߰��ȴ�
	std::vector<TileTextureFrameData> m_vecTextureFrameData;


	// ComboBox�� �ڵ�
	// Dialog ���� �� ��ҵ鵵 Window !
	HWND m_EditModeCombo;
	// ���� � Edit�� ���� Mode �� ���õǾ� �ִ���
	ETileEditMode m_TileEditMode;

	HWND m_TileOptionCombo;
	ETileOption m_TileOption;

	HWND m_MonsterOptionCombo;
	EMonsterOption m_MonsterOption;

	// FrameData ����� ����ִ� ListBox
	HWND m_FrameListBox;
	// ���� ������ Frame ��ȣ
	int m_SelectFrameIndex;

	bool m_SideCollision;


public :
	bool GetSideCollision() const
	{
		return m_SideCollision;
	}
	ETileOption GetTileOption() const
	{
		// ������ idx�� return ���ش� 
		return (ETileOption)SendMessage(m_TileOptionCombo, CB_GETCURSEL, 0, 0);
	}
	EMonsterOption GetMonsterOption() const
	{
		return (EMonsterOption)SendMessage(m_MonsterOptionCombo, CB_GETCURSEL, 0, 0);
	}
	TileFrameData GetTileFrameData()	const
	{
		// Texture�� ���õ��� �ʾҰų�, 
		// ������ Texture�� Frame�� �������� �ʾҴٸ�
		if (m_SelectTextureListIndex == -1 ||
			m_SelectFrameIndex == -1)
			return TileFrameData(); // Default �����ڷ� 0,0 return;
		return m_vecTextureFrameData[m_SelectTextureListIndex].vecData[m_SelectFrameIndex];
	}
	ETileEditMode GetTileEditMode() const
	{
		// ���� Dialogue â���� ���õ� ���� �ٷ� return ��Ų��.
		return (ETileEditMode)SendMessage(m_EditModeCombo, CB_GETCURSEL, 0, 0);
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
	void SetAllTexture();
	// ListBox���� ������ ���� �����ص� ���̴�
	void SelectList();

	void AddFrame();
	void DeleteFrame();
	void ModifyFrame();
	void ChangeFrame();

	void Save();
	void Load();
	void TileImageAllClear();

	HWND m_SideCollisionCheckHandle;
	void SideCollisionCheck();

	// Image Setting
	HBITMAP hLogoImage;
	HBITMAP hGenerateImage;
	void LoadImages();

private :
	// WindowProc is a user-defined callback function 
	// that processes messages sent to a window
	// This function is specified when an application
	// registers its window class
	// and can be named anything ( not necessarily WindowProc )
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

