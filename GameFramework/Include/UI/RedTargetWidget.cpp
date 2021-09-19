#include "RedTargetWidget.h"

CRedTargetWidget::CRedTargetWidget()
{
}

CRedTargetWidget::CRedTargetWidget(const CRedTargetWidget& widget) :
    CUIImage(widget)
{
}

CRedTargetWidget::~CRedTargetWidget()
{
}

bool CRedTargetWidget::Init()
{
    if (!CUIImage::Init()) return false;
    return false;
}

void CRedTargetWidget::Update(float DeltaTime)
{
    CUIImage::Update(DeltaTime);
}

void CRedTargetWidget::PostUpdate(float DeltaTime)
{
    CUIImage::PostUpdate(DeltaTime);
}

void CRedTargetWidget::Collision(float DeltaTime)
{
    CUIImage::Collision(DeltaTime);
}

void CRedTargetWidget::Render(HDC hDC)
{
}

void CRedTargetWidget::Render(const Vector2& Pos, HDC hDC)
{
}

CRedTargetWidget* CRedTargetWidget::Clone()
{
    return new CRedTargetWidget(*this);
}
