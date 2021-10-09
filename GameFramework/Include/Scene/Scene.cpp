
#include "Scene.h"
#include "SceneResource.h"
#include "SceneManager.h"
#include "SceneCollision.h"
#include "Camera.h"
#include "../Object/EffectHit.h"
#include "../Object/EffectDash.h"
#include "../Object/DamageFont.h"
#include "../Object/Monster.h"
#include "../Object/HPPotion.h"
#include "../Object/MPPotion.h"
#include "../Map/MapBase.h"

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
	// m_LaserCollidePos = Vector2(-1.f, -1.f);
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

	// Right Attack
	/*
	GetSceneResource()->CreateAnimationSequence(PLAYER_RIGHT_ATTACK,
		PLAYER_RIGHT_ATTACK, TEXT("Player/Right/aswing.bmp"));

	GetSceneResource()->SetTextureColorKey(PLAYER_RIGHT_ATTACK,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(PLAYER_RIGHT_ATTACK,
			i * 176.f, 0.f, 176.f, 89.f);
	}
	*/
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

	// Left Attack
	/*
	GetSceneResource()->CreateAnimationSequence(PLAYER_LEFT_ATTACK,
		PLAYER_LEFT_ATTACK, TEXT("Player/Left/aswing_left.bmp"));

	GetSceneResource()->SetTextureColorKey(PLAYER_LEFT_ATTACK,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(PLAYER_LEFT_ATTACK,
			i * 176.f, 0.f, 176.f, 89.f);
	}
	*/
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
	GetSceneResource()->CreateAnimationSequence(PLAYER_DEATH,
		PLAYER_LEFT_DASH, TEXT("images/Character/jimmy/death.bmp"));

	GetSceneResource()->SetTextureColorKey(PLAYER_DEATH,
		255, 255, 255);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; j++)
		{
			GetSceneResource()->AddAnimationFrameData(PLAYER_DEATH,
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

	// 총알
	GetSceneResource()->CreateAnimationSequence(BULLET_LIGHT,
		BULLET_LIGHT, TEXT("images/Weapon/Gun/Bullet/spr_bullet_light.bmp"));
	GetSceneResource()->SetTextureColorKey(BULLET_LIGHT,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(BULLET_LIGHT,
			i * 66.f, 0, 66.f, 50.f);
	}

	// 총알
	GetSceneResource()->CreateAnimationSequence(BULLET_MEDIUM,
		BULLET_MEDIUM, TEXT("images/Weapon/Gun/Bullet/spr_bullet_medium.bmp"));
	GetSceneResource()->SetTextureColorKey(BULLET_MEDIUM,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(BULLET_MEDIUM,
			i * 66.f, 0, 66.f, 50.f);
	}

	// 총알
	GetSceneResource()->CreateAnimationSequence(BULLET_HEAVY,
		BULLET_HEAVY, TEXT("images/Weapon/Gun/Bullet/spr_bullet_heavy.bmp"));
	GetSceneResource()->SetTextureColorKey(BULLET_HEAVY,
		255, 255, 255);
	for (int i = 0; i < 2; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(BULLET_HEAVY,
			i * 66.f, 0, 66.f, 50.f);
	}
}

void CScene::SetCollideAnimation()
{
	// 충돌 효과 애니메이션 
	GetSceneResource()->CreateAnimationSequence("HitRight",
		"HitRight", TEXT("Hit2.bmp"));
	GetSceneResource()->SetTextureColorKey("HitRight",
		255, 0, 255);

	for (int i = 0; i < 6; ++i)
	{
		GetSceneResource()->AddAnimationFrameData("HitRight",
			i * 178.f, 0.f, 178.f, 164.f);
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

void CScene::SetLevel1MonsterAnimation()
{
	SetDuck11MonsterAnimation();
}

void CScene::SetDuck11MonsterAnimation()
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
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK1_RIGHT_RUN,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
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

	// Right Dash
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK1_RIGHT_DASH,
		MONSTER_DUCK1_RIGHT_DASH, TEXT("images/Monster/Duck1/right_dash.bmp"));
	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK1_RIGHT_DASH,
		255, 255, 255);
	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK1_RIGHT_DASH,
			i * 134.f, 0, 134.f, 114.f);
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
		for (int j = 0; j < 3; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK1_LEFT_RUN,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
	}

	// Left Dash
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK1_LEFT_DASH,
		MONSTER_DUCK1_LEFT_DASH, TEXT("images/Monster/Duck1/left_dash.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK1_LEFT_DASH,
		255, 255, 255);
	for (int i = 0; i < 3; ++i)
	{
		GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK1_LEFT_DASH,
			i * 134.f, 0.f, 134.f, 114.f);
	}

	// Left Attack
	GetSceneResource()->CreateAnimationSequence(MONSTER_DUCK1_LEFT_DASH,
		MONSTER_DUCK1_LEFT_DASH, TEXT("images/Monster/Duck1/left_dash.bmp"));

	GetSceneResource()->SetTextureColorKey(MONSTER_DUCK1_LEFT_DASH,
		255, 255, 255);

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; j++)
		{
			GetSceneResource()->AddAnimationFrameData(MONSTER_DUCK1_LEFT_DASH,
				j * 134.f, i * 114.f, 134.f, 114.f);
		}
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

