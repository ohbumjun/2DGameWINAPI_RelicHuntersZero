
#include "Character.h"
#include "../Scene/Camera.h"
#include "../GameManager.h"
#include "../Scene/Scene.h"
#include "../Object/Gun.h"
#include "../Object/WallObject.h"
#include "../Collision/ColliderBox.h"
#include "EffectShield.h"

CCharacter::CCharacter() : 
	m_CharacterInfo{},
	m_GunEquipment{},
	m_CurrentGun(nullptr),
	m_HitEnable(false),
	m_HitTime(0.f),
	m_HitDir(Vector2(0.f,0.f)),
	m_ShieldEnable(false),
	m_FireEnable(true),
	m_ShieldTime(0.f),
	m_ShieldTimeMax(10.f),
	m_ShieldEnableTime(2.f),
	m_Shield(nullptr),
	m_FireTime(0.f),
	m_FireTimeMax(1.f),
	m_mapAnimName{}
{
	m_ObjType = EObject_Type::Character;
	m_MoveSpeed = m_CharacterInfo.MoveSpeed;
}

CCharacter::CCharacter(const CCharacter &obj) : CGameObject(obj)
{
	m_CharacterInfo = obj.m_CharacterInfo;

	m_HitEnable    = false;
	m_HitTime      = 0.f;
	m_HitDir = Vector2(0.f, 0.f);

	m_ShieldEnable = false;
	m_ShieldTime = 0.f;
	m_ShieldTimeMax = 10.f;
	m_ShieldEnableTime = 2.f;

	m_FireTime    = obj.m_FireTime;
	m_FireTimeMax = obj.m_FireTimeMax;
	m_FireEnable = true;

	for (int i = 0; i < EGunClass::End; i++)
	{
		if (obj.m_GunEquipment[i])
		{
			m_GunEquipment[i] = obj.m_GunEquipment[i]->Clone();
			if (obj.m_GunEquipment[i]->GetName() == obj.m_CurrentGun->GetName())
			{
				m_CurrentGun = m_GunEquipment[i];
				m_FireTime    = m_CurrentGun->GetFireTime();
				m_FireTimeMax = m_CurrentGun->GetFireTimeMax();
			}
		}
	}
	
}

CCharacter::~CCharacter()
{
	m_mapAnimName.clear();
}


void CCharacter::Start()
{
	CGameObject::Start();
}

bool CCharacter::Init()
{
	if (!CGameObject::Init())
		return false;

	return true;
}

void CCharacter::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	MoveWithinWorldResolution();
	
	if (m_CurrentGun)
		m_CurrentGun->Update(DeltaTime);

	// MP, HP Max 조절
	if (m_CharacterInfo.MP > m_CharacterInfo.MPMax)
		m_CharacterInfo.MP = m_CharacterInfo.MPMax;

	if (m_CharacterInfo.HP > m_CharacterInfo.HPMax)
		m_CharacterInfo.HP = m_CharacterInfo.HPMax;

	// Hit 조절 
	if (m_HitTime >= 0.f) m_HitTime -= DeltaTime;
	if (m_HitEnable)
	{
		HitMove();
		if (m_HitTime < 0.f) HitEnd();
	}
}

void CCharacter::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
	if (m_CurrentGun)
		m_CurrentGun->PostUpdate(DeltaTime);
}

void CCharacter::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CCharacter::PrevRender()
{
	CGameObject::PrevRender();
	if (m_CurrentGun)
		m_CurrentGun->PrevRender();
}

void CCharacter::Render(HDC hDC)
{
	CGameObject::Render(hDC);
	if (m_CurrentGun)
		m_CurrentGun->Render(hDC);
}

CCharacter *CCharacter::Clone()
{
	return nullptr;
}

int CCharacter::SetDamage(int Damage)
{
	Damage = CGameObject::SetDamage(Damage);
	m_CharacterInfo.HP -= Damage;
	return Damage;
}

void CCharacter::SetScene(CScene* Scene)
{
	CGameObject::SetScene(Scene);
	// Guns
	for (int i = 0; i < EGunClass::End; i++)
	{
		if (m_GunEquipment[i])
		{
			CGun* Gun = m_GunEquipment[i];
			Gun->SetScene(Scene);
			Gun->SetOwner(this);
			if (!m_Scene->FindObject(Gun))
				m_Scene->PushObjectToScene(Gun);
		}
	}
}

void CCharacter::ChangeHitAnimation()
{
	if (!m_HitEnable) return;
}

void CCharacter::ChangeIdleAnimation()
{
	if (m_HitEnable) return;
}

void CCharacter::ChangeRunAnimation()
{
	if (m_HitEnable) return;
}

void CCharacter::ChangeMoveAnimation()
{
	if (m_HitEnable) return;
}

void CCharacter::ChangeDeathAnimation()
{
}

