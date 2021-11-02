#include "Npc.h"
#include "../UI/UIText.h"
#include "../UI/NumberWidget.h"
#include "../Collision/ColliderBox.h"

CNpc::CNpc() :
    m_CostHunderedWidget(nullptr),
    m_CostTenWidget(nullptr),
    m_CostOneWidget(nullptr),
    m_TypeChanged(false),
    m_CostChanged(false)
{
    m_ObjType = EObject_Type::NPC;
    m_NpcType = ENpc_Type::Mp;
    m_Cost = 30;
    m_TypeWidget = nullptr;
}

CNpc::CNpc(const CNpc& obj) : CGameObject(obj)
{
    m_ObjType = EObject_Type::NPC;
    m_NpcType = obj.m_NpcType;
    m_Cost = obj.m_Cost;
    m_TypeChanged = false;
    m_CostChanged = false;
    m_CostHunderedWidget = obj.m_CostHunderedWidget->Clone();
    m_CostTenWidget = obj.m_CostTenWidget->Clone();
    m_CostOneWidget = obj.m_CostOneWidget->Clone();
}

CNpc::~CNpc()
{
}

void CNpc::CostInit()
{
    // Widgets
    std::vector<std::wstring>	vecNumberFileName;
    for (int i = 0; i < 10; ++i)
    {
        TCHAR	FileName[256] = {};
        wsprintf(FileName, TEXT("Number/%d.bmp"), i);
        vecNumberFileName.push_back(FileName);
    }
    m_CostHunderedWidget = CreateWidgetComponent("HundWidget");
    CNumberWidget* HunderedWidget = m_CostHunderedWidget->CreateWidget<CNumberWidget>("HundWidget");
    HunderedWidget->SetTexture("HundWidget", vecNumberFileName);
    for (int i = 0; i < 10; i++)
    {
        HunderedWidget->SetTextureColorKey(255, 255, 255, i);
    }
    m_CostHunderedWidget->SetPos(30.f, -90.f);

    m_CostTenWidget = CreateWidgetComponent("TenWidget");
    CNumberWidget* TenWidget = m_CostTenWidget->CreateWidget<CNumberWidget>("TenWidget");
    TenWidget->SetTexture("TenWidget", vecNumberFileName);
    for (int i = 0; i < 10; i++)
    {
        TenWidget->SetTextureColorKey(255, 255, 255, i);
    }
    m_CostTenWidget->SetPos(50.f, -90.f);

    m_CostOneWidget = CreateWidgetComponent("OneWidget");
    CNumberWidget* OneWidget = m_CostOneWidget->CreateWidget<CNumberWidget>("OneWidget");
    OneWidget->SetTexture("OneWidget", vecNumberFileName);
    for (int i = 0; i < 10; i++)
    {
        OneWidget->SetTextureColorKey(255, 255, 255, i);
    }
    m_CostOneWidget->SetPos(70.f, -90.f);

}

void CNpc::CostUpdate()
{
    CNumberWidget* HunderedWidget = (CNumberWidget*)m_CostHunderedWidget->GetWidget();
    CNumberWidget* TenWidget = (CNumberWidget*)m_CostTenWidget->GetWidget();
    CNumberWidget* OneWidget = (CNumberWidget*)m_CostOneWidget->GetWidget();
    // HPBar --- 
    int FullH = m_Cost / 100, FullT = (m_Cost % 100) / 10, FullO = m_Cost % 10;
    if (FullH != 0)
        HunderedWidget->SetNumber(FullH);
    else
        HunderedWidget->SetRenderEnable(false);
    if (FullH != 0 || FullT != 0)
        TenWidget->SetNumber(FullT);
    else
        TenWidget->SetRenderEnable(false);
    if (FullH != 0 || FullT != 0 || FullO != 0)
        OneWidget->SetNumber(FullO);
    else
        OneWidget->SetRenderEnable(false);
}

void CNpc::TypeChange()
{
    m_TypeChanged = false;

    CUIText* NameText = (CUIText*)m_TypeWidget->GetWidget();
    switch (m_NpcType)
    {
    case ENpc_Type::Hp:
        NameText->SetText(TEXT("HP"));
        m_TypeWidget->SetPos(-15.f, -90.f);
        break;
    case ENpc_Type::Mp:
        NameText->SetText(TEXT("MP"));
        m_TypeWidget->SetPos(-15.f, -90.f);
        break;
    case ENpc_Type::Shield:
        NameText->SetText(TEXT("SHIELD"));
        m_TypeWidget->SetPos(-40.f, -90.f);
        break;
    }
}

void CNpc::CostChange()
{
    m_CostChanged = false;
    CostUpdate();
}

void CNpc::Start()
{
    CGameObject::Start();
}


bool CNpc::Init()
{
    if (!CGameObject::Init())
        return false;

    SetPivot(0.5f, 0.5f);
    CreateAnimation();
    AddAnimation(NPC_IDLE, true, 2.f);

    // Widget 
    m_TypeWidget = CreateWidgetComponent("TypeWidget");
    CUIText* NameText = m_TypeWidget->CreateWidget<CUIText>("TypeText");
    NameText->SetText(TEXT("MP"));
    NameText->SetTextColor(10, 10, 10);
    m_TypeWidget->SetPos(-15.f, -90.f);
    
    // Cost Widget
    CostInit();

    CostUpdate();
   
    // Collider
    CColliderBox* Body = AddCollider<CColliderBox>("Body");
    Body->SetExtent(90.f, 130.f);
    Body->SetOffset(-5.f, -5.f);
    Body->SetCollisionProfile("Default");


    return true;
}

void CNpc::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
    if (m_TypeChanged)
        TypeChange();
    if (m_CostChanged)
        CostChange();
}

void CNpc::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

void CNpc::Collision(float DeltaTime)
{
    CGameObject::Collision(DeltaTime);
}

void CNpc::Render(HDC hDC)
{
    CGameObject::Render(hDC);
}
