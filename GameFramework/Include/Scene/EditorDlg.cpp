#include "EditorDlg.h"
#include "../GameManager.h"
#include "../PathManager.h"
#include "EditorScene.h"
#include "Scene.h"
#include "SceneResource.h"
#include "../Resource/Texture.h"

// static ��� �Լ��� WndProc �� ȣ���ϱ� ����
// ���� ������ �����صд� 
CEditorDlg* g_Dlg;

CEditorDlg::CEditorDlg() :
	m_ID(0),
	m_hDlg(0),
	m_Open(false),
	m_Scene(nullptr),
	m_SelectTextureListText{}
{
	g_Dlg = this;
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

	// ���̾�α� input(editor�Է�â)��
	// ���� ���� ���� �մ�
	// default�� �������� 
	SetDlgItemInt(m_hDlg,IDC_EDIT_TILECOUNTX,40,TRUE);
	SetDlgItemInt(m_hDlg,IDC_EDIT_TILECOUNTY,40,TRUE);
	
	// Ÿ�� �ϳ��� 40 * 53 
	SetDlgItemInt(m_hDlg,IDC_EDIT_TILESIZEX,40,TRUE);
	SetDlgItemInt(m_hDlg,IDC_EDIT_TILESIZEY,53,TRUE);

	// �ش� ListBox�� �ڵ��� ���� ���̴�
	m_TextureListBox = GetDlgItem(m_hDlg,IDC_LIST_TILETEXTURE);

	// ó������ ListBox�� load �� texture �� � �͵�
	// ���õǾ� ���� �ʴ�
	// -1�� �ʱ�ȭ�Ѵ�
	m_SelectTextureListIndex = -1;
	
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

void CEditorDlg::CreateMap()
{
	// Dialog�� edit control �� �Է��� ������ ���´�
	BOOL Transfer = FALSE;

	// 4��° ���� : ��ȣ�� �ִ��� 
	int	TileCountX = GetDlgItemInt(m_hDlg, IDC_EDIT_TILECOUNTX, &Transfer, TRUE);
	int	TileCountY = GetDlgItemInt(m_hDlg, IDC_EDIT_TILECOUNTY, &Transfer, TRUE);
	int	TileSizeX = GetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEX, &Transfer, TRUE);
	int	TileSizeY = GetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEY, &Transfer, TRUE);

	m_Scene->CreateTileMap();

	// Tile ������ ���Ӱ� ������ 
	m_Scene->SetTileInfo(
		TileCountX, TileCountY,TileSizeX,TileSizeY);

}

void CEditorDlg::LoadTileTexture()
{
	TCHAR FilePath[MAX_PATH] = {};

	// ������ �� �� ����ϴ� ����ü
	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = m_hDlg;
	// � ������ ���ϵ��� ���� ���ΰ�
	OpenFile.lpstrFilter = TEXT("�������\0*.*\0BmpFile\0.bmp");
	// ������ ������ Ǯ ��ΰ� ���´�
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	// Texture Path�� �⺻ Path�� �����ϸ�
	// �ٷ� �ش� ����� ������ �� �� �ְ� �ȴ� 
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;

	// ���� ������ "Ȯ��"�� �����ٸ� 
	if (GetOpenFileName(&OpenFile) != 0)
	{
		// ���� �̸��� �̾ƿ���
		TCHAR FileName[128] = {};

		// ������ OpenFile�� ���� �����ؼ� ���� ������������, �̸��� �������� �ڵ� 
		// FileNAme�̶�� ������ ������ �ִ´� 
		_wsplitpath_s(FilePath, nullptr, 0, nullptr, 0, FileName, 128,
			nullptr, 0);

		char	TextureName[256] = {};

#ifdef UNICODE
		// �����ڵ���, ��Ƽ����Ʈ�� ��ȯ���ش�
		// �����ڵ� ���ڿ��� ��Ƽ����Ʈ ���ڿ��� ��ȯ�Ѵ�.
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, 
			FileName, -1, nullptr, 0, 0, 0);

		// FileName�� TextureName�� ��ȯ�ؼ� �־�� 
		WideCharToMultiByte(CP_ACP, 0, FileName, -1,
			TextureName, ConvertLength, 0, 0);
#else
		strcpy_s(TextureName, FileName);
#endif // UNICODE

		// TextureName ���� ��, Texture�� Load ��Ų�� 
		if (!m_Scene->GetSceneResource()->LoadTextureFullPath(TextureName,FilePath))
			return;

		// Texture�� �о�� 
		CTexture* Texture = m_Scene->GetSceneResource()->FindTexture(TextureName);
		
		// �츮�� ���� ListBos ( Dialog )�� �ֱ�
		// SendMessage ��� ����� Ȱ���Ѵ� 
		// (2��° ���� ) LB_ADDSTRING : ���ڸ� �߰��ϰڴ� 
		// (4��° ���� ) FileName ����
		SendMessage(m_TextureListBox, LB_ADDSTRING,
			0,(LPARAM)FileName);
	
	}

}