void CCharacter::Move(const Vector2& Dir)
{
	// PreventWallMove();
	if (!m_HitEnable)
		CGameObject::Move(Dir);
}

void CCharacter::Move(const Vector2& Dir, float Speed)
{
	// PreventWallMove();
	if (!m_HitEnable)
		CGameObject::Move(Dir, Speed);
}

void CCharacter::MoveWithinWorldResolution()
{
	float Space = 10.f;
	Vector2 WorldResolution = m_Scene->GetCamera()->GetWorldResolution();
	if (m_Pos.x <= Space)
		m_Pos.x = Space;
	if (m_Pos.x + m_Size.x >= WorldResolution.x - Space)
		m_Pos.x = WorldResolution.x - Space - m_Size.x;
	if (m_Pos.y <= Space)
		m_Pos.y = Space;
	if (m_Pos.y + m_Size.y >= WorldResolution.y - Space)
		m_Pos.y = WorldResolution.y - Space - m_Size.y;
}

bool CCharacter::CollisionCheck()
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsCollisionListEmpty())
		{
			return true;
		}
	}
	return false;
}

bool CCharacter::ObstacleCollisionCheck() const
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->DidCollideWithObstacles())
			return true;
	}

	return false;
}

CGameObject* CCharacter::MonsterCollisionCheck()
{
	
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		CGameObject* Monster = (*iter)->IsCollisionWithMonster();
		if (Monster)
		{
			if (m_ShieldEnable)
			{
				CEffectShield* ShieldEffect = m_Scene->CreateObject<CEffectShield>("Shield", SHIELD_PROTO, m_Pos);
				ShieldEffect->SetShieldType(EShield_Type::Player);
				ShieldEffect->SetOwner(this);
				ShieldEffect->SetLifeTime(0.5f);
				return nullptr;
			}
			else
				return Monster;
		}
	}
	return nullptr;
}

std::vector<CGameObject*> CCharacter::WallCollisionCheck()
{
	std::vector<CGameObject*> vecWalls;
	vecWalls.reserve(m_ColliderList.size());
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		std::vector<CGameObject*> WallsCollide = (*iter)->IsCollisionWithWall();
		if (!WallsCollide.empty())
		{
			size_t WallsCollideN = WallsCollide.size();
			for(size_t i = 0 ; i < WallsCollideN;i++)
				vecWalls.push_back(WallsCollide[i]);
		}
	}
	return vecWalls;
}

void CCharacter::CollideBounceBack(Vector2 Dir)
{
	Vector2 OppDir = Dir;
	OppDir.Normalize();
	SetHitDir(OppDir);
	Hit();
}

/*
CCollider* CCharacter::GetColliderBox()
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == "Body")
			return (*iter);
	}
	return nullptr;
}
*/

RectInfo CCharacter::GetInterCollideRect(RectInfo Rect1, RectInfo Rect2)
{
	RectInfo Intersect;
	// max among left 
	Intersect.Left  = Rect1.Left > Rect2.Left ? Rect1.Left : Rect2.Left;
	// Min among Right 
	Intersect.Right = Rect1.Right < Rect2.Right ? Rect1.Right : Rect2.Right;
	// Max among y1
	Intersect.Top   = Rect1.Top > Rect2.Top ? Rect1.Top : Rect2.Top;
	// Min among y2
	Intersect.Bottom = Rect1.Bottom < Rect2.Bottom ? Rect1.Bottom : Rect2.Bottom;
	return Intersect;
}

