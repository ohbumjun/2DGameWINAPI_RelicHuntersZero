#pragma once

#include "../GameInfo.h"
#include "../Object/GameObject.h"
#include "../UI/UIWindow.h"
#include "../Map/TileMap.h"
#include "../Map/Tile.h"
#include "../Object/Player.h"

class CScene
{
	friend class CSceneManager;

protected:
	CScene();
	virtual ~CScene();

protected:
	class CSceneResource* m_Resource;
	class CSceneCollision* m_Collision;
	class CCamera* m_Camera;

public:
	class CSceneResource* GetSceneResource()	const;
	class CSceneCollision* GetSceneCollision()	const;
	class CCamera* GetCamera()	const;

private:
	// 맵 목록 
	std::vector<class CMapBase*> m_MapList;
	CSharedPtr<CGameObject>	m_Player;
	std::list<CSharedPtr<CGameObject>>	m_ObjList;
	CGameObject** m_RenderArray;
	int		m_RenderCount;
	int		m_RenderCapacity;
	std::unordered_map<std::string, CSharedPtr<CGameObject>>	m_mapPrototype;
	//std::list<CSharedPtr<CUIWindow>>	m_UIList;
	CUIWindow** m_UIArray;
	int		m_UICount;
	int		m_UICapacity;
	class CTileMap* m_TileMap;

	// Player Laser 충돌 위치 : Vector2
	// Vector2 m_LaserCollidePos;

public:
	CGameObject* FindObject(const std::string& Name);
	CGameObject* FindObject(CGameObject* Obj);
	CGameObject* FindClosestMonsterToPlayer(Vector2 PlayerPos);
	void DestroyAllAttackObjects();
	CGameObject* SetPlayer(const std::string& Name);
	CGameObject* SetPlayer(CGameObject* Player);
	CGameObject* GetPlayer()	const
	{
		return m_Player;
	}
	CTileMap* GetTileMap()	const
	{
		return m_TileMap;
	}

// Animation Setting 
public :
	void SetPlayerAnimation();
	void SetItemsAnimation();
	void SetBulletsAnimation();
	void SetCollideAnimation();
	void SetMouseAnimation();
	void SetSkillAnimation();
// Monster Setting 
public :
	void SetLevel1MonsterAnimation();
	void SetDuck1MonsterAnimation();
	void SetDuck2MonsterAnimation();
	void SetDuck3MonsterAnimation();
	template <typename T>
	void SetMonsterOnTileMap(
		const std::string&  MonsterProtoEasy,
		const std::string& MonsterProtoMid,
		const std::string& MonsterProtoHard
		);
public:
	void SetLevel2MonsterAnimation();
	void SetTurtle1MonsterAnimation();
	void SetTurtle2MonsterAnimation();
	void SetTurtle3MonsterAnimation();
// Gun Setting 
public :
	void SetBasicObjectGuns();
// UI Setiing
public :
	void SetBasicUIs();

// Proto Setting
public :
	void SetBasicProtoTypes();
	void SetBasicProtoGuns();
public:
	virtual bool Init();
	virtual bool Update(float DeltaTime);
	virtual bool PostUpdate(float DeltaTime);
	virtual bool Collision(float DeltaTime);
	virtual bool Render(HDC hDC);
// Objects
public :
	void PushObjectToScene(CGameObject* Obj)
	{
		m_ObjList.push_back(Obj);
	}
public:
	static int SortY(const void* Src, const void* Dest);
	static int SortZOrder(const void* Src, const void* Dest);
	static int SortZOrderMap(const void* Src, const void* Dest);
private:
	CGameObject* FindPrototype(const std::string& Name);
// Wall Object
public :
	void SetObjectsToWall();

public:
	class CPlayer* CreatePlayer(const std::string& Name,
		const Vector2& Pos = Vector2(0.f, 0.f),
		const Vector2& Size = Vector2(100.f, 100.f));

	template <typename T>
	T* CreateObject(const std::string& Name,
		const Vector2& Pos = Vector2(0.f, 0.f),
		const Vector2& Size = Vector2(100.f, 100.f))
	{
		T* Obj = new T;

		Obj->SetScene(this);
		Obj->SetPos(Pos);
		Obj->SetSize(Size);
		Obj->SetName(Name);

		if (!Obj->Init())
		{
			SAFE_DELETE(Obj);
			return nullptr;
		}

		m_ObjList.push_back(Obj);

		return Obj;
	}
	
	template <typename T>
	T* CreatePrototype(const std::string& Name)
	{
		T* Obj = new T;

		Obj->SetScene(this);
		Obj->SetName(Name);
		Obj->SetProtoTypeName(Name);

		if (!Obj->Init())
		{
			SAFE_DELETE(Obj);
			return nullptr;
		}

		m_mapPrototype.insert(std::make_pair(Name, Obj));

		return Obj;
	}

	template <typename T>
	T* CreateObject(const std::string& Name, const std::string& PrototypeName,
		const Vector2& Pos = Vector2(0.f, 0.f),
		const Vector2& Size = Vector2(100.f, 100.f))
	{
		CGameObject* Prototype = FindPrototype(PrototypeName);

		if (!Prototype)
			return nullptr;

		T* Obj = (T*)Prototype->Clone();


		Obj->SetScene(this);
		Obj->SetPos(Pos);
		Obj->SetSize(Size);
		Obj->SetName(Name);

		m_ObjList.push_back(Obj);

		return Obj;
	}

