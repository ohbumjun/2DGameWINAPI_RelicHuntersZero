#include "EditorDlg.h"
#include "../GameManager.h"
#include "../PathManager.h"
#include "EditorScene.h"
#include "Scene.h"
#include "SceneResource.h"
#include "../Resource/Texture.h"

// static 멤버 함수인 WndProc 를 호출하기 위해
// 전역 변수를 세팅해둔다 
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


	// 다이얼로그를 나오게 하기
	ShowWindow(m_hDlg, SW_SHOW);

	// 다이얼로그 input(editor입력창)에
	// 값을 넣을 수도 잇다
	// default를 만들어놓자 
	SetDlgItemInt(m_hDlg,IDC_EDIT_TILECOUNTX,40,TRUE);
	SetDlgItemInt(m_hDlg,IDC_EDIT_TILECOUNTY,40,TRUE);
	
	// 타일 하나당 40 * 53 
	SetDlgItemInt(m_hDlg,IDC_EDIT_TILESIZEX,40,TRUE);
	SetDlgItemInt(m_hDlg,IDC_EDIT_TILESIZEY,53,TRUE);

	// TileFrameData Setting
	SetDlgItemInt(m_hDlg, IDC_EDIT_STARTFRAMEX, 0, TRUE);
	SetDlgItemInt(m_hDlg, IDC_EDIT_STARTFRAMEY, 0, TRUE);

	// 타일 하나당 40 * 53 
	SetDlgItemInt(m_hDlg, IDC_EDIT_ENDFRAMEX, 40, TRUE);
	SetDlgItemInt(m_hDlg, IDC_EDIT_ENDFRAMEY, 53, TRUE);

	// 해당 ListBox의 핸들을 얻어온 것이다(왼쪽)
	m_TextureListBox = GetDlgItem(m_hDlg,IDC_LIST_TILETEXTURE);
	// 오른쪽
	m_FrameListBox = GetDlgItem(m_hDlg, IDC_LIST_TEXTUREFRAME);

	// 처음에는 ListBox에 load 된 texture 중 어떤 것도
	// 선택되어 있지 않다
	// -1로 초기화한다
	m_SelectTextureListIndex = -1;

	// Dialog 받아놓기
	m_EditModeCombo = GetDlgItem(m_hDlg, IDC_COMBO_EDITMODE);

	TCHAR	TileEditMode[(int)ETileEditMode::End][30] =
	{
		TEXT("타일옵션"),
		TEXT("타일이미지")
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
	// Dialog의 edit control 에 입력한 내용을 얻어온다
	BOOL Transfer = FALSE;

	// 4번째 인자 : 부호가 있느냐 
	int	TileCountX = GetDlgItemInt(m_hDlg, IDC_EDIT_TILECOUNTX, &Transfer, TRUE);
	int	TileCountY = GetDlgItemInt(m_hDlg, IDC_EDIT_TILECOUNTY, &Transfer, TRUE);
	int	TileSizeX = GetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEX, &Transfer, TRUE);
	int	TileSizeY = GetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEY, &Transfer, TRUE);

	m_Scene->CreateTileMap();

	// Tile 정보를 새롭게 만들어낸다 
	m_Scene->SetTileInfo(
		TileCountX, TileCountY,TileSizeX,TileSizeY);
}

void CEditorDlg::LoadTileTexture()
{
	TCHAR FilePath[MAX_PATH] = {};

	// 파일을 열 때 사용하는 구조체
	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = m_hDlg;

	// 어떤 형식의 파일들을 읽을 것인가
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0BmpFile\0.bmp");

	// 열려는 파일의 풀 경로가 들어온다
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;

	// Texture Path를 기본 Path로 세팅하면
	// 바로 해당 경로의 파일을 열 수 있게 된다 
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;

	// 파일 선택후 "확인"을 눌렀다면 
	if (GetOpenFileName(&OpenFile) != 0)
	{
		// 파일 이름만 뽑아오기
		TCHAR FileName[128] = {};

		// 위에서 OpenFile을 통해 선택해서 얻어온 파일정보에서, 이름만 빼내오는 코드 
		// FileNAme이라는 변수에 정보를 넣는다 
		_wsplitpath_s(FilePath, nullptr, 0, nullptr, 0, FileName, 128, nullptr, 0);

		char	TextureName[256] = {};
#ifdef UNICODE
		// 유니코드라면, 멀티바이트로 변환해준다
		// 유니코드 문자열을 멀티바이트 문자열로 변환한다.
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, 
			FileName, -1, nullptr, 0, 0, 0);

		// FileName을 TextureName에 변환해서 넣어라 
		WideCharToMultiByte(CP_ACP, 0, FileName, -1,
			TextureName, ConvertLength, 0, 0);
#else
		strcpy_s(TextureName, FileName);
#endif // UNICODE

		// TextureName 으로 된, Texture를 Load 시킨다. 
		if (!m_Scene->GetSceneResource()->LoadTextureFullPath(TextureName,FilePath))
			return;

		// Texture를 읽어옴 
		CTexture* Texture = m_Scene->GetSceneResource()->FindTexture(TextureName);
		Texture->SetColorKey(255, 0, 255);

		// 우리가 만든 ListBos ( Dialog )에 넣기
		// SendMessage 라는 방식을 활용한다 
		// (2번째 인자 ) LB_ADDSTRING : 문자를 추가하겠다 
		// (4번째 인자 ) FileName 전달
		SendMessage(m_TextureListBox, LB_ADDSTRING,
			0,(LPARAM)FileName);
	
		TileTextureFrameData data;
		m_vecTextureFrameData.push_back(data);
	}

}

