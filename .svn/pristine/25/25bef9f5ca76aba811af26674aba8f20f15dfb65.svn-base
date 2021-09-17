#include "Input.h"
#include "Resource/ResourceManager.h"

CInput* CInput::m_Inst = nullptr;

CInput::CInput()
{
	// vector는 size와 capacity가 있다.
	// capacity 는 실제 배열이 할당된 개수이다.
	// size 는 실제 추가된 개수이다.
	// vector resize 함수 : 이 함수로 원하는 개수를 지정해두면
	// 해당 개수만큼을 미리 할당해둔다.
	m_vecKeyState.resize(KEYCOUNT_MAX);

	for (int i = 0; i < KEYCOUNT_MAX; ++i)
	{
		m_vecKeyState[i].Key = i;
	}

	// vector reserve 함수 : capacity를 원하는 개수만큼 예약해둔다.

	m_Ctrl = false;
	m_Alt = false;
	m_Shift = false;

	m_MouseType = Mouse_Default;
}

CInput::~CInput()
{
	std::unordered_map<std::string, KeyInfo*>::iterator iter = m_mapInfo.begin();
	std::unordered_map<std::string, KeyInfo*>::iterator iterEnd = m_mapInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CInput::CreateKey(const std::string& Name, int Key)
{
	KeyInfo* Info = FindKeyInfo(Name);

	if (Info)
		return false;

	Info = new KeyInfo;

	Info->Name = Name;
	Info->State.Key = Key;

	m_mapInfo.insert(std::make_pair(Name, Info));

	bool	Add = false;

	size_t	Size = m_vecAddKey.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecAddKey[i] == Key)
		{
			// 이미 같은 키가 추가가 되어있는 경우라면
			// Add를 true로 만들어준다.
			Add = true;
			break;
		}
	}

	// 이미 같은 키가 추가가 되어 있다면 AddKey에 등록을 안한다.
	if (!Add)
		m_vecAddKey.push_back(Key);

	return true;
}

bool CInput::SetCtrlKey(const std::string& Name, bool State)
{
	KeyInfo* Info = FindKeyInfo(Name);

	if (!Info)
		return false;

	Info->Ctrl = State;

	return true;
}

bool CInput::SetAltKey(const std::string& Name, bool State)
{
	KeyInfo* Info = FindKeyInfo(Name);

	if (!Info)
		return false;

	Info->Alt = State;

	return true;
}

bool CInput::SetShiftKey(const std::string& Name, bool State)
{
	KeyInfo* Info = FindKeyInfo(Name);

	if (!Info)
		return false;

	Info->Shift = State;

	return true;
}

KeyInfo* CInput::FindKeyInfo(const std::string& Name)
{
	std::unordered_map<std::string, KeyInfo*>::iterator iter = m_mapInfo.find(Name);

	// 못찾았을 경우 end를 리턴한다.
	if (iter == m_mapInfo.end())
		return nullptr;

	return iter->second;
}

bool CInput::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	m_MouseDown = false;
	m_MousePush = false;
	m_MouseUp = false;

	// 게임에서 사용하는 키를 설정한다.
	CreateKey("MoveUp", 'W');
	CreateKey("MoveDown", 'S');
	CreateKey("MoveLeft", 'A');
	CreateKey("MoveRight", 'D');
	CreateKey("Fire", VK_SPACE);
	//SetCtrlKey("Fire", true);
	
	CreateKey("Pause", 'P');
	SetCtrlKey("Pause", true);
	CreateKey("Resume", 'R');
	SetCtrlKey("Resume", true);

	CreateKey("Skill1", '1');

	ShowCursor(FALSE);


	CUIImage* MouseDefault = new CUIImage;

	MouseDefault->SetSize(32.f, 31.f);

	std::vector<std::wstring>	vecFileName;

	for (int i = 0; i <= 10; ++i)
	{
		TCHAR	FileName[MAX_PATH] = {};
		wsprintf(FileName, TEXT("Mouse/%d.bmp"), i);

		vecFileName.push_back(FileName);
	}

	CResourceManager::GetInst()->LoadTexture("MouseDefault", vecFileName);

	CTexture* Texture = CResourceManager::GetInst()->FindTexture("MouseDefault");

	for (int i = 0; i <= 10; ++i)
	{
		Texture->SetColorKey(255, 0, 255, i);
	}

	MouseDefault->SetTexture(Texture);

	for (int i = 0; i <= 10; ++i)
	{
		MouseDefault->AddFrameData(Vector2(0.f, 0.f), Vector2(32.f, 31.f));
	}

	m_vecMouseImage.push_back(MouseDefault);


	return true;
}

