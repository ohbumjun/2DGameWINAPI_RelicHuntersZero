#pragma once
#include "GameObject.h"
class CEffectBigWindow :
    public CGameObject
{
	friend class CScene;

protected:
	CEffectBigWindow();
	virtual ~CEffectBigWindow();

public:
	virtual bool Init();
};

