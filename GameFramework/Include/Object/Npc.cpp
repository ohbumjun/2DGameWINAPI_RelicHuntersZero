#include "Npc.h"

CNpc::CNpc()
{
    m_ObjType = EObject_Type::NPC;
}

CNpc::CNpc(const CNpc& obj) : CGameObject(obj)
{
}

CNpc::~CNpc()
{
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
}

void CNpc::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
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
