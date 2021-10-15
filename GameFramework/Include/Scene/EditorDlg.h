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
	// 얘도 window 이기 때문에 window 한개를 들고
	// 있어야 한다 
	HWND m_hDlg;
	int  m_ID;
	bool m_Open;
	// 자기가 열려있는 scene이 어디인지 알고 있게 하기
	class CEditorScene* m_Scene;

	// Button, ListBos 등 하나하나가 다 window 이다
	// 즉, 각각마다 window handle이 나온다
	// 따라서, 해당 window handle들을 통해
	// dialog 내 각각의 요소를 constrol할 수 있다.
	HWND  m_TextureListBox;
	int   m_SelectTextureListIndex;
	TCHAR m_SelectTextureListText[256];
	// 선택한 Texture 정보
	CSharedPtr<CTexture> m_SelectTileTexture;
	// Texture가 추가될때마다, 얘가 하나씩 추가된다
	std::vector<TileTextureFrameData> m_vecTextureFrameData;


	// ComboBox용 핸들
	// Dialog 안의 각 요소들도 Window !
	HWND m_EditModeCombo;
	// 현재 어떤 Edit의 편집 Mode 가 선택되어 있는지
	ETileEditMode m_TileEditMode;

	HWND m_TileOptionCombo;
	ETileOption m_TileOption;

	HWND m_MonsterOptionCombo;
	EMonsterOption m_MonsterOption;

	// FrameData 목록이 들어있는 ListBox
	HWND m_FrameListBox;
	// 현재 선택한 Frame 번호
	int m_SelectFrameIndex;

	bool m_SideCollision;


public :
	bool GetSideCollision() const
	{
		return m_SideCollision;
	}
	ETileOption GetTileOption() const
	{
		// 선택한 idx를 return 해준다 
		return (ETileOption)SendMessage(m_TileOptionCombo, CB_GETCURSEL, 0, 0);
	}
	EMonsterOption GetMonsterOption() const
	{
		return (EMonsterOption)SendMessage(m_MonsterOptionCombo, CB_GETCURSEL, 0, 0);
	}
	TileFrameData GetTileFrameData()	const
	{
		// Texture가 선택되지 않았거나, 
		// 선택한 Texture의 Frame을 선택하지 않았다면
		if (m_SelectTextureListIndex == -1 ||
			m_SelectFrameIndex == -1)
			return TileFrameData(); // Default 생성자로 0,0 return;
		return m_vecTextureFrameData[m_SelectTextureListIndex].vecData[m_SelectFrameIndex];
	}
	ETileEditMode GetTileEditMode() const
	{
		// 현재 Dialogue 창에서 선택된 것을 바로 return 시킨다.
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
		// Dialog 크기만큼의 RECT 를 얻어온다 
		GetWindowRect(m_hDlg, &rc);
		return rc;
	}
	void SetScene(class CEditorScene* Scene)
	{
		m_Scene = Scene;
	}

public :
	// 이 녀석도 window
	// 메세지를 처리해야 한다 
	// 따라서 message procedure가 필요하다 
	bool Init(int ID);
	void Show();
	void CreateMap();
	void LoadTileTexture();
	void SelectTexture();
	void SetAllTexture();
	// ListBox에서 선택한 것을 저장해둘 것이다
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

