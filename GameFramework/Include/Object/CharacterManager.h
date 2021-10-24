#pragma once
#include "../GameInfo.h"

class CCharacterManager
{
private:
	CCharacterManager();
	~CCharacterManager();
private:
	std::unordered_map<EChar_Type, CharacterInfo> m_CharAbilityInfo;
public:
	void InitCharAbilityInfos();
	CharacterInfo FindCharInfo(EChar_Type CharType);
public:
	bool Init();
private:
	static CCharacterManager* m_Inst;

public:
	static CCharacterManager* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CCharacterManager;

		return m_Inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}
};

