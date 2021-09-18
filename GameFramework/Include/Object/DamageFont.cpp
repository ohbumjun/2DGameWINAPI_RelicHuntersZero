#include "DamageFont.h"
#include "Bullet.h"
#include "../Scene/Scene.h"

CDamageFont::CDamageFont()
{
}

CDamageFont::CDamageFont(const CDamageFont& obj)
{
}

CDamageFont::~CDamageFont()
{
}

void CDamageFont::Start()
{
	CGameObject::Start();
	m_DirX = rand() % 2 == 0 ? 1.f : -1.f;
	m_SpeedX = rand() % 300;
}

bool CDamageFont::Init()
{
	if (!CGameObject::Init()) return false;

	// DamageFont �ȿ�
		// --> Widget �� ����ְ� 
			// --> �� �ȿ� WidgetComp ��� Widget �� ����ִ�
	SetJumpVelocity(30.f);
	SetPhysicsSimulate(true);
	Jump();
	SetLifeTime(1.f);

	CWidgetComponent* WidgetComp =  CreateWidgetComponent("DamageFont");
	m_NumberWidget = WidgetComp->CreateWidget<CNumberWidget>("DamageFont");

	std::vector<std::wstring>	vecNumberFileName;
	for (int i = 0; i < 10; ++i)
	{
		TCHAR	FileName[256] = {};
		wsprintf(FileName, TEXT("Number/%d.bmp"), i);
		vecNumberFileName.push_back(FileName);
	}
	m_NumberWidget->SetTexture("Number", vecNumberFileName);

	// ���� positioin�� �������� �ʿ� ����
	// �ֳ��ϸ�, CreateWidgetComponent �� ������ ����
	// Owner�� G.O�� ��ġ�� �°� ���õǱ� �����̴�.
	m_NumberWidget->SetSize(29.f, 48.f);

	for (int i = 0; i < 10; i++)
	{
		m_NumberWidget->SetTextureColorKey(255, 255, 255, i);
	}
	return true;
}

void CDamageFont::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	Move(Vector2(m_DirX * m_SpeedX * DeltaTime, 0.f));
}

void CDamageFont::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CDamageFont::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CDamageFont::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CDamageFont* CDamageFont::Clone()
{
	return nullptr;
}
