
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
	// �Ʒ��� id�� resource.h�� ����ִ� 
	// Editor Scene�� ��������ڸ���, Dialog�� �������´�
	m_Dlg->Init(IDD_DIALOG_EDITOR);

	return true;
}

bool CEditorScene::Update(float DeltaTime)
{
	CScene::Update(DeltaTime);
	if (!m_Start)
	{
		m_Start = true;
		// ����Ű�� ����, Dialog ���, 
		// Dialog �󿡼� Ÿ�� �����ϱ� 
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

	// Dialog â �ȿ�����, ���콺 Ŀ�� ���̰� �ϱ� 
	RECT rc = m_Dlg->GetRect();
	POINT ptMouse;

	// Screen ��ǥ�� ���´�
	GetCursorPos(&ptMouse);

	if (rc.left <= ptMouse.x
		&& ptMouse.x <= rc.right &&
		rc.top <= ptMouse.y &&
		ptMouse.y <= rc.bottom)
	{
		// cursor�� Dialog �ȿ� ���´ٸ�
		if (!CInput::GetInst()->IsShowCursor())
		{
			// �Ⱥ��̴� ���¶�� ���̴� ���·� 
			CInput::GetInst()->SetShowCursor(true);
			ShowCursor(TRUE);
		}
	}
	else
	{
		// ���̴� ���¿��� ���������ٸ�
		if (CInput::GetInst()->IsShowCursor())
		{
			// �Ⱥ��̴� ���·� �ٲ��ش� 
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
	// �̹� TileMap ������ return;
	if (m_TileMap) return;

	// TileMap�� ������ ���� 
	// �̷��� Map�� ������ִ� ���̴� 
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

	// Tile ������ ���콺���� Z �浹�� �ؾ� �Ѵ� 
	// Tile ������ idx X,Y�� ���ؾ� �Ѵ�
	// �ϳ�TileSize �� �ش� Tile�� World ��ġ�� ������ ���´�
	ETileEditMode EditMode = m_Dlg->GetTileEditMode();
	
	// Mouse Pos
	Vector2 MousePos = CInput::GetInst()->GetMousePos();
	
	// ī�޶� Pos�� �����༭ World �� ��ġ�� ���Ѵ�.
	CCamera* Camera = GetCamera();
	MousePos += Camera->GetPos();

	switch (EditMode)
	{
		// �ɼ� ���� 
	case ETileEditMode::Option:
	{
		// � Tile Option�� �����ߴ���
		ETileOption Option = m_Dlg->GetTileOption();
		m_TileMap->ChangeTileOption(MousePos, Option);
	}
		break;
	case ETileEditMode::Image:
	{
		// �ش� Texture�� Frame ������ �����;� �Ѵ�
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

	// �ǵ����� ���
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
