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
	int m_Cost;
	bool m_TypeChanged;
public :
	void SetNpcType(ENpc_Type NpcType)
	{
		m_NpcType = NpcType;
		m_TypeChanged = true;
	}
	void SetCost(int Amount)
	{
		m_Cost = Amount;
	}
protected:
	CSharedPtr<CWidgetComponent> m_TypeWidget;
public :
	void TypeChange();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
};

