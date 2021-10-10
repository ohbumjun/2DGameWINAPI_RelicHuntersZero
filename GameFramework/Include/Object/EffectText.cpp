
#include "EffectText.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/ResourceManager.h"

CEffectText::CEffectText() :
	m_DirX(0)
{
	m_TextCapacity = 32;
	m_TextCount = 0;

	m_Text = new TCHAR[m_TextCapacity];

	memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);

	m_TextColor = RGB(0, 0, 0);
}

CEffectText::CEffectText(const CEffectText& obj) :
	CGameObject(obj)
{
	m_TextCapacity  = obj.m_TextCapacity;
	m_TextCount		= obj.m_TextCount;
	m_TextColor		= obj.m_TextColor;
	m_DirX			= obj.m_DirX;

	m_Text = new TCHAR[m_TextCapacity];

	memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);

	memcpy(m_Text, obj.m_Text, sizeof(TCHAR) * m_TextCount);

	m_Font = obj.m_Font;
}

CEffectText::~CEffectText()
{
	SAFE_DELETE_ARRAY(m_Text);
}

void CEffectText::SetFont(const std::string& Name)
{
	m_Font = CResourceManager::GetInst()->FindFont(Name);
}

void CEffectText::Start()
{
	CGameObject::Start();
	// Random Jump Velocity
	SetJumpVelocity(30.f);
	Jump();
}

bool CEffectText::Init()
{
	if (!CGameObject::Init()) return false;

 	SetJumpVelocity(30.f);
	SetPhysicsSimulate(true);
	Jump();
	SetLifeTime(0.5f);

	m_Font = CResourceManager::GetInst()->FindFont("DefaultFont");
	return true;
}

void CEffectText::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CEffectText::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CEffectText::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CEffectText::Render(HDC hDC)
{
	CGameObject::Render(hDC);
	m_Font->SetFont(hDC);

	SetBkMode(hDC, TRANSPARENT);
	// 앞에 :: 을 붙여서 전역함수 SetTextColor 를 호출한다.
	::SetTextColor(hDC, m_TextColor);
	TextOut(hDC, (int)m_Pos.x, (int)m_Pos.y, m_Text, m_TextCount);

	m_Font->ResetFont(hDC);
}

void CEffectText::Render(const Vector2& Pos, HDC hDC)
{
	m_Font->SetFont(hDC);

	SetBkMode(hDC, TRANSPARENT);
	// 앞에 :: 을 붙여서 전역함수 SetTextColor 를 호출한다.
	::SetTextColor(hDC, m_TextColor);
	TextOut(hDC, (int)Pos.x, (int)Pos.y, m_Text, m_TextCount);

	m_Font->ResetFont(hDC);
}

CEffectText* CEffectText::Clone()
{
	return new CEffectText(*this);
}
