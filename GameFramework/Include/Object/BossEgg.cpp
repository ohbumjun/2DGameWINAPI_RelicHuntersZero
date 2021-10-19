#include "BossEgg.h"
#include "Gun.h"
#include "BossMonster.h"
#include "EffectSurprise.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Collision/ColliderBox.h"

CBossEgg::CBossEgg() :
    m_EggStartEnable(true),
    m_EggStartTime(1.f),
    m_EggCrackEnable(false),
    m_EggCrackTime(2.f)
{
}

CBossEgg::CBossEgg(const CBossEgg& obj) : CMonster(obj)
{
    m_EggStartEnable = true;
    m_EggCrackEnable = false;
    m_EggStartTime = 1.f;
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
            Vector2 LT = m_Pos - m_Pivot * m_Size + m_Offset;
            Vector2 RT = Vector2(LT.x + m_Size.x * 0.8f, LT.y + m_Size.y * 0.4f);
            CEffectSurprise* Surprise = m_Scene->CreateObject<CEffectSurprise>(SURPRISE_EFFECT, EFFECT_SURPRISE_PROTO,
                RT, Vector2(10.f, 10.f));
            m_Scene->GetSceneResource()->SoundPlay("Fire");
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
    BossMonster->SetCharacterInfo(NORMAL_MONSTER_ATTACK, NORMAL_MONSTER_ARMOR, 1000,
        NORMAL_MONSTER_MP_MAX, 1, 100, 100, 600, NORMAL_MONSTER_ATTACK_DISTANCE, NORMAL_MONSTER_DASH_DISTANCE);
    CGun* BossGun = m_Scene->CreateObject<CGun>("BossGun", BOSS_GUN_PROTO, m_Pos);
    BossMonster->Equip(BossGun);
    Destroy();
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

    CColliderBox* Body = (CColliderBox*)FindCollider("Body");
    Body->Destroy();

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
