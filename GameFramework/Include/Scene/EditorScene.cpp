
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
	// �Ʒ��� id�� resource.h�� ����ִ� 
	// Editor Scene�� ��������ڸ���, Dialog�� �������´�
	m_Dlg->Init(IDD_DIALOG_EDITOR);


	return true;
}

bool CEditorScene::Update(float DeltaTime)
{
	if (!m_Start)
	{
		m_Start = true;
		// ����Ű�� ����, Dialog ���, 
		// Dialog �󿡼� Ÿ�� �����ϱ� 
		CInput::GetInst()->SetCallback<CEditorScene>("Editor", KeyState_Down,
			this, &CEditorScene::OnEditor);
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
