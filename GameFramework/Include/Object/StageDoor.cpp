#include "StageDoor.h"

CStageDoor::CStageDoor()
{
    m_DoorStageType = EDoorStage_Type::Stage_Default;
}

CStageDoor::CStageDoor(const CStageDoor& obj) :
    CGameObject(obj)
{
}

CStageDoor::~CStageDoor()
{
}

void CStageDoor::Start()
{
	CGameObject::Start();
}

bool CStageDoor::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);
	CreateAnimation();
	AddAnimation("StageDoor", true, 0.3f);
}

void CStageDoor::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CStageDoor::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CStageDoor::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CStageDoor::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CStageDoor* CStageDoor::Clone()
{
	return new CStageDoor(*this);
}
