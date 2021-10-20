#include "Scene.h"
#include "SceneResource.h"
#include "SceneManager.h"
#include "SceneCollision.h"
#include "Camera.h"
// UI
#include "../Object/EffectHit.h"
#include "../Object/EffectSurprise.h"
#include "../Object/EffectCasing.h"
#include "../Object/EffectAbilityUp.h"
#include "../Object/EffectGrenade.h"
#include "../Object/EffectExplodeTrace.h"
#include "../Object/EffectBulletStart.h"
#include "../Object/EffectDash.h"
#include "../Object/EffectText.h"
#include "../Object/EffectShield.h"
#include "../Object/GeneratorTower.h"
#include "../Object/DamageFont.h"
#include "../Object/WallObject.h"
#include "../Object/DuckMonster.h"
#include "../Object/TurtleMonster.h"
#include "../Object/KamikazeMonster.h"
#include "../Object/KamikazeCage.h"
#include "../Object/HPPotion.h"
#include "../Object/MPPotion.h"
#include "../Object/Coin.h"
#include "../Object/PistolGun.h"
#include "../Object/ShotGun.h"
// Map
#include "../Map/MapBase.h"
// UI
#include "../UI/UIMain.h"
#include "../UI/UICommon.h"
#include "../UI/UICharacterStateHUD.h"
#include "../UI/UIGunStateHUD.h"

CScene::CScene()
{
	m_Resource = new CSceneResource;
	m_Collision = new CSceneCollision;

	m_Collision->m_Scene = this;

	m_RenderCount = 0;
	m_RenderCapacity = 100;
	m_RenderArray = new CGameObject *[m_RenderCapacity];

	m_UICount = 0;
	m_UICapacity = 10;
	m_UIArray = new CUIWindow *[m_UICapacity];

	m_Camera = new CCamera;

	m_Camera->Init();

	m_TileMap = nullptr;
}

CScene::~CScene()
{
	SAFE_DELETE(m_Camera);
	SAFE_DELETE_ARRAY(m_RenderArray);

	for (int i = 0; i < m_UICount; ++i)
	{
		SAFE_RELEASE(m_UIArray[i]);
	}
	SAFE_DELETE_ARRAY(m_UIArray);

	{
		auto iter = m_MapList.begin();
		auto iterEnd = m_MapList.end();
		for (; iter != iterEnd; ++iter)
		{
			SAFE_DELETE((*iter));
		}
		m_MapList.clear();
	}

	m_ObjList.clear();
	m_mapPrototype.clear();
	m_Player = nullptr; // m_Player = nullptr;

	SAFE_DELETE(m_Collision);
	SAFE_DELETE(m_Resource);
}

CSceneResource *CScene::GetSceneResource() const
{
	return m_Resource;
}

CSceneCollision *CScene::GetSceneCollision() const
{
	return m_Collision;
}

CCamera *CScene::GetCamera() const
{
	return m_Camera;
}

CGameObject *CScene::FindObject(const std::string &Name)
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
			return *iter;
	}

	return nullptr;
}

CGameObject* CScene::FindObject(CGameObject* Obj)
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter) == Obj) return (*iter);
	}
	return nullptr;
}

CGameObject *CScene::FindClosestMonsterToPlayer(Vector2 PlayerPos)
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();
	float MinLength = (float)INT_MAX;
	float FLOAT_MAX = (float)INT_MAX;
	CGameObject *ClosestObj = nullptr;

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetObjType() != EObject_Type::Monster)
			continue;

		float LengthToObj = Vector2((*iter)->GetPos() - PlayerPos).Length();
		if (LengthToObj < MinLength)
		{
			MinLength = LengthToObj;
			ClosestObj = (*iter);
		}
	}
	if (MinLength == FLOAT_MAX)
		return nullptr;
	return ClosestObj;
}

void CScene::DestroyAllAttackObjects()
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();
	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetObjType() == EObject_Type::Bullet)
		{
			Vector2 ObjPos = (*iter)->GetPos();
			(*iter)->Destroy();
			CEffectHit *Hit = CreateObject<CEffectHit>(EFFECT_HIT_PROTO, EFFECT_HIT_PROTO,
													   ObjPos, Vector2(178.f, 164.f));
			GetSceneResource()->SoundPlay("Fire");
		}
	}
}

CGameObject *CScene::SetPlayer(const std::string &Name)
{
	CGameObject *Player = FindObject(Name);
	return SetPlayer(Player);
}

CGameObject *CScene::SetPlayer(CGameObject *Player)
{
	if (!Player)
		return nullptr;

	m_Player = Player;

	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == Player)
		{
			m_ObjList.erase(iter);
			break;
		}
	}
	return m_Player;
}

