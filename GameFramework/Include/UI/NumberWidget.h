#pragma once

#include "UIImage.h"
#include "../Resource/Texture.h"

class CNumberWidget :
    public CUIImage
{
public:
	CNumberWidget();
	CNumberWidget(const CNumberWidget& widget);
	virtual ~CNumberWidget();

protected:
	int		m_Number;
	std::vector<int>	m_vecNumber;
	bool    m_RenderEnable;

public:
	void SetNumber(int Number)
	{
		m_Number = Number;
		m_RenderEnable = true;
	}
	void SetRenderEnable(bool state)
	{
		m_RenderEnable = state;
	}
	void AddNumber(int Number)
	{
		m_Number += Number;
	}
	void ClearNumber()
	{
		m_vecNumber.clear();
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& Pos, HDC hDC);
	virtual CNumberWidget* Clone();
};

