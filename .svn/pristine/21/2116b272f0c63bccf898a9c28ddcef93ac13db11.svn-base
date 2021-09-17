
#include "GameManager.h"
#include "Timer.h"
#include "Scene/SceneManager.h"
#include "Scene/MainScene.h"
#include "Scene/StartScene.h"
#include "Input.h"
#include "PathManager.h"
#include "Resource/ResourceManager.h"
#include "Collision/CollisionManager.h"

CGameManager* CGameManager::m_Inst = nullptr;
bool CGameManager::m_Loop = true;

CGameManager::CGameManager()	:
	m_Timer(nullptr),
	m_TimeScale(1.f)
{
}

CGameManager::~CGameManager()
{
	CSceneManager::DestroyInst();

	CInput::DestroyInst();

	CCollisionManager::DestroyInst();

	CResourceManager::DestroyInst();
	CPathManager::DestroyInst();

	SAFE_DELETE(m_Timer);

	SelectObject(m_hBackDC, m_hPrevBackBmp);

	DeleteObject(m_hBackBmp);

	DeleteDC(m_hBackDC);

	DeleteObject(m_hGreenBrush);
	DeleteObject(m_hRedBrush);
	DeleteObject(m_hGreenPen);
	DeleteObject(m_hRedPen);

	// GetDC를 이용해서 생성한 DC는 반드시 ReleaseDC를 해주어야 한다.
	ReleaseDC(m_hWnd, m_hDC);
}

float CGameManager::GetDeltaTime() const
{
	return m_Timer->GetDeltaTime() * m_TimeScale;
}

void CGameManager::Exit()
{
	DestroyWindow(m_hWnd);
}

bool CGameManager::Init(HINSTANCE hInst)
{
	m_hInst = hInst;

	m_RS.Width = 1280;
	m_RS.Height = 720;

	Register();
	Create();

	// DC를 생성한다.
	m_hDC = GetDC(m_hWnd);

	// 경로관리자 초기화
	if (!CPathManager::GetInst()->Init())
		return false;

	// 리소스 관리자 초기화
	if (!CResourceManager::GetInst()->Init())
		return false;

	// 충돌 관리자 초기화
	if (!CCollisionManager::GetInst()->Init())
		return false;

	// 입력관리자 초기화
	if (!CInput::GetInst()->Init(m_hWnd))
		return false;

	// 장면관리자 초기화
	if (!CSceneManager::GetInst()->Init())
		return false;

	CSceneManager::GetInst()->CreateScene<CStartScene>();

	// 타이머를 생성한다.
	m_Timer = new CTimer;

	// 백버퍼 생성
	m_hBackDC = CreateCompatibleDC(m_hDC);

	m_hBackBmp = CreateCompatibleBitmap(m_hDC, m_RS.Width, m_RS.Height);

	m_hPrevBackBmp = (HBITMAP)SelectObject(m_hBackDC, m_hBackBmp);

	m_hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));
	m_hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
	m_hGreenPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_hRedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

	return true;
}

