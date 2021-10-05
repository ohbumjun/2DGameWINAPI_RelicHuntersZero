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
	m_SelectTextureListText{},
	m_SelectFrameIndex(-1)
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

	// TileFrameData Setting
	SetDlgItemInt(m_hDlg, IDC_EDIT_STARTFRAMEX, 0, TRUE);
	SetDlgItemInt(m_hDlg, IDC_EDIT_STARTFRAMEY, 0, TRUE);

	// Ÿ�� �ϳ��� 40 * 53 
	SetDlgItemInt(m_hDlg, IDC_EDIT_ENDFRAMEX, 40, TRUE);
	SetDlgItemInt(m_hDlg, IDC_EDIT_ENDFRAMEY, 53, TRUE);

	// �ش� ListBox�� �ڵ��� ���� ���̴�(����)
	m_TextureListBox = GetDlgItem(m_hDlg,IDC_LIST_TILETEXTURE);
	// ������
	m_FrameListBox = GetDlgItem(m_hDlg, IDC_LIST_TEXTUREFRAME);

	// ó������ ListBox�� load �� texture �� � �͵�
	// ���õǾ� ���� �ʴ�
	// -1�� �ʱ�ȭ�Ѵ�
	m_SelectTextureListIndex = -1;

	// Dialog �޾Ƴ���
	m_EditModeCombo = GetDlgItem(m_hDlg, IDC_COMBO_EDITMODE);

	TCHAR	TileEditMode[(int)ETileEditMode::End][30] =
	{
		TEXT("Ÿ�Ͽɼ�"),
		TEXT("Ÿ���̹���")
	};

	for (int i = 0; i < (int)ETileEditMode::End; ++i)
	{
		SendMessage(m_EditModeCombo, CB_ADDSTRING, 0, (LPARAM)TileEditMode[i]);
	}

	SendMessage(m_EditModeCombo, CB_SETCURSEL, 0, 0);

	m_TileEditMode = ETileEditMode::Option;


	m_TileOptionCombo = GetDlgItem(m_hDlg, IDC_COMBO_TILEOPTION);

	TCHAR	TileOptionText[(int)ETileOption::End][30] =
	{
		TEXT("Normal"),
		TEXT("Wall"),
		TEXT("Slow")
	};

	for (int i = 0; i < (int)ETileOption::End; ++i)
	{
		SendMessage(m_TileOptionCombo, CB_ADDSTRING, 0, (LPARAM)TileOptionText[i]);
	}

	SendMessage(m_TileOptionCombo, CB_SETCURSEL, 0, 0);

	m_TileOption = ETileOption::Normal;

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
		_wsplitpath_s(FilePath, nullptr, 0, nullptr, 0, FileName, 128, nullptr, 0);

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

		// TextureName ���� ��, Texture�� Load ��Ų��. 
		if (!m_Scene->GetSceneResource()->LoadTextureFullPath(TextureName,FilePath))
			return;

		// Texture�� �о�� 
		CTexture* Texture = m_Scene->GetSceneResource()->FindTexture(TextureName);
		Texture->SetColorKey(255, 0, 255);

		// �츮�� ���� ListBos ( Dialog )�� �ֱ�
		// SendMessage ��� ����� Ȱ���Ѵ� 
		// (2��° ���� ) LB_ADDSTRING : ���ڸ� �߰��ϰڴ� 
		// (4��° ���� ) FileName ����
		SendMessage(m_TextureListBox, LB_ADDSTRING,
			0,(LPARAM)FileName);
	
		TileTextureFrameData data;
		m_vecTextureFrameData.push_back(data);
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
	
		m_Scene->CreateTileMap();

		m_Scene->SetTileTexture(m_SelectTileTexture);
	
	}
}

