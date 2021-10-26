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
	m_GunInfo.m_Damage         = NORMAL_MONSTER_ATTACK;
	m_GunInfo.m_BulletsLoaded  = PISTOL_BULLET_NUM;
	m_GunInfo.m_BulletsFullNum = PISTOL_BULLET_NUM;
	m_GunInfo.m_BulletLoadTime = 0.1f;
	m_GunInfo.m_BulletDistance = NORMAL_BULLET_DISTANCE;
	
	m_ObjType = EObject_Type::Weapon;
	m_FireTime    = 0.f;
	m_FireTimeMax = 1.f;
}

CGun::CGun(const CGun& obj) : CGameObject(obj)
{
	m_Owner       = nullptr;
	m_GunInfo     = obj.m_GunInfo;
	m_FireToggle  = false;

	m_FireTime    = obj.m_FireTime;
	m_FireTimeMax = obj.m_FireTimeMax;

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
		// ���
		ShowNoBulletSign();
		return;
	}
	
	// Adjust Gun Texture
	AdjustGunTexture();

	// Offet�� ��� , Gun�� Offset ��ġ�� ����� �Ѵ� 
	CScene* Scene = m_Owner->GetScene();
	// Vector2 BulletOffset = m_Owner->CheckCurrentAnimation(PLAYER_RIGHT_ATTACK) ? Vector2(m_Size.x * 0.15, -m_Size.y * 0.3f) : Vector2(m_Size.x * 0.15, -m_Size.y * 0.3f);
	Vector2 BulletOffset = m_Owner->GetDir().x > 0 ? Vector2(m_Size.x * 0.15f, -m_Size.y * 0.3f) : Vector2(m_Size.x * 0.15f, -m_Size.y * 0.3f);
	CSharedPtr<CBullet> Bullet = m_Owner->GetScene()->CreateObject<CBullet>("Bullet",
		PLAYER_BULLET_PROTO,
		Vector2(m_Pos + BulletOffset),
		Vector2(50.f, 50.f));
	Bullet->SetOwner(this);
	// Bullet Type
	EBullet_Type BulletType = MatchBulletToGun();
	Bullet->SetBulletType(BulletType);
	// Bullet Nums
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
	// this
	CScene* Scene = m_Owner->GetScene();
	Vector2 BulletOffset = m_Owner->GetDir().x > 0 ? Vector2(m_Size.x * 0.15f, -m_Size.y * 0.3f) : Vector2(m_Size.x * 0.15f, -m_Size.y * 0.3f);
	CSharedPtr<CBullet> Bullet = Scene->CreateObject<CBullet>("Bullet",
		MONSTER_BULLET_PROTO,
		Vector2(m_Pos + BulletOffset),
		Vector2(50.f, 50.f));
	EBullet_Type BulletType = MatchBulletToGun();
	// Bullet Type
	Bullet->SetBulletType(BulletType);
	// if Boss Bullet, Set Owner
	if (m_GunInfo.m_GunClass == EGunClass::Boss)
	{
		if (m_Dir.x < 0)
			Bullet->SetCurrentAnimation(BULLET_LEFT_BOSS);
		else
			Bullet->SetCurrentAnimation(BULLET_RIGHT_BOSS);
	}
	Bullet->SetOwner(this);
	// Bullet Damage
	Bullet->SetBulletDamage(OwnerAttackDamage + (float)m_GunInfo.m_Damage);
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

void CGun::SkillSlowMotionAttack()
{
	for (int i = 0; i < 4; i++)
	{
		for (float f = 0.0f; f < 2 * M_PI; f += M_PI / 30.0f)
		{
			CSharedPtr<CBullet> Bullet = m_Scene->CreateObject<CBullet>("Bullet",
				PLAYER_BULLET_PROTO,
				Vector2(
					(m_Pos.x - m_Offset.x) + m_Size.Length() * i * cos(f),
					(m_Pos.y - m_Offset.y) + m_Size.Length() * i * sin(f)),
				Vector2(m_Size.x, m_Size.y));
			Bullet->SetOwner(this);

			CPlayer* Owner = (CPlayer*)m_Owner;
			CGameObject* ClosestMonster = Owner->FindClosestTarget(Bullet->GetPos());
			if (ClosestMonster)
			{
				float AngleBtwBulletMonster = GetAngle(Bullet->GetPos(), ClosestMonster->GetPos());
				Bullet->SetDir(AngleBtwBulletMonster);
			}
			else
			{
				Bullet->SetDir(m_Dir);
			}

			int AttackDamage = Owner->GetAttack();
			Bullet->SetBulletDamage((float)AttackDamage);
			Bullet->SetTimeScale(m_TimeScale);
		}
	}
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
	float CasingDir        = m_Owner->GetDir().x > 0 ? -2.f : 2.f;
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
			if (m_Owner->GetDir().x < 0) // ���� --> ������ Casing
				Casing->SetTexture("MediumCasing", TEXT(TEXTURE_CASING_M_TOWARD_R));
			else
				Casing->SetTexture("MediumCasing", TEXT(TEXTURE_CASING_M_TOWARD_L));
		}
		break;
		case Heavy:
		{
			if (m_Owner->GetDir().x < 0) // ���� --> ������ Casing
				Casing->SetTexture("HeavyCasing", TEXT(TEXTURE_CASING_H_TOWARD_R));
			else
				Casing->SetTexture("HeavyCasing", TEXT(TEXTURE_CASING_H_TOWARD_L));
		}
		break;
	}
	Casing->SetTextureColorKey(255, 255, 255);

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
		// ��ġ ���� 
		SetPos(m_Owner->GetPos().x, m_Owner->GetPos().y - m_Owner->GetSize().y * 0.3f);
		// ���� ���� 
		if (m_Owner->GetDir().x < 0)
		{
			SetTexture(m_TextureImgNames[ETexture_Dir::Texture_Left]);
			SetOffset(-OwnerSize.x * 0.1f, 0);
			// �������̶��
		}
		else
		{
			SetTexture(m_TextureImgNames[ETexture_Dir::Texture_Right]);
			SetOffset(OwnerSize.x * 0.1f, 0);
		}
	}
}

EBullet_Type CGun::MatchBulletToGun()
{
	switch (m_GunInfo.m_GunClass)
	{
	case Light:
		return EBullet_Type::Light;
	case Medium:
		return EBullet_Type::Medium;
	case Heavy:
		return EBullet_Type::Heavy;
	case Boss:
		return EBullet_Type::Boss;
	default:
		return EBullet_Type::Light;
	}
}

void CGun::RemoveGunCollider()
{
	CColliderSphere* ColliderSphere = (CColliderSphere*)FindCollider("Body");
	if(ColliderSphere)
		ColliderSphere->Destroy();
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
	Body->SetRadius(100.f);
	Body->SetOffset(0.f, 0.f);

	return true;
}

void CGun::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	if (m_Owner)
		m_Dir = m_Owner->GetDir();
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
