#include "TeleportMouse.h"

CTeleportMouse::CTeleportMouse()
{
    m_ObjType = EObject_Type::Effect;
}

CTeleportMouse::CTeleportMouse(const CTeleportMouse& obj) :
    CGameObject(obj)
{
}

CTeleportMouse::~CTeleportMouse()
{
}

void CTeleportMouse::Start()
{
    CGameObject::Start();
    SetAnimationEndNotify<CTeleportMouse>("TeleportMouseDisplay", this, &CTeleportMouse::AnimationFinish);
}

bool CTeleportMouse::Init()
{
    if (!CGameObject::Init()) return false;

    SetPivot(0.5f, 0.5f);
    CreateAnimation();
    AddAnimation("TeleportMouseDisplay", true, TELEPORT_MOUSE_DISPLAY_TIME);

    return true;
}

void CTeleportMouse::Update(float DeltaTime)
{
    CGameObject::Update(DeltaTime);
}

void CTeleportMouse::PostUpdate(float DeltaTime)
{
    CGameObject::PostUpdate(DeltaTime);
}

void CTeleportMouse::Collision(float DeltaTime)
{
    CGameObject::Collision(DeltaTime);
}

void CTeleportMouse::Render(HDC hDC)
{
    CGameObject::Render(hDC);
}

CTeleportMouse* CTeleportMouse::Clone()
{
    return new CTeleportMouse(*this);
}

void CTeleportMouse::AnimationFinish()
{
    Destroy();
}
