#include "BossEgg.h"
#include "BossMonster.h"
#include "../Scene/Scene.h"

CBossEgg::CBossEgg() :
    m_EggStartEnable(true),
    m_EggStartTime(5.f),
    m_EggCrackEnable(false),
    m_EggCrackTime(2.f)
{
}

CBossEgg::CBossEgg(const CBossEgg& obj) : CMonster(obj)
{
    m_EggStartEnable = true;
    m_EggCrackEnable = false;
    m_EggStartTime = 5.f;
    m_EggCrackTime = 2.f;
}

CBossEgg::~CBossEgg()
{
}

void CBossEgg::EggStartUpdate(float DeltaTime)
{
    if (m_EggStartEnable && m_EggStartTime >= 0.f)
    {
        ChangeAnimation(MONSTER_BOSS_EGG_START);
        m_EggStartTime -= DeltaTime;
        if (m_EggStartTime < 0.f)
        {
            m_EggStartEnable = false;
            m_EggCrackEnable = true;
        }
    }
}

void CBossEgg::EggCrackUpdate(float DeltaTime)
{
    if (m_EggCrackTime >= 0.f && m_EggCrackEnable)
    {
        ChangeAnimation(MONSTER_BOSS_EGG_CRACK);
        m_EggCrackTime -= DeltaTime;
        if (m_EggCrackTime < 0.f)
        {
            m_EggCrackEnable = false;
            MonsterAppear();
        }
    }
}

void CBossEgg::EggCrack()
{
}

void CBossEgg::MonsterAppear()
{
    CBossMonster* BossMonster = m_Scene->CreateObject<CBossMonster>("BossMonster",
        Vector2(1000.f, 500.f));
    BossMonster->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, NORMAL_MONSTER_HP_MAX,
        NORMAL_MONSTER_MP_MAX, 1, 100, 100, 600, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
}

void CBossEgg::SetAnimation()
{
}

void CBossEgg::CharacterDestroy()
{
    CMonster::CharacterDestroy();
}

void CBossEgg::Start()
{
    CMonster::Start();
}

bool CBossEgg::Init()
{
    if (!CMonster::Init()) return false;
    // Egg
    AddAnimation(MONSTER_BOSS_EGG_START, true, 5.f);
    AddAnimation(MONSTER_BOSS_EGG_CRACK, true, 5.f);
    return true;
}

void CBossEgg::Update(float DeltaTime)
{
    CCharacter::Update(DeltaTime);
    EggStartUpdate(DeltaTime);
    EggCrackUpdate(DeltaTime);
}

void CBossEgg::PostUpdate(float DeltaTime)
{
    CMonster::PostUpdate(DeltaTime);
}

void CBossEgg::Collision(float DeltaTime)
{
    CMonster::Collision(DeltaTime);
}

void CBossEgg::Render(HDC hDC)
{
    CMonster::Render(hDC);
}
