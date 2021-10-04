#include "UIHome.h"

CUIHome::CUIHome()
{
}

CUIHome::~CUIHome()
{
}

bool CUIHome::Init()
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

	// m_NumberWidget
	m_NumberWidget = CreateWidget<CNumberWidget>("Number");
	std::vector<std::wstring>	vecNumberFileName;
	for (int i = 0; i < 10; ++i)
	{
		TCHAR	FileName[256] = {};
		wsprintf(FileName, TEXT("Number/%d.bmp"), i);
		vecNumberFileName.push_back(FileName);
	}

	m_NumberWidget->SetTexture("Number", vecNumberFileName);
	m_NumberWidget->SetPos(560.f, 100.f);
	m_NumberWidget->SetSize(29.f, 48.f);
	for (int i = 0; i < 10; i++)
	{
		m_NumberWidget->SetTextureColorKey(255, 255, 255, i);
	}

	// m_Number1Widget
	m_Number1Widget = CreateWidget<CNumberWidget>("Number");
	m_Number1Widget->SetTexture("Number", vecNumberFileName);
	m_Number1Widget->SetPos(590.f, 100.f);
	m_Number1Widget->SetSize(29.f, 48.f);
	for (int i = 0; i < 10; i++)
	{
		m_Number1Widget->SetTextureColorKey(255, 255, 255, i);
	}

	// m_MinuteWidget
	m_MinuteWidget = CreateWidget<CNumberWidget>("Number");
	m_MinuteWidget->SetTexture("Number", vecNumberFileName);
	m_MinuteWidget->SetPos(500.f, 100.f);
	m_MinuteWidget->SetSize(29.f, 48.f);
	for (int i = 0; i < 10; i++)
	{
		m_MinuteWidget->SetTextureColorKey(255, 255, 255, i);
	}

	// Colon
	CUIImage* Colon = CreateWidget<CNumberWidget>("Colon");
	Colon->SetTexture("Colon", TEXT("Number/Colon.bmp"));
	Colon->SetPos(530.f, 100.f);
	Colon->SetSize(29.f, 48.f);
	m_Time = 0.f;

	return true;
}

void CUIHome::Update(float DeltaTime)
{
	CUIWindow::Update(DeltaTime);
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

	m_Time += DeltaTime;
	if (m_Time >= 60.f)
	{
		m_Minute += 1;
		m_Time -= 60.f;
	}

	m_MinuteWidget->SetNumber((int)(m_Minute));
	m_NumberWidget->SetNumber((int)(m_Time / 10));
	m_Number1Widget->SetNumber((int)(m_Time) % 10);
}