void CScene::SetPlayerAnimation()
{
	// RightIdle
	GetSceneResource()->CreateAnimationSequence(PLAYER_RIGHT_IDLE,
		PLAYER_RIGHT_IDLE, TEXT("images/Character/jimmy/right_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(PLAYER_RIGHT_IDLE,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(PLAYER_RIGHT_IDLE,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}
	// Right Walk
	GetSceneResource()->CreateAnimationSequence(PLAYER_RIGHT_WALK,
		PLAYER_RIGHT_WALK, TEXT("images/Character/jimmy/right_walk.bmp"));
	GetSceneResource()->SetTextureColorKey(PLAYER_RIGHT_WALK,
		255, 255, 255);

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(PLAYER_RIGHT_WALK,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Right Run
	GetSceneResource()->CreateAnimationSequence(PLAYER_RIGHT_RUN,
		PLAYER_RIGHT_RUN, TEXT("images/Character/jimmy/right_run.bmp"));

	GetSceneResource()->SetTextureColorKey(PLAYER_RIGHT_RUN,
		255, 255, 255);

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(PLAYER_RIGHT_RUN,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	GetSceneResource()->CreateAnimationSequence(PLAYER_RIGHT_ATTACK,
		PLAYER_RIGHT_ATTACK, TEXT("images/Character/jimmy/right_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(PLAYER_RIGHT_ATTACK,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(PLAYER_RIGHT_ATTACK,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Right Dash
	GetSceneResource()->CreateAnimationSequence(PLAYER_RIGHT_DASH,
		PLAYER_RIGHT_DASH, TEXT("images/Character/jimmy/right_dash.bmp"));

	GetSceneResource()->SetTextureColorKey(PLAYER_RIGHT_DASH,
		255, 255, 255);

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; j++)
		{
			if (i == 1 && j == 1) break;
			GetSceneResource()->AddAnimationFrameData(PLAYER_RIGHT_DASH,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Player Left ---
	// Left Idle 
	GetSceneResource()->CreateAnimationSequence(PLAYER_LEFT_IDLE,
		PLAYER_LEFT_IDLE, TEXT("images/Character/jimmy/left_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(PLAYER_LEFT_IDLE, 255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(PLAYER_LEFT_IDLE,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Left  Walk
	GetSceneResource()->CreateAnimationSequence(PLAYER_LEFT_WALK,
		PLAYER_LEFT_WALK, TEXT("images/Character/jimmy/left_walk.bmp"));

	GetSceneResource()->SetTextureColorKey(PLAYER_LEFT_WALK,
		255, 255, 255);

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(PLAYER_LEFT_WALK,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Left Run
	GetSceneResource()->CreateAnimationSequence(PLAYER_LEFT_RUN,
		PLAYER_LEFT_RUN, TEXT("images/Character/jimmy/left_run.bmp"));

	GetSceneResource()->SetTextureColorKey(PLAYER_LEFT_RUN,
		255, 255, 255);

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(PLAYER_LEFT_RUN,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Left Dash
	GetSceneResource()->CreateAnimationSequence(PLAYER_LEFT_DASH,
		PLAYER_LEFT_DASH, TEXT("images/Character/jimmy/left_dash.bmp"));

	GetSceneResource()->SetTextureColorKey(PLAYER_LEFT_DASH,
		255, 255, 255);

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; j++)
		{
			if (i == 1 && j == 1) break;
			GetSceneResource()->AddAnimationFrameData(PLAYER_LEFT_DASH,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}


	GetSceneResource()->CreateAnimationSequence(PLAYER_LEFT_ATTACK,
		PLAYER_LEFT_ATTACK, TEXT("images/Character/jimmy/left_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(PLAYER_LEFT_ATTACK, 255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(PLAYER_LEFT_ATTACK,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Death ---
	GetSceneResource()->CreateAnimationSequence(PLAYER_LEFT_DEATH,
		PLAYER_LEFT_DEATH, TEXT("images/Character/jimmy/left_death.bmp"));

	GetSceneResource()->SetTextureColorKey(PLAYER_LEFT_DEATH,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(PLAYER_LEFT_DEATH,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}
	GetSceneResource()->CreateAnimationSequence(PLAYER_RIGHT_DEATH,
		PLAYER_RIGHT_DEATH, TEXT("images/Character/jimmy/death.bmp"));

	GetSceneResource()->SetTextureColorKey(PLAYER_RIGHT_DEATH,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(PLAYER_RIGHT_DEATH,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}
	
	// Hit ---
	// Stun
	GetSceneResource()->CreateAnimationSequence(PLAYER_LEFT_HIT,
		PLAYER_LEFT_HIT, TEXT("images/Character/jimmy/left_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(PLAYER_LEFT_HIT,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(PLAYER_LEFT_HIT,
			i * 134.f, 0, 134.f, 114.f);
	}
	GetSceneResource()->CreateAnimationSequence(PLAYER_RIGHT_HIT,
		PLAYER_RIGHT_HIT, TEXT("images/Character/jimmy/right_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(PLAYER_RIGHT_HIT,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(PLAYER_RIGHT_HIT,
			i * 134.f, 0, 134.f, 114.f);
	}

	// Teleport --- 
	GetSceneResource()->CreateAnimationSequence(PLAYER_TELEPORT,
		PLAYER_TELEPORT, TEXT("images/Character/jimmy/teleport.bmp"));

	GetSceneResource()->SetTextureColorKey(PLAYER_TELEPORT,
		255, 255, 255);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(PLAYER_TELEPORT,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}
}

void CScene::SetItemsAnimation()
{
	// Door ---
	GetSceneResource()->CreateAnimationSequence("StageDoor",
		"StageDoor", TEXT("DoorToNextStage.bmp"));
	GetSceneResource()->SetTextureColorKey("StageDoor",
		255, 0, 255);
	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 5; j++)
		{
			GetSceneResource()->AddAnimationFrameData("StageDoor",
				j * 192.f, i * 192.f, 192.f, 192.f);
		}
	}
	
	// Potion
	// HP
	GetSceneResource()->CreateAnimationSequence(POTION_HP_ANIM,
		POTION_HP_ANIM, TEXT("images/Items/HPPotion.bmp"));
	GetSceneResource()->SetTextureColorKey(POTION_HP_ANIM,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(POTION_HP_ANIM,
			i * 70.f, 0, 70.f, 70.f);
	}
	// MP
	GetSceneResource()->CreateAnimationSequence(POTION_MP_ANIM,
		POTION_MP_ANIM, TEXT("images/Items/MPPotion.bmp"));
	GetSceneResource()->SetTextureColorKey(POTION_MP_ANIM,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(POTION_MP_ANIM,
			i * 70.f, 0, 70.f, 70.f);
	}
	
	// Surprise
	GetSceneResource()->CreateAnimationSequence(SURPRISE_EFFECT,
		SURPRISE_EFFECT, TEXT("images/Items/Surprise.bmp"));
	GetSceneResource()->SetTextureColorKey(SURPRISE_EFFECT,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == 1 && j == 3) break;
			GetSceneResource()->AddAnimationFrameData(SURPRISE_EFFECT,
				j * 38.f, i * 54.f, 38.f, 54.f);
		}
	}

	// ABILITYUP_EFFECT
	GetSceneResource()->CreateAnimationSequence(ABILITYUP_EFFECT,
		ABILITYUP_EFFECT, TEXT("images/Items/HPMPUp.bmp"));
	GetSceneResource()->SetTextureColorKey(ABILITYUP_EFFECT,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i == 2 && j == 1) break;
			GetSceneResource()->AddAnimationFrameData(ABILITYUP_EFFECT,
				j * 58.f, i * 56.f, 58.f, 56.f);
		}
	}
	
	// Hp, Mp Up
	GetSceneResource()->CreateAnimationSequence(MPHPUP_EFFECT,
		MPHPUP_EFFECT, TEXT("images/Items/HPMPUp.bmp"));
	GetSceneResource()->SetTextureColorKey(MPHPUP_EFFECT,
		255, 255, 255);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i == 2 && j == 1) break;
			GetSceneResource()->AddAnimationFrameData(MPHPUP_EFFECT,
				j * 58.f, i * 56.f, 58.f, 56.f);
		}
	}

	// Coin
	GetSceneResource()->CreateAnimationSequence("Coin",
		"Coin", TEXT("images/Items/coin.bmp"));
	GetSceneResource()->SetTextureColorKey("Coin",
		255, 255, 255);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == 3 && j == 1) break;
			GetSceneResource()->AddAnimationFrameData("Coin",
				j * 46.f, i * 46.f, 46.f, 46.f);
		}
	}
}

void CScene::SetBulletsAnimation()
{
	// 총알
	GetSceneResource()->CreateAnimationSequence("Bullet",
		"Bullet", TEXT("Smoke.bmp"));
	GetSceneResource()->SetTextureColorKey("Bullet",
		255, 0, 255);
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			GetSceneResource()->AddAnimationFrameData("Bullet",
				j * 92.f, i * 92.f, 92.f, 92.f);
		}
	}

	// Bullet Light
	GetSceneResource()->CreateAnimationSequence(BULLET_LIGHT,
		BULLET_LIGHT, TEXT("images/Weapon/Gun/Bullet/spr_bullet_light.bmp"));
	GetSceneResource()->SetTextureColorKey(BULLET_LIGHT,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(BULLET_LIGHT,
			i * 66.f, 0, 66.f, 50.f);
	}

	// Bullet Medium
	GetSceneResource()->CreateAnimationSequence(BULLET_MEDIUM,
		BULLET_MEDIUM, TEXT("images/Weapon/Gun/Bullet/spr_bullet_medium.bmp"));
	GetSceneResource()->SetTextureColorKey(BULLET_MEDIUM,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(BULLET_MEDIUM,
			i * 66.f, 0, 66.f, 50.f);
	}

	// Bullet Heavy 
	GetSceneResource()->CreateAnimationSequence(BULLET_HEAVY,
		BULLET_HEAVY, TEXT("images/Weapon/Gun/Bullet/spr_bullet_heavy.bmp"));
	GetSceneResource()->SetTextureColorKey(BULLET_HEAVY,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(BULLET_HEAVY,
			i * 66.f, 0, 66.f, 50.f);
	}

	// Bullet Boss Right
	GetSceneResource()->CreateAnimationSequence(BULLET_RIGHT_BOSS,
		BULLET_RIGHT_BOSS, TEXT("images/Weapon/Gun/Bullet/spr_bullet_boss.bmp"));
	GetSceneResource()->SetTextureColorKey(BULLET_RIGHT_BOSS,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(BULLET_RIGHT_BOSS,
			i * 58.f, 0, 58.f, 56.f);
	}
	// Bullet Boss Left 
	GetSceneResource()->CreateAnimationSequence(BULLET_LEFT_BOSS,
		BULLET_LEFT_BOSS, TEXT("images/Weapon/Gun/Bullet/left_spr_bullet_boss.bmp"));
	GetSceneResource()->SetTextureColorKey(BULLET_LEFT_BOSS,
		255, 255, 255);
	for (int i = 1; i >= 0;--i)
	{
		GetSceneResource()->AddAnimationFrameData(BULLET_LEFT_BOSS,
			i * 58.f, 0, 58.f, 56.f);
	}

	// Bullet Start Left
	GetSceneResource()->CreateAnimationSequence(BULLETSTART_EFFECT_L,
		BULLETSTART_EFFECT_L, TEXT("images/Weapon/Gun/Bullet/left_spr_bullet_start.bmp"));
	GetSceneResource()->SetTextureColorKey(BULLETSTART_EFFECT_L,
		0, 0, 0);
	for (int i = 0; i < 1; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(BULLETSTART_EFFECT_L,
			i * 46.f, 0, 46.f, 30.f);
	}

	// Bullet Start Right 
	GetSceneResource()->CreateAnimationSequence(BULLETSTART_EFFECT_R,
		BULLETSTART_EFFECT_R, TEXT("images/Weapon/Gun/Bullet/spr_bullet_start.bmp"));
	GetSceneResource()->SetTextureColorKey(BULLETSTART_EFFECT_R,
		255, 0, 255);
	for (int i = 0; i < 1; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(BULLETSTART_EFFECT_R,
			i * 46.f, 0, 46.f, 30.f);
	}

	// Bullet Reload
	GetSceneResource()->CreateAnimationSequence(RELOAD_EFFECT,
		RELOAD_EFFECT, TEXT("images/Weapon/Gun/Bullet/GunReload.bmp"));
	GetSceneResource()->SetTextureColorKey(RELOAD_EFFECT,
		255, 0, 255);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			GetSceneResource()->AddAnimationFrameData(RELOAD_EFFECT,
				j * 46.f, i * 46.f, 46.f, 46.f);
		}
	}
	
}

void CScene::SetCollideAnimation()
{
	// 충돌 효과 애니메이션 
	GetSceneResource()->CreateAnimationSequence(HIT_EFFECT,
		HIT_EFFECT, TEXT("HitEffect.bmp"));
	GetSceneResource()->SetTextureColorKey(HIT_EFFECT,
		255, 255, 255);

	for (int i = 0; i < 6; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(HIT_EFFECT,
			i * 96.f, 0.f, 96.f, 96.f);
	}

	// 충돌 효과 애니메이션 
	GetSceneResource()->CreateAnimationSequence(DASH_EFFECT,
		DASH_EFFECT, TEXT("images/Weapon/Sprint/dashEffect.bmp"));
	GetSceneResource()->SetTextureColorKey(DASH_EFFECT,
		255, 255, 255);

	for (int i = 0; i < 6; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(DASH_EFFECT,
			i * 178.f, 0.f, 178.f, 164.f);
	}
}

void CScene::SetMouseAnimation()
{
	// 텔리포트 마우스 
	GetSceneResource()->CreateAnimationSequence("TeleportMouseDisplay",
		"TeleportMouseDisplay", TEXT("Mouse/h.bmp"));
	GetSceneResource()->SetTextureColorKey("TeleportMouseDisplay", 255, 0, 255);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			GetSceneResource()->AddAnimationFrameData("TeleportMouseDisplay",
				j * 61.f, i * 61.f, 61.f, 61.f);
		}
	}
}

void CScene::SetSkillAnimation()
{
	// Slow Motion ---
	GetSceneResource()->CreateAnimationSequence("SkillSlowMotionAttack",
		"SkillSlowMotionAttack", TEXT("Player/Right/ashoot1.bmp"));
	GetSceneResource()->SetTextureColorKey("SkillSlowMotionAttack",
		255, 0, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("SkillSlowMotionAttack",
			i * 70.f, 0.f, 70.f, 81.f);
	}

	// Destory All ---
	GetSceneResource()->CreateAnimationSequence("SkillDestoryAll",
		"SkillDestoryAll", TEXT("Player/Right/ashoot1.bmp"));
	GetSceneResource()->SetTextureColorKey("SkillDestoryAll",
		255, 0, 255);
	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("SkillDestoryAll",
			i * 70.f, 0.f, 70.f, 81.f);
	}
}

void CScene::SetNpcAnimation()
{
	GetSceneResource()->CreateAnimationSequence(NPC_IDLE,
		NPC_IDLE, TEXT("images/Character/ass/spr_npc.bmp"));
	GetSceneResource()->SetTextureColorKey(NPC_IDLE,
		255, 0, 255);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			GetSceneResource()->AddAnimationFrameData(NPC_IDLE,
				j * 104.f, i * 130.f, 104.f, 130.f);
		}
	}
}

void CScene::SetLevel1MonsterAnimation()
{
	SetDuck1MonsterAnimation();
	SetDuck2MonsterAnimation();
	SetDuck3MonsterAnimation();
	SetKamikaze1MonsterAnimation();
}

