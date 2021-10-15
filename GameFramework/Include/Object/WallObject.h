#pragma once
#include "GameObject.h"
class CWallObject :
    public CGameObject
{
	friend class CScene;
public:
	CWallObject();
	CWallObject(const CWallObject& obj);
	virtual ~CWallObject();

private:
	// Door Type : ���� Scene�� ��� ������ �����Ѵ�
	EDoorStage_Type m_DoorStageType;

public:
	void SetDoorStageType(EDoorStage_Type DoorStageType)
	{
		m_DoorStageType = DoorStageType;
	}
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CWallObject* Clone();
};

