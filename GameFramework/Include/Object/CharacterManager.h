#pragma once
#include "../GameInfo.h"

struct ECharMaxInfo
{
	int AttackMax;
	int ArmorMax;
	int HPMax;
	int MPMax;
	int SpeedMax;
	ECharMaxInfo() :
		AttackMax(20), ArmorMax(20), HPMax(250), MPMax(200), SpeedMax(300){}
};

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
private :
	ECharMaxInfo m_CharMaxInfo;
public :
	int GetCharMaxHP() const     { return m_CharMaxInfo.HPMax; }
	int GetCharMaxMP() const     { return m_CharMaxInfo.MPMax; }
	int GetCharMaxAttack() const { return m_CharMaxInfo.AttackMax; }
	int GetCharMaxArmor() const  { return m_CharMaxInfo.ArmorMax; }
	int GetCharMaxSpeed() const  { return m_CharMaxInfo.SpeedMax; }
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