void CScene::SetDuck1MonsterAnimation()
{
	// RightIdle
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK1_RIGHT_IDLE,
		MONSTER_DUCK1_RIGHT_IDLE, TEXT("images/Monster/Duck1/right_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK1_RIGHT_IDLE,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i == 2 && j == 1) break;
			GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK1_RIGHT_IDLE,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}
	// Right Walk
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK1_RIGHT_WALK,
		MONSTER_DUCK1_RIGHT_WALK, TEXT("images/Monster/Duck1/right_walk.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK1_RIGHT_WALK,
		255, 255, 255);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == 3 && j == 1) break;
			GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK1_RIGHT_WALK,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Right Run
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK1_RIGHT_RUN,
		MONSTER_DUCK1_RIGHT_RUN, TEXT("images/Monster/Duck1/right_run.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK1_RIGHT_RUN,
		255, 255, 255);

	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK1_RIGHT_RUN,
			i * 134.f, 0.f, 134.f, 114.f);
	}

	// Right Attack
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK1_RIGHT_ATTACK,
		MONSTER_DUCK1_RIGHT_ATTACK, TEXT("Player/Right/aswing.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK1_RIGHT_ATTACK,
		255, 255, 255);
	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK1_RIGHT_ATTACK,
			i * 176.f, 0.f, 176.f, 89.f);
	}

	// Player Left ---
	// Left Idle 
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK1_LEFT_IDLE,
		MONSTER_DUCK1_LEFT_IDLE, TEXT("images/Monster/Duck1/left_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK1_LEFT_IDLE, 255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i == 2 && j == 1) break;
			GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK1_LEFT_IDLE,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Left  Walk
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK1_LEFT_WALK,
		MONSTER_DUCK1_LEFT_WALK, TEXT("images/Monster/Duck1/left_walk.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK1_LEFT_WALK,
		255, 255, 255);
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == 3 && j == 1) break;
			GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK1_LEFT_WALK,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Left Run
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK1_LEFT_RUN,
		MONSTER_DUCK1_LEFT_RUN, TEXT("images/Monster/Duck1/left_run.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK1_LEFT_RUN,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK1_LEFT_RUN,
			i * 134.f, 0.f, 134.f, 114.f);
	}
	

	// Death ---
	// Left 
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK1_LEFT_DEATH,
		MONSTER_DUCK1_LEFT_DEATH, TEXT("images/Monster/Duck1/left_death.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK1_LEFT_DEATH,
		255, 255, 255);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 5; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK1_LEFT_DEATH,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Right 
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK1_RIGHT_DEATH,
		MONSTER_DUCK1_RIGHT_DEATH, TEXT("images/Monster/Duck1/right_death.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK1_RIGHT_DEATH,
		255, 255, 255);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 5; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK1_RIGHT_DEATH,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Hit ---
	// Left 
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK1_LEFT_HIT,
		MONSTER_DUCK1_LEFT_HIT, TEXT("images/Monster/Duck1/left_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK1_LEFT_HIT,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK1_LEFT_HIT,
			i * 134.f, 0, 134.f, 114.f);
	}

	// Right 
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK1_RIGHT_HIT,
		MONSTER_DUCK1_RIGHT_HIT, TEXT("images/Monster/Duck1/right_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK1_RIGHT_HIT,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK1_RIGHT_HIT,
			i * 134.f, 0, 134.f, 114.f);
	}
}

