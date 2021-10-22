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
	bool m_CostChanged;
public :
	ENpc_Type GetNpcType() const
	{
		return m_NpcType;
	}
	int GetCost() const
	{
		return m_Cost;
	}

public :
	void SetNpcType(ENpc_Type NpcType)
	{
		m_NpcType = NpcType;
		m_TypeChanged = true;
	}
	void SetCost(int Cost)
	{
		m_Cost = Cost;
		m_CostChanged = true;
	}
private :
	CSharedPtr<CWidgetComponent> m_TypeWidget;
private :
	CSharedPtr<CWidgetComponent> m_CostHunderedWidget;
	CSharedPtr<CWidgetComponent> m_CostTenWidget;
	CSharedPtr<CWidgetComponent> m_CostOneWidget;
private :
	void CostInit();
	void CostUpdate();
public :
	void TypeChange();
	void CostChange();
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
};

