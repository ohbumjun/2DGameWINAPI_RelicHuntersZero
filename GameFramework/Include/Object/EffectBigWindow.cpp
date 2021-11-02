#include "EffectBigWindow.h"
#include "../Scene/Scene.h"
#include "../Collision/ColliderSphere.h"

CEffectBigWindow::CEffectBigWindow()
{
	m_ObjType = EObject_Type::Effect;
}


CEffectBigWindow::~CEffectBigWindow()
{
}

bool CEffectBigWindow::Init()
{
	if (!CGameObject::Init())
		return false;

	SetPivot(0.5f, 0.5f);
	CreateAnimation();
	AddAnimation("BigWindow", true, 3.f);

	return true;
}