void CScene::SetBasicProtoTypes()
{
	// EffectHit
	CEffectHit* EffectHitPrototype = CreatePrototype<CEffectHit>(EFFECT_HIT_PROTO);
	// EffectDash
	CEffectDash* EffectDashPrototype = CreatePrototype<CEffectDash>(DASH_EFFECT);
	// Teleport
	CTeleportMouse* TeleportMousePrototype = CreatePrototype<CTeleportMouse>(TELEPORT_MOUSE_PROTO);
	// Player Bullet 
	CBullet* PlayerBullet = CreatePrototype<CBullet>(PLAYER_BULLET_PROTO);
	PlayerBullet->SetCollisionProfile("PlayerAttack");

	/*
	CBullet* PlayerLaserBullet = CreatePrototype<CBullet>("PlayerLaserProto");
	PlayerLaserBullet->SetCollisionProfile("PlayerLaser");
	PlayerLaserBullet->SetMoveSpeed(LASER_SPEED);
	PlayerLaserBullet->SetDistance(LASER_DISTANCE);
	*/

	// Player SlowMotion Bullet 
	CBullet* SlowMotionAttackBullet = CreatePrototype<CBullet>(PLAYER_SKILL_SLOWMOTION_BULLET_PROTO);
	SlowMotionAttackBullet->SetCollisionProfile("PlayerAttack");

	// Monster
	CMonster* MonsterPrototype = CreatePrototype<CMonster>(MONSTER_DUCK1_PROTO);
	MonsterPrototype->SetMonsterType(EMonster_Type::Duck1);

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
	// Guns
	CGun* GunPistolLight = CreatePrototype<CGun>(GUN_PISTOL_LIGHT_PROTO);
	GunPistolLight->SetTexture(GUN_PISTOL_LIGHT_R, TEXT(TEXTURE_GUN_PISTOL_LIGHT_R));
	GunPistolLight->SetTextureColorKey(255.f, 255.f, 255.f);
	GunPistolLight->SetTexture(GUN_PISTOL_LIGHT_L, TEXT(TEXTURE_GUN_PISTOL_LIGHT_L));
	GunPistolLight->SetTextureColorKey(255.f, 255.f, 255.f);
	GunPistolLight->SetPos(200.f, 200.f);
	GunPistolLight->SetRightTextureName(GUN_PISTOL_LIGHT_R);
	GunPistolLight->SetLeftTextureName(GUN_PISTOL_LIGHT_L);

	GunPistolLight = CreatePrototype<CGun>(GUN_PISTOL_MEDIUM_PROTO);
	GunPistolLight->SetTexture(GUN_PISTOL_MEDIUM_R, TEXT(TEXTURE_GUN_PISTOL_MEDIUM_R));
	GunPistolLight->SetTextureColorKey(255.f, 255.f, 255.f);
	GunPistolLight->SetTexture(GUN_PISTOL_MEDIUM_L, TEXT(TEXTURE_GUN_PISTOL_MEDIUM_L));
	GunPistolLight->SetTextureColorKey(255.f, 255.f, 255.f);
	GunPistolLight->SetPos(200.f, 300.f);
	GunPistolLight->SetGunClass(EGunClass::Medium);
	GunPistolLight->SetGunType(EGun_Type::Medium_Pistol);
	GunPistolLight->SetLeftTextureName(GUN_PISTOL_MEDIUM_L);
	GunPistolLight->SetRightTextureName(GUN_PISTOL_MEDIUM_R);

	GunPistolLight = CreatePrototype<CGun>(GUN_PISTOL_HEAVY_PROTO);
	GunPistolLight->SetTexture(GUN_PISTOL_HEAVY_R, TEXT(TEXTURE_GUN_PISTOL_HEAVY_R));
	GunPistolLight->SetTextureColorKey(255.f, 255.f, 255.f);
	GunPistolLight->SetTexture(GUN_PISTOL_HEAVY_L, TEXT(TEXTURE_GUN_PISTOL_HEAVY_L));
	GunPistolLight->SetTextureColorKey(255.f, 255.f, 255.f);
	GunPistolLight->SetPos(200.f, 400.f);
	GunPistolLight->SetGunClass(EGunClass::Heavy);
	GunPistolLight->SetGunType(EGun_Type::Heavy_Pistol);
	GunPistolLight->SetLeftTextureName(GUN_PISTOL_HEAVY_L);
	GunPistolLight->SetRightTextureName(GUN_PISTOL_HEAVY_R);

	GunPistolLight = CreatePrototype<CGun>(GUN_SHOTGUN_LIGHT_PROTO);
	GunPistolLight->SetTexture(GUN_SHOTGUN_LIGHT_R, TEXT(TEXTURE_GUN_SHOTGUN_LIGHT_R));
	GunPistolLight->SetTextureColorKey(255.f, 255.f, 255.f);
	GunPistolLight->SetTexture(GUN_SHOTGUN_LIGHT_L, TEXT(TEXTURE_GUN_SHOTGUN_LIGHT_L));
	GunPistolLight->SetTextureColorKey(255.f, 255.f, 255.f);
	GunPistolLight->SetPos(200.f, 500.f);
	GunPistolLight->SetGunClass(EGunClass::Light);
	GunPistolLight->SetGunType(EGun_Type::Light_ShotGun);
	GunPistolLight->SetLeftTextureName(GUN_SHOTGUN_LIGHT_L);
	GunPistolLight->SetRightTextureName(GUN_SHOTGUN_LIGHT_R);

	GunPistolLight = CreatePrototype<CGun>(GUN_SHOTGUN_MEDIUM_PROTO);
	GunPistolLight->SetTexture(GUN_SHOTGUN_MEDIUM_R, TEXT(TEXTURE_GUN_SHOTGUN_MEDIUM_R));
	GunPistolLight->SetTextureColorKey(255.f, 255.f, 255.f);
	GunPistolLight->SetTexture(GUN_SHOTGUN_MEDIUM_L, TEXT(TEXTURE_GUN_SHOTGUN_MEDIUM_L));
	GunPistolLight->SetTextureColorKey(255.f, 255.f, 255.f);
	GunPistolLight->SetPos(200.f, 600.f);
	GunPistolLight->SetGunClass(EGunClass::Medium);
	GunPistolLight->SetGunType(EGun_Type::Medium_ShotGun);
	GunPistolLight->SetLeftTextureName(GUN_SHOTGUN_MEDIUM_L);
	GunPistolLight->SetRightTextureName(GUN_SHOTGUN_MEDIUM_R);

	GunPistolLight = CreatePrototype<CGun>(GUN_SHOTGUN_HEAVY_PROTO);
	GunPistolLight->SetTexture(GUN_SHOTGUN_HEAVY_R, TEXT(TEXTURE_GUN_SHOTGUN_HEAVY_R));
	GunPistolLight->SetTextureColorKey(255.f, 255.f, 255.f);
	GunPistolLight->SetTexture(GUN_SHOTGUN_HEAVY_L, TEXT(TEXTURE_GUN_SHOTGUN_HEAVY_L));
	GunPistolLight->SetTextureColorKey(255.f, 255.f, 255.f);
	GunPistolLight->SetPos(200.f, 700.f);
	GunPistolLight->SetGunClass(EGunClass::Heavy);
	GunPistolLight->SetGunType(EGun_Type::Heavy_ShotGun);
	GunPistolLight->SetLeftTextureName(GUN_SHOTGUN_HEAVY_L);
	GunPistolLight->SetRightTextureName(GUN_SHOTGUN_HEAVY_R);
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
	{
		m_Player->Update(DeltaTime);
	}

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
	{
		m_Player->PrevRender();
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

	// ��� ����� �����Ѵ�.
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

	// Bottom ���� ���Ѵ�.
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

	// Bottom ���� ���Ѵ�.
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

	// Bottom ���� ���Ѵ�
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

CPlayer *CScene::CreatePlayer(const std::string &Name, const Vector2 &Pos, const Vector2 &Size)
{
	CPlayer *Player = (CPlayer*)CSceneManager::GetInst()->GetPlayer();

	if (!Player)
	{
		Player = CreateObject<CPlayer>("Player", Pos, Size);
		Player->SetCharacterInfo(0.000000000000001, 20, PLAYER_INIT_HP, PLAYER_INIT_MP, 1, 1, 1,
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
