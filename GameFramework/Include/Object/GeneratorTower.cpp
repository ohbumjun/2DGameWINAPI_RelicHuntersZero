#include "GeneratorTower.h"
#include "../GameManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Camera.h"

CGeneratorTower::CGeneratorTower() 
{
	m_IsCreated = true;
}

CGeneratorTower::CGeneratorTower(const CGeneratorTower& obj) : CMonster(obj)
{
	m_IsCreated = true;
}

CGeneratorTower::~CGeneratorTower()
{
}

void CGeneratorTower::ChangeIdleAnimation()
{
	CMonster::ChangeIdleAnimation();
	ChangeAnimation(GENERATOR_IDLE);
}

void CGeneratorTower::ChangeMoveAnimation()
{
	CMonster::ChangeMoveAnimation();
	ChangeAnimation(GENERATOR_IDLE);
}

void CGeneratorTower::ChangeRunAnimation()
{
	CMonster::ChangeRunAnimation();
	ChangeAnimation(GENERATOR_IDLE);
}

void CGeneratorTower::ChangeDeathAnimation()
{
	CMonster::ChangeDeathAnimation();
	ChangeAnimation(GENERATOR_OFF);
}

void CGeneratorTower::ChangeHitAnimation()
{
	CMonster::ChangeHitAnimation();
	ChangeAnimation(GENERATOR_HIT);
}


void CGeneratorTower::CharacterDestroy()
{
	Destroy();
	m_BossMonster->SetShieldEnable(false);
}

void CGeneratorTower::Start()
{
	CMonster::Start();
	SetAnimationEndNotify<CGeneratorTower>(GENERATOR_OFF, this, &CGeneratorTower::CharacterDestroy);
	SetAnimationEndNotify<CGeneratorTower>(GENERATOR_START, this, &CGeneratorTower::ChangeIdleAnimation);
}

bool CGeneratorTower::Init()
{
	if (!CMonster::Init()) return false;

	AddAnimation(GENERATOR_IDLE, true, 2.f);
	AddAnimation(GENERATOR_START, false, 5.f);
	AddAnimation(GENERATOR_OFF, true, 1.f);
	AddAnimation(GENERATOR_HIT, false, 0.6f);
	SetCurrentAnimation(GENERATOR_START);

	return true;
}

void CGeneratorTower::Update(float DeltaTime)
{
	CMonster::Update(DeltaTime);
	// prevent Movement
	m_MoveSpeed = 0.f;
}

void CGeneratorTower::PostUpdate(float DeltaTime)
{
	CMonster::PostUpdate(DeltaTime);
}

void CGeneratorTower::Collision(float DeltaTime)
{
	CMonster::Collision(DeltaTime);
}

void CGeneratorTower::Render(HDC hDC)
{
	CMonster::Render(hDC);
	if (m_BossMonster) RenderLineToBossMonster(hDC);
}

CGeneratorTower* CGeneratorTower::Clone()
{
	return new CGeneratorTower(*this);
}

void CGeneratorTower::RenderLineToBossMonster(HDC hDC)
{
	Vector2 CameraPos = m_Scene->GetCamera()->GetPos();
	HPEN Pen = CGameManager::GetInst()->GetBluePen();

	Vector2 BossPos = m_BossMonster->GetPos() - CameraPos;
	BossPos.y -= m_BossMonster->GetSize().y * 0.5f;
	Vector2 TowerPos = m_Pos - CameraPos;
	TowerPos.y -= m_Size.y * 0.5f;

	HGDIOBJ Prev = SelectObject(hDC, Pen);
	MoveToEx(hDC, (int)TowerPos.x, (int)TowerPos.y, nullptr);
	LineTo(hDC, (int)BossPos.x, (int)(BossPos.y));
	SelectObject(hDC, Prev);
}