void CScene::SetDuck2MonsterAnimation()
{
	// RightIdle
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK2_RIGHT_IDLE,
		MONSTER_DUCK2_RIGHT_IDLE, TEXT("images/Monster/Duck2/right_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK2_RIGHT_IDLE,
		255, 255, 255);

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK2_RIGHT_IDLE,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}
	// Right Walk
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK2_RIGHT_WALK,
		MONSTER_DUCK2_RIGHT_WALK, TEXT("images/Monster/Duck2/right_walk.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK2_RIGHT_WALK,
		255, 255, 255);

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK2_RIGHT_WALK,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Right Run
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK2_RIGHT_RUN,
		MONSTER_DUCK2_RIGHT_RUN, TEXT("images/Monster/Duck2/right_run.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK2_RIGHT_RUN,
		255, 255, 255);

	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK2_RIGHT_RUN,
			i * 134.f, 0.f, 134.f, 114.f);
	}

	// Right Attack
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK2_RIGHT_ATTACK,
		MONSTER_DUCK2_RIGHT_ATTACK, TEXT("Player/Right/aswing.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK2_RIGHT_ATTACK,
		255, 255, 255);
	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK2_RIGHT_ATTACK,
			i * 176.f, 0.f, 176.f, 89.f);
	}

	// Player Left ---
	// Left Idle 
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK2_LEFT_IDLE,
		MONSTER_DUCK2_LEFT_IDLE, TEXT("images/Monster/Duck2/left_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK2_LEFT_IDLE, 255, 255, 255);

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK2_LEFT_IDLE,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Left  Walk
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK2_LEFT_WALK,
		MONSTER_DUCK2_LEFT_WALK, TEXT("images/Monster/Duck2/left_walk.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK2_LEFT_WALK,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK2_LEFT_WALK,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Left Run
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK2_LEFT_RUN,
		MONSTER_DUCK2_LEFT_RUN, TEXT("images/Monster/Duck2/left_run.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK2_LEFT_RUN,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK2_LEFT_RUN,
			i * 134.f, 0.f, 134.f, 114.f);
	}


	// Death ---
	// Left 
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK2_LEFT_DEATH,
		MONSTER_DUCK2_LEFT_DEATH, TEXT("images/Monster/Duck2/left_death.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK2_LEFT_DEATH,
		255, 255, 255);

	for (int j = 3; j >= 0; --j)
	{
		for (int i = 0; i < 3; i++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK2_LEFT_DEATH,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Right 
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK2_RIGHT_DEATH,
		MONSTER_DUCK2_RIGHT_DEATH, TEXT("images/Monster/Duck2/right_death.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK2_RIGHT_DEATH,
		255, 255, 255);

	for (int j = 0; j < 4; ++j)
	{
		for (int i = 0; i < 3; i++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK2_RIGHT_DEATH,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Hit ---
	// Left 
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK2_LEFT_HIT,
		MONSTER_DUCK2_LEFT_HIT, TEXT("images/Monster/Duck2/left_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK2_LEFT_HIT,
		255, 255, 255);
	for (int i = 1; i >=0 ; --i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK2_LEFT_HIT,
			i * 134.f, 0, 134.f, 114.f);
	}

	// Right 
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK2_RIGHT_HIT,
		MONSTER_DUCK2_RIGHT_HIT, TEXT("images/Monster/Duck2/right_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK2_RIGHT_HIT,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK2_RIGHT_HIT,
			i * 134.f, 0, 134.f, 114.f);
	}
}

void CScene::SetDuck3MonsterAnimation()
{
	// RightIdle
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK3_RIGHT_IDLE,
		MONSTER_DUCK3_RIGHT_IDLE, TEXT("images/Monster/Duck3/right_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK3_RIGHT_IDLE,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK3_RIGHT_IDLE,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}
	// Right Walk
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK3_RIGHT_WALK,
		MONSTER_DUCK3_RIGHT_WALK, TEXT("images/Monster/Duck3/right_walk.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK3_RIGHT_WALK,
		255, 255, 255);

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK3_RIGHT_WALK,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Right Run
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK3_RIGHT_RUN,
		MONSTER_DUCK3_RIGHT_RUN, TEXT("images/Monster/Duck3/right_run.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK3_RIGHT_RUN,
		255, 255, 255);

	for (int i = 0; i < 1; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK3_RIGHT_RUN,
			i * 134.f, 0.f, 134.f, 114.f);
	}

	// Right Attack
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK3_RIGHT_ATTACK,
		MONSTER_DUCK3_RIGHT_ATTACK, TEXT("Player/Right/aswing.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK3_RIGHT_ATTACK,
		255, 255, 255);
	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK3_RIGHT_ATTACK,
			i * 176.f, 0.f, 176.f, 89.f);
	}

	// Player Left ---
	// Left Idle 
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK3_LEFT_IDLE,
		MONSTER_DUCK3_LEFT_IDLE, TEXT("images/Monster/Duck3/left_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK3_LEFT_IDLE, 255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 1; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK3_LEFT_IDLE,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Left  Walk
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK3_LEFT_WALK,
		MONSTER_DUCK3_LEFT_WALK, TEXT("images/Monster/Duck3/left_walk.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK3_LEFT_WALK,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK3_LEFT_WALK,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Left Run
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK3_LEFT_RUN,
		MONSTER_DUCK3_LEFT_RUN, TEXT("images/Monster/Duck3/left_run.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK3_LEFT_RUN,
		255, 255, 255);
	for (int i = 0; i < 1; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK3_LEFT_RUN,
			i * 134.f, 0.f, 134.f, 114.f);
	}


	// Death ---
	// Left 
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK3_LEFT_DEATH,
		MONSTER_DUCK3_LEFT_DEATH, TEXT("images/Monster/Duck3/left_death.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK3_LEFT_DEATH,
		255, 255, 255);

	for (int j = 3; j >= 0; --j)
	{
		for (int i = 0; i < 3; i++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK3_LEFT_DEATH,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Right 
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK3_RIGHT_DEATH,
		MONSTER_DUCK3_RIGHT_DEATH, TEXT("images/Monster/Duck3/right_death.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK3_RIGHT_DEATH,
		255, 255, 255);

	for (int j = 0; j < 3; ++j)
	{
		for (int i = 0; i < 3; i++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK3_RIGHT_DEATH,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Hit ---
	// Left 
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK3_LEFT_HIT,
		MONSTER_DUCK3_LEFT_HIT, TEXT("images/Monster/Duck3/left_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK3_LEFT_HIT,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK3_LEFT_HIT,
			i * 134.f, 0, 134.f, 114.f);
	}

	// Right 
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK3_RIGHT_HIT,
		MONSTER_DUCK3_RIGHT_HIT, TEXT("images/Monster/Duck3/right_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK3_RIGHT_HIT,
		255, 255, 255);
	for (int i = 1; i >= 0; --i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK3_RIGHT_HIT,
			i * 134.f, 0, 134.f, 114.f);
	}
}


void CScene::SetBasicObjectGuns()
{
	CGameObject* GunProto = FindPrototype(GUN_PISTOL_LIGHT_PROTO);
	CGun* GunPistolLight = CreateObject<CGun>(GUN_PISTOL_LIGHT, GUN_PISTOL_LIGHT_PROTO, GunProto->m_Pos,GunProto->m_Size);

	GunProto = FindPrototype(GUN_PISTOL_MEDIUM_PROTO);
	GunPistolLight		 = CreateObject<CGun>(GUN_PISTOL_MEDIUM, GUN_PISTOL_MEDIUM_PROTO, GunProto->m_Pos, GunProto->m_Size);

	GunProto = FindPrototype(GUN_PISTOL_HEAVY_PROTO);
	GunPistolLight		 = CreateObject<CGun>(GUN_PISTOL_HEAVY, GUN_PISTOL_HEAVY_PROTO, GunProto->m_Pos, GunProto->m_Size);

	GunProto = FindPrototype(GUN_SHOTGUN_LIGHT_PROTO);
	GunPistolLight		 = CreateObject<CGun>(GUN_SHOTGUN_LIGHT, GUN_SHOTGUN_LIGHT_PROTO, GunProto->m_Pos, GunProto->m_Size);

	GunProto = FindPrototype(GUN_SHOTGUN_MEDIUM_PROTO);
	GunPistolLight		 = CreateObject<CGun>(GUN_SHOTGUN_MEDIUM, GUN_SHOTGUN_MEDIUM_PROTO, GunProto->m_Pos, GunProto->m_Size);

	GunProto = FindPrototype(GUN_SHOTGUN_HEAVY_PROTO);
	GunPistolLight		 = CreateObject<CGun>(GUN_SHOTGUN_HEAVY, GUN_SHOTGUN_HEAVY_PROTO, GunProto->m_Pos, GunProto->m_Size);
}

void CScene::SetBasicUIs()
{
	// Window
	CUICharacterStateHUD* StateWindow = CreateUIWindow<CUICharacterStateHUD>("CharacterStateHUD");
	CUIGunStateHUD*		  GunStateWindow = CreateUIWindow<CUIGunStateHUD>("GunStateHUD");
	CUICommon*			  CommonWindow = CreateUIWindow<CUICommon>("CommonUI");
}

void CScene::SetBasicProtoTypes()
{
	// EffectHit
	CEffectHit* EffectHitPrototype           = CreatePrototype<CEffectHit>(EFFECT_HIT_PROTO);
	// Effect Surpise
	CEffectSurprise* EffectSurprisePrototype = CreatePrototype<CEffectSurprise>(EFFECT_SURPRISE_PROTO);
	// EffectDash
	CEffectDash* EffectDashPrototype         = CreatePrototype<CEffectDash>(EFFECT_DASH_PROTO);
	// EffectCasing
	CEffectCasing* EffectCasingPrototype = CreatePrototype<CEffectCasing>(EFFECT_CASING_PROTO);
	// EffectBulletStart
	CEffectBulletStart* EffectBulletSTPrototype = CreatePrototype<CEffectBulletStart>(EFFECT_BULLETSTART_PROTO);
	// EffectText
	CEffectText* EffectTextPrototype = CreatePrototype<CEffectText>(EFFECT_TEXT_PROTO);
	// EffectAbilityUp
	CEffectAbilityUp* EffectAbilityUpPrototype = CreatePrototype<CEffectAbilityUp>(EFFECT_ABILITYUP_PROTO);
	// EffectGrenade
	CEffectGrenade* EffectGrenadeProto = CreatePrototype<CEffectGrenade>(GRENADE_PROTO);
	// Effect Explosion After
	CEffectExplodeTrace* EffectExplodeTraceProto = CreatePrototype<CEffectExplodeTrace>(EXPLOSION_AFTER_PROTO);
	// DamageFont 
	CDamageFont* DamageFontProto = CreatePrototype<CDamageFont>(DAMAGEFONT_PROTO);
	// Generator Tower 
	// CGeneratorTower* GeneratorTowerProto = CreatePrototype<CGeneratorTower>(GENERATOR_PROTO);
	
	// Teleport
	CTeleportMouse* TeleportMousePrototype = CreatePrototype<CTeleportMouse>(TELEPORT_MOUSE_PROTO);
	// Player Bullet 
	CBullet* PlayerBullet = CreatePrototype<CBullet>(PLAYER_BULLET_PROTO);
	PlayerBullet->SetCollisionProfile("PlayerAttack");

	// Player SlowMotion Bullet 
	CBullet* SlowMotionAttackBullet = CreatePrototype<CBullet>(PLAYER_SKILL_SLOWMOTION_BULLET_PROTO);
	SlowMotionAttackBullet->SetCollisionProfile("PlayerAttack");

	// Duck Monster
	CDuckMonster* DuckMonsterPrototype = CreatePrototype<CDuckMonster>(MONSTER_DUCK1_PROTO);
	DuckMonsterPrototype->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, NORMAL_MONSTER_HP_MAX,
		NORMAL_MONSTER_MP_MAX, 1, 100, 100, 600, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
	DuckMonsterPrototype->SetMoveSpeed(NORMAL_MONSTER_MOVE_SPEED);
	DuckMonsterPrototype->SetMonsterType(EMonster_Type::Duck1);

	DuckMonsterPrototype = CreatePrototype<CDuckMonster>(MONSTER_DUCK2_PROTO);
	DuckMonsterPrototype->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, NORMAL_MONSTER_HP_MAX,
		NORMAL_MONSTER_MP_MAX, 1, 100, 100, 600, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
	DuckMonsterPrototype->SetMoveSpeed(NORMAL_MONSTER_MOVE_SPEED);
	DuckMonsterPrototype->SetMonsterType(EMonster_Type::Duck2);

	DuckMonsterPrototype = CreatePrototype<CDuckMonster>(MONSTER_DUCK3_PROTO);
	DuckMonsterPrototype->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, NORMAL_MONSTER_HP_MAX,
		NORMAL_MONSTER_MP_MAX, 1, 100, 100, 600, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
	DuckMonsterPrototype->SetMoveSpeed(NORMAL_MONSTER_MOVE_SPEED);
	DuckMonsterPrototype->SetMonsterType(EMonster_Type::Duck3);

	// Turtle Monster
	CTurtleMonster* TurtleMonsterPrototype = CreatePrototype<CTurtleMonster>(MONSTER_TURTLE1_PROTO);
	TurtleMonsterPrototype->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, NORMAL_MONSTER_HP_MAX,
		NORMAL_MONSTER_MP_MAX, 1, 100, 100, 600, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
	TurtleMonsterPrototype->SetMoveSpeed(NORMAL_MONSTER_MOVE_SPEED);
	TurtleMonsterPrototype->SetMonsterType(EMonster_Type::Turtle1);

	TurtleMonsterPrototype = CreatePrototype<CTurtleMonster>(MONSTER_TURTLE2_PROTO);
	TurtleMonsterPrototype->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, NORMAL_MONSTER_HP_MAX,
		NORMAL_MONSTER_MP_MAX, 1, 100, 100, 600, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
	TurtleMonsterPrototype->SetMoveSpeed(NORMAL_MONSTER_MOVE_SPEED);
	TurtleMonsterPrototype->SetMonsterType(EMonster_Type::Turtle2);

	TurtleMonsterPrototype = CreatePrototype<CTurtleMonster>(MONSTER_TURTLE3_PROTO);
	TurtleMonsterPrototype->SetMonsterType(EMonster_Type::Turtle3);
	TurtleMonsterPrototype->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, NORMAL_MONSTER_HP_MAX,
		NORMAL_MONSTER_MP_MAX, 1, 100, 100, 600, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
	TurtleMonsterPrototype->SetMoveSpeed(NORMAL_MONSTER_MOVE_SPEED);

	// KamiKaze Monster
	CKamiKazeMonster* KamiKazeMonsterPrototype = CreatePrototype<CKamiKazeMonster>(MONSTER_KAMIKAZE1_PROTO);
	KamiKazeMonsterPrototype->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, NORMAL_MONSTER_HP_MAX,
		NORMAL_MONSTER_MP_MAX, 1, 100, 100, 600, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
	KamiKazeMonsterPrototype->SetMoveSpeed(NORMAL_MONSTER_MOVE_SPEED);
	KamiKazeMonsterPrototype->SetMonsterType(EMonster_Type::KamiKaze1);

	KamiKazeMonsterPrototype = CreatePrototype<CKamiKazeMonster>(MONSTER_KAMIKAZE2_PROTO);
	KamiKazeMonsterPrototype->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, NORMAL_MONSTER_HP_MAX,
		NORMAL_MONSTER_MP_MAX, 1, 100, 100, 600, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
	KamiKazeMonsterPrototype->SetMoveSpeed(NORMAL_MONSTER_MOVE_SPEED);
	KamiKazeMonsterPrototype->SetMonsterType(EMonster_Type::KamiKaze2);

	// KamiKaze Cage
	CKamiKazeCage* KamiKazeCagePrototype = CreatePrototype<CKamiKazeCage>(MONSTER_KAMIKAZECAGE_PROTO);
	KamiKazeCagePrototype->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, 1000.f,
		NORMAL_MONSTER_MP_MAX, 1, 100, 100, 600, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
	KamiKazeCagePrototype->SetMoveSpeed(NORMAL_MONSTER_MOVE_SPEED);
	KamiKazeCagePrototype->SetMonsterType(EMonster_Type::KamiKazeCage1);
	
	// Collider
	CCollider* Collider = PlayerBullet->FindCollider("Body");
	if (Collider)
		Collider->SetCollisionProfile("PlayerAttack");

	// Monster Bullet 
	CBullet* MonsterBullet = CreatePrototype<CBullet>(MONSTER_BULLET_PROTO);
	Collider = MonsterBullet->FindCollider("Body");
	if (Collider)
		Collider->SetCollisionProfile("MonsterAttack");

	// Potions
	CMPPotion* MPPotionPrototype = CreatePrototype<CMPPotion>(POTION_MP_PROTO);
	CHPPotion* HPPotionPrototype = CreatePrototype<CHPPotion>(POTION_HP_PROTO);

	// Coin
	CCoin* CoinPrototype = CreatePrototype<CCoin>(COIN_PROTO);

	// Shield
	CEffectShield* ShieldPrototype = CreatePrototype<CEffectShield>(SHIELD_PROTO);

	// Guns
	CPistolGun* GunPistolProto= CreatePrototype<CPistolGun>(GUN_PISTOL_LIGHT_PROTO);
	GunPistolProto->SetTexture(GUN_PISTOL_LIGHT_R, TEXT(TEXTURE_GUN_PISTOL_LIGHT_R));
	GunPistolProto->SetTextureColorKey(255, 255, 255);
	GunPistolProto->SetTexture(GUN_PISTOL_LIGHT_L, TEXT(TEXTURE_GUN_PISTOL_LIGHT_L));
	GunPistolProto->SetTextureColorKey(255, 255, 255);
	GunPistolProto->SetPos(200.f, 200.f);
	GunPistolProto->SetRightTextureName(GUN_PISTOL_LIGHT_R);
	GunPistolProto->SetLeftTextureName(GUN_PISTOL_LIGHT_L);

	GunPistolProto = CreatePrototype<CPistolGun>(GUN_PISTOL_MEDIUM_PROTO);
	GunPistolProto->SetTexture(GUN_PISTOL_MEDIUM_R, TEXT(TEXTURE_GUN_PISTOL_MEDIUM_R));
	GunPistolProto->SetTextureColorKey(255, 255, 255);
	GunPistolProto->SetTexture(GUN_PISTOL_MEDIUM_L, TEXT(TEXTURE_GUN_PISTOL_MEDIUM_L));
	GunPistolProto->SetTextureColorKey(255, 255, 255);
	GunPistolProto->SetPos(200.f, 300.f);
	GunPistolProto->SetGunClass(EGunClass::Medium);
	GunPistolProto->SetLeftTextureName(GUN_PISTOL_MEDIUM_L);
	GunPistolProto->SetRightTextureName(GUN_PISTOL_MEDIUM_R);

	GunPistolProto = CreatePrototype<CPistolGun>(GUN_PISTOL_HEAVY_PROTO);
	GunPistolProto->SetTexture(GUN_PISTOL_HEAVY_R, TEXT(TEXTURE_GUN_PISTOL_HEAVY_R));
	GunPistolProto->SetTextureColorKey(255, 255, 255);
	GunPistolProto->SetTexture(GUN_PISTOL_HEAVY_L, TEXT(TEXTURE_GUN_PISTOL_HEAVY_L));
	GunPistolProto->SetTextureColorKey(255, 255, 255);
	GunPistolProto->SetPos(200.f, 400.f);
	GunPistolProto->SetGunClass(EGunClass::Heavy);
	GunPistolProto->SetLeftTextureName(GUN_PISTOL_HEAVY_L);
	GunPistolProto->SetRightTextureName(GUN_PISTOL_HEAVY_R);

	CShotGun* GunShotProto = CreatePrototype<CShotGun>(GUN_SHOTGUN_LIGHT_PROTO);
	GunShotProto->SetTexture(GUN_SHOTGUN_LIGHT_R, TEXT(TEXTURE_GUN_SHOTGUN_LIGHT_R));
	GunShotProto->SetTextureColorKey(255, 255, 255);
	GunShotProto->SetTexture(GUN_SHOTGUN_LIGHT_L, TEXT(TEXTURE_GUN_SHOTGUN_LIGHT_L));
	GunShotProto->SetTextureColorKey(255, 255, 255);
	GunShotProto->SetPos(200.f, 500.f);
	GunShotProto->SetGunClass(EGunClass::Light);
	GunShotProto->SetLeftTextureName(GUN_SHOTGUN_LIGHT_L);
	GunShotProto->SetRightTextureName(GUN_SHOTGUN_LIGHT_R);

	GunShotProto = CreatePrototype<CShotGun>(GUN_SHOTGUN_MEDIUM_PROTO);
	GunShotProto->SetTexture(GUN_SHOTGUN_MEDIUM_R, TEXT(TEXTURE_GUN_SHOTGUN_MEDIUM_R));
	GunShotProto->SetTextureColorKey(255, 255, 255);
	GunShotProto->SetTexture(GUN_SHOTGUN_MEDIUM_L, TEXT(TEXTURE_GUN_SHOTGUN_MEDIUM_L));
	GunShotProto->SetTextureColorKey(255, 255, 255);
	GunShotProto->SetPos(200.f, 600.f);
	GunShotProto->SetGunClass(EGunClass::Medium);
	GunShotProto->SetLeftTextureName(GUN_SHOTGUN_MEDIUM_L);
	GunShotProto->SetRightTextureName(GUN_SHOTGUN_MEDIUM_R);

	GunShotProto = CreatePrototype<CShotGun>(GUN_SHOTGUN_HEAVY_PROTO);
	GunShotProto->SetTexture(GUN_SHOTGUN_HEAVY_R, TEXT(TEXTURE_GUN_SHOTGUN_HEAVY_R));
	GunShotProto->SetTextureColorKey(255, 255, 255);
	GunShotProto->SetTexture(GUN_SHOTGUN_HEAVY_L, TEXT(TEXTURE_GUN_SHOTGUN_HEAVY_L));
	GunShotProto->SetTextureColorKey(255, 255, 255);
	GunShotProto->SetPos(200.f, 700.f);
	GunShotProto->SetGunClass(EGunClass::Heavy);
	GunShotProto->SetLeftTextureName(GUN_SHOTGUN_HEAVY_L);
	GunShotProto->SetRightTextureName(GUN_SHOTGUN_HEAVY_R);

	GunShotProto = CreatePrototype<CShotGun>(BOSS_GUN_PROTO);
	GunShotProto->SetTexture(GUN_BOSS_L, TEXT(TEXTURE_BOSS_GUN_L));
	GunShotProto->SetTextureColorKey(255, 255, 255);
	GunShotProto->SetTexture(GUN_BOSS_R, TEXT(TEXTURE_BOSS_GUN_R));
	GunShotProto->SetTextureColorKey(255, 255, 255);
	GunShotProto->SetPos(200.f, 700.f);
	GunShotProto->SetGunClass(EGunClass::Boss);
	GunShotProto->SetLeftTextureName(GUN_BOSS_L);
	GunShotProto->SetRightTextureName(GUN_BOSS_R);
}

void CScene::SetBasicProtoGuns()
{
}

bool CScene::Init()
{
	return true;
}

bool CScene::Update(float DeltaTime)
{

	if (m_Player)
		m_Player->Update(DeltaTime * (m_Player->m_TimeScale));

	{
		auto iter = m_ObjList.begin();
		auto iterEnd = m_ObjList.end();
		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				iter = m_ObjList.erase(iter);
				iterEnd = m_ObjList.end();
				continue;
			}
			(*iter)->Update(DeltaTime * (*iter)->m_TimeScale);
			++iter;
		}
	}

	{
		for (int i = 0; i < m_UICount;)
		{
			if (!m_UIArray[i]->IsActive())
			{
				--m_UICount;
				for (int j = i; j < m_UICount; ++j)
				{
					m_UIArray[j] = m_UIArray[j + 1];
				}
				continue;
			}
			else if (!m_UIArray[i]->GetVisibility())
			{
				++i;
				continue;
			}
			m_UIArray[i]->Update(DeltaTime);
			++i;
		}
	}

	return false;
}

bool CScene::PostUpdate(float DeltaTime)
{
	if (m_Player)
	{
		m_Player->PostUpdate(DeltaTime);
	}
	{
		std::list<CSharedPtr<CGameObject>>::iterator iter = m_ObjList.begin();
		std::list<CSharedPtr<CGameObject>>::iterator iterEnd = m_ObjList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				iter = m_ObjList.erase(iter);
				iterEnd = m_ObjList.end();
				continue;
			}

			(*iter)->PostUpdate(DeltaTime * (*iter)->m_TimeScale);
			++iter;
		}
	}

	{
		for (int i = 0; i < m_UICount;)
		{
			if (!m_UIArray[i]->IsActive())
			{
				--m_UICount;
				for (int j = i; j < m_UICount; ++j)
				{
					m_UIArray[j] = m_UIArray[j + 1];
				}
				continue;
			}
			else if (!m_UIArray[i]->GetVisibility())
			{
				++i;
				continue;
			}
			m_UIArray[i]->PostUpdate(DeltaTime);
			++i;
		}
	}

	m_Camera->Update(DeltaTime);

	{
		auto iter = m_MapList.begin();
		auto iterEnd = m_MapList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				SAFE_DELETE((*iter));
				iter = m_MapList.erase(iter);
				iterEnd = m_MapList.end();
				continue;
			}

			else if (!(*iter)->IsEnable())
			{
				++iter;
				continue;
			}

			(*iter)->Update(DeltaTime);
			++iter;
		}
	}

	{
		auto iter = m_MapList.begin();
		auto iterEnd = m_MapList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				SAFE_DELETE((*iter));
				iter = m_MapList.erase(iter);
				iterEnd = m_MapList.end();
				continue;
			}

			else if (!(*iter)->IsEnable())
			{
				++iter;
				continue;
			}

			(*iter)->PostUpdate(DeltaTime);
			++iter;
		}
	}

	return false;
}

