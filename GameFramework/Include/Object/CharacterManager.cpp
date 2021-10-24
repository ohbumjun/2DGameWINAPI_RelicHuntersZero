#include "CharacterManager.h"

CCharacterManager* CCharacterManager::m_Inst = nullptr;

CCharacterManager::CCharacterManager() : m_CharAbilityInfo{}
{
}

CCharacterManager::~CCharacterManager()
{
}

void CCharacterManager::InitCharAbilityInfos()
{
	CharacterInfo AssCharInfo;
	m_CharAbilityInfo.insert(std::make_pair(EChar_Type::Ass, AssCharInfo));
	CharacterInfo BiuCharInfo;
	m_CharAbilityInfo.insert(std::make_pair(EChar_Type::Biu, BiuCharInfo));
	CharacterInfo JimmyCharInfo;
	m_CharAbilityInfo.insert(std::make_pair(EChar_Type::Jimmy, JimmyCharInfo));
	CharacterInfo PinkyCharInfo;
	m_CharAbilityInfo.insert(std::make_pair(EChar_Type::Pinky, PinkyCharInfo));
	CharacterInfo PunnyCharInfo;
	m_CharAbilityInfo.insert(std::make_pair(EChar_Type::Punny, PunnyCharInfo));
	CharacterInfo RaffCharInfo;
	m_CharAbilityInfo.insert(std::make_pair(EChar_Type::Raff, RaffCharInfo));
}

CharacterInfo CCharacterManager::FindCharInfo(EChar_Type CharType)
{
	auto iter = m_CharAbilityInfo.find(CharType);
	if (iter == m_CharAbilityInfo.end())
	{
		CharacterInfo nullCharInfo;
		nullCharInfo.HP = -1;
		return nullCharInfo;
	}
	return iter->second;
}

bool CCharacterManager::Init()
{
	InitCharAbilityInfos();
	return true;
}
