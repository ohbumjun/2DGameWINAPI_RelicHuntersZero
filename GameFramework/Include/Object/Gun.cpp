#include "Gun.h"
#include "../Collision/ColliderSphere.h"
#include "Bullet.h"
#include "DamageFont.h"
#include "EffectCasing.h"
#include "EffectText.h"
#include "EffectBulletStart.h"
#include"../Scene/Scene.h"
#include"../Scene/Camera.h"
// UI
#include "../UI/UIText.h"

CGun::CGun() :
	m_TextureImgNames{},
	m_Owner(nullptr),
	m_FireToggle(false)
{
	m_GunInfo.m_GunClass       = EGunClass::Light;
	m_GunInfo.m_GunType        = EGun_Type::Light_Pistol;
	m_GunInfo.m_Damage         = NORMAL_MONSTER_ATTACK;
	// m_GunInfo.m_BulletsLoaded  = PISTOL_BULLET_NUM;
	m_GunInfo.m_BulletsLoaded  = PISTOL_BULLET_NUM;
	m_GunInfo.m_BulletsFullNum = PISTOL_BULLET_NUM;
	m_GunInfo.m_BulletLoadTime = 0.1f;
	m_GunInfo.m_BulletDistance = NORMAL_BULLET_DISTANCE;
	
	m_ObjType = EObject_Type::Weapon;
}

CGun::CGun(const CGun& obj) : CGameObject(obj)
{
	m_Owner      = nullptr;
	m_GunInfo    = obj.m_GunInfo;
	m_FireToggle = false;

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
	if (m_GunInfo.m_BulletsLoaded <= 0)
	{
		// 경고
		ShowNoBulletSign();
		return;
	}
	
	// Adjust Gun Texture
	AdjustGunTexture();

	// Offet의 경우 , Gun의 Offset 위치에 맞춰야 한다 
	CScene* Scene = m_Owner->GetScene();
	// Vector2 BulletOffset = m_Owner->CheckCurrentAnimation(PLAYER_RIGHT_ATTACK) ? Vector2(m_Size.x * 0.15, -m_Size.y * 0.3f) : Vector2(m_Size.x * 0.15, -m_Size.y * 0.3f);
	Vector2 BulletOffset = m_Owner->GetDir().x > 0 ? Vector2(m_Size.x * 0.15f, -m_Size.y * 0.3f) : Vector2(m_Size.x * 0.15f, -m_Size.y * 0.3f);
	CSharedPtr<CBullet> Bullet = m_Owner->GetScene()->CreateObject<CBullet>("Bullet",
		PLAYER_BULLET_PROTO,
		Vector2(m_Pos + BulletOffset),
		Vector2(50.f, 50.f));

	m_GunInfo.m_BulletsLoaded -= 1;

	// Angle 
	float Angle = GetAngle(Bullet->GetPos(), TargetPos);
	Bullet->SetDir(Angle);
	// Damage 
	Bullet->SetBulletDamage(OwnerAttackDamage + (float)m_GunInfo.m_Damage);
	// Collision Profile
	CCollider* BulletBody = Bullet->FindCollider("Body");
	BulletBody->SetCollisionProfile("PlayerAttack");

	// Casing
	CreateCasing(Bullet);

	// Bullet Effect
	CreateBulletEffect();

}

void CGun::MonsterFire(Vector2 TargetPos, float OwnerAttackDamage)
{
	// Adjust Gun Texture
	AdjustGunTexture();

	CScene* Scene = m_Owner->GetScene();
	// Vector2 BulletOffset = m_Owner->CheckCurrentAnimation(MONSTER_RIGHT_ATTACK) ? Vector2(m_Size.x * 0.15, -m_Size.y * 0.3f) : Vector2(m_Size.x * 0.15, -m_Size.y * 0.3f);
	Vector2 BulletOffset = m_Owner->GetDir().x > 0 ? Vector2(m_Size.x * 0.15f, -m_Size.y * 0.3f) : Vector2(m_Size.x * 0.15f, -m_Size.y * 0.3f);
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
	CreateCasing(Bullet);

	// Bullet Effect
	CreateBulletEffect();
}

