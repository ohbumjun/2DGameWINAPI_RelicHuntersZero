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
	// 350, 2500
	CharacterInfo AssCharInfo;
	AssCharInfo.Attack = 310, AssCharInfo.Armor = 80, AssCharInfo.HP = 10000,
		AssCharInfo.HPMax = 10000, AssCharInfo.MP = 400, AssCharInfo.MPMax = 400,
		AssCharInfo.Stemina = 6.f, AssCharInfo.SteminaMax = 6.f, AssCharInfo.MoveSpeed = 300.f,
		AssCharInfo.AttackSpeed = 1.f, AssCharInfo.AttackDistance = 600.f;
	m_CharAbilityInfo.insert(std::make_pair(EChar_Type::Ass, AssCharInfo));

	CharacterInfo BiuCharInfo;
	BiuCharInfo.Attack = 350, BiuCharInfo.Armor = 100, BiuCharInfo.HP = 12500,
		BiuCharInfo.HPMax = 12500, BiuCharInfo.MP = 300, BiuCharInfo.MPMax = 300,
		BiuCharInfo.Stemina = 4.f, BiuCharInfo.SteminaMax = 4.f, BiuCharInfo.MoveSpeed = 230.f,
		BiuCharInfo.AttackSpeed = 1.f, BiuCharInfo.AttackDistance = 500.f;
	m_CharAbilityInfo.insert(std::make_pair(EChar_Type::Biu, BiuCharInfo));

	CharacterInfo JimmyCharInfo;
	JimmyCharInfo.Attack = 300, JimmyCharInfo.Armor = 70, JimmyCharInfo.HP = 9800,
		JimmyCharInfo.HPMax = 9800, JimmyCharInfo.MP = 400, JimmyCharInfo.MPMax = 400,
		JimmyCharInfo.Stemina = 5.f, JimmyCharInfo.SteminaMax = 5.f, JimmyCharInfo.MoveSpeed = 260.f,
		JimmyCharInfo.AttackSpeed = 1.f, JimmyCharInfo.AttackDistance = 600.f;
	m_CharAbilityInfo.insert(std::make_pair(EChar_Type::Jimmy, JimmyCharInfo));

	CharacterInfo PinkyCharInfo;
	PinkyCharInfo.Attack = 290, PinkyCharInfo.Armor = 70, PinkyCharInfo.HP = 9200,
		PinkyCharInfo.HPMax = 9200, PinkyCharInfo.MP = 600, PinkyCharInfo.MPMax = 600,
		PinkyCharInfo.Stemina = 3.f, PinkyCharInfo.SteminaMax = 3.f, PinkyCharInfo.MoveSpeed = 350.f,
		PinkyCharInfo.AttackSpeed = 1.f, PinkyCharInfo.AttackDistance = 800.f;
	m_CharAbilityInfo.insert(std::make_pair(EChar_Type::Pinky, PinkyCharInfo));

	CharacterInfo PunnyCharInfo;
	PunnyCharInfo.Attack = 210, PunnyCharInfo.Armor = 60, PunnyCharInfo.HP = 10150,
		PunnyCharInfo.HPMax = 10150, PunnyCharInfo.MP = 500, PunnyCharInfo.MPMax = 500,
		PunnyCharInfo.Stemina = 5.f, PunnyCharInfo.SteminaMax = 5.f, PunnyCharInfo.MoveSpeed = 400.f,
		PunnyCharInfo.AttackSpeed = 1.f, PunnyCharInfo.AttackDistance = 1000.f;
	m_CharAbilityInfo.insert(std::make_pair(EChar_Type::Punny, PunnyCharInfo));

	CharacterInfo RaffCharInfo;
	RaffCharInfo.Attack = 290, RaffCharInfo.Armor = 90, RaffCharInfo.HP = 9750,
		RaffCharInfo.HPMax = 9750, RaffCharInfo.MP = 700, RaffCharInfo.MPMax = 700,
		RaffCharInfo.Stemina = 9.f, RaffCharInfo.SteminaMax = 9.f, RaffCharInfo.MoveSpeed = 350.f,
		RaffCharInfo.AttackSpeed = 1.f, RaffCharInfo.AttackDistance = 700.f;
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
