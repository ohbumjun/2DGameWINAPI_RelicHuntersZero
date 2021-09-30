
#include "EditorScene.h"
#include "SceneResource.h"
#include "../Object/EffectHit.h"
#include "Camera.h"
#include "../UI/UIStart.h"
#include "../Input.h"
#include "EditorDlg.h"

CEditorScene::CEditorScene() :
	m_Dlg(nullptr),
	m_Start(false)
{
}

CEditorScene::~CEditorScene()
{
	SAFE_DELETE(m_Dlg);
}

bool CEditorScene::Init()
{
	LoadSound();

	LoadAnimationSequence();

	m_Dlg = new CEditorDlg;
	// 아래의 id는 resource.h에 들어있다 
	// Editor Scene이 만들어지자마자, Dialog도 만들어놓는다
	m_Dlg->Init(IDD_DIALOG_EDITOR);


	return true;
}

bool CEditorScene::Update(float DeltaTime)
{
	if (!m_Start)
	{
		m_Start = true;
		// 단축키를 통해, Dialog 열어서, 
		// Dialog 상에서 타일 생성하기 
		CInput::GetInst()->SetCallback<CEditorScene>("Editor", KeyState_Down,
			this, &CEditorScene::OnEditor);
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
	return false;
}

void CEditorScene::OnEditor(float DeltaTime)
{
	
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