int CGameManager::Run()
{
	MSG msg;

	while (m_Loop)
	{
		// GetMessage는 메세지가 없을 경우 다른일을 할 수 없다.
		// 메세지가 올때까지 대기하고 있는 시간을 윈도우의 데드타임이라고 한다.
		// 실제로 메세지가 있는 시간보다 없는 시간이 훨씬 길다.
		// 그래서 게임은 윈도우의 데드타임동안 게임이 동작될 수 있게 제작한다.
		// PeekMessage는 메세지큐에서 메세지를 얻어온다.
		// 그런데 만약 메세지가 없다면 false를 리턴하면서 바로 빠져나오고
		// 메세지가 있다면 true를 리턴하면서 해당 메세지를 꺼내온다.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// WM_KEYDOWN 이라는 메세지가 있다. 이는 키보드 키를 눌렀을때 발생되는 메세지
			// 이다. TranslateMessage 함수에서는 메세지를 전달해주면 WM_KEYDOWN 인지를
			// 판단해주고 눌려진 키가 문자 키인지를 판단해서 일반 무자 키라면 WM_CHAR라는
			// 메시지를 만들어서 메세지 큐에 추가해주게 된다.
			TranslateMessage(&msg);

			// DispatchMessage 함수는 메세지를 윈도우 프로시저로 보내준다.
			DispatchMessage(&msg);
		}

		// 윈도우가 데드타임일 경우 여기로 들어오게 된다.
		else
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CGameManager::Logic()
{
	float DeltaTime = m_Timer->Update() * m_TimeScale;

	CResourceManager::GetInst()->Update();

	// 입력 업데이트
	CInput::GetInst()->Update(DeltaTime);

	if (Update(DeltaTime))
		return;

	if (PostUpdate(DeltaTime))
		return;

	if (Collision(DeltaTime))
		return;

	Render(DeltaTime);
}

bool CGameManager::Update(float DeltaTime)
{
	return CSceneManager::GetInst()->Update(DeltaTime);
}

bool CGameManager::PostUpdate(float DeltaTime)
{
	return CSceneManager::GetInst()->PostUpdate(DeltaTime);
}

bool CGameManager::Collision(float DeltaTime)
{
	return CSceneManager::GetInst()->Collision(DeltaTime);
}

void CGameManager::Render(float DeltaTime)
{
	// 임시로 화면을 지우기 위한 Rect를 그려준다.
	Rectangle(m_hBackDC, -1, -1, m_RS.Width + 1, m_RS.Height + 1);

	CSceneManager::GetInst()->Render(m_hBackDC);

	// 마지막으로 마우스를 출력한다.
	CInput::GetInst()->Render(m_hBackDC);

	BitBlt(m_hDC, 0, 0, m_RS.Width, m_RS.Height, m_hBackDC, 0, 0, SRCCOPY);
}

ATOM CGameManager::Register()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	// 작업영역이 바뀔때 다시 그리게끔 해준다. 가로와 세로 모두 다시 그린다.
	wcex.style = CS_HREDRAW | CS_VREDRAW;

	// 메세지가 큐에 들어왔을때 해당 메세지를 꺼내오고 꺼내온 메세지를 인자로 전달해서
	// 호출해줄 함수를 지정한다.
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	// Window Instance를 지정한다.
	wcex.hInstance = m_hInst;

	// 실행파일이 만들어질때 사용할 아이콘 이미지를 지정한다.
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ICON1));


	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	// 유니코드 문자열을 사용할때 L"" 을 붙여서 사용을 한다.
	// TEXT 매크로는 "" 앞에 L 을 붙여서 L"" 로 만들어준다.
	// 유니코드일때는 이렇게 해주고 multibyte  일때는 그냥 "" 로 만들어준다.
	wcex.lpszClassName = TEXT("GameFramework");
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassExW(&wcex);
}

BOOL CGameManager::Create()
{
	m_hWnd = CreateWindowW(TEXT("GameFramework"), TEXT("GameFramework"), WS_OVERLAPPEDWINDOW,
		1920, 100, m_RS.Width, m_RS.Height, nullptr, nullptr, m_hInst, nullptr);

	if (!m_hWnd)
	{
		return FALSE;
	}

	// 클라이언트 영역을 1280, 720으로 만들어준다.
	// RECT : 사각형을 표현하는 구조체이다.
	// left, top, right, bottom 4개의 값으로 구성되어 있다.
	RECT    rc = { 0, 0, m_RS.Width, m_RS.Height };

	// 1번인자 : 클라이언트 영역의 크기를 전달한다.
	// 이 함수가 완료되면 rc에는 실제 클라이언트 영역이 원하는 크기 될 수 있는
	// 전체 윈도우의 left, top, right, bottom 정보가 들어오게 된다.
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// 위에서 얻어온 Rect를 이용해서 윈도우 크기를 지정한다.
	SetWindowPos(m_hWnd, HWND_TOPMOST, 1920, 100, rc.right - rc.left, 
		rc.bottom - rc.top,
		SWP_NOZORDER);

	// SW_SHOW : 윈도우창을 화면에 보여준다.
	// SW_HIDE : 창을 만들긴 하지만 화면에 보여주는건 아니다.
	ShowWindow(m_hWnd, SW_SHOW);

	// UpdateWindow 함수는 갱신할 부분이 있다면 갱신하라는 명령을 내려주는 함수이다.
	UpdateWindow(m_hWnd);

	return TRUE;
}

LRESULT CGameManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;

		// HDC : 화면에 출력하기 위한 그리기 도구이다.
		HDC hdc = BeginPaint(hWnd, &ps);
		
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		m_Loop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