void CGun::CreateCasing(CBullet* Bullet)
{
	// Casing Effect
	// Casing Offset Set According To Owner Dir
	Vector2 CasingOffset   = m_Owner->GetDir().x > 0 ? Vector2(-m_Size.x * 0.7f, -m_Size.y * 0.5f) : Vector2(m_Size.x * 0.4f, -m_Size.y * 0.5f);
	Vector2 CasingPos      = m_Pos + CasingOffset;

	// DirX Will Set Differently according to "CasingName"
	std::string CasingName = m_Owner->GetDir().x > 0 ? "CasingLeft" : "CasingRight";
	CSharedPtr<CEffectCasing> Casing = m_Owner->GetScene()->CreateObject<CEffectCasing>(CasingName,
		EFFECT_CASING_PROTO,
		CasingPos,
		Vector2(20.f, 20.f));

	// Owner Left : Casing Right / Owner Right : Casing Left
	float CasingDir        = m_Owner->GetDir().x > 0 ? -20.f : 20.f;
	Casing->SetDirX(CasingDir);

	switch (m_GunInfo.m_GunClass)
	{
		case Light:
		{
			if (m_Owner->GetDir().x < 0) // Owner Lft --> Right Casing
				Casing->SetTexture("LightCasing", TEXT(TEXTURE_CASING_L_TOWARD_R));
			else // Owner Right --> Left Casing
				Casing->SetTexture("LightCasing", TEXT(TEXTURE_CASING_L_TOWARD_L));
		}
		break;
		case Medium:
		{
			if (m_Owner->GetDir().x < 0) // 왼쪽 --> 오른쪽 Casing
				Casing->SetTexture("MediumCasing", TEXT(TEXTURE_CASING_M_TOWARD_R));
			else
				Casing->SetTexture("MediumCasing", TEXT(TEXTURE_CASING_M_TOWARD_L));
		}
		break;
		case Heavy:
		{
			if (m_Owner->GetDir().x < 0) // 왼쪽 --> 오른쪽 Casing
				Casing->SetTexture("HeavyCasing", TEXT(TEXTURE_CASING_H_TOWARD_R));
			else
				Casing->SetTexture("HeavyCasing", TEXT(TEXTURE_CASING_H_TOWARD_L));
		}
		break;
	}
	Casing->SetTextureColorKey(255, 255, 255);

	//CDamageFont* DamageFont = m_Scene->CreateObject<CDamageFont>("DamageFont", CasingPos);
	//DamageFont->SetDamageNumber(30);
}

void CGun::CreateBulletEffect()
{
	Vector2 BulletStartOffset = m_Owner->GetDir().x > 0 ? Vector2(m_Size.x * 0.7f, -m_Size.y * 0.4f) : Vector2(-m_Size.x * 0.7f, -m_Size.y * 0.4f);
	Vector2 BulletStartPos = m_Pos + BulletStartOffset;

	CSharedPtr<CEffectBulletStart> Casing = m_Owner->GetScene()->CreateObject<CEffectBulletStart>("BulletEffect",
		EFFECT_BULLETSTART_PROTO,
		BulletStartPos,
		Vector2(20.f, 20.f));

}

void CGun::ShowNoBulletSign()
{
	CScene* Scene = m_Owner->GetScene();
	Vector2 CameraPos = m_Owner->GetScene()->GetCamera()->GetPos();
	CSharedPtr<CEffectText> NoBulletText = Scene->CreateObject<CEffectText>(
		"CEffectText",
		EFFECT_TEXT_PROTO,
		Vector2(m_Owner->GetPos().x-50.f,m_Owner->GetPos().y-m_Owner->GetSize().y*0.8f),
		Vector2(50.f,10.f));
	NoBulletText->SetText(TEXT("NO BULLET"));
	NoBulletText->SetTextColor(255, 0, 0);
}

void CGun::AdjustGunTexture()
{
	if (m_Owner)
	{
		Vector2 OwnerPos = m_Owner->GetPos();
		Vector2 OwnerSize = m_Owner->GetSize();

		// 위치 조정 
		SetPos(m_Owner->GetPos().x, m_Owner->GetPos().y - m_Owner->GetSize().y * 0.3f);

		// 방향 조정 
		if (m_Owner->GetDir().x < 0)
		{
			SetTexture(m_TextureImgNames[ETexture_Dir::Texture_Left]);
			SetOffset(-OwnerSize.x * 0.1f, 0);
			// 공격중이라면
		}
		else
		{
			SetTexture(m_TextureImgNames[ETexture_Dir::Texture_Right]);
			SetOffset(OwnerSize.x * 0.1f, 0);
		}
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
	AdjustGunTexture();
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
