
#pragma once

/*
stl : standard template library�̴�.
�迭, ������ũ�帮��Ʈ �� �ڷᱸ������ �̸� �����س��� ���̺귯���̴�.
*/

#include <Windows.h>
#include <list>		// ������ũ�帮��Ʈ ���ø� �ڷᱸ��
#include <vector>	// �����迭 ���ø� �ڷᱸ��
#include <stack>
#include <string>
#include <unordered_map>
#include <crtdbg.h>
#include <functional>
#include "resource.h"

#include "SharedPtr.h"
#include "Math.h"
#include "Flag.h"
#include "MonsterInfo.h"
#include "PlayerInfo.h"
#include "fmod.hpp"

#pragma comment(lib, "msimg32.lib")

#pragma comment(lib, "fmod64_vc.lib")

#define GRAVITY 9.8f

#define	KEYCOUNT_MAX	256

// Path
#define	ROOT_PATH		"RootPath"
#define	TEXTURE_PATH	"TexturePath"
#define	SOUND_PATH		"SoundPath"
#define	FONT_PATH		"FontPath"

// Delete 
#define	SAFE_DELETE(p)	if(p)	{ delete p; p = nullptr; }
#define	SAFE_DELETE_ARRAY(p)	if(p)	{ delete[] p; p = nullptr; }
#define	SAFE_RELEASE(p)	if(p)	{ p->Release(); p = nullptr; }

// Bullet Distance 
#define NORMAL_BULLET_DISTANCE 800.f;


struct Resolution
{
	int	Width;
	int	Height;
};

struct RectInfo
{
	float	Left;
	float	Top;
	float	Right;
	float	Bottom;

	RectInfo() :
		Left(0.f),
		Top(0.f),
		Right(0.f),
		Bottom(0.f)
	{
	}
};

struct SphereInfo
{
	Vector2	Center;
	float	Radius;
};

struct AnimationFrameData
{
	Vector2	StartPos;
	Vector2	Size;
};

struct CharacterInfo
{
	int	Attack;
	int	Armor;
	int	HP;
	int	HPMax;
	float MP;
	float MPMax;
	int	Level;
	int	Exp;
	int	Gold;
	float	AttackSpeed;
	float	AttackDistance;
	float   DashDistance;
};

struct CollisionProfile
{
	std::string			Name;
	ECollision_Channel	Channel;
	bool				CollisionEnable;
	std::vector<ECollision_State>	vecState;
};