void CEditorDlg::SelectTexture()
{
	// m_SelectTextureListText : Load�� ��� �߿���
	// ������ texture�� �̸��� �� �ִ�
	// �׷���, TCHAR ���·� �� �ִ� 

	// ���õ� Texture�� ���� ����
	if (m_SelectTextureListIndex != -1)
	{
		char TextureName[256] = {};

		// �����ڵ���, ��Ƽ����Ʈ�� ��ȯ���ش�
#ifdef UNICODE
// �����ڵ� ���ڿ��� ��Ƽ����Ʈ ���ڿ��� ��ȯ�Ѵ�.
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, m_SelectTextureListText, -1, nullptr, 0, 0, 0);

		// FileName�� TextureName�� ��ȯ�ؼ� �־�� 
		WideCharToMultiByte(CP_ACP, 0, m_SelectTextureListText, -1,
			TextureName, ConvertLength, 0, 0);
#else
		strcpy_s(TextureName, FileName);
#endif // UNICODE

		// Load �� Texture ��ϵ� �߿��� 
		// TextureName�� �ش��ϴ� Texture�� �����´� 
		m_SelectTileTexture = m_Scene->GetSceneResource()->FindTexture(TextureName);
	
		//
		m_Scene->CreateTileMap();
		m_Scene->SetTileTexture(m_SelectTileTexture);
	
	}
}

void CEditorDlg::SelectList()
{
	// m_TextureListBox : Listbox�� handle ������ ����ִ�.
	// LB_GETCURSEL : idx ������
	// ������ �ָ� �����ڴ� 
	m_SelectTextureListIndex = SendMessage(m_TextureListBox,
		LB_GETCURSEL,
		0,0
		);

	if (m_SelectTextureListIndex != -1)
	{
		// ������ �̸��� �̸� ���´� 
		// 1) �켱 ���� 0���� �ʱ�ȭ �Ѵ�
		memset(m_SelectTextureListText,
			0, sizeof(TCHAR)*256);
	
		// m_SelectTextureListText ��
		// �̸��� ���´� 
		SendMessage(m_TextureListBox, LB_GETTEXT, m_SelectTextureListIndex,
			(LPARAM)m_SelectTextureListText);
	
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

	case WM_COMMAND :
		// ���� 16bit�� �̾ƿ´�  
		switch (LOWORD(wParam))
		{
		case IDC_LIST_TILETEXTURE :
			// wparm�� high word�� ���´�
			// ���� 16bit
			switch (HIWORD(wParam))
			{
			// cell�� �ٲ𶧸��� ���´� 
			case LBN_SELCHANGE :
			{
				g_Dlg->SelectList();
			}
				break;
			}
			break;
		case IDOK :
			break;
		case IDCANCEL:
			break;
		case IDC_BUTTON_CREATEMAP :
			// �̶� map �������ش� 
			// MessageBox(0, TEXT("�� ����"), TEXT("�� ����"), MB_OK);
			g_Dlg->CreateMap();
			break;
		// ���� �ҷ����� 
		case IDC_BUTTON_TILETEXTURELOAD:
			g_Dlg->LoadTileTexture();
			break;
		// �ҷ��� ���� �� �����ϱ�
		case IDC_BUTTON_SETTEXTURE:
			g_Dlg->SelectTexture();
		default:
			break;
		}
	default:
		break;
	}

	return 0;
}
