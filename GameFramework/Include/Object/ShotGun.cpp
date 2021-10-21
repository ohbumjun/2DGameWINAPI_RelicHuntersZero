#include "ShotGun.h"
#include "Gun.h"
#include "EffectCasing.h"
#include "../Scene/Scene.h"

CShotGun::CShotGun()
{
	m_FireTimeMax = 1.f;
}

CShotGun::CShotGun(const CShotGun& obj) : CGun(obj)
{
	m_FireTimeMax = obj.m_FireTimeMax;
}

CShotGun::~CShotGun()
{
}

void CShotGun::Start()
{
	CGun::Start();
}

bool CShotGun::Init()
{
	if (!CGun::Init()) return false;
	return true;
}

void CShotGun::Update(float DeltaTime)
{
	CGun::Update(DeltaTime);
}

void CShotGun::PostUpdate(float DeltaTime)
{
	CGun::PostUpdate(DeltaTime);
}

void CShotGun::Collision(float DeltaTime)
{
	CGun::Collision(DeltaTime);
}

void CShotGun::Render(HDC hDC)
{
	CGun::Render(hDC);
}

CShotGun* CShotGun::Clone()
{
	return new CShotGun(*this);
}

void CShotGun::CreateCasing(CBullet* Bullet)
{
	Vector2 CasingOffset = m_Owner->GetDir().x > 0 ? Vector2(-m_Size.x * 0.7f, -m_Size.y * 0.5f) : Vector2(m_Size.x * 0.4f, -m_Size.y * 0.5f);
	Vector2 CasingPos = m_Pos + CasingOffset;
	// Owner Left : Casing Right / Owner Right : Casing Left
	float CasingDir = m_Owner->GetDir().x > 0 ? -2.f : 2.f;

	for (int i = 0; i < 5; i++)
	{

		// DirX Will Set Differently according to "CasingName"
		std::string CasingName = m_Owner->GetDir().x > 0 ? "CasingLeft" : "CasingRight";
		CSharedPtr<CEffectCasing> Casing = m_Owner->GetScene()->CreateObject<CEffectCasing>(CasingName,
			EFFECT_CASING_PROTO,
			CasingPos,
			Vector2(20.f, 20.f));

		Casing->SetDirX(CasingDir);

		switch (m_GunInfo.m_GunClass)
		{
		case Light:
		{
			if (m_Owner->GetDir().x < 0) // Owner Lft --> Right Casing
				Casing->SetTexture("LightCasing", TEXT(TEXTURE_CASING_L_TOWARD_R));
			else // Owner Right --> Left Casing
				Casing->SetTexture("LightCasing", TEXT(TEXTURE_CASING_L_TOWARD_L));
		}
		break;
		case Medium:
		{
			if (m_Owner->GetDir().x < 0) // 哭率 --> 坷弗率 Casing
				Casing->SetTexture("MediumCasing", TEXT(TEXTURE_CASING_M_TOWARD_R));
			else
				Casing->SetTexture("MediumCasing", TEXT(TEXTURE_CASING_M_TOWARD_L));
		}
		break;
		case Heavy:
		{
			if (m_Owner->GetDir().x < 0) // 哭率 --> 坷弗率 Casing
				Casing->SetTexture("HeavyCasing", TEXT(TEXTURE_CASING_H_TOWARD_R));
			else
				Casing->SetTexture("HeavyCasing", TEXT(TEXTURE_CASING_H_TOWARD_L));
		}
		break;
		}
		Casing->SetTextureColorKey(255, 255, 255);

	}
}
