
#include "EditorScene.h"
#include "SceneResource.h"
#include "../Object/EffectHit.h"
#include "Camera.h"
#include "../UI/UIStart.h"
#include "../Input.h"
#include "EditorDlg.h"
#include "../Map/TileMap.h"
#include "../GameManager.h"

CEditorScene::CEditorScene() :
	m_Dlg(nullptr),
	m_Start(false),
	m_TileMap(nullptr),
	m_ScrollSpeed(500.f)
{
	CGameManager::GetInst()->SetEditorMode(true);
}

CEditorScene::~CEditorScene()
{
	CGameManager::GetInst()->SetEditorMode(false);
	SAFE_DELETE(m_Dlg);
}

bool CEditorScene::Init()
{
	if (!CScene::Init()) return false;
	LoadSound();

	LoadAnimationSequence();

	m_Dlg = new CEditorDlg;
	m_Dlg->SetScene(this);
	// 아래의 id는 resource.h에 들어있다 
	// Editor Scene이 만들어지자마자, Dialog도 만들어놓는다
	m_Dlg->Init(IDD_DIALOG_EDITOR);

	return true;
}

bool CEditorScene::Update(float DeltaTime)
{
	CScene::Update(DeltaTime);
	if (!m_Start)
	{
		m_Start = true;
		// 단축키를 통해, Dialog 열어서, 
		// Dialog 상에서 타일 생성하기 
		CInput::GetInst()->SetCallback<CEditorScene>("Editor", KeyState_Down,
			this, &CEditorScene::OnEditor);

		CInput::GetInst()->SetCallback<CEditorScene>("MoveUp", KeyState_Push,
			this, &CEditorScene::CameraMoveUp);
		CInput::GetInst()->SetCallback<CEditorScene>("MoveDown", KeyState_Push,
			this, &CEditorScene::CameraMoveDown);
		CInput::GetInst()->SetCallback<CEditorScene>("MoveLeft", KeyState_Push,
			this, &CEditorScene::CameraMoveLeft);
		CInput::GetInst()->SetCallback<CEditorScene>("MoveRight", KeyState_Push,
			this, &CEditorScene::CameraMoveRight);
		
		CInput::GetInst()->SetCallback<CEditorScene>("MouseLButton", KeyState_Push,
			this, &CEditorScene::MouseLButton);
		CInput::GetInst()->SetCallback<CEditorScene>("MouseRButton", KeyState_Push,
			this, &CEditorScene::MouseRButton);

	}

	// Dialog 창 안에서도, 마우스 커서 보이게 하기 
	RECT rc = m_Dlg->GetRect();
	POINT ptMouse;

	// Screen 좌표로 나온다
	GetCursorPos(&ptMouse);

	if (rc.left <= ptMouse.x
		&& ptMouse.x <= rc.right &&
		rc.top <= ptMouse.y &&
		ptMouse.y <= rc.bottom)
	{
		// cursor가 Dialog 안에 들어온다면
		if (!CInput::GetInst()->IsShowCursor())
		{
			// 안보이는 상태라면 보이는 상태로 
			CInput::GetInst()->SetShowCursor(true);
			ShowCursor(TRUE);
		}
	}
	else
	{
		// 보이는 상태에서 빠져나갔다면
		if (CInput::GetInst()->IsShowCursor())
		{
			// 안보이는 상태로 바꿔준다 
			CInput::GetInst()->SetShowCursor(false);
			ShowCursor(FALSE);
		}
	}

	return false;
}

bool CEditorScene::PostUpdate(float DeltaTime)
{
	CScene::PostUpdate(DeltaTime);
	return false;
}

void CEditorScene::OnEditor(float DeltaTime)
{
	
}

void CEditorScene::CreateTileMap()
{
	// 이미 TileMap 있으면 return;
	if (m_TileMap) return;

	// TileMap이 없으면 생성 
	// 이렇게 Map만 만들어주는 것이다 
	m_TileMap = CreateMap<CTileMap>("TileMap");

}

void CEditorScene::SetTileInfo(int CountX, int CountY, int SizeX, int SizeY)
{
	m_TileMap->CreateTile(CountX, CountY, 
		Vector2((float)SizeX, (float)SizeY));
}

void CEditorScene::SetTileTexture(CTexture* Texture)
{
	m_TileMap->SetTileTexture(Texture);
}

void CEditorScene::CameraMoveUp(float DeltaTime)
{
	CCamera* Camera = GetCamera();
	Camera->Move(Vector2(-1.f, 0.f) * m_ScrollSpeed * DeltaTime);
}

void CEditorScene::CameraMoveDown(float DeltaTime)
{
	CCamera* Camera = GetCamera();
	Camera->Move(Vector2(0.f, -1.f)*m_ScrollSpeed * DeltaTime);
}

void CEditorScene::CameraMoveRight(float DeltaTime)
{
	CCamera* Camera = GetCamera();
	Camera->Move(Vector2(1.f, 0.f) * m_ScrollSpeed * DeltaTime);
}

void CEditorScene::CameraMoveLeft(float DeltaTime)
{
	CCamera* Camera = GetCamera();
	Camera->Move(Vector2(01.f, 0.f) * m_ScrollSpeed * DeltaTime);
}

void CEditorScene::MouseLButton(float DeltaTime)
{
	if (!m_TileMap) return;

	// Tile 각각과 마우스간의 Z 충돌을 해야 한다 
	// Tile 각각의 idx X,Y를 구해야 한다
	// 하나TileSize 로 해당 Tile의 World 위치를 나누면 나온다
	ETileEditMode EditMode = m_Dlg->GetTileEditMode();
	
	// Mouse Pos
	Vector2 MousePos = CInput::GetInst()->GetMousePos();
	
	// 카메라 Pos를 더해줘서 World 상 위치를 구한다.
	CCamera* Camera = GetCamera();
	MousePos += Camera->GetPos();

	switch (EditMode)
	{
		// 옵션 편집 
	case ETileEditMode::Option:
	{
		// 어떤 Tile Option을 선택했는지
		ETileOption Option = m_Dlg->GetTileOption();
		m_TileMap->ChangeTileOption(MousePos, Option);
	}
		break;
	case ETileEditMode::Image:
	{
		// 해당 Texture의 Frame 정보를 가져와야 한다
		TileFrameData FrameData = m_Dlg->GetTileFrameData();

		m_TileMap->SetTileFrame(MousePos, FrameData.Start, FrameData.End);

	}
		break;
	default:
		break;
	}
}

void CEditorScene::MouseRButton(float DeltaTime)
{
	if (!m_TileMap) return;
	ETileEditMode EditMode = m_Dlg->GetTileEditMode();
	Vector2 MousePos = CInput::GetInst()->GetMousePos();
	CCamera* Camera = GetCamera();
	MousePos += Camera->GetPos();

	// 되돌리기 기능
	m_TileMap->ChangeTileOption(MousePos,ETileOption::Normal);
}

void CEditorScene::LoadAnimationSequence()
{
}

void CEditorScene::LoadSound()
{
	GetSceneResource()->LoadSound("UI", false, "ButtonMouseOn", "TeemoSmile.mp3");
	GetSceneResource()->LoadSound("UI", false, "ButtonClick", "TeemoEditorClicck.mp3");
	GetSceneResource()->SetVolume("UI", 3);
}
