#include "Npc.h"
#include "../UI/UIText.h"
#include "../UI/NumberWidget.h"
#include "../Collision/ColliderBox.h"

CNpc::CNpc()
{
    m_ObjType = EObject_Type::NPC;
    m_NpcType = ENpc_Type::Mp;
    m_Cost = 100.f;
    m_TypeChanged = false;
    m_TypeWidget = nullptr;
}

CNpc::CNpc(const CNpc& obj) : CGameObject(obj)
{
    m_ObjType = EObject_Type::NPC;
    m_NpcType = obj.m_NpcType;
    m_Cost = obj.m_Cost;
    m_TypeChanged = false;
}

CNpc::~CNpc()
{
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
