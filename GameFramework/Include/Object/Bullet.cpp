#include "Bullet.h"
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderSphere.h"
#include "EffectHit.h"
#include "EffectShield.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../UI//NumberWidget.h"
#include "../UI/WidgetComponent.h"
#include "DamageFont.h"

CBullet::CBullet() : m_Owner(nullptr)
{
	m_ObjType   = EObject_Type::Bullet;
	m_BulletType = EBullet_Type::Light;
	m_Dir.x     = 1.f;
	m_Dir.y     = 0.f;
	m_Damage    = NORMAL_MONSTER_ATTACK;
	m_Distance  = NORMAL_BULLET_DISTANCE;
	m_MoveSpeed = BULLET_SPEED;
}

CBullet::CBullet(const CBullet& obj) : CGameObject(obj)
{
	m_Damage = obj.m_Damage;
	m_Dir = obj.m_Dir;
	m_Distance = obj.m_Distance;
	// m_TimeScale = obj.m_TimeScale;
	m_TimeScale = 1.f;
	if (obj.m_Owner)
		m_Owner = obj.m_Owner;
	else
		m_Owner = nullptr;
}

CBullet::~CBullet()
{
}

void CBullet::SetBulletType(EBullet_Type BType)
{
	m_BulletType = BType;
	switch (m_BulletType)
	{
	case EBullet_Type::Light:
		SetCurrentAnimation(BULLET_LIGHT);
		break;
	case EBullet_Type::Medium:
		SetCurrentAnimation(BULLET_MEDIUM);
		break;
	case EBullet_Type::Heavy:
		SetCurrentAnimation(BULLET_HEAVY);
		break;
	case EBullet_Type::Boss:
		SetCurrentAnimation(BULLET_RIGHT_BOSS);
		m_MoveSpeed = 200.f;
		break;
	}
}

void CBullet::Start()
{
	CGameObject::Start();
	CCollider* Body = FindCollider("Body");
	Body->SetCollisionBeginFunction<CBullet>(this, &CBullet::CollisionBegin);
}

bool CBullet::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);

	CColliderSphere* Body = AddCollider<CColliderSphere>("Body");
	Body->SetRadius(25.f);
	Body->SetOffset(0.f, 0.f);

	CreateAnimation();
	AddAnimation(BULLET_LIGHT,true,0.1f);
	AddAnimation(BULLET_MEDIUM,true,0.1f);
	AddAnimation(BULLET_HEAVY,true,0.1f);
	AddAnimation(BULLET_LEFT_BOSS,true,0.1f);
	AddAnimation(BULLET_RIGHT_BOSS,true,0.1f);
	return true;
}

void CBullet::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	Vector2 Dir = m_Dir;
	Dir.Normalize();
	Move(Dir);

	UpdateDistance();
	UpdateBossBullet();

	WallCollision(); 
}

void CBullet::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CBullet::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CBullet::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CBullet* CBullet::Clone()
{
	return new CBullet(*this);
}

RectInfo CBullet::GetInterCollideRect(RectInfo Rect1, RectInfo Rect2)
{
	RectInfo Intersect;
	// max among left 
	Intersect.Left = Rect1.Left > Rect2.Left ? Rect1.Left : Rect2.Left;
	// Min among Right 
	Intersect.Right = Rect1.Right < Rect2.Right ? Rect1.Right : Rect2.Right;
	// Max among y1
	Intersect.Top = Rect1.Top > Rect2.Top ? Rect1.Top : Rect2.Top;
	// Min among y2
	Intersect.Bottom = Rect1.Bottom < Rect2.Bottom ? Rect1.Bottom : Rect2.Bottom;
	return Intersect;
}


