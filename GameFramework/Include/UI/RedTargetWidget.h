#pragma once
#include"UIImage.h"
class CRedTargetWidget : public CUIImage
{
public:
	CRedTargetWidget();
	CRedTargetWidget(const CRedTargetWidget& widget);
	virtual ~CRedTargetWidget();

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& Pos, HDC hDC);
	virtual CRedTargetWidget* Clone();
};