void CEditorDlg::SelectList()
{
	// m_TextureListBox : TextureListbox�� handle ������ ����ִ�.
	// LB_GETCURSEL : idx ������
	// ������ �ָ� �����ڴ� 
	m_SelectTextureListIndex = (int)SendMessage(m_TextureListBox,
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
	
		// ���ο� Texture�� ������ ������
		// FrameListBox �ȿ� �ִ� �༮���� �α׸� ��� ���������� �Ѵ�

		// LB_GETCOUNT : �׸��� ��ΰ�
		int Count = (int)SendMessage(m_FrameListBox, LB_GETCOUNT, 0, 0);
		// �׸� ����ŭ m_FrameListBox �ȿ� ����ִ� ������� �����ش� 
		for (int i = 0; i < Count; i++)
		{
			SendMessage(m_FrameListBox,LB_DELETESTRING,0,0);
		}

		// ���ο� �׸��� FrameListBox�� ä���
		// ��, Texture�� ��ü�ϸ�, �׿� �´�
		// FrameData�� m_FrameListBox�� ä������ ���ش�
		size_t FrameBoxDataSize = m_vecTextureFrameData[m_SelectTextureListIndex].vecData.size();
		for (size_t i = 0; i < FrameBoxDataSize; ++i)
		{
			TCHAR Text[32] = {};
			wsprintf(Text, TEXT("%d"),(int)i);
			SendMessage(m_FrameListBox, LB_ADDSTRING, 0, (LPARAM)Text);
		}

		// ���ο� Texture�� �����ϸ�
		// �ش� Texture�� ���ο� FrameData �����
		// �ش� Texture�� ���ο� FrameData �����
		// ������ Frame List Box�� ������ �� ���̰�
		// ���� ������ Frame ��ȣ�� ���� ������ ����
		// m_SelectFrameIndex�� -1�� �ٽ� �ʱ�ȭ ���ش�
		m_SelectFrameIndex = -1;
	}
}

void CEditorDlg::AddFrame()
{
	// ���õ� Texture�� ���ٸ� return;
	if (m_SelectTextureListIndex == -1) return;
	
	BOOL Transfer = FALSE;

	// �Է��� ����,�� ������ ������ �����´� 
	int	StartFrameX = GetDlgItemInt(m_hDlg, IDC_EDIT_STARTFRAMEX, &Transfer, TRUE);
	int	StartFrameY = GetDlgItemInt(m_hDlg, IDC_EDIT_STARTFRAMEY, &Transfer, TRUE);
	int	EndFrameX = GetDlgItemInt(m_hDlg, IDC_EDIT_ENDFRAMEX, &Transfer, TRUE);
	int	EndFrameY = GetDlgItemInt(m_hDlg, IDC_EDIT_ENDFRAMEY, &Transfer, TRUE);

	TileFrameData Data;
	Data.Start.x = (float)StartFrameX;
	Data.Start.y = (float)StartFrameY;
	Data.End.x = (float)EndFrameX;
	Data.End.y = (float)EndFrameY;

	TCHAR Text[32] = {};
	// list�� 0,1,2,3 ���ʴ�� ���� �ȴ�
	wsprintf(Text, TEXT("%d"), (int)m_vecTextureFrameData[m_SelectTextureListIndex].vecData.size());
	SendMessage(m_FrameListBox,
		LB_ADDSTRING, 0, (LPARAM)Text);
	m_vecTextureFrameData[m_SelectTextureListIndex].vecData.push_back(Data);


}

void CEditorDlg::DeleteFrame()
{
}

void CEditorDlg::ModifyFrame()
{
}

void CEditorDlg::ChangeFrame()
{
	// m_TextureListBox : TextureListbox�� handle ������ ����ִ�.
	// LB_GETCURSEL : idx ������
	// ������ �ָ� �����ڴ� 
	m_SelectFrameIndex = (int)SendMessage(m_FrameListBox,LB_GETCURSEL,0, 0);

	if (m_SelectFrameIndex != -1)
	{
		// ������ Tile Frame Data�� ������ �´�.
		TileFrameData Data = m_vecTextureFrameData[m_SelectTextureListIndex].vecData[m_SelectFrameIndex];

		// TileFrameData Setting
		SetDlgItemInt(m_hDlg, IDC_EDIT_STARTFRAMEX, (int)Data.Start.x, TRUE);
		SetDlgItemInt(m_hDlg, IDC_EDIT_STARTFRAMEY, (int)Data.Start.y, TRUE);

		// Ÿ�� �ϳ��� 40 * 53 
		SetDlgItemInt(m_hDlg, IDC_EDIT_ENDFRAMEX, (int)Data.End.x, TRUE);
		SetDlgItemInt(m_hDlg, IDC_EDIT_ENDFRAMEY, (int)Data.End.y, TRUE);
	}

	
}

