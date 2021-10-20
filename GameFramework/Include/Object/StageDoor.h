#pragma once
#include "GameObject.h"
class CStageDoor :
    public CGameObject
{
	friend class CScene;
public:
	CStageDoor();
	CStageDoor(const CStageDoor& obj);
	virtual ~CStageDoor();

private:
	// Door Type : 다음 Scene이 어디가 될지를 결정한다
	EDoorStage_Type m_DoorStageType;
private :
	bool m_DoorEffectEnable;
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
	virtual CStageDoor* Clone();
public :
	void ChangeScene();

};

