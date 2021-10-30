
#include "UIBossEnd.h"
#include "../GameManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/MenuScene.h"
#include "../Scene/EndScene.h"
#include "UIText.h"
#include "UIImage.h"

CUIBossEnd::CUIBossEnd()
{
}

CUIBossEnd::~CUIBossEnd()
{
}

bool CUIBossEnd::Init()
{
	Resolution	RS = CGameManager::GetInst()->GetResolution();


	// CUIImage* Back = CreateWidget<CUIImage>("Back");
	// Back->SetTexture("MenuBack", TEXT("images/MenuScene/bg_menu_main.bmp"));

	// Upper, Downward Img
	m_UpperImg = CreateWidget<CUIImage>("m_UpperImg");
	m_UpperImg->SetTexture("m_UpperImg", TEXT("images/MenuScene/black_background.bmp"));
	m_UpperImg->SetPos(0.f, -720.f);
	m_DownImg = CreateWidget<CUIImage>("m_DownImg");
	m_DownImg->SetTexture("m_DownImg", TEXT("images/MenuScene/black_background.bmp"));
	m_DownImg->SetPos(0.f, 720.f);

	return true;
}

void CUIBossEnd::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
	if (m_UpperImg)
	{
		Vector2 UpperImgPos = m_UpperImg->GetPos();
		if (UpperImgPos.y <= -520.f)
		{
			float UpdateAmount = DeltaTime * 15;
			Vector2 Dir = Vector2(0.f, 1.f);
			m_UpperImg->Move(Dir, UpdateAmount);
		}
		else
			CSceneManager::GetInst()->CreateScene<CEndScene>();
	}
	if (m_DownImg)
	{
		Vector2 DownImgPos = m_DownImg->GetPos();
		if (DownImgPos.y >= 520.f)
		{
			float UpdateAmount = DeltaTime * 15;
			Vector2 Dir = Vector2(0.f, -1.f);
			m_DownImg->Move(Dir, UpdateAmount);
		}
	}
}

