#pragma once
#include "GameObject.h"
class CEquipment : public CGameObject
{
public :
	CEquipment();
	CEquipment(const CEquipment& Equip);
	virtual ~CEquipment();

protected :
	EEquipType EquipType;
};

