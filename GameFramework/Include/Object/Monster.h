#pragma once
#include "Character.h"
class CMonster :
    public CCharacter
{
	friend class CScene;

protected:
	CMonster();
	CMonster(const CMonster& obj);
	virtual ~CMonster();

private:
	float	m_FireTime;
	float	m_FireTimeMax;
	float   m_RandomMoveTime;
	Vector2 m_MoveTargetPos;
	int		m_Count;
private :
	CSharedPtr<CWidgetComponent> m_HPBarWidget;
	CSharedPtr<CWidgetComponent> m_MPBarWidget;
private :
	CSharedPtr<class CPlayer> m_Player;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CMonster* Clone();
	virtual float SetDamage(float Damage);
	virtual void CharacterDestroy();
public :
	void SetPlayer(class CPlayer* Player)
	{
		m_Player = Player;
	}
private :
	void SetRandomTargetDir();
	Vector2 SetRandomTargetPos();
};