void CEditorDlg::SelectTexture()
{
	// m_SelectTextureListText : Load한 목록 중에서
	// 선택한 texture의 이름이 들어가 있다
	// 그런데, TCHAR 형태로 들어가 있다 

	// 선택된 Texture가 있을 때만
	if (m_SelectTextureListIndex != -1)
	{
		char TextureName[256] = {};

		// 유니코드라면, 멀티바이트로 변환해준다
#ifdef UNICODE
// 유니코드 문자열을 멀티바이트 문자열로 변환한다.
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, m_SelectTextureListText, -1, nullptr, 0, 0, 0);

		// FileName을 TextureName에 변환해서 넣어라 
		WideCharToMultiByte(CP_ACP, 0, m_SelectTextureListText, -1,
			TextureName, ConvertLength, 0, 0);
#else
		strcpy_s(TextureName, FileName);
#endif // UNICODE

		// Load 된 Texture 목록들 중에서 
		// TextureName에 해당하는 Texture를 가져온다 
		m_SelectTileTexture = m_Scene->GetSceneResource()->FindTexture(TextureName);
	
		m_Scene->CreateTileMap();

		m_Scene->SetTileTexture(m_SelectTileTexture);
	
	}
}

void CEditorDlg::SelectList()
{
	// m_TextureListBox : TextureListbox의 handle 정보가 들어있다.
	// LB_GETCURSEL : idx 얻어오기
	// 선택한 애를 얻어오겠다 
	m_SelectTextureListIndex = (int)SendMessage(m_TextureListBox,
		LB_GETCURSEL,
		0,0
		);

	if (m_SelectTextureListIndex != -1)
	{
		// 선택한 이름을 미리 얻어온다 
		// 1) 우선 전부 0으로 초기화 한다
		memset(m_SelectTextureListText,
			0, sizeof(TCHAR)*256);
	
		// m_SelectTextureListText 에
		// 이름을 얻어온다 
		SendMessage(m_TextureListBox, LB_GETTEXT, m_SelectTextureListIndex,
			(LPARAM)m_SelectTextureListText);
	
		// 새로운 Texture를 선택할 때마다
		// FrameListBox 안에 있는 녀석들을 싸그리 모두 지워버려야 한다

		// LB_GETCOUNT : 항목이 몇개인가
		int Count = (int)SendMessage(m_FrameListBox, LB_GETCOUNT, 0, 0);
		// 항목 수만큼 m_FrameListBox 안에 들어있는 내용들을 지워준다 
		for (int i = 0; i < Count; i++)
		{
			SendMessage(m_FrameListBox,LB_DELETESTRING,0,0);
		}

		// 새로운 항목들로 FrameListBox를 채운다
		// 즉, Texture를 교체하면, 그에 맞는
		// FrameData가 m_FrameListBox에 채워지게 해준다
		size_t FrameBoxDataSize = m_vecTextureFrameData[m_SelectTextureListIndex].vecData.size();
		for (size_t i = 0; i < FrameBoxDataSize; ++i)
		{
			TCHAR Text[32] = {};
			wsprintf(Text, TEXT("%d"),(int)i);
			SendMessage(m_FrameListBox, LB_ADDSTRING, 0, (LPARAM)Text);
		}

		// 새로운 Texture를 선택하면
		// 해당 Texture의 새로운 FrameData 목록이
		// 해당 Texture의 새로운 FrameData 목록이
		// 오른쪽 Frame List Box에 들어오게 될 것이고
		// 현재 선택한 Frame 번호에 대한 정보를 담은
		// m_SelectFrameIndex는 -1로 다시 초기화 해준다
		m_SelectFrameIndex = -1;
	}
}

