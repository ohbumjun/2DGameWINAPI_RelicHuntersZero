#include "Potion.h"
#include "../Collision/ColliderSphere.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CPotion::CPotion()
{
}

CPotion::CPotion(const CPotion& Potion)
{
}

CPotion::~CPotion()
{
}

void CPotion::SetTexture(const std::string& Name)
{
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CPotion::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CPotion::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, FullPath);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CPotion::SetTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);
	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CPotion::SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
	if (!m_Texture) return;
	m_Texture->SetColorKey(r, g, b, Index);
}

void CPotion::Start()
{
}

bool CPotion::Init()
{
	if (!CGameObject::Init())
		return false;

	// Add Collider
	CColliderSphere* Body = AddCollider<CColliderSphere>("Body");
	Body->SetRadius(20.f);
	Body->SetOffset(0.f, 25.f);
	Body->SetCollisionProfile("Default");

	return true;
}

void CPotion::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CPotion::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CPotion::Collision(float DeltaTime)
{
	CGameObject::Collision(DeltaTime);
}

void CPotion::Render(HDC hDC)
{
	CGameObject::Render(hDC);
}

CPotion* CPotion::Clone()
{
	return new CPotion();
}
