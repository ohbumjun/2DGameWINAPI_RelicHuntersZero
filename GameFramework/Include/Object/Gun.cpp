#include "Gun.h"
#include "../Collision/ColliderSphere.h"
#include "Bullet.h"
#include "EffectCasing.h"
#include"../Scene/Scene.h"

CGun::CGun() :
	m_TextureImgNames{},
	m_Owner(nullptr)
{
	m_GunInfo.m_GunClass       = EGunClass::Light;
	m_GunInfo.m_GunType        = EGun_Type::Light_Pistol;
	m_GunInfo.m_Damage         = NORMAL_MONSTER_ATTACK;
	m_GunInfo.m_BulletsLoaded  = true;
	m_GunInfo.m_BulletsFullNum = PISTOL_BULLET_NUM;
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


void CGun::PlayerFire(Vector2 TargetPos, float OwnerAttackDamage)
{
	// Offet의 경우 , Gun의 Offset 위치에 맞춰야 한다 
	CScene* Scene = m_Owner->GetScene();
	Vector2 BulletOffset = m_Owner->CheckCurrentAnimation(PLAYER_RIGHT_ATTACK) ? Vector2(m_Size.x * 0.15, -m_Size.y * 0.3f) : Vector2(m_Size.x * 0.15, -m_Size.y * 0.3f);
	CSharedPtr<CBullet> Bullet = m_Owner->GetScene()->CreateObject<CBullet>("Bullet",
		PLAYER_BULLET_PROTO,
		Vector2(m_Pos + BulletOffset),
		Vector2(50.f, 50.f));
	// Angle 
	float Angle = GetAngle(Bullet->GetPos(), TargetPos);
	Bullet->SetDir(Angle);
	// Damage 
	Bullet->SetBulletDamage(OwnerAttackDamage + (float)m_GunInfo.m_Damage);
	// Collision Profile
	CCollider* BulletBody = Bullet->FindCollider("Body");
	BulletBody->SetCollisionProfile("PlayerAttack");

	// Casing
	CreateCasing();
}

void CGun::MonsterFire(Vector2 TargetPos, float OwnerAttackDamage)
{

	CScene* Scene = m_Owner->GetScene();
	Vector2 BulletOffset = m_Owner->CheckCurrentAnimation(MONSTER_RIGHT_ATTACK) ? Vector2(m_Size.x * 0.15, -m_Size.y * 0.3f) : Vector2(m_Size.x * 0.15, -m_Size.y * 0.3f);
	CSharedPtr<CBullet> Bullet = Scene->CreateObject<CBullet>("Bullet",
		MONSTER_BULLET_PROTO,
		Vector2(m_Pos + BulletOffset),
		Vector2(50.f, 50.f));
	// Bullet Damage
	Bullet->SetBulletDamage(OwnerAttackDamage + (float)m_GunInfo.m_Damage);
	// Speed 
	Bullet->SetMoveSpeed(BOSS_MONSTER_ATTACK_SPEED);
	// Bullet Dir 
	float Angle = GetAngle(Bullet->GetPos(), TargetPos);
	Bullet->SetDir(Angle);

	// Collision Profile
	CCollider* BulletBody = Bullet->FindCollider("Body");
	BulletBody->SetCollisionProfile("MonsterAttack");

	// Casing
	CreateCasing();
}

void CGun::CreateCasing()
{
	// Casing Effect
	Vector2 CasingOffset = m_Owner->CheckCurrentAnimation(PLAYER_RIGHT_ATTACK) ? Vector2(m_Size.x * 0.15, -m_Size.y * 0.3f) : Vector2(m_Size.x * 0.15, -m_Size.y * 0.3f);
	CSharedPtr<CEffectCasing> Casing = m_Owner->GetScene()->CreateObject<CEffectCasing>("Casing",
		EFFECT_CASING_PROTO,
		Vector2(m_Pos + CasingOffset),
		Vector2(50.f, 50.f));
	float CasingDir = m_Owner->CheckCurrentAnimation(PLAYER_RIGHT_ATTACK) ? -1 : 1;
	Casing->SetDirX(CasingDir);
	switch (m_GunInfo.m_GunClass)
	{
	case Light:
		Casing->SetTexture("LightCasing",
			TEXT("images/Weapon/Casing/spr_casing_0.bmp"));
		Casing->SetTextureColorKey(255, 255, 255);
		break;
	case Medium:
		Casing->SetTexture("MediumCasing",
			TEXT("images/Weapon/Casing/spr_casing_2.bmp"));
		Casing->SetTextureColorKey(255, 255, 255);
		break;
	case Heavy:
		Casing->SetTexture("HeavyCasing",
			TEXT("images/Weapon/Casing/spr_casing_1.bmp"));
		Casing->SetTextureColorKey(255, 255, 255);
		break;
	}
}

void CGun::Start()
{
}

bool CGun::Init()
{
	if (!CGameObject::Init()) return false;

	// CreateAnimation();

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
		Vector2 OwnerDir  = m_Owner->GetDir();
		Vector2 OwnerPos  = m_Owner->GetPos();
		Vector2 OwnerSize = m_Owner->GetSize();

		// 위치 조정 
		// Gun->SetPos(m_Size.x * 0.1, -m_Size.y * 0.3);
		SetPos(m_Owner->GetPos().x,m_Owner->GetPos().y - m_Owner->GetSize().y * 0.3 );
		// SetPos(m_Owner->GetSize().x * 0.1, -m_Owner->GetSize().y * 0.3);

		// 방향 조정 
		if (OwnerDir.x < 0)
		{
			SetTexture(m_TextureImgNames[ETexture_Dir::Texture_Left]);
			// SetOffset(-OwnerSize.x * 0.1, -OwnerSize.y * 0.3);
			SetOffset(-OwnerSize.x * 0.1, 0 );
		}
		else
		{
			SetTexture(m_TextureImgNames[ETexture_Dir::Texture_Right]);
			// SetOffset(OwnerSize.x * 0.1, -OwnerSize.y * 0.3);
			SetOffset(OwnerSize.x * 0.1, 0 );
		}
		
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