void CEditorDlg::AddFrame()
{
	// 선택된 Texture가 없다면 return;
	if (m_SelectTextureListIndex == -1) return;
	
	BOOL Transfer = FALSE;

	// 입력한 시작,끝 프레임 정보를 가져온다 
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
	// list에 0,1,2,3 차례대로 들어가게 된다
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
	// m_TextureListBox : TextureListbox의 handle 정보가 들어있다.
	// LB_GETCURSEL : idx 얻어오기
	// 선택한 애를 얻어오겠다 
	m_SelectFrameIndex = (int)SendMessage(m_FrameListBox,LB_GETCURSEL,0, 0);

	if (m_SelectFrameIndex != -1)
	{
		// 선택한 Tile Frame Data를 가지고 온다.
		TileFrameData Data = m_vecTextureFrameData[m_SelectTextureListIndex].vecData[m_SelectFrameIndex];

		// TileFrameData Setting
		SetDlgItemInt(m_hDlg, IDC_EDIT_STARTFRAMEX, (int)Data.Start.x, TRUE);
		SetDlgItemInt(m_hDlg, IDC_EDIT_STARTFRAMEY, (int)Data.Start.y, TRUE);

		// 타일 하나당 40 * 53 
		SetDlgItemInt(m_hDlg, IDC_EDIT_ENDFRAMEX, (int)Data.End.x, TRUE);
		SetDlgItemInt(m_hDlg, IDC_EDIT_ENDFRAMEY, (int)Data.End.y, TRUE);
	}

	
}

void CEditorDlg::Save()
{
	TCHAR FilePath[MAX_PATH] = {};

	// 파일을 열 때 사용하는 구조체
	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = m_hDlg;
	// 어떤 형식의 파일들을 읽을 것인가
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0MapFile\0.map");
	// 열려는 파일의 풀 경로가 들어온다
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	// Map Path를 기본 Path로 세팅하면
	// 바로 해당 경로의 파일을 열 수 있게 된다 
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(MAP_PATH)->Path;

	if (GetSaveFileName(&OpenFile) != 0)
	{

		char	FullPath[MAX_PATH] = {};

#ifdef UNICODE
		// 유니코드라면, 멀티바이트로 변환해준다
		// 유니코드 문자열을 멀티바이트 문자열로 변환한다.
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0,
			FilePath, -1, nullptr, 0, 0, 0);

		// FileName을 TextureName에 변환해서 넣어라 
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

	// 파일을 열 때 사용하는 구조체
	OPENFILENAME OpenFile = {};
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = m_hDlg;
	// 어떤 형식의 파일들을 읽을 것인가
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0MapFile\0.map");
	// 열려는 파일의 풀 경로가 들어온다
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	// Map Path를 기본 Path로 세팅하면
	// 바로 해당 경로의 파일을 열 수 있게 된다 
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(MAP_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{

		char	FullPath[MAX_PATH] = {};

#ifdef UNICODE
		// 유니코드라면, 멀티바이트로 변환해준다
		// 유니코드 문자열을 멀티바이트 문자열로 변환한다.
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0,
			FilePath, -1, nullptr, 0, 0, 0);

		// FileName을 TextureName에 변환해서 넣어라 
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
	// 전체 tile 개수만큼 반복
	m_Scene->TileImageAllClear();
}

LRESULT CEditorDlg::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE :
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY : // 윈도우를 지웠다면 아에 종료시키기
		PostQuitMessage(0);
		break;

	case WM_COMMAND :
		// 하위 16bit를 뽑아온다  
		switch (LOWORD(wParam))
		{
		case IDC_LIST_TILETEXTURE :
			// wparm이 high word로 들어온다
			// 상위 16bit
			switch (HIWORD(wParam))
			{
			// cell이 바뀔때마다 들어온다 
			case LBN_SELCHANGE :
			{
				g_Dlg->SelectList();
			}
				break;
			}
			break;
		case IDC_LIST_TEXTUREFRAME : // FrameListBox 
			// cell이 바뀔때마다 들어온다 
			switch (HIWORD(wParam))
			{
				// cell이 바뀔때마다 들어온다 
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
			// 이때 map 생성해준다 
			// MessageBox(0, TEXT("맵 생성"), TEXT("맵 생성"), MB_OK);
			g_Dlg->CreateMap();
			break;
		// 파일 불러오기 
		case IDC_BUTTON_TILETEXTURELOAD:
			g_Dlg->LoadTileTexture();
			break;
		// 불러온 파일 중 선택하기
		case IDC_BUTTON_SETTEXTURE:
			g_Dlg->SelectTexture();
			break;
		// 프레임 추가 하기
		case IDC_BUTTON_ADDFRAME:
			g_Dlg->AddFrame();
			break;
		// 프레임 삭제 하기
		case IDC_BUTTON_DELETEFRAME:
			g_Dlg->DeleteFrame();
			break;
		// 프레임 수정 하기
		case IDC_BUTTON_MODIFYFRAME:
			g_Dlg->ModifyFrame();
			break;
		// Map 저장 하기
		case IDC_BUTTON_SAVE:
			g_Dlg->Save();
			break;
		// Map Load 하기
		case IDC_BUTTON_LOAD:
			g_Dlg->Load();
			break;// Map Load 하기
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