void CEditorDlg::Save()
{
	TCHAR FilePath[MAX_PATH] = {};

	// ������ �� �� ����ϴ� ����ü
	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = m_hDlg;
	// � ������ ���ϵ��� ���� ���ΰ�
	OpenFile.lpstrFilter = TEXT("�������\0*.*\0MapFile\0.map");
	// ������ ������ Ǯ ��ΰ� ���´�
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	// Map Path�� �⺻ Path�� �����ϸ�
	// �ٷ� �ش� ����� ������ �� �� �ְ� �ȴ� 
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(MAP_PATH)->Path;

	if (GetSaveFileName(&OpenFile) != 0)
	{

		char	FullPath[MAX_PATH] = {};

#ifdef UNICODE
		// �����ڵ���, ��Ƽ����Ʈ�� ��ȯ���ش�
		// �����ڵ� ���ڿ��� ��Ƽ����Ʈ ���ڿ��� ��ȯ�Ѵ�.
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0,
			FilePath, -1, nullptr, 0, 0, 0);

		// FileName�� TextureName�� ��ȯ�ؼ� �־�� 
		WideCharToMultiByte(CP_ACP, 0, FilePath, -1,
			FullPath, ConvertLength, 0, 0);
#else
		strcpy_s(FullPath, FilePath);
#endif // UNICODE
		m_Scene->Save(FullPath);
	}
}

void CEditorDlg::Load()
{
	TCHAR FilePath[MAX_PATH] = {};

	// ������ �� �� ����ϴ� ����ü
	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = m_hDlg;
	// � ������ ���ϵ��� ���� ���ΰ�
	OpenFile.lpstrFilter = TEXT("�������\0*.*\0MapFile\0.map");
	// ������ ������ Ǯ ��ΰ� ���´�
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	// Map Path�� �⺻ Path�� �����ϸ�
	// �ٷ� �ش� ����� ������ �� �� �ְ� �ȴ� 
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(MAP_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{

		char	FullPath[MAX_PATH] = {};

#ifdef UNICODE
		// �����ڵ���, ��Ƽ����Ʈ�� ��ȯ���ش�
		// �����ڵ� ���ڿ��� ��Ƽ����Ʈ ���ڿ��� ��ȯ�Ѵ�.
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0,
			FilePath, -1, nullptr, 0, 0, 0);

		// FileName�� TextureName�� ��ȯ�ؼ� �־�� 
		WideCharToMultiByte(CP_ACP, 0, FilePath, -1,
			FullPath, ConvertLength, 0, 0);
#else
		strcpy_s(FullPath, FilePath);
#endif // UNICODE
		m_Scene->Load(FullPath);
	}
}

void CEditorDlg::TileImageAllClear()
{
	// ��ü tile ������ŭ �ݺ�
	m_Scene->TileImageAllClear();
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
		case IDC_LIST_TEXTUREFRAME : // FrameListBox 
			// cell�� �ٲ𶧸��� ���´� 
			switch (HIWORD(wParam))
			{
				// cell�� �ٲ𶧸��� ���´� 
			case LBN_SELCHANGE:
			{
				g_Dlg->ChangeFrame();
			}
			break;
			}
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
			break;
		// ������ �߰� �ϱ�
		case IDC_BUTTON_ADDFRAME:
			g_Dlg->AddFrame();
			break;
		// ������ ���� �ϱ�
		case IDC_BUTTON_DELETEFRAME:
			g_Dlg->DeleteFrame();
			break;
		// ������ ���� �ϱ�
		case IDC_BUTTON_MODIFYFRAME:
			g_Dlg->ModifyFrame();
			break;
		// Map ���� �ϱ�
		case IDC_BUTTON_SAVE:
			g_Dlg->Save();
			break;
		// Map Load �ϱ�
		case IDC_BUTTON_LOAD:
			g_Dlg->Load();
			break;// Map Load �ϱ�
		case IDC_BUTTON_IMAGECLEAR:
			g_Dlg->TileImageAllClear();
			break;
		default:
			break;
		}
	default:
		break;
	}

	return 0;
}