bool CScene::Collision(float DeltaTime)
{
	if (m_Player)
	{
		m_Player->Collision(DeltaTime);
	}
	{
		std::list<CSharedPtr<CGameObject>>::iterator iter = m_ObjList.begin();
		std::list<CSharedPtr<CGameObject>>::iterator iterEnd = m_ObjList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				iter = m_ObjList.erase(iter);
				iterEnd = m_ObjList.end();
				continue;
			}

			(*iter)->Collision(DeltaTime * (*iter)->m_TimeScale);
			++iter;
		}
	}

	{
		if (m_UICount >= 2)
		{
			qsort(m_UIArray, (size_t)m_UICount, sizeof(CUIWindow *),
				  CScene::SortZOrder);
		}

		for (int i = 0; i < m_UICount;)
		{
			if (!m_UIArray[i]->IsActive())
			{
				--m_UICount;
				for (int j = i; j < m_UICount; ++j)
				{
					m_UIArray[j] = m_UIArray[j + 1];
				}
				continue;
			}

			else if (!m_UIArray[i]->GetVisibility())
			{
				++i;
				continue;
			}

			m_UIArray[i]->Collision(DeltaTime);

			m_Collision->AddUIWindow(m_UIArray[i]);

			++i;
		}
	}

	m_Collision->CollisionMouse(DeltaTime);
	m_Collision->Collision(DeltaTime);

	return false;
}

bool CScene::Render(HDC hDC)
{
	if (m_MapList.size() >= 2)
	{
		qsort(&m_MapList[0], (size_t)m_MapList.size(),
			  sizeof(CMapBase *), CScene::SortZOrderMap);
	}
	{
		auto iter = m_MapList.begin();
		auto iterEnd = m_MapList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				SAFE_DELETE((*iter));
				iter = m_MapList.erase(iter);
				iterEnd = m_MapList.end();
				continue;
			}

			else if (!(*iter)->IsEnable())
			{
				++iter;
				continue;
			}

			(*iter)->Render(hDC);
			++iter;
		}
	}

	if (m_Player)
		m_Player->PrevRender();

	{
		std::list<CSharedPtr<CGameObject>>::iterator iter = m_ObjList.begin();
		std::list<CSharedPtr<CGameObject>>::iterator iterEnd = m_ObjList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				iter = m_ObjList.erase(iter);
				iterEnd = m_ObjList.end();
				continue;
			}

			(*iter)->PrevRender();

			if (!(*iter)->IsCull())
			{
				if (m_RenderCount == m_RenderCapacity)
				{
					m_RenderCapacity *= 2;
					CGameObject **Array = new CGameObject *[m_RenderCapacity];
					memcpy(Array, m_RenderArray, sizeof(CGameObject *) * m_RenderCount);
					delete[] m_RenderArray;
					m_RenderArray = Array;
				}

				m_RenderArray[m_RenderCount] = *iter;
				++m_RenderCount;
			}

			++iter;
		}
	}

	if (m_RenderCount == m_RenderCapacity)
	{
		m_RenderCapacity *= 2;
		CGameObject **Array = new CGameObject *[m_RenderCapacity];
		memcpy(Array, m_RenderArray, sizeof(CGameObject *) * m_RenderCount);
		delete[] m_RenderArray;
		m_RenderArray = Array;
	}

	if (m_Player)
	{
		if (!m_Player->IsCull())
		{
			m_RenderArray[m_RenderCount] = m_Player;
			++m_RenderCount;
		}
	}

	qsort(m_RenderArray, (size_t)m_RenderCount, sizeof(CGameObject *),
		  CScene::SortY);

	for (int i = 0; i < m_RenderCount; ++i)
	{
		m_RenderArray[i]->Render(hDC);
	}
	m_RenderCount = 0;

	{
		for (int i = 0; i < m_UICount;)
		{
			if (!m_UIArray[i]->IsActive())
			{
				--m_UICount;
				for (int j = i; j < m_UICount; ++j)
				{
					m_UIArray[j] = m_UIArray[j + 1];
				}
				continue;
			}
			++i;
		}

		for (int i = m_UICount - 1; i >= 0;)
		{
			if (!m_UIArray[i]->GetVisibility())
			{
				--i;
				continue;
			}
			m_UIArray[i]->Render(hDC);
			--i;
		}
	}

	return false;
}



int CScene::SortY(const void *Src, const void *Dest)
{
	CGameObject *SrcObj = *(CGameObject **)Src;
	CGameObject *DestObj = *(CGameObject **)Dest;

	float SrcY = SrcObj->GetBottom();
	float DestY = DestObj->GetBottom();

	if (SrcY < DestY)
		return -1;
	else if (SrcY > DestY)
		return 1;

	return 0;
}

