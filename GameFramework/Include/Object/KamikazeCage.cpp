#include "KamiKazeCage.h"
#include "KamiKazeMonster.h"
#include "DamageFont.h"
#include "../Scene/Scene.h"
#include "../Collision/ColliderBox.h"
#include "../Collision/Collider.h"

CKamiKazeCage::CKamiKazeCage()
{
	m_MonsterType = EMonster_Type::KamiKazeCage1;
}

CKamiKazeCage::CKamiKazeCage(const CKamiKazeCage& obj) : CMonster(obj)
{
}

CKamiKazeCage::~CKamiKazeCage()
{
}

void CKamiKazeCage::ChangeIdleAnimation()
{
	CMonster::ChangeIdleAnimation();
	if (m_CharacterInfo.HP >= m_CharacterInfo.HPMax * 0.7)
		ChangeAnimation(MONSTER_KAMIKAZECAGE_1_IDLE);
	if (m_CharacterInfo.HP >= m_CharacterInfo.HPMax * 0.3 && m_CharacterInfo.HP < m_CharacterInfo.HPMax * 0.7)
		ChangeAnimation(MONSTER_KAMIKAZECAGE_2_IDLE);
	else
		ChangeAnimation(MONSTER_KAMIKAZECAGE_3_IDLE);
}

void CKamiKazeCage::ChangeDeathAnimation()
{
	CMonster::ChangeDeathAnimation();
	ChangeAnimation(MONSTER_KAMIKAZECAGE_DEATH);
}

void CKamiKazeCage::ChangeHitAnimation()
{
	CMonster::ChangeHitAnimation();
	if (m_CharacterInfo.HP >= m_CharacterInfo.HPMax * 0.7)
		ChangeAnimation(MONSTER_KAMIKAZECAGE_1_HIT);
	if (m_CharacterInfo.HP >= m_CharacterInfo.HPMax * 0.3 && m_CharacterInfo.HP < m_CharacterInfo.HPMax * 0.7)
		ChangeAnimation(MONSTER_KAMIKAZECAGE_2_HIT);
	else
		ChangeAnimation(MONSTER_KAMIKAZECAGE_3_HIT);
}

void CKamiKazeCage::SetAnimation()
{
	AddAnimation(MONSTER_KAMIKAZECAGE_1_IDLE, true, 2.f);
	AddAnimation(MONSTER_KAMIKAZECAGE_1_HIT, true, 2.f);
	AddAnimation(MONSTER_KAMIKAZECAGE_2_IDLE, true, 2.f);
	AddAnimation(MONSTER_KAMIKAZECAGE_2_HIT, true, 2.f);
	AddAnimation(MONSTER_KAMIKAZECAGE_3_IDLE, true, 2.f);
	AddAnimation(MONSTER_KAMIKAZECAGE_3_HIT, true, 2.f);
	AddAnimation(MONSTER_KAMIKAZECAGE_DEATH, true, 2.f);
}

void CKamiKazeCage::Start()
{
	CMonster::Start();
	SetAnimNames();
	SetAnimationEndNotify<CKamiKazeCage>(MONSTER_KAMIKAZECAGE_DEATH, this, &CKamiKazeCage::CharacterDestroy);

	// Adjust size of collider
	CColliderBox* Body = (CColliderBox*)FindCollider("Body");
	Body->SetExtent(130.f, 100.f);
	Body->SetOffset(0.f, -70.f);
	Body->SetCollisionProfile("Monster");
}

bool CKamiKazeCage::Init()
{
	if (!CMonster::Init()) return false;
	CreateAnimation();
	SetAnimation();

	return true;
}

void CKamiKazeCage::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
}

void CKamiKazeCage::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

void CKamiKazeCage::Collision(float DeltaTime)
{
	CMonster::Collision(DeltaTime);
}

void CKamiKazeCage::Render(HDC hDC)
{
	CMonster::Render(hDC);
}

CKamiKazeCage* CKamiKazeCage::Clone()
{
	return new CKamiKazeCage(*this);
}

void CKamiKazeCage::AIIdle(float DeltaTime)
{
	m_MoveSpeed = NORMAL_MONSTER_MOVE_SPEED;
	m_TraceSurprise = false;
	ChangeIdleAnimation();
}

void CKamiKazeCage::AIWalk(float DeltaTime)
{
	m_TraceSurprise = false;
	m_MoveSpeed = NORMAL_MONSTER_MOVE_SPEED;
	m_MoveSpeed = 150.f;
}

void CKamiKazeCage::AITrace(float DeltaTime, Vector2 PlayerPos)
{
	m_MoveSpeed = NORMAL_MONSTER_MOVE_SPEED;
	ChangeRunAnimation();
	float Angle = GetAngle(m_Pos, PlayerPos);
	SetDir(Angle);
	m_RandomMoveTime = MONSTER_TARGET_POS_LIMIT_TIME;
}

void CKamiKazeCage::AIDeath(float DeltaTime)
{
	CMonster::AIDeath(DeltaTime);
}

void CKamiKazeCage::AIAttack(float DeltaTime, Vector2 PlayerPos)
{
	CMonster::AIAttack(DeltaTime, PlayerPos);
}

void CKamiKazeCage::AIHit(float DeltaTime)
{
	CMonster::AIHit(DeltaTime);
}

void CKamiKazeCage::CharacterDestroy()
{
	// create 6 kamikaes 
	EMonster_Type newMType = m_MonsterType == EMonster_Type::KamiKazeCage1 ? EMonster_Type::KamiKaze1 : EMonster_Type::KamiKaze2;
	std::string ProtoName = m_MonsterType == EMonster_Type::KamiKazeCage1 ? MONSTER_KAMIKAZE1_PROTO : MONSTER_KAMIKAZE2_PROTO;
	CKamiKazeMonster * KamikazeMonster = nullptr;

	for (int i = 0; i < 5; i++)
	{
		KamikazeMonster = m_Scene->CreateObject<CKamiKazeMonster>(
			"Kamikaze"+std::to_string(i), ProtoName,
			m_Pos + i*2.f);
		KamikazeMonster->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, NORMAL_MONSTER_HP_MAX,
			NORMAL_MONSTER_MP_MAX, 1, 100, 100, 100, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
		KamikazeMonster->SetMoveSpeed(NORMAL_MONSTER_MOVE_SPEED);
		KamikazeMonster->SetMonsterType(newMType);
		KamikazeMonster->SetDir(float(1*i % 2), float(1*i % 2));
	}

	CCharacter::CharacterDestroy();
}