bool CCharacter::PreventWallMove()
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

				if (TileMap->GetTile(Index)->GetTileOption() != ETileOption::Wall) continue;

				// 해당 Tile의 위치, size 정보를 가져온다 
				// TilePos : Tile의 위쪽을 의미한다 
				Vector2	TilePos = TileMap->GetTile(Index)->GetPos();
				Vector2	TileSize = TileMap->GetTile(Index)->GetSize();

				// 렉트충돌
				if (LT.x <= TilePos.x + TileSize.x &&
					LT.y <= TilePos.y + TileSize.y &&
					RB.x >= TilePos.x &&
					RB.y >= TilePos.y)
				{
					WallCollision = true;

					RectInfo CharRect, WallRect;
					CharRect.Left   = LT.x;
					CharRect.Right  = RB.x;
					CharRect.Top    = LT.y;
					CharRect.Bottom = RB.y;

					WallRect.Left   = TilePos.x;
					WallRect.Right  = TilePos.x + TileSize.x;
					WallRect.Top    = TilePos.y;
					WallRect.Bottom = TilePos.y + TileSize.y;
					
					RectInfo Intersect = GetInterCollideRect(CharRect, WallRect);

					float UpDownSpace = Intersect.Bottom - Intersect.Top;
					float LeftRightSpace = Intersect.Right - Intersect.Left;

					// 좌우 이동 
					if (LeftRightSpace <= 5.f)
					{
						// Going Right
						if (TilePos.x > LT.x)
						{
							m_Pos.x -= LeftRightSpace;
							m_Velocity.x += LeftRightSpace;
						}
						// Going Left
						if (TilePos.x <= LT.x)
						{
							m_Pos.x += LeftRightSpace;
							m_Velocity.x += LeftRightSpace;
						}
					}
					// 상하 이동
					if (UpDownSpace <= 5.f)
					{
						// Going Down
						if (TilePos.y > LT.y)
						{
							m_Pos.y -= UpDownSpace;
							m_Velocity.y += UpDownSpace;
						}
						// Going Up
						if (TilePos.y <= LT.y)
						{
							m_Pos.y += UpDownSpace;
							m_Velocity.y += UpDownSpace;
						}
					}
				}
			}
		}
	}

	return WallCollision;
	/*
	std::vector<CGameObject*> CollideWalls = WallCollisionCheck();
	if (!CollideWalls.empty())
	{
		size_t WallNums = CollideWalls.size();
		for (size_t i = 0; i < WallNums; i++)
		{
			CGameObject* CollideWall = CollideWalls[i];
			CWallObject* WallObj = (CWallObject*)CollideWall;
			Vector2 WallObjPos = WallObj->GetPos();
			RECT rcInter;

			CColliderBox* CharCollideBox = (CColliderBox*)GetColliderBox();
			CColliderBox* WallCollideBox = (CColliderBox*)WallObj->GetColliderBox();
			RectInfo CharRect = CharCollideBox->GetInfo();
			RectInfo WallRect = WallCollideBox->GetInfo();
			RectInfo Intersect = GetInterCollideRect(CharRect, WallRect);
			float UpDownSpace = Intersect.Bottom - Intersect.Top;
			float LeftRightSpace = Intersect.Right - Intersect.Left;

			// 좌우 이동 
			if (LeftRightSpace <= 5.f)
			{
				// Going Right
				if (WallObjPos.x > m_Pos.x)
					m_Pos.x -= LeftRightSpace;
				// Going Left
				if (WallObjPos.x <= m_Pos.x)
					m_Pos.x += LeftRightSpace;
			}
			// 상하 이동
			if (UpDownSpace <= 5.f)
			{
				// Going Down
				if (WallObjPos.y > m_Pos.y)
					m_Pos.y -= UpDownSpace;
				// Going Up
				if (WallObjPos.y <= m_Pos.y)
					m_Pos.y += UpDownSpace;
			}
		}
	}
	*/

}

void CCharacter::SetHitDir(Vector2 Dir)
{
	m_HitDir = Dir;
	m_HitDir.Normalize();
}

void CCharacter::Hit()
{
	m_HitTime = HIT_TIME;
	m_HitEnable = true;
	// m_Pos += m_HitDir * m_Size.Length() * 0.1f;
	ChangeHitAnimation(); //
}
void CCharacter::HitMove()
{
	Vector2	CurrentMove = m_HitDir * HIT_SPEED * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale;
	m_Velocity += CurrentMove;
	m_Pos += CurrentMove;
	PreventWallMove();
}

void CCharacter::HitEnd()
{
	m_HitEnable = false;
	ChangeIdleAnimation();
}

void CCharacter::CharacterDestroy()
{
	Destroy();

	// Destroy Equipments 
	for (int i = 0; i < EGunClass::End; i++)
	{
		if (m_GunEquipment[i])
			m_GunEquipment[i]->Destroy();
	}
}

void CCharacter::SetInitGun(CGun* Gun)
{
	EGunClass GunClass = Gun->GetGunClass();
	m_GunEquipment[GunClass] = Gun;
	m_CurrentGun = Gun;
}

CGun* CCharacter::Equip(CGun* Gun)
{
	EGunClass GunClass = EGunClass::Light;
	switch (Gun->GetGunClass())
	{
	case EGunClass::Light :
		GunClass = Light;
		break;
	case EGunClass::Medium:
		GunClass = Medium;
		break;
	case EGunClass::Heavy :
		GunClass = Heavy;
		break;
	}
	// Chnage Equipment 
	CGun*ExistingGun = m_GunEquipment[GunClass];
	m_GunEquipment[GunClass] = Gun;

	// Set Current Gun
	if (m_CurrentGun) m_CurrentGun->Destroy();
	m_CurrentGun = m_GunEquipment[GunClass];
	m_CurrentGun->RemoveGunCollider();

	// Attack Dist Setting
	m_CurrentGun->SetBulletDistance(m_CharacterInfo.AttackDistance);

	// Set Owner, Pos 
	Gun->SetOwner(this);

	// Fire Time Setting
	m_FireTime = m_CurrentGun->GetFireTime();
	m_FireTimeMax = m_CurrentGun->GetFireTimeMax();

	return ExistingGun;
}