int CScene::SortZOrder(const void *Src, const void *Dest)
{
	CUIWindow *SrcObj = *(CUIWindow **)Src;
	CUIWindow *DestObj = *(CUIWindow **)Dest;

	int SrcZ = SrcObj->GetZOrder();
	int DestZ = DestObj->GetZOrder();

	if (SrcZ > DestZ)
		return -1;

	else if (SrcZ < DestZ)
		return 1;

	return 0;
}

int CScene::SortZOrderMap(const void *Src, const void *Dest)
{
	CMapBase *SrcObj = *(CMapBase **)Src;
	CMapBase *DestObj = *(CMapBase **)Dest;

	int SrcZ = SrcObj->GetZOrder();
	int DestZ = DestObj->GetZOrder();

	if (SrcZ > DestZ)
		return 1;
	else if (SrcZ < DestZ)
		return -1;

	return 0;
}

CGameObject *CScene::FindPrototype(const std::string &Name)
{
	auto iter = m_mapPrototype.find(Name);

	if (iter == m_mapPrototype.end())
		return nullptr;

	return iter->second;
}

/*
void CScene::SetObjectsToWall()
{
	CTileMap* TileMap = GetTileMap();

	if (TileMap)
	{
		// Tile Idx 
		int LeftIndexX, TopIndexY, RightIndexX, BottomIndexY;
		LeftIndexX = TileMap->GetOriginTileIndexX(0);
		RightIndexX = TileMap->GetTileCountX() - 1;
		TopIndexY = TileMap->GetOriginTileIndexY(0);
		BottomIndexY = TileMap->GetTileCountY() - 1;

		int WallNum = 0;

		CGameObject* WallObj = nullptr;

		// From Down to Up
		for (int i = TopIndexY; i <= BottomIndexY; i++)
		{
			for (int j = LeftIndexX; j <= RightIndexX; j++)
			{
				Vector2 TilePos        = TileMap->GetTile(j, i)->GetPos();
				Vector2 TileSize       = TileMap->GetTile(j, i)->GetSize();
				ETileOption TileOption = TileMap->GetTile(j, i)->GetTileOption();
				Vector2 WallObjPos     = Vector2(TilePos.x + 0.5*TileSize.x, TilePos.y);
				if (TileOption == ETileOption::Wall)
				{
					WallObj = CreateObject<CWallObject>(std::to_string(WallNum),WALL_PROTO, WallObjPos, TileSize);
					WallNum += 1;
				}
			}
		}
	}
}
*/

CPlayer *CScene::CreatePlayer(const std::string &Name, const Vector2 &Pos, const Vector2 &Size)
{
	CPlayer *Player = (CPlayer*)CSceneManager::GetInst()->GetPlayer();
	if (!Player)
	{
		Player = CreateObject<CPlayer>("Player", Pos, Size);
		Player->SetCharacterInfo(40, 20, PLAYER_INIT_HP, PLAYER_INIT_MP, PLAYER_INIT_STEMINA, 1, 1, 1,
								 NORMAL_SPEED, NORMAL_ATTACK_DISTANCE, NORMAL_ATTACK_DISTANCE);
		return Player;
	}

	Player->SetScene(this);
	Player->SetPos(Pos);
	Player->SetSize(Size);
	Player->SetName(Name);

	Player->SetNotifyFunctions();
	m_ObjList.push_back(Player);

	return Player;
}


void CScene::SetLevel2MonsterAnimation()
{
	SetTurtle1MonsterAnimation();
	SetTurtle2MonsterAnimation();
	SetTurtle3MonsterAnimation();
	SetKamikaze1MonsterAnimation();
	SetKamikazeCageMonsterAnimation();
}

void CScene::SetShieldAnimation()
{
	// Turtle Shield 
	GetSceneResource()->CreateAnimationSequence(SHIELD_TURTLE,
		SHIELD_TURTLE, TEXT("images/Weapon/Shield/TurtleShield.bmp"));
	GetSceneResource()->SetTextureColorKey(SHIELD_TURTLE,
		255, 255, 255);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == 3 && j == 1) break;
			GetSceneResource()->AddAnimationFrameData(SHIELD_TURTLE,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Player Shield 
	GetSceneResource()->CreateAnimationSequence(SHIELD_PLAYER,
		SHIELD_PLAYER, TEXT("images/Weapon/Shield/player_shield.bmp"));
	GetSceneResource()->SetTextureColorKey(SHIELD_PLAYER,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(SHIELD_PLAYER,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Boss Shield 
	GetSceneResource()->CreateAnimationSequence(SHIELD_BOSS,
		SHIELD_BOSS, TEXT("images/Weapon/Shield/boss_shleld.bmp"));
	GetSceneResource()->SetTextureColorKey(SHIELD_BOSS,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i == 2 && j == 1) break;
			GetSceneResource()->AddAnimationFrameData(SHIELD_BOSS,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}
}

void CScene::SetTurtle1MonsterAnimation()
{
	// RightIdle
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE1_RIGHT_IDLE,
		MONSTER_TURTLE1_RIGHT_IDLE, TEXT("images/Monster/TURTLE1/right_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE1_RIGHT_IDLE,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE1_RIGHT_IDLE,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}
	// Right Walk
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE1_RIGHT_WALK,
		MONSTER_TURTLE1_RIGHT_WALK, TEXT("images/Monster/TURTLE1/right_walk.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE1_RIGHT_WALK,
		255, 255, 255);

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE1_RIGHT_WALK,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Right Run
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE1_RIGHT_RUN,
		MONSTER_TURTLE1_RIGHT_RUN, TEXT("images/Monster/TURTLE1/right_run.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE1_RIGHT_RUN,
		255, 255, 255);

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE1_RIGHT_RUN,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}


	// Right Attack
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE1_RIGHT_ATTACK,
		MONSTER_TURTLE1_RIGHT_ATTACK, TEXT("Player/Right/aswing.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE1_RIGHT_ATTACK,
		255, 255, 255);
	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE1_RIGHT_ATTACK,
			i * 176.f, 0.f, 176.f, 89.f);
	}

	// Player Left ---
	// Left Idle 
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE1_LEFT_IDLE,
		MONSTER_TURTLE1_LEFT_IDLE, TEXT("images/Monster/TURTLE1/left_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE1_LEFT_IDLE, 255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 3; j >= 0; --j)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE1_LEFT_IDLE,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Left  Walk
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE1_LEFT_WALK,
		MONSTER_TURTLE1_LEFT_WALK, TEXT("images/Monster/TURTLE1/left_walk.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE1_LEFT_WALK,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 2; j >= 0; j--)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE1_LEFT_WALK,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Left Run
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE1_LEFT_RUN,
		MONSTER_TURTLE1_LEFT_RUN, TEXT("images/Monster/TURTLE1/left_run.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE1_LEFT_RUN,
		255, 255, 255);
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 2; j >= 0; --j)
			{
				GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE1_LEFT_RUN,
					j * 134.f, i * 114.f, 134.f, 114.f);
			}
		}


	// Death ---
	// Left 
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE1_LEFT_DEATH,
		MONSTER_TURTLE1_LEFT_DEATH, TEXT("images/Monster/TURTLE1/left_death.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE1_LEFT_DEATH,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 3; j >= 0; j--)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE1_LEFT_DEATH,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Right 
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE1_RIGHT_DEATH,
		MONSTER_TURTLE1_RIGHT_DEATH, TEXT("images/Monster/TURTLE1/right_death.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE1_RIGHT_DEATH,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE1_RIGHT_DEATH,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Hit ---
	// Left 
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE1_LEFT_HIT,
		MONSTER_TURTLE1_LEFT_HIT, TEXT("images/Monster/TURTLE1/left_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE1_LEFT_HIT,
		255, 255, 255);
	for (int i = 1; i >= 0; --i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE1_LEFT_HIT,
			i * 134.f, 0, 134.f, 114.f);
	}

	// Right 
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE1_RIGHT_HIT,
		MONSTER_TURTLE1_RIGHT_HIT, TEXT("images/Monster/TURTLE1/right_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE1_RIGHT_HIT,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE1_RIGHT_HIT,
			i * 134.f, 0, 134.f, 114.f);
	}
}


void CScene::SetTurtle2MonsterAnimation()
{
	// RightIdle
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE2_RIGHT_IDLE,
		MONSTER_TURTLE2_RIGHT_IDLE, TEXT("images/Monster/TURTLE2/right_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE2_RIGHT_IDLE,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE2_RIGHT_IDLE,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}
	// Right Walk
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE2_RIGHT_WALK,
		MONSTER_TURTLE2_RIGHT_WALK, TEXT("images/Monster/TURTLE2/right_walk.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE2_RIGHT_WALK,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE2_RIGHT_WALK,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Right Run
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE2_RIGHT_RUN,
		MONSTER_TURTLE2_RIGHT_RUN, TEXT("images/Monster/TURTLE2/right_run.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE2_RIGHT_RUN,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE2_RIGHT_RUN,
				j * 134.f,i * 114.f, 134.f, 114.f);
		}
	}

	// Right Attack
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE2_RIGHT_ATTACK,
		MONSTER_TURTLE2_RIGHT_ATTACK, TEXT("Player/Right/aswing.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE2_RIGHT_ATTACK,
		255, 255, 255);
	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE2_RIGHT_ATTACK,
			i * 176.f, 0.f, 176.f, 89.f);
	}

	// Player Left ---
	// Left Idle 
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE2_LEFT_IDLE,
		MONSTER_TURTLE2_LEFT_IDLE, TEXT("images/Monster/TURTLE2/left_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE2_LEFT_IDLE, 255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 3; j>= 0; j--)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE2_LEFT_IDLE,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Left  Walk
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE2_LEFT_WALK,
		MONSTER_TURTLE2_LEFT_WALK, TEXT("images/Monster/TURTLE2/left_walk.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE2_LEFT_WALK,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 2; j >= 0; j--)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE2_LEFT_WALK,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Left Run
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE2_LEFT_RUN,
		MONSTER_TURTLE2_LEFT_RUN, TEXT("images/Monster/TURTLE2/left_run.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE2_LEFT_RUN,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 2; j >= 0; j--)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE2_LEFT_RUN,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Death ---
	// Left 
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE2_LEFT_DEATH,
		MONSTER_TURTLE2_LEFT_DEATH, TEXT("images/Monster/TURTLE2/left_death.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE2_LEFT_DEATH,
		255, 255, 255);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 3; j >= 0; j--)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE2_LEFT_DEATH,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Right 
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE2_RIGHT_DEATH,
		MONSTER_TURTLE2_RIGHT_DEATH, TEXT("images/Monster/TURTLE2/right_death.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE2_RIGHT_DEATH,
		255, 255, 255);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE2_RIGHT_DEATH,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Hit ---
	// Left 
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE2_LEFT_HIT,
		MONSTER_TURTLE2_LEFT_HIT, TEXT("images/Monster/TURTLE2/left_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE2_LEFT_HIT,
		255, 255, 255);
	for (int i = 1; i >= 0; --i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE2_LEFT_HIT,
			i * 134.f, 0, 134.f, 114.f);
	}

	// Right 
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE2_RIGHT_HIT,
		MONSTER_TURTLE2_RIGHT_HIT, TEXT("images/Monster/TURTLE2/right_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE2_RIGHT_HIT,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE2_RIGHT_HIT,
			i * 134.f, 0, 134.f, 114.f);
	}
}

