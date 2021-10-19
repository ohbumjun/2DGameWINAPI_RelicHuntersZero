#include "BossMonster.h"
#include "../UI/UIBossStateHUD.h"
#include "../Collision/ColliderBox.h"
#include "../Scene/Scene.h"
#include"../Scene/SceneResource.h"

CBossMonster::CBossMonster() :
	m_MissileAttackMaxTime(5.f),
	m_MissileAttackTime(0.f),
	m_GrenadeTime(0.f),
	m_GrenadMaxTime(15.f),
	m_IsGeneratorAlive(false)
{
}

CBossMonster::CBossMonster(const CBossMonster& obj) : CMonster(obj)
{
	m_MissileAttackMaxTime = 5.f;
	m_MissileAttackTime = 0.f;
	m_GrenadeTime = 0.f;
	m_GrenadMaxTime = 15.f;
	m_IsGeneratorAlive = false;
}

CBossMonster::~CBossMonster()
{
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
	}
}

void CBossMonster::GrenadeAttack(float DeltaTime)
{
}

void CBossMonster::GeneratorUpdate(float DeltaTime)
{
	if (m_CharacterInfo.HP <= m_CharacterInfo.HPMax * 0.7)
	{

	}
	else if (m_CharacterInfo.HP <= m_CharacterInfo.HPMax * 0.3)
	{

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