void CInput::Update(float DeltaTime)
{
	// 키 상태를 업데이트 해준다.
	UpdateKeyState();

	// 마우스 업데이트
	UpdateMouse(DeltaTime);

	// 위에서 업데이트를 해준 상태를 이용해서 실제 키 정보를 업데이트한다.
	UpdateKeyInfo(DeltaTime);

	m_vecMouseImage[m_MouseType]->Update(DeltaTime);
}

void CInput::Render(HDC hDC)
{
	m_vecMouseImage[m_MouseType]->Render(m_MousePos, hDC);
}

void CInput::UpdateMouse(float DeltaTime)
{
	POINT	ptMouse;

	// 아래 함수는 마우스의 위치를 스크린좌표 기준으로 구해주고 있다.
	GetCursorPos(&ptMouse);

	// 스크린 좌표를 클라이언트 좌표로 변환한다.
	ScreenToClient(m_hWnd, &ptMouse);

	Vector2	Pos;
	Pos.x = (float)ptMouse.x;
	Pos.y = (float)ptMouse.y;

	m_MouseMove = Pos - m_MousePos;

	m_MousePos = Pos;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (!m_MouseDown && !m_MousePush)
		{
			m_MouseDown = true;
			m_MousePush = true;
		}

		else
		{
			m_MouseDown = false;
		}
	}

	else if (m_MousePush)
	{
		m_MouseDown = false;
		m_MousePush = false;
		m_MouseUp = true;
	}

	else if (m_MouseUp)
	{
		m_MouseUp = false;
	}
}

void CInput::UpdateKeyState()
{
	// 등록된 키 수만큼 반복하며 해당 키가 눌러졌는지를 판단해놓는다.
	size_t	Size = m_vecAddKey.size();

	for (size_t i = 0; i < Size; ++i)
	{
		int	Key = m_vecAddKey[i];

		if (GetAsyncKeyState(Key) & 0x8000)
		{
			// 지금 막 눌렀는지 아니면 이전 프레임부터 계속 누르고 있던건지를
			// 판단한다.
			if (!m_vecKeyState[Key].State[KeyState_Down] &&
				!m_vecKeyState[Key].State[KeyState_Push])
			{
				m_vecKeyState[Key].State[KeyState_Down] = true;
				m_vecKeyState[Key].State[KeyState_Push] = true;
			}

			else
			{
				m_vecKeyState[Key].State[KeyState_Down] = false;
			}
		}

		else if (m_vecKeyState[Key].State[KeyState_Push])
		{
			m_vecKeyState[Key].State[KeyState_Push] = false;
			m_vecKeyState[Key].State[KeyState_Down] = false;
			m_vecKeyState[Key].State[KeyState_Up] = true;
		}

		else if(m_vecKeyState[Key].State[KeyState_Up])
		{
			m_vecKeyState[Key].State[KeyState_Up] = false;
		}
	}

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		m_Ctrl = true;
	}

	else
		m_Ctrl = false;

	if (GetAsyncKeyState(VK_MENU) & 0x8000)
	{
		m_Alt = true;
	}

	else
		m_Alt = false;

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		m_Shift = true;
	}

	else
		m_Shift = false;
}

void CInput::UpdateKeyInfo(float DeltaTime)
{
	std::unordered_map<std::string, KeyInfo*>::iterator iter = m_mapInfo.begin();
	std::unordered_map<std::string, KeyInfo*>::iterator iterEnd = m_mapInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		int	Key = iter->second->State.Key;

		if (m_vecKeyState[Key].State[KeyState_Down] &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			if (iter->second->Callback[KeyState_Down])
				iter->second->Callback[KeyState_Down](DeltaTime);
		}

		if (m_vecKeyState[Key].State[KeyState_Push] &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			if (iter->second->Callback[KeyState_Push])
				iter->second->Callback[KeyState_Push](DeltaTime);
		}

		if (m_vecKeyState[Key].State[KeyState_Up] &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			if (iter->second->Callback[KeyState_Up])
				iter->second->Callback[KeyState_Up](DeltaTime);
		}
	}
}

void CInput::ClearCallback()
{
	auto	iter = m_mapInfo.begin();
	auto	iterEnd = m_mapInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		for (int i = 0; i < KeyState_Max; ++i)
		{
			iter->second->Callback[i] = nullptr;
		}
	}
}