void CBullet::WallCollision()
{
	bool WallCollision = false;
	int LTIndexX, LTIndexY, RBIndexX, RBIndexY;
	CTileMap* TileMap = m_Scene->GetTileMap();
	if (TileMap)
	{
		// 좌상단, 우하단 위치 구하기 
		// 충돌해서 겹치는 부분의 

		Vector2 LT = m_Pos - m_Pivot * m_Size;
		Vector2 RB = LT + m_Size;

		// LT ,RB 영억이 바로 충돌 영역 
		// 이 부분에 있는 tile 들을 구해줄 것이다
		LTIndexX = TileMap->GetOriginTileIndexX(LT.x);
		LTIndexY = TileMap->GetOriginTileIndexY(LT.y);
		RBIndexX = TileMap->GetOriginTileIndexX(RB.x);
		RBIndexY = TileMap->GetOriginTileIndexY(RB.y);

		// 범위 제한 ( 혹시나 )
		LTIndexX = LTIndexX < 0 ? 0 : LTIndexX;
		LTIndexY = LTIndexY < 0 ? 0 : LTIndexY;

		RBIndexX = RBIndexX > TileMap->GetTileCountX() - 1 ? TileMap->GetTileCountX() - 1 : RBIndexX;
		RBIndexY = RBIndexY > TileMap->GetTileCountY() - 1 ? TileMap->GetTileCountY() - 1 : RBIndexY;

		for (int i = RBIndexY; i >= LTIndexY; --i)
		{
			for (int j = LTIndexX; j <= RBIndexX; ++j)
			{
				// Tile의 일차원 배열상의 idx 
				int	Index = i * TileMap->GetTileCountX() + j;
				if (TileMap->GetTile(Index)->GetTileOption() == ETileOption::Wall)
				{
					Destroy();
					CEffectHit* Hit = m_Scene->CreateObject<CEffectHit>("HitEffect", EFFECT_HIT_PROTO,
						m_Pos, Vector2(178.f, 164.f));
					m_Scene->GetSceneResource()->SoundPlay("Fire");
				}
			}
		}
	}
}

void CBullet::CollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime)
{
	Destroy();

	MakeHitEffect();

	CGameObject* DestOwner = Dest->GetOwner();
	EObject_Type DestType = DestOwner->GetObjType();
	Vector2 DestSize = DestOwner->GetSize();

	if (DestType == EObject_Type::Player ||
		DestType == EObject_Type::Monster)
	{
		CCharacter* DestChar  = (CCharacter*)Dest->GetOwner();
		bool DestShieldEnable = DestChar->GetShieldEnable();
		if(!DestShieldEnable)
		{
			HitObject(DestChar);
			DamageObject(Dest);
		}
		if (DestShieldEnable && DestType == EObject_Type::Player)
		{
			ShieldObject(DestOwner);
		}
	}
}

void CBullet::UpdateDistance()
{
	m_Distance -= GetMoveSpeedFrame();
	if (m_Distance <= 0.f) Destroy();
}

void CBullet::UpdateBossBullet()
{
	if (m_BulletType == EBullet_Type::Boss)
	{
		CPlayer* Player = (CPlayer*)m_Scene->GetPlayer();
		float Angle = GetAngle(m_Pos, Player->GetPos());
		SetDir(Angle);
	}
}


void CBullet::HitObject(CCharacter* const DestChar)
{//
	// Disable If if Boss Monster 
	CMonster* DestM = (CMonster*)DestChar;
	if (DestM->GetMonsterType() == EMonster_Type::Boss) return;
	// Hit 
	DestChar->SetHitDir(m_Dir);
	DestChar->Hit();
}

void CBullet::DamageObject(CCollider* const Dest)
{
	// Damage 
	int Armor = 0;
	if (Dest->GetOwner()->GetObjType() == EObject_Type::Monster ||
		Dest->GetOwner()->GetObjType() == EObject_Type::Player)
		Armor = Dest->GetOwner()->GetArmor();
	// Damage Font
	CDamageFont* DamageFont = m_Scene->CreateObject<CDamageFont>("DamageFont", DAMAGEFONT_PROTO, m_Pos);
	DamageFont->SetDamageNumber((int)(m_Damage - Armor));
	// Damage
	Dest->GetOwner()->SetDamage(m_Damage - Armor);
}

void CBullet::ShieldObject(CGameObject* const  DestOwner)
{
	CEffectShield* ShieldEffect = m_Scene->CreateObject<CEffectShield>("Shield", SHIELD_PROTO, m_Pos);
	ShieldEffect->SetShieldType(EShield_Type::Player);
	ShieldEffect->SetOwner(DestOwner);
	ShieldEffect->SetLifeTime(0.5f);
}

void CBullet::MakeHitEffect()
{
	CEffectHit* HitEffect = m_Scene->CreateObject<CEffectHit>("HitEffect", EFFECT_HIT_PROTO,
			m_Pos, Vector2(178.f, 164.f));
	if (m_Owner->GetGunClass() == EGunClass::Boss)
		HitEffect->SetIsBossGun();
	m_Scene->GetSceneResource()->SoundPlay("Fire");
}
