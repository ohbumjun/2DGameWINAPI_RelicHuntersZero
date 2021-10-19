#pragma once


// Monster Move Direction Limit Time
#define MONSTER_TARGET_POS_LIMIT_TIME 5.f

// Monster Attack
#define NORMAL_MONSTER_ATTACK 30
#define MEDIUM_MONSTER_ATTACK 50
#define HARD_MONSTER_ATTACK 100
#define BOSS_MONSTER_ATTACK 500

// Monster Armor
#define NORMAL_MONSTER_ARMOR 10
#define MEDIUM_MONSTER_ARMOR 30
#define HARD_MONSTER_ARMOR 50
#define BOSS_MONSTER_ARMOR 100

// Monster HP
#define NORMAL_MONSTER_HP_MAX 200
#define MEDIUM_MONSTER_HP_MAX 300
#define HARD_MONSTER_HP_MAX 400
#define BOSS_MONSTER_HP_MAX 5000

// Monster MP
#define NORMAL_MONSTER_MP_MAX 5.f
#define MEDIUM_MONSTER_MP_MAX 5.f
#define HARD_MONSTER_MP_MAX 5.f
#define BOSS_MONSTER_MP_MAX 5.f

// Monster Attack Distance
#define NORMAL_MONSTER_ATTACK_DISTANCE 200.f
#define MEDIUM_MONSTER_ATTACK_DISTANCE 250.f
#define HARD_MONSTER_ATTACK_DISTANCE   300.f
#define BOSS_MONSTER_ATTACK_DISTANCE   2000.f

// Monster Dash Distance
#define NORMAL_MONSTER_DASH_DISTANCE 350.f
#define MEDIUM_MONSTER_DASH_DISTANCE 400.f
#define KAMIKAZE_MONSTER_DASH_DISTANCE	800.f
#define BOSS_MONSTER_DASH_DISTANCE 1000.f

// Monster Move Speed
#define NORMAL_MONSTER_MOVE_SPEED 100.f
#define MEDIUM_MONSTER_MOVE_SPEED 150.f
#define HARD_MONSTER_MOVE_SPEED 200.f
#define BOSS_MONSTER_MOVE_SPEED 200.f

// Monster Attack Speed
#define NORMAL_MONSTER_ATTACK_SPEED 1000.f
#define MEDIUM_MONSTER_ATTACK_SPEED 1500.f
#define HARD_MONSTER_ATTACK_SPEED 2000.f
#define BOSS_MONSTER_ATTACK_SPEED 2000.f

// Monster Pause Time
#define MONSTER_ATTACK_PAUSE_TIME 5.f


enum class EMonsterAI
{
	Idle,
	Walk,
	Trace,
	Hit,
	Attack,
	Death
};