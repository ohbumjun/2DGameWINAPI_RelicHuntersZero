#include "Gun.h"
#include "../Collision/ColliderSphere.h"

CGun::CGun() :
	m_TextureImgNames{},
	m_Owner(nullptr)
{
	m_GunInfo.m_GunClass = EGunClass::Light;
	m_GunInfo.m_GunType        = EGun_Type::Light_Pistol;
	m_GunInfo.m_Damage         = NORMAL_MONSTER_ATTACK;
	m_GunInfo.m_BulletsLoaded  = true;
	m_GunInfo.m_BulletLoadTime = 0.1f;
	m_GunInfo.m_BulletDistance = NORMAL_BULLET_DISTANCE;
	
	m_ObjType = EObject_Type::Weapon;
}

CGun::CGun(const CGun& obj) : CGameObject(obj)
{
	m_Owner = nullptr;
	m_GunInfo = obj.m_GunInfo;

	for (int i = 0; i < ETexture_Dir::Texture_End; i++)
	{
		m_TextureImgNames[i] = obj.m_TextureImgNames[i];
	}
}

CGun::~CGun()
{
}


void CGun::Shoot()
{
}

void CGun::Start()
{
}

bool CGun::Init()
{
	if (!CGameObject::Init()) return false;

	CreateAnimation();

	SetPivot(0.5f, 0.5f);
	CColliderSphere* Body = AddCollider<CColliderSphere>("Body");
	Body->SetRadius(25.f);
	Body->SetOffset(0.f, 0.f);

	return true;
}

void CGun::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CGun::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
	if (m_Owner)
	{
		Vector2 OwnerDir = m_Owner->GetDir();
		Vector2 OwnerPos = m_Owner->GetPos();

		// 위치 조정 
		SetPos(m_Owner->GetPos());
		// 방향 조정 
		/*
		if (OwnerDir.x < 0)
			SetTexture(GUN_PISTOL_LIGHT_L, TEXT(TEXTURE_GUN_PISTOL_LIGHT_L));
		else
			SetTexture(GUN_PISTOL_LIGHT_R, TEXT(TEXTURE_GUN_PISTOL_LIGHT_R));
		*/
		if (OwnerDir.x < 0)
			SetTexture(m_TextureImgNames[ETexture_Dir::Texture_Left]);
		else
			SetTexture(m_TextureImgNames[ETexture_Dir::Texture_Right]);
		
	}
}

void CGun::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CGun::PrevRender()
{
	CGameObject::PrevRender();
}

void CGun::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CGun* CGun::Clone()
{
	return new CGun(*this);
}
