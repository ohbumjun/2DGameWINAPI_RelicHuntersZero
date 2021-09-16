
#include "UIMain.h"
#include "Button.h"
#include "UIImage.h"
#include "../GameManager.h"
#include "../Scene/MainScene.h"
#include "../Scene/SceneManager.h"
#include "../Scene/SceneResource.h"
#include "UIText.h"

CUIMain::CUIMain()
{
}

CUIMain::~CUIMain()
{
}

bool CUIMain::Init()
{
	Resolution	RS = CGameManager::GetInst()->GetResolution();

	CUIImage* Back = CreateWidget<CUIImage>("HPBarBack");

	Back->SetTexture("Back", TEXT("Back.bmp"));

	Back->SetPos(RS.Width / 2.f - 200.f, RS.Height - 150.f);

	m_Text = CreateWidget<CUIText>("TestText");

	m_Text->SetPos(RS.Width / 2.f - 180.f, RS.Height - 130.f);
	m_Text->SetTextColor(255, 255, 255);
	m_Text->SetZOrder(1);

	m_TextTime = 0.f;
	m_OutputText = 0;
	m_OutputIndex = 0;

	return true;
}

void CUIMain::Update(float DeltaTime)
{
	m_TextTime += DeltaTime;

	if (m_TextTime >= 0.1f)
	{
		m_TextTime -= 0.1f;

		const TCHAR* TestText[2] =
		{
			TEXT("abcdefghijklmnopqrstuvwxyz"),
			TEXT("이게 게임이냐")
		};

		TCHAR	Select[2] = { TestText[m_OutputText][m_OutputIndex], 0 };

		m_Text->AddText(Select);

		m_Scene->GetSceneResource()->SoundPlay("TextSound");

		++m_OutputIndex;

		if (lstrlen(TestText[m_OutputText]) == m_OutputIndex)
		{
			m_OutputIndex = 0;
			m_Text->Clear();
			m_OutputText = (m_OutputText + 1) % 2;
		}
	}
}
