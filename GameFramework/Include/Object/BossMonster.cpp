#include "BossMonster.h"
#include "../UI/UIBossStateHUD.h"
#include "../Collision/ColliderBox.h"
#include "../Scene/Scene.h"
#include"../Scene/SceneResource.h"
#include "EffectGrenade.h"
#include "GeneratorTower.h"

CBossMonster::CBossMonster() :
	m_MissileAttackMaxTime(5.f),
	m_MissileAttackTime(0.f),
	m_GrenadeTime(0.f),
	m_GrenadMaxTime(3.f),
	m_IsGenerator1Alive(false),
	m_IsGenerator2Alive(false)
{
	m_MonsterType = EMonster_Type::Boss;
}

CBossMonster::CBossMonster(const CBossMonster& obj) : CMonster(obj)
{
	m_MissileAttackMaxTime = 5.f;
	m_MissileAttackTime = 0.f;
	m_GrenadeTime = 0.f;
	m_GrenadMaxTime = 3.f;
	m_IsGenerator1Alive = false;
	m_IsGenerator2Alive = false;
}

CBossMonster::~CBossMonster()
{
}

void CBossMonster::CreateGenerator(int GeneratorNum)
{
	CGeneratorTower* GeneratorTower = m_Scene->CreateObject<CGeneratorTower>("GeneratorTower", Vector2(600.f, 1200.f));
	GeneratorTower->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, 1000,
		NORMAL_MONSTER_MP_MAX, 1, 100, 100, 600, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
	GeneratorTower->SetBossMonster(this);
	if (GeneratorNum == 1) m_IsGenerator1Alive = true;
	else m_IsGenerator2Alive = true;
	m_ShieldEnable = true;
}


void CBossMonster::MissileUpdate(float DeltaTime)
{
	m_MissileAttackTime += DeltaTime;
	if (m_MissileAttackTime >= m_MissileAttackMaxTime)
	{
		m_MissileAttackTime -= m_MissileAttackMaxTime;
	}
}

void CBossMonster::MissileAttack(float DeltaTime)
{
}

void CBossMonster::GrenadeUpdate(float DeltaTime)
{
	m_GrenadeTime += DeltaTime;
	if (m_GrenadeTime >= m_GrenadMaxTime)
	{
		m_GrenadeTime -= m_GrenadMaxTime;
		for (float f = 0.0f; f < 2 * M_PI; f += M_PI / 6.f )
		{
			CEffectGrenade* EffectGrenade = m_Scene->CreateObject<CEffectGrenade>(
				"GrenadeEffect",
				GRENADE_PROTO, 
				Vector2(
					(m_Pos.x - m_Offset.x) + m_Size.Length() * 1.8f * cos(f), 
					(m_Pos.y - m_Size.y - m_Offset.y) + m_Size.Length() * 1.8f * sin(f))
				);
			EffectGrenade->SetTexture("Grenade", TEXT("images/Monster/Boss/grenade.bmp"));
			EffectGrenade->SetTextureColorKey(255, 255, 255);
		}
	}
}

void CBossMonster::GrenadeAttack(float DeltaTime)
{
}

void CBossMonster::GeneratorUpdate(float DeltaTime)
{
	if (m_CharacterInfo.HP > m_CharacterInfo.HPMax * 0.45f
		&& m_CharacterInfo.HP <= m_CharacterInfo.HPMax * 0.7f)
	{
		// Make Generator 
		if (!m_IsGenerator1Alive)
			CreateGenerator(1);
		// Make Kage 

		// Make Red Line To Generator From Monster
	}
	else if (m_CharacterInfo.HP > m_CharacterInfo.HPMax * 0.25f && 
		m_CharacterInfo.HP <= m_CharacterInfo.HPMax * 0.45f)
	{
		if (!m_IsGenerator2Alive)
			CreateGenerator(2);
	}
}

void CBossMonster::GeneratorAttack(float DeltaTime)
{
}

