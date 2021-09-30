#include "MapBase.h"

CMapBase::CMapBase() :
	m_Scene(nullptr),
	m_ZOrder(0),
	m_Enable(true),
	m_Active(true)
{
}

CMapBase::~CMapBase()
{
}

void CMapBase::Start()
{
}

bool CMapBase::Init()
{
	return true;
}

void CMapBase::Update(float DeltaTime)
{
}

void CMapBase::PostUpdate(float DeltaTime)
{
}

void CMapBase::PrevRender()
{
}

void CMapBase::Render(HDC hDC)
{
}
