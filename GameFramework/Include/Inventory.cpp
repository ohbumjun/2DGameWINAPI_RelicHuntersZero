#include "Inventory.h"
#include "Object/Gun.h"
CInventory::CInventory() : m_GunItems{}
{
}

CInventory::~CInventory()
{
// Delete Gun Class
	for (int i = 0; i < EGunClass::End; i++)
	{
		SAFE_DELETE(m_GunItems[i]);
	}
}

void CInventory::AddGun(CGun* Gun)
{
	EGunClass GunClass;
	switch (Gun->GetGunClass())
	{
	case Light:
		GunClass = Light;
		break;
	case Medium:
		GunClass = Medium;
		break;
	case Heavy:
		GunClass = Heavy;
		break;
	}
	SAFE_DELETE(m_GunItems[GunClass]);
	m_GunItems[GunClass] = Gun;
}

void CInventory::Start()
{
}

bool CInventory::Init()
{
	return true;
}

void CInventory::Update(float DeltaTime)
{
}

void CInventory::PostUpdate(float DeltaTime)
{
}

void CInventory::Collision(float DeltaTime)
{
}

void CInventory::PrevRender()
{
}

void CInventory::Render(HDC hDC)
{
}

CInventory* CInventory::Clone()
{
	return nullptr;
}