	template <typename T>
	T* CreateUIWindow(const std::string& Name)
	{
		T* Window = new T;

		Window->SetName(Name);
		Window->SetScene(this);

		if (!Window->Init())
		{
			SAFE_DELETE(Window);
			return nullptr;
		}

		if (m_UICount == m_UICapacity)
		{
			m_UICapacity *= 2;

			CUIWindow** Array = new CUIWindow * [m_UICapacity];

			memcpy(Array, m_UIArray, sizeof(CUIWindow*) * m_UICount);

			SAFE_DELETE_ARRAY(m_UIArray);

			m_UIArray = Array;
		}

		m_UIArray[m_UICount] = Window;
		++m_UICount;

		return Window;
	}

	template <typename T>
	T* FindUIWindow(const std::string& Name)
	{
		for (int i = 0; i < m_UICount; ++i)
		{
			if (m_UIArray[i]->GetName() == Name)
				return (T*)m_UIArray[i];
		}

		return nullptr;
	}

	template <typename T>
	T* CreateMap(const std::string& Name,
		const Vector2& Pos = Vector2(0.f, 0.f),
		const Vector2& Size = Vector2(1000.f, 1000.f))
	{
		T* Map = new T;

		Map->SetScene(this);
		Map->SetPos(Pos);
		Map->SetSize(Size);
		Map->SetName(Name);
		
		// TileMap 세팅해주기 
		if (typeid(T).hash_code() ==
			typeid(CTileMap).hash_code())
			m_TileMap = (CTileMap*)Map;

		if (!Map->Init())
		{
			SAFE_DELETE(Map);
			return nullptr;
		}

		m_MapList.push_back(Map);

		return Map;
	}
};

template<typename T>
inline void CScene::SetMonsterOnTileMap(
	const std::string& MonsterProtoEasy,
	const std::string& MonsterProtoMid,
	const std::string& MonsterProtoHard)
{
	T* Monster = nullptr;
	CGun* PistolGun = nullptr;
	CTileMap* TileMap = GetTileMap();

	if (TileMap)
	{
		// Tile Idx 
		int LeftIndexX, TopIndexY, RightIndexX, BottomIndexY;
		LeftIndexX   = TileMap->GetOriginTileIndexX(0);
		// RightIndexX = TileMap->GetOriginTileIndexX(TileMap->GetTileCountX() - 1);
		RightIndexX  = TileMap->GetTileCountX() - 1;
		TopIndexY    = TileMap->GetOriginTileIndexY(0);
		// BottomIndexY = TileMap->GetOriginTileIndexY(TileMap->GetTileCountY() - 1);
		BottomIndexY = TileMap->GetTileCountY() - 1;

		int EasyMNum = 0, MidMNum = 0, HardMNum = 0;

		// From Down to Up
		for (int i = TopIndexY; i <= BottomIndexY; i++)
		{
			for (int j = LeftIndexX; j <= RightIndexX; j++)
			{
				Vector2 TilePos = TileMap->GetTile(j, i)->GetPos();
				ETileOption TileOption = TileMap->GetTile(j, i)->GetTileOption();

				if (TileOption == ETileOption::MonsterLight)
				{

					PistolGun = CreateObject<CGun>(GUN_PISTOL_LIGHT, GUN_PISTOL_LIGHT_PROTO);
					Monster = CreateObject<T>(std::to_string(EasyMNum), MonsterProtoEasy,
						TilePos);
					Monster->Equip(PistolGun);
					Monster->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, NORMAL_MONSTER_HP_MAX,
						NORMAL_MONSTER_MP_MAX, 1, 100, 100, 100, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
					EasyMNum += 1;
					// m_Pos.y = (TilePosY + TileSizeY) + m_Pivot.y * m_Size.y + 0.1f;
					break;
				}
				if (TileOption == ETileOption::MonsterMedium)
				{
					PistolGun = CreateObject<CGun>(GUN_PISTOL_MEDIUM, GUN_PISTOL_MEDIUM_PROTO);
					Monster = CreateObject<T>(std::to_string(MidMNum), MonsterProtoMid,
						TilePos);
					Monster->Equip(PistolGun);
					Monster->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, NORMAL_MONSTER_HP_MAX,
						NORMAL_MONSTER_MP_MAX, 1, 100, 100, 100, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
					MidMNum += 1;
					break;
				}
				if (TileOption == ETileOption::MonsterHard)
				{
					PistolGun = CreateObject<CGun>(GUN_PISTOL_HEAVY, GUN_PISTOL_HEAVY_PROTO);
					Monster = CreateObject<T>(std::to_string(HardMNum), MonsterProtoHard,
						TilePos);
					Monster->Equip(PistolGun);
					Monster->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, NORMAL_MONSTER_HP_MAX,
						NORMAL_MONSTER_MP_MAX, 1, 100, 100, 100, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
					HardMNum += 1;
				}
			}
		}
	}
}