
#include "GameManager.h"
#include "Timer.h"
#include "Scene/SceneManager.h"
#include "Scene/MainScene.h"
#include "Scene/StartScene.h"
#include "Input.h"
#include "PathManager.h"
#include "Resource/ResourceManager.h"
#include "Collision/CollisionManager.h"

CGameManager *CGameManager::m_Inst = nullptr;
bool CGameManager::m_Loop = true;

CGameManager::CGameManager() : m_Timer(nullptr),
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

	// GetDC�� �̿��ؼ� ������ DC�� �ݵ�� ReleaseDC�� ���־�� �Ѵ�.
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

	// DC�� �����Ѵ�.
	m_hDC = GetDC(m_hWnd);

	// ��ΰ����� �ʱ�ȭ
	if (!CPathManager::GetInst()->Init())
		return false;

	// ���ҽ� ������ �ʱ�ȭ
	if (!CResourceManager::GetInst()->Init())
		return false;

	// �浹 ������ �ʱ�ȭ
	if (!CCollisionManager::GetInst()->Init())
		return false;

	// �Է°����� �ʱ�ȭ
	if (!CInput::GetInst()->Init(m_hWnd))
		return false;

	// �������� �ʱ�ȭ
	if (!CSceneManager::GetInst()->Init())
		return false;

	CSceneManager::GetInst()->CreateScene<CStartScene>();

	// Ÿ�̸Ӹ� �����Ѵ�.
	m_Timer = new CTimer;

	// ����� ����
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
		// GetMessage�� �޼����� ���� ��� �ٸ����� �� �� ����.
		// �޼����� �ö����� ����ϰ� �ִ� �ð��� �������� ����Ÿ���̶�� �Ѵ�.
		// ������ �޼����� �ִ� �ð����� ���� �ð��� �ξ� ���.
		// �׷��� ������ �������� ����Ÿ�ӵ��� ������ ���۵� �� �ְ� �����Ѵ�.
		// PeekMessage�� �޼���ť���� �޼����� ���´�.
		// �׷��� ���� �޼����� ���ٸ� false�� �����ϸ鼭 �ٷ� ����������
		// �޼����� �ִٸ� true�� �����ϸ鼭 �ش� �޼����� �����´�.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// WM_KEYDOWN �̶�� �޼����� �ִ�. �̴� Ű���� Ű�� �������� �߻��Ǵ� �޼���
			// �̴�. TranslateMessage �Լ������� �޼����� �������ָ� WM_KEYDOWN ������
			// �Ǵ����ְ� ������ Ű�� ���� Ű������ �Ǵ��ؼ� �Ϲ� ���� Ű��� WM_CHAR���
			// �޽����� ���� �޼��� ť�� �߰����ְ� �ȴ�.
			TranslateMessage(&msg);

			// DispatchMessage �Լ��� �޼����� ������ ���ν����� �����ش�.
			DispatchMessage(&msg);
		}

		// �����찡 ����Ÿ���� ��� ����� ������ �ȴ�.
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

	// �Է� ������Ʈ
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
	// �ӽ÷� ȭ���� ����� ���� Rect�� �׷��ش�.
	Rectangle(m_hBackDC, -1, -1, m_RS.Width + 1, m_RS.Height + 1);

	CSceneManager::GetInst()->Render(m_hBackDC);

	BitBlt(m_hDC, 0, 0, m_RS.Width, m_RS.Height, m_hBackDC, 0, 0, SRCCOPY);
}

ATOM CGameManager::Register()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	// �۾������� �ٲ� �ٽ� �׸��Բ� ���ش�. ���ο� ���� ��� �ٽ� �׸���.
	wcex.style = CS_HREDRAW | CS_VREDRAW;

	// �޼����� ť�� �������� �ش� �޼����� �������� ������ �޼����� ���ڷ� �����ؼ�
	// ȣ������ �Լ��� �����Ѵ�.
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	// Window Instance�� �����Ѵ�.
	wcex.hInstance = m_hInst;

	// ���������� ��������� ����� ������ �̹����� �����Ѵ�.
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ICON1));

	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	// �����ڵ� ���ڿ��� ����Ҷ� L"" �� �ٿ��� ����� �Ѵ�.
	// TEXT ��ũ�δ� "" �տ� L �� �ٿ��� L"" �� ������ش�.
	// �����ڵ��϶��� �̷��� ���ְ� multibyte  �϶��� �׳� "" �� ������ش�.
	wcex.lpszClassName = TEXT("GameFramework");
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassExW(&wcex);
}

BOOL CGameManager::Create()
{
	m_hWnd = CreateWindowW(TEXT("GameFramework"), TEXT("GameFramework"), WS_OVERLAPPEDWINDOW,
						   0, 100, m_RS.Width, m_RS.Height, nullptr, nullptr, m_hInst, nullptr);

	if (!m_hWnd)
	{
		return FALSE;
	}

	// Ŭ���̾�Ʈ ������ 1280, 720���� ������ش�.
	// RECT : �簢���� ǥ���ϴ� ����ü�̴�.
	// left, top, right, bottom 4���� ������ �����Ǿ� �ִ�.
	RECT rc = {0, 0, m_RS.Width, m_RS.Height};

	// 1������ : Ŭ���̾�Ʈ ������ ũ�⸦ �����Ѵ�.
	// �� �Լ��� �Ϸ�Ǹ� rc���� ���� Ŭ���̾�Ʈ ������ ���ϴ� ũ�� �� �� �ִ�
	// ��ü �������� left, top, right, bottom ������ ������ �ȴ�.
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// ������ ���� Rect�� �̿��ؼ� ������ ũ�⸦ �����Ѵ�.
	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 100, rc.right - rc.left,
				 rc.bottom - rc.top,
				 SWP_NOZORDER);

	// SW_SHOW : ������â�� ȭ�鿡 �����ش�.
	// SW_HIDE : â�� ����� ������ ȭ�鿡 �����ִ°� �ƴϴ�.
	ShowWindow(m_hWnd, SW_SHOW);

	// UpdateWindow �Լ��� ������ �κ��� �ִٸ� �����϶�� ������ �����ִ� �Լ��̴�.
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

		// HDC : ȭ�鿡 ����ϱ� ���� �׸��� �����̴�.
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
