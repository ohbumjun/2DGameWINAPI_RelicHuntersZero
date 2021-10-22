#include "UIHome.h"
#include "../GameManager.h"
#include "../Object/StageDoor.h"

CUIHome::CUIHome() :
    m_HpCostHunderedWidget(nullptr),
    m_HpCostTenWidget(nullptr),
    m_HpCostOneWidget(nullptr),
    m_MpCostHunderedWidget(nullptr),
    m_MpCostTenWidget(nullptr),
    m_MpCostOneWidget(nullptr),
    m_ShieldCostHunderedWidget(nullptr),
    m_ShieldCostTenWidget(nullptr),
    m_ShieldCostOneWidget(nullptr),
    m_HpCost(100),
    m_MpCost(100),
    m_ShieldCost(300)
{
}

CUIHome::~CUIHome()
{
}

bool CUIHome::Init()
{
    Resolution	RS = CGameManager::GetInst()->GetResolution();

    // Widgets
    std::vector<std::wstring>	vecNumberFileName;
    for (int i = 0; i < 10; ++i)
    {
        TCHAR	FileName[256] = {};
        wsprintf(FileName, TEXT("Number/%d.bmp"), i);
        vecNumberFileName.push_back(FileName);
    }

    HpCostInit(vecNumberFileName);
    MpCostInit(vecNumberFileName);
    ShieldCostInit(vecNumberFileName);

    return true;
}

void CUIHome::Update(float DeltaTime)
{
    CUIWindow::Update(DeltaTime);
}

void CUIHome::HpCostInit(std::vector<std::wstring>& vecNumberFileName)
{
    // Hp ---
    m_HpCostHunderedWidget = CreateWidget<CNumberWidget>("HPCostHundredNumber");
    m_HpCostHunderedWidget->SetTexture("HPCostHundredNumber", vecNumberFileName);
    m_HpCostHunderedWidget->SetPos(550.f, 210.f);
    m_HpCostHunderedWidget->SetSize(16.f, 30.f);
    for (int i = 0; i < 10; i++)
    {
        m_HpCostHunderedWidget->SetTextureColorKey(255, 255, 255, i);
    }

    m_HpCostTenWidget = CreateWidget<CNumberWidget>("HPCostTenNumber");
    m_HpCostTenWidget->SetTexture("HPCostTenNumber", vecNumberFileName);
    m_HpCostTenWidget->SetPos(580.f, 210.f);
    m_HpCostTenWidget->SetSize(16.f, 30.f);
    for (int i = 0; i < 10; i++)
    {
        m_HpCostTenWidget->SetTextureColorKey(255, 255, 255, i);
    }

    m_HpCostOneWidget = CreateWidget<CNumberWidget>("HPCostOneNumber");
    m_HpCostOneWidget->SetTexture("HPCostOneNumber", vecNumberFileName);
    m_HpCostOneWidget->SetPos(610.f, 210.f);
    m_HpCostOneWidget->SetSize(16.f, 30.f);
    for (int i = 0; i < 10; i++)
    {
        m_HpCostOneWidget->SetTextureColorKey(255, 255, 255, i);
    }

    // HPBar --- 
    int FullH = m_HpCost / 100, FullT = (m_HpCost % 100) / 10, FullO = m_HpCost % 10;
    if (FullH != 0)
        SetHpCostHundredWidget(FullH);
    else
        SetHpCostHundredRenderEnable(false);
    if (FullH != 0 || FullT != 0)
        SetHpCostTenWidget(FullT);
    else
        SetHpCostTenRenderEnable(false);
    if (FullH != 0 || FullT != 0 || FullO != 0)
        SetHpCostOneWidget(FullO);
    else
        SetHpCostOneRenderEnable(false);
}