void CScene::SetTurtle3MonsterAnimation()
{
	// RightIdle
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE3_RIGHT_IDLE,
		MONSTER_TURTLE3_RIGHT_IDLE, TEXT("images/Monster/TURTLE3/right_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE3_RIGHT_IDLE,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE3_RIGHT_IDLE,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}
	// Right Walk
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE3_RIGHT_WALK,
		MONSTER_TURTLE3_RIGHT_WALK, TEXT("images/Monster/TURTLE3/right_walk.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE3_RIGHT_WALK,
		255, 255, 255);

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE3_RIGHT_WALK,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Right Run
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE3_RIGHT_RUN,
		MONSTER_TURTLE3_RIGHT_RUN, TEXT("images/Monster/TURTLE3/right_run.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE3_RIGHT_RUN,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE3_RIGHT_RUN,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Right Attack
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE3_RIGHT_ATTACK,
		MONSTER_TURTLE3_RIGHT_ATTACK, TEXT("Player/Right/aswing.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE3_RIGHT_ATTACK,
		255, 255, 255);
	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE3_RIGHT_ATTACK,
			i * 176.f, 0.f, 176.f, 89.f);
	}

	// Player Left ---
	// Left Idle 
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE3_LEFT_IDLE,
		MONSTER_TURTLE3_LEFT_IDLE, TEXT("images/Monster/TURTLE3/left_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE3_LEFT_IDLE, 255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 3; j >=0 ; j--)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE3_LEFT_IDLE,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Left  Walk
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE3_LEFT_WALK,
		MONSTER_TURTLE3_LEFT_WALK, TEXT("images/Monster/TURTLE3/left_walk.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE3_LEFT_WALK,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 2; j >=0; j--)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE3_LEFT_WALK,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Left Run
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE3_LEFT_RUN,
		MONSTER_TURTLE3_LEFT_RUN, TEXT("images/Monster/TURTLE3/left_run.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE3_LEFT_RUN,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 2; j >= 0; j--)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE3_LEFT_RUN,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}


	// Death ---
	// Left 
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE3_LEFT_DEATH,
		MONSTER_TURTLE3_LEFT_DEATH, TEXT("images/Monster/TURTLE3/left_death.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE3_LEFT_DEATH,
		255, 255, 255);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE3_LEFT_DEATH,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Right 
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE3_RIGHT_DEATH,
		MONSTER_TURTLE3_RIGHT_DEATH, TEXT("images/Monster/TURTLE3/right_death.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE3_RIGHT_DEATH,
		255, 255, 255);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE3_RIGHT_DEATH,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Hit ---
	// Left 
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE3_LEFT_HIT,
		MONSTER_TURTLE3_LEFT_HIT, TEXT("images/Monster/TURTLE3/left_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE3_LEFT_HIT,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE3_LEFT_HIT,
			i * 134.f, 0, 134.f, 114.f);
	}

	// Right 
	GetSceneResource()->CreateAnimationSequence(MONSTER_TURTLE3_RIGHT_HIT,
		MONSTER_TURTLE3_RIGHT_HIT, TEXT("images/Monster/TURTLE3/right_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_TURTLE3_RIGHT_HIT,
		255, 255, 255);
	for (int i = 1; i >= 0; --i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_TURTLE3_RIGHT_HIT,
			i * 134.f, 0, 134.f, 114.f);
	}
}

void CScene::SetKamikaze1MonsterAnimation()
{
	// RightIdle
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZE1_RIGHT_IDLE,
		MONSTER_KAMIKAZE1_RIGHT_IDLE, TEXT("images/Monster/Kamikaze1/right_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZE1_RIGHT_IDLE,
		255, 255, 255);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZE1_RIGHT_IDLE,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}
	// Right Walk
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZE1_RIGHT_WALK,
		MONSTER_KAMIKAZE1_RIGHT_WALK, TEXT("images/Monster/Kamikaze1/right_walk.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZE1_RIGHT_WALK,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i == 2 and j == 2) break;
			GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZE1_RIGHT_WALK,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Right Run
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZE1_RIGHT_RUN,
		MONSTER_KAMIKAZE1_RIGHT_RUN, TEXT("images/Monster/Kamikaze1/right_run.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZE1_RIGHT_RUN,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i == 2 and j == 2) break;
			GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZE1_RIGHT_RUN,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}


	// Player Left ---
	// Left Idle 
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZE1_LEFT_IDLE,
		MONSTER_KAMIKAZE1_LEFT_IDLE, TEXT("images/Monster/Kamikaze1/left_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZE1_LEFT_IDLE, 255, 255, 255);

	for (int i = 3; i >= 0; --i)
	{
		for (int j = 3; j >= 0; --j)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZE1_LEFT_IDLE,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Left  Walk
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZE1_LEFT_WALK,
		MONSTER_KAMIKAZE1_LEFT_WALK, TEXT("images/Monster/Kamikaze1/left_walk.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZE1_LEFT_WALK,
		255, 255, 255);
	for (int j = 2; j >=0 ; --j)
	{
		for (int i = 0; i < 3; i++)
		{
			if (i == 2 and j == 0) break;
			GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZE1_LEFT_WALK,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Left Run
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZE1_LEFT_RUN,
		MONSTER_KAMIKAZE1_LEFT_RUN, TEXT("images/Monster/Kamikaze1/left_run.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZE1_LEFT_RUN,
		255, 255, 255);
	for (int j = 2; j >= 0; --j)
	{
		for (int i = 0; i < 3; i++)
		{
			if (i == 2 and j == 0) break;
			GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZE1_LEFT_RUN,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Death ---
	// Left 
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZE1_LEFT_DEATH,
		MONSTER_KAMIKAZE1_LEFT_DEATH, TEXT("images/Monster/Kamikaze1/left_death.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZE1_LEFT_DEATH,
		255, 255, 255);

	for (int i = 0; i <= 3; ++i)
	{
		for (int j = 3; j >= 0; --j)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZE1_LEFT_DEATH,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Right 
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZE1_RIGHT_DEATH,
		MONSTER_KAMIKAZE1_RIGHT_DEATH, TEXT("images/Monster/Kamikaze1/right_death.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZE1_RIGHT_DEATH,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZE1_RIGHT_DEATH,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Hit ---
	// Left 
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZE1_LEFT_HIT,
		MONSTER_KAMIKAZE1_LEFT_HIT, TEXT("images/Monster/Kamikaze1/left_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZE1_LEFT_HIT,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZE1_LEFT_HIT,
			i * 134.f, 0, 134.f, 114.f);
	}

	// Right 
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZE1_RIGHT_HIT,
		MONSTER_KAMIKAZE1_RIGHT_HIT, TEXT("images/Monster/Kamikaze1/right_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZE1_RIGHT_HIT,
		255, 255, 255);
	for (int i = 1; i >= 0; --i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZE1_RIGHT_HIT,
			i * 134.f, 0, 134.f, 114.f);
	}
}

void CScene::SetLevel3MonsterAnimation()
{
	SetKamikaze1MonsterAnimation();
	SetKamikazeCageMonsterAnimation();
	SetBossAnimation();
	SetGeneratorAnimation();
	SetGrenadeAnimation();
}

void CScene::SetKamikazeCageMonsterAnimation()
{
	// 1 Idle
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZECAGE_1_IDLE,
		MONSTER_KAMIKAZECAGE_1_IDLE, TEXT("images/Monster/Cage/cage_1_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZECAGE_1_IDLE,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZECAGE_1_IDLE,
				j * 190.f, i * 180.f, 190.f, 180.f);
		}
	}

	// 1 Hit 
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZECAGE_1_HIT,
		MONSTER_KAMIKAZECAGE_1_HIT, TEXT("images/Monster/Cage/cage_1_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZECAGE_1_HIT,
		255, 255, 255);
	GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZECAGE_1_HIT,
		0.f, 0.f, 190.f, 180.f);
	
	// 2 Idle
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZECAGE_2_IDLE,
		MONSTER_KAMIKAZECAGE_2_IDLE, TEXT("images/Monster/Cage/cage_2_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZECAGE_2_IDLE,
		255, 255, 255);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZECAGE_2_IDLE,
				j * 190.f, i * 180.f, 190.f, 180.f);
		}
	}

	// 2 Hit 
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZECAGE_2_HIT,
		MONSTER_KAMIKAZECAGE_2_HIT, TEXT("images/Monster/Cage/cage_2_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZECAGE_2_HIT,
		255, 255, 255);
	GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZECAGE_2_HIT,
		0.f, 0.f, 190.f, 180.f);

	// 3 Idle
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZECAGE_3_IDLE,
		MONSTER_KAMIKAZECAGE_3_IDLE, TEXT("images/Monster/Cage/cage_3_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZECAGE_3_IDLE,
		255, 255, 255);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZECAGE_3_IDLE,
				j * 190.f, i * 180.f, 190.f, 180.f);
		}
	}

	// 3 Hit 
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZECAGE_3_HIT,
		MONSTER_KAMIKAZECAGE_3_HIT, TEXT("images/Monster/Cage/cage_3_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZECAGE_3_HIT,
		255, 255, 255);
	GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZECAGE_3_HIT,
		0.f, 0.f, 190.f, 180.f);

	// 3 Idle
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZECAGE_DEATH,
		MONSTER_KAMIKAZECAGE_DEATH, TEXT("images/Monster/Cage/cage_death.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZECAGE_DEATH,
		255, 255, 255);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == 2 && j == 1) break;
			GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZECAGE_DEATH,
				j * 190.f, i * 180.f, 190.f, 180.f);
		}
	}
	
}

