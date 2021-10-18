#pragma once
#include "GameObject.h"

class CNpc :
    public CGameObject
{
	friend class CScene;
	friend class CSceneManager;

protected:
	CNpc();
	CNpc(const CNpc& obj);
	virtual ~CNpc();
private :
	ENpc_Type m_NpcType;

protected:
	CSharedPtr<CWidgetComponent> m_NameWidget;
	CSharedPtr<CWidgetComponent> m_TypeWidget;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
};

