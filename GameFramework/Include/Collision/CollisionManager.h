#pragma once

#include "../GameInfo.h"

class CCollisionManager
{
private:
	CCollisionManager();
	~CCollisionManager();

private:
	std::unordered_map<std::string, CollisionProfile*>	m_mapProfile;

public:
	bool Init();
	bool CreateProfile(const std::string& Name, ECollision_Channel Channel,
		bool Enable, ECollision_State State = ECollision_State::Collision);
	bool SetCollisionState(const std::string& Name, ECollision_Channel Channel,
		ECollision_State State);
	CollisionProfile* FindProfile(const std::string& Name);

private:
	static CCollisionManager* m_Inst;

public:
	static CCollisionManager* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CCollisionManager;

		return m_Inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}
};

