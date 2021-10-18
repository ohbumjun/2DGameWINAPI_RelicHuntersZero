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

enum ETexture_Dir
{
	Texture_Left,
	Texture_Right,
	Texture_End
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
	Channel_MonsterAttack,
	Channel_Wall,
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
	Character,
	Monster,
	Player,
	Potion,
	Weapon,
	Obstacle,
	Coin,
	Equipment,
	Bullet,
	WallObject,
	NPC,
	Effect
};

enum class EMonster_Type
{
	Duck1,
	Duck2,
	Duck3,
	Turtle1,
	Turtle2,
	Turtle3,
	KamiKaze1,
	KamiKaze2,
	KamiKazeCage1,
	KamiKazeCage2,
	Tentacle,
	Boss
};

enum class EShield_Type
{
	Turtle,
	Player,
	Boss
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

enum class ENpc_Type
{
	Hp,
	Mp,
	Shield
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
	MonsterLight,
	MonsterMedium,
	MonsterHard,
	End
};

enum class ETileEditMode
{
	Option,
	Image,
	End
};