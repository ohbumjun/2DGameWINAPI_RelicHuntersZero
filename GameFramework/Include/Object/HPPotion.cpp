#include "HPPotion.h"
#include "../Collision/ColliderBox.h"

CHPPotion::CHPPotion()
{
	m_PotionType = EPotion_Type::HP;
	m_PotionAmount = 50;
};

CHPPotion::~CHPPotion()
{
}

void CHPPotion::Start()
{
}

bool CHPPotion::Init()
{
    if (!CPotion::Init()) return false;
	return true;
}

void CHPPotion::Update(float DeltaTime)
{
	CPotion::Update(DeltaTime);
}

void CHPPotion::PostUpdate(float DeltaTime)
{
	CPotion::PostUpdate(DeltaTime);
}

void CHPPotion::Collision(float DeltaTime)
{
	CPotion::Collision(DeltaTime);

}

void CHPPotion::Render(HDC hDC)
{
	CPotion::Render(hDC);
}

CHPPotion* CHPPotion::Clone()
{
	return new CHPPotion(*this);
}