void CUIHome::MpCostInit(std::vector<std::wstring>& vecNumberFileName)
{
    // Mp ---
    m_MpCostHunderedWidget = CreateWidget<CNumberWidget>("MPCostHundredNumber");
    m_MpCostHunderedWidget->SetTexture("MPCostHundredNumber", vecNumberFileName);
    m_MpCostHunderedWidget->SetPos(750.f, 200.f);
    m_MpCostHunderedWidget->SetSize(16.f, 30.f);
    for (int i = 0; i < 10; i++)
    {
       m_MpCostHunderedWidget->SetTextureColorKey(255, 255, 255, i);
    }

    m_MpCostTenWidget = CreateWidget<CNumberWidget>("MPCostTenNumber");
    m_MpCostTenWidget->SetTexture("MPCostTenNumber", vecNumberFileName);
    m_MpCostTenWidget->SetPos(780.f, 200.f);
    m_MpCostTenWidget->SetSize(16.f, 30.f);
    for (int i = 0; i < 10; i++)
    {
        m_MpCostTenWidget->SetTextureColorKey(255, 255, 255, i);
    }

    m_MpCostOneWidget = CreateWidget<CNumberWidget>("MPCostOneNumber");
    m_MpCostOneWidget->SetTexture("MPCostOneNumber", vecNumberFileName);
    m_MpCostOneWidget->SetPos(810.f, 200.f);
    m_MpCostOneWidget->SetSize(16.f, 30.f);
    for (int i = 0; i < 10; i++)
    {
       m_MpCostOneWidget->SetTextureColorKey(255, 255, 255, i);
    }

    // HPBar --- 
    int FullH = m_MpCost / 100, FullT = (m_MpCost % 100) / 10, FullO = m_MpCost % 10;
    if (FullH != 0)
        SetMpCostHundredWidget(FullH);
    else
        SetMpCostHundredRenderEnable(false);
    if (FullH != 0 || FullT != 0)
        SetMpCostTenWidget(FullT);
    else
        SetMpCostTenRenderEnable(false);
    if (FullH != 0 || FullT != 0 || FullO != 0)
        SetMpCostOneWidget(FullO);
    else
        SetMpCostOneRenderEnable(false);
}

void CUIHome::ShieldCostInit(std::vector<std::wstring>& vecNumberFileName)
{
    // Shield ---
    m_ShieldCostHunderedWidget = CreateWidget<CNumberWidget>("ShieldCostHundredNumber");
    m_ShieldCostHunderedWidget->SetTexture("ShieldCostHundredNumber", vecNumberFileName);
    m_ShieldCostHunderedWidget->SetPos(950.f, 300.f);
    m_ShieldCostHunderedWidget->SetSize(16.f, 30.f);
    for (int i = 0; i < 10; i++)
    {
        m_ShieldCostHunderedWidget->SetTextureColorKey(255, 255, 255, i);
    }

    m_ShieldCostTenWidget = CreateWidget<CNumberWidget>("ShieldCostTenNumber");
    m_ShieldCostTenWidget->SetTexture("ShieldCostTenNumber", vecNumberFileName);
    m_ShieldCostTenWidget->SetPos(980.f, 300.f);
    m_ShieldCostTenWidget->SetSize(16.f, 30.f);
    for (int i = 0; i < 10; i++)
    {
        m_ShieldCostTenWidget->SetTextureColorKey(255, 255, 255, i);
    }

    m_ShieldCostOneWidget = CreateWidget<CNumberWidget>("ShieldCostOneNumber");
    m_ShieldCostOneWidget->SetTexture("ShieldCostOneNumber", vecNumberFileName);
    m_ShieldCostOneWidget->SetPos(1010.f, 300.f);
    m_ShieldCostOneWidget->SetSize(16.f, 30.f);
    for (int i = 0; i < 10; i++)
    {
        m_ShieldCostOneWidget->SetTextureColorKey(255, 255, 255, i);
    }

    // HPBar --- 
    int FullH = m_ShieldCost / 100, FullT = (m_ShieldCost % 100) / 10, FullO = m_ShieldCost % 10;
    if (FullH != 0)
        SetShieldCostHundredWidget(FullH);
    else
        SetShieldCostHundredRenderEnable(false);
    if (FullH != 0 || FullT != 0)
        SetShieldCostTenWidget(FullT);
    else
        SetShieldCostTenRenderEnable(false);
    if (FullH != 0 || FullT != 0 || FullO != 0)
        SetShieldCostOneWidget(FullO);
    else
        SetShieldCostOneRenderEnable(false);
}
