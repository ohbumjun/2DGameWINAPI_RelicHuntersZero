#pragma once

#define _CRTDBG_MAP_ALLOC

#include <Windows.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <list>	  
#include <vector> 
#include <stack>
#include <string>
#include <unordered_map>
#include <functional>
#include "resource.h"

// Header
#include "SharedPtr.h"
#include "Math.h"
#include "Flag.h"
#include "MonsterInfo.h"
#include "NameInfo.h"
#include "PlayerInfo.h"
#include "ProtoInfo.h"

// Sound 
#include "fmod.hpp"
#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "fmod64_vc.lib")

#ifdef _DEBUG

#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

#endif // _DEBUG

#define GRAVITY 9.8f
#define M_PI 3.14f
#define KEYCOUNT_MAX 256

// Path
#define ROOT_PATH "RootPath"
#define TEXTURE_PATH "TexturePath"
#define SOUND_PATH "SoundPath"
#define FONT_PATH "FontPath"
#define MAP_PATH "MapPath"

// Delete
#define SAFE_DELETE(p) \
	if (p)             \
	{                  \
		delete p;      \
		p = nullptr;   \
	}
#define SAFE_DELETE_ARRAY(p) \
	if (p)                   \
	{                        \
		delete[] p;          \
		p = nullptr;         \
	}
#define SAFE_RELEASE(p) \
	if (p)              \
	{                   \
		p->Release();   \
		p = nullptr;    \
	}

// Bullet Distance
#define NORMAL_BULLET_DISTANCE 800.f;

// Bullets Nums
#define PISTOL_BULLET_NUM 100
#define SHOTGUN_BULLET_NUM 10

// Widget Componenet Names
#define HPWIDGET_COMPONENET        "HPBarWidget"
#define MPWIDGET_COMPONENET        "MPBarWidget"
#define NAMEWIDGET_COMPONENET      "NameWidget"
#define STEMINAMEWIDGET_COMPONENET "SteminaWidget"
#define GOLD_COMPONENET			   "GoldWidget"

struct Resolution
{
	int Width;
	int Height;
};

struct RectInfo
{
	float Left;
	float Top;
	float Right;
	float Bottom;

	RectInfo() : Left(0.f),
				 Top(0.f),
				 Right(0.f),
				 Bottom(0.f)
	{
	}
};

struct SphereInfo
{
	Vector2 Center;
	float Radius;
};

struct AnimationFrameData
{
	Vector2 StartPos;
	Vector2 Size;
};

struct EGunInfo
{
	EItem_Type m_ItemType = EItem_Type::Item_Gun;
	EGunClass m_GunClass;
	EGun_Type m_GunType;
	float m_Damage;
	int   m_BulletsLoaded;
	int   m_BulletsFullNum;
	bool  m_BulletEmpty;
	float m_BulletLoadTime;
	float m_BulletDistance;
};

struct CharacterInfo
{
	int Attack;
	int Armor;
	int HP;
	int HPMax;
	float MP;
	float MPMax;
	float Stemina;
	float SteminaMax;
	int Level;
	int Exp;
	int Gold;
	float AttackSpeed;
	float AttackDistance;
		
};

struct CollisionProfile
{
	std::string Name;
	ECollision_Channel Channel;
	bool CollisionEnable;
	std::vector<ECollision_State> vecState;
};