void CScene::SetBossAnimation()
{
	// Egg Start
	GetSceneResource()->CreateAnimationSequence(MONSTER_BOSS_EGG_START,
		MONSTER_BOSS_EGG_START, TEXT("images/Monster/Boss/Boss_Egg_Start.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_BOSS_EGG_START,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_BOSS_EGG_START,
				j * 248.f, i * 208.f, 248.f, 208.f);
		}
	}
	// Egg Crack
	GetSceneResource()->CreateAnimationSequence(MONSTER_BOSS_EGG_CRACK,
		MONSTER_BOSS_EGG_CRACK, TEXT("images/Monster/Boss/Boss_Egg_Crack.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_BOSS_EGG_CRACK,
		255, 255, 255);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 5; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_BOSS_EGG_CRACK,
				j * 248.f, i * 208.f, 248.f, 208.f);
		}
	}

	// RightIdle
	GetSceneResource()->CreateAnimationSequence(MONSTER_BOSS_RIGHT_IDLE,
		MONSTER_BOSS_RIGHT_IDLE, TEXT("images/Monster/Boss/right_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_BOSS_RIGHT_IDLE,
		255, 255, 255);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == 3 && j == 2) break;
			GetSceneResource()->AddAnimationFrameData(MONSTER_BOSS_RIGHT_IDLE,
				j * 248.f, i * 208.f, 248.f, 208.f);
		}
	}
	// Right Walk
	GetSceneResource()->CreateAnimationSequence(MONSTER_BOSS_RIGHT_WALK,
		MONSTER_BOSS_RIGHT_WALK, TEXT("images/Monster/Boss/right_walk.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_BOSS_RIGHT_WALK,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_BOSS_RIGHT_WALK,
				j * 248.f, i * 208.f, 248.f, 208.f);
		}
	}

	// Right Run
	GetSceneResource()->CreateAnimationSequence(MONSTER_BOSS_RIGHT_RUN,
		MONSTER_BOSS_RIGHT_RUN, TEXT("images/Monster/Boss/right_walk.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_BOSS_RIGHT_RUN,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_BOSS_RIGHT_RUN,
				j * 248.f, i * 208.f, 248.f, 208.f);
		}
	}

	// Player Left ---
	// Left Idle 
	GetSceneResource()->CreateAnimationSequence(MONSTER_BOSS_LEFT_IDLE,
		MONSTER_BOSS_LEFT_IDLE, TEXT("images/Monster/Boss/left_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_BOSS_LEFT_IDLE, 255, 255, 255);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 3; j >= 0; j--)
		{
			if (i == 3 && j == 1) break;
			GetSceneResource()->AddAnimationFrameData(MONSTER_BOSS_LEFT_IDLE,
				j * 248.f, i * 208.f, 248.f, 208.f);
		}
	}

	// Left  Walk
	GetSceneResource()->CreateAnimationSequence(MONSTER_BOSS_LEFT_WALK,
		MONSTER_BOSS_LEFT_WALK, TEXT("images/Monster/Boss/left_walk.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_BOSS_LEFT_WALK,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 2; j >= 0; j--)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_BOSS_LEFT_WALK,
				j * 248.f, i * 208.f, 248.f, 208.f);
		}
	}

	// Left Run
	GetSceneResource()->CreateAnimationSequence(MONSTER_BOSS_LEFT_RUN,
		MONSTER_BOSS_LEFT_RUN, TEXT("images/Monster/Boss/left_walk.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_BOSS_LEFT_RUN,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 2; j >= 0; j--)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_BOSS_LEFT_RUN,
				j * 248.f, i * 208.f, 248.f, 208.f);
		}
	}

	// Death ---
	// Left 
	GetSceneResource()->CreateAnimationSequence(MONSTER_BOSS_LEFT_DEATH,
		MONSTER_BOSS_LEFT_DEATH, TEXT("images/Monster/Boss/left_death.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_BOSS_LEFT_DEATH,
		255, 255, 255);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 3; j >= 0; j--)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_BOSS_LEFT_DEATH,
				j * 248.f, i * 208.f, 248.f, 208.f);
		}
	}

	// Right 
	GetSceneResource()->CreateAnimationSequence(MONSTER_BOSS_RIGHT_DEATH,
		MONSTER_BOSS_RIGHT_DEATH, TEXT("images/Monster/Boss/right_death.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_BOSS_RIGHT_DEATH,
		255, 255, 255);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_BOSS_RIGHT_DEATH,
				j * 248.f, i * 208.f, 248.f, 208.f);
		}
	}

	// Hit ---
	// Left 
	GetSceneResource()->CreateAnimationSequence(MONSTER_BOSS_LEFT_HIT,
		MONSTER_BOSS_LEFT_HIT, TEXT("images/Monster/Boss/left_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_BOSS_LEFT_HIT,
		255, 255, 255);
	for (int i = 1; i >= 0; --i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_BOSS_LEFT_HIT,
			i * 248.f, 0.f, 248.f, 208.f);
	}

	// Right 
	GetSceneResource()->CreateAnimationSequence(MONSTER_BOSS_RIGHT_HIT,
		MONSTER_BOSS_RIGHT_HIT, TEXT("images/Monster/Boss/right_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_BOSS_RIGHT_HIT,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_BOSS_RIGHT_HIT,
			i * 248.f, 0.f, 248.f, 208.f);
	}
}

void CScene::SetGeneratorAnimation()
{
	// Idle
	GetSceneResource()->CreateAnimationSequence(GENERATOR_IDLE,
		GENERATOR_IDLE, TEXT("images/Monster/Boss/generator_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(GENERATOR_IDLE,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 5; j++)
		{
			GetSceneResource()->AddAnimationFrameData(GENERATOR_IDLE,
				j * 134.f, i * 155.f, 134.f, 155.f);
		}
	}

	// Start ---
	GetSceneResource()->CreateAnimationSequence(GENERATOR_START,
		GENERATOR_START, TEXT("images/Monster/Boss/generator_start.bmp"));
	GetSceneResource()->SetTextureColorKey(GENERATOR_START,
		255, 255, 255);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 5; j++)
		{
			GetSceneResource()->AddAnimationFrameData(GENERATOR_START,
				j * 134.f, i * 148.f, 134.f, 148.f);
		}
	}

	// Hit ---
	GetSceneResource()->CreateAnimationSequence(GENERATOR_HIT,
		GENERATOR_HIT, TEXT("images/Monster/Boss/generator_hit.bmp"));

	GetSceneResource()->SetTextureColorKey(GENERATOR_HIT,
		255, 255, 255);
	for (int j = 0; j < 2; j++)
	{
		GetSceneResource()->AddAnimationFrameData(GENERATOR_HIT,
			j * 134.f, 0.f, 134.f, 155.f);
	}

	// Off --- 
	GetSceneResource()->CreateAnimationSequence(GENERATOR_OFF,
		GENERATOR_OFF, TEXT("images/Monster/Boss/generator_off.bmp"));
	GetSceneResource()->SetTextureColorKey(GENERATOR_OFF, 255, 255, 255);

	for (int j = 0; j <= 1; j++)
	{
		GetSceneResource()->AddAnimationFrameData(GENERATOR_OFF,
			j * 134.f, 0.f, 134.f, 148.f);
	}
}

void CScene::SetGrenadeAnimation()
{
	// GreNade on
	GetSceneResource()->CreateAnimationSequence(GRENADE_ON,
		GRENADE_ON, TEXT("images/Monster/Boss/grenade_explosion.bmp"));
	GetSceneResource()->SetTextureColorKey(GRENADE_ON,
		255, 255, 255);

	for (int i = 0; i < 8; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(GRENADE_ON,
			i * 194.f, 0.f, 194.f, 172.f);
	}

	// GreNade off
	GetSceneResource()->CreateAnimationSequence(GRENADE_OFF,
		GRENADE_OFF, TEXT("images/Monster/Boss/explosion_trace.bmp"));
	GetSceneResource()->SetTextureColorKey(GRENADE_OFF,
		255, 255, 255);

	for (int i = 0; i < 8; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(GRENADE_OFF,
			i * 87.f, 0.f, 87.f, 53.f);
	}
}

void CScene::SetKamikaze2MonsterAnimation()
{
	// RightIdle
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZE2_RIGHT_IDLE,
		MONSTER_KAMIKAZE2_RIGHT_IDLE, TEXT("images/Monster/Kamikaze2/right_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZE2_RIGHT_IDLE,
		255, 255, 255);

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZE2_RIGHT_IDLE,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}
	// Right Walk
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZE2_RIGHT_WALK,
		MONSTER_KAMIKAZE2_RIGHT_WALK, TEXT("images/Monster/Kamikaze2/right_walk.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZE2_RIGHT_WALK,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i == 2 and j == 2) break;
			GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZE2_RIGHT_WALK,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Right Run
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZE2_RIGHT_RUN,
		MONSTER_KAMIKAZE2_RIGHT_RUN, TEXT("images/Monster/Kamikaze2/right_run.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZE2_RIGHT_RUN,
		255, 255, 255);
	for (int j = 0; j < 1; j++)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZE2_RIGHT_RUN,
			j * 134.f, 0.f, 134.f, 114.f);
	}

	// Player Left ---
	// Left Idle 
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZE2_LEFT_IDLE,
		MONSTER_KAMIKAZE2_LEFT_IDLE, TEXT("images/Monster/Kamikaze2/left_idle.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZE2_LEFT_IDLE, 255, 255, 255);

	for (int i = 3; i >= 0; --i)
	{
		for (int j = 3; j >= 0; --j)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZE2_LEFT_IDLE,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Left  Walk
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZE2_LEFT_WALK,
		MONSTER_KAMIKAZE2_LEFT_WALK, TEXT("images/Monster/Kamikaze2/left_walk.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZE2_LEFT_WALK,
		255, 255, 255);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 2; j >= 0; --j)
		{
			if (i == 2 and j == 0) break;
			GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZE2_LEFT_WALK,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Left Run
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZE2_LEFT_RUN,
		MONSTER_KAMIKAZE2_LEFT_RUN, TEXT("images/Monster/Kamikaze2/left_run.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZE2_LEFT_RUN,
		255, 255, 255);
	for (int j = 0; j < 1; ++j)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZE2_LEFT_RUN,
			j * 134.f, 0.f, 134.f, 114.f);
	}

	// Death ---
	// Left 
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZE2_LEFT_DEATH,
		MONSTER_KAMIKAZE2_LEFT_DEATH, TEXT("images/Monster/Kamikaze2/left_death.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZE2_LEFT_DEATH,
		255, 255, 255);

	for (int i = 0; i <= 3; ++i)
	{
		for (int j = 3; j >= 0; --j)
		{
			if (i == 2 && j == 2) break;
			GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZE2_LEFT_DEATH,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Right 
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZE2_RIGHT_DEATH,
		MONSTER_KAMIKAZE2_RIGHT_DEATH, TEXT("images/Monster/Kamikaze2/right_death.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZE2_RIGHT_DEATH,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; j++)
		{
			if (i == 2 && j == 1) break;
			GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZE2_RIGHT_DEATH,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Hit ---
	// Left 
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZE2_LEFT_HIT,
		MONSTER_KAMIKAZE2_LEFT_HIT, TEXT("images/Monster/Kamikaze2/left_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZE2_LEFT_HIT,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZE2_LEFT_HIT,
			i * 134.f, 0, 134.f, 114.f);
	}

	// Right 
	GetSceneResource()->CreateAnimationSequence(MONSTER_KAMIKAZE2_RIGHT_HIT,
		MONSTER_KAMIKAZE2_RIGHT_HIT, TEXT("images/Monster/Kamikaze2/right_hit.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_KAMIKAZE2_RIGHT_HIT,
		255, 255, 255);
	for (int i = 1; i >= 0; --i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_KAMIKAZE2_RIGHT_HIT,
			i * 134.f, 0, 134.f, 114.f);
	}
}
