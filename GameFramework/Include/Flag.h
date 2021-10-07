#pragma once

enum EKeyState
{
	KeyState_Down,
	KeyState_Push,
	KeyState_Up,
	KeyState_Max
};

enum class ETexture_Type
{
	Atlas,
	Frame
};

enum class ECollider_Type
{
	Box,
	Sphere,
	Point
};

enum class EBullet_Type
{
	Light,
	Medium,
	Heavy
};

enum EItem_Type
{
	Item_Gun,
	Item_End
};

enum EGunClass
{
	Light,
	Medium,
	Heavy,
	End
};
enum class EGun_Type
{
	Light_Pistol,
	Medium_Pistol,
	Heavy_Pistol,
	Light_ShotGun,
	Medium_ShotGun,
	Heavy_ShotGun
};

enum ECollision_Channel
{
	Channel_Object,
	Channel_Player,
	Channel_Monster,
	Channel_PlayerAttack,
	Channel_PlayerLaser,
	Channel_MonsterAttack,
	Channel_Max
};

enum class ECollision_State
{
	Ignore,
	Collision
};

enum class EButton_State
{
	Normal,
	MouseOn,
	Click,
	Disable,
	End
};

enum class EObject_Type
{
	GameObject,
	Laser,
	Character,
	Monster,
	Player,
	Potion,
	Obstacle,
	Equipment,
	Bullet,
	Effect
};

enum class EMonster_Type
{
	Duck1,
	Duck2,
	Duck3,
	Turtle1,
	Turtle2,
	Turtle3
};

enum class EPotion_Type
{
	HP,
	MP
};

enum class EEquip_Type
{
	Attack,
	Armor
};

enum class EDoorStage_Type
{
	Stage_Default,
	Stage_Home,
	Stage_One,
	Stage_Two,
	Stage_Three
};

enum class ETileOption
{
	Normal,
	Wall,
	Slow,
	End
};

enum class ETileEditMode
{
	Option,
	Image,
	End
};