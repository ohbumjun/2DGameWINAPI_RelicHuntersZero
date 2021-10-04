#pragma once

#include "GameObject.h"
#include "../UI/NumberWidget.h"

class CDamageFont : public CGameObject
{
	friend class CScene;
protected:
	CDamageFont();
	CDamageFont(const CDamageFont& obj);
	virtual ~CDamageFont();

private :
	CNumberWidget* m_NumberWidget;
	
	// 랜덤 방향과 속도
	float m_DirX;
	float m_SpeedX;

public :
	void SetDamageNumber(int Damage)
	{
		m_NumberWidget->SetNumber(Damage);
	}
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CDamageFont* Clone();

};