void CBossMonster::UIUpdate(float DeltaTime)
{
	CUIBossStateHUD* State = m_Scene->FindUIWindow<CUIBossStateHUD>("BossStateHUD");
	State->SetHPPercent(m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax);
}

void CBossMonster::ChangeIdleAnimation()
{
	CMonster::ChangeIdleAnimation();

	if (m_Dir.x < 0)
		ChangeAnimation(MONSTER_BOSS_LEFT_IDLE);
	else
		ChangeAnimation(MONSTER_BOSS_RIGHT_IDLE);
}

void CBossMonster::ChangeMoveAnimation()
{
	CMonster::ChangeMoveAnimation();
	if (m_Dir.x < 0)
		ChangeAnimation(MONSTER_BOSS_LEFT_WALK);
	else
		ChangeAnimation(MONSTER_BOSS_RIGHT_WALK);
}

void CBossMonster::ChangeRunAnimation()
{
	CMonster::ChangeRunAnimation();
	if (m_Dir.x < 0)
		ChangeAnimation(MONSTER_BOSS_LEFT_RUN);
	else
		ChangeAnimation(MONSTER_BOSS_RIGHT_RUN);
}

void CBossMonster::ChangeDeathAnimation()
{
	CMonster::ChangeDeathAnimation();
	if (m_Dir.x < 0)
		ChangeAnimation(MONSTER_BOSS_LEFT_DEATH);
	else
		ChangeAnimation(MONSTER_BOSS_RIGHT_DEATH);
}

void CBossMonster::ChangeHitAnimation()
{
	CMonster::ChangeHitAnimation();
	if (m_Dir.x < 0)
		ChangeAnimation(MONSTER_BOSS_LEFT_HIT);
	else
		ChangeAnimation(MONSTER_BOSS_RIGHT_HIT);
}

void CBossMonster::SetAnimation()
{
	// Right
	AddAnimation(MONSTER_BOSS_RIGHT_IDLE, true, 2.f);
	AddAnimation(MONSTER_BOSS_RIGHT_WALK, true, 1.f);
	AddAnimation(MONSTER_BOSS_RIGHT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_BOSS_RIGHT_RUN, true, 0.6f);
	AddAnimation(MONSTER_BOSS_RIGHT_HIT, false, 1.f);
	AddAnimation(MONSTER_BOSS_RIGHT_DEATH, false, 1.0f);

	// Left
	AddAnimation(MONSTER_BOSS_LEFT_IDLE, true, 2.f);
	AddAnimation(MONSTER_BOSS_LEFT_WALK, true, 1.f);
	AddAnimation(MONSTER_BOSS_LEFT_ATTACK, false, 0.1f);
	AddAnimation(MONSTER_BOSS_LEFT_RUN, true, 0.6f);
	AddAnimation(MONSTER_BOSS_LEFT_HIT, false, 1.f);
	AddAnimation(MONSTER_BOSS_LEFT_DEATH, false, 1.0f);

}

void CBossMonster::CharacterDestroy()
{
	CMonster::CharacterDestroy();
}

void CBossMonster::Start()
{
	CMonster::Start();
}

bool CBossMonster::Init()
{
	if (!CMonster::Init()) return false;
	SetAnimation();

	m_HPBarWidget->SetPos(-25.f, -155.f);
	m_MPBarWidget->SetPos(-25.f, -145.f);

	CColliderBox* Body = (CColliderBox*)FindCollider("Body");
	Body->SetOffset(0.f, -60.5f);

	return true;
}

void CBossMonster::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);

	MissileUpdate(DeltaTime);
	GrenadeUpdate(DeltaTime);
	GeneratorUpdate(DeltaTime);
	UIUpdate(DeltaTime);
}

void CBossMonster::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

void CBossMonster::Collision(float DeltaTime)
{
	CMonster::Collision(DeltaTime);
}

void CBossMonster::Render(HDC hDC)
{
	CMonster::Render(hDC);
}

CBossMonster* CBossMonster::Clone()
{
	return new CBossMonster(*this);
}

