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
	AssCharInfo.Attack = 180, AssCharInfo.Armor = 60, AssCharInfo.HP = 400,
		AssCharInfo.HPMax = 400, AssCharInfo.MP = 200, AssCharInfo.MPMax = 200,
		AssCharInfo.Stemina = 6.f, AssCharInfo.SteminaMax = 6.f, AssCharInfo.MoveSpeed = 300.f,
		AssCharInfo.AttackSpeed = 1.f, AssCharInfo.AttackDistance = 600.f;
	m_CharAbilityInfo.insert(std::make_pair(EChar_Type::Ass, AssCharInfo));

	CharacterInfo BiuCharInfo;
	BiuCharInfo.Attack = 210, BiuCharInfo.Armor = 80, BiuCharInfo.HP = 600,
		BiuCharInfo.HPMax = 600, BiuCharInfo.MP = 100, BiuCharInfo.MPMax = 100,
		BiuCharInfo.Stemina = 4.f, BiuCharInfo.SteminaMax = 4.f, BiuCharInfo.MoveSpeed = 230.f,
		BiuCharInfo.AttackSpeed = 1.f, BiuCharInfo.AttackDistance = 500.f;
	m_CharAbilityInfo.insert(std::make_pair(EChar_Type::Biu, BiuCharInfo));

	CharacterInfo JimmyCharInfo;
	JimmyCharInfo.Attack = 170, JimmyCharInfo.Armor = 80, JimmyCharInfo.HP = 500,
		JimmyCharInfo.HPMax = 500, JimmyCharInfo.MP = 300, JimmyCharInfo.MPMax = 300,
		JimmyCharInfo.Stemina = 5.f, JimmyCharInfo.SteminaMax = 5.f, JimmyCharInfo.MoveSpeed = 260.f,
		JimmyCharInfo.AttackSpeed = 1.f, JimmyCharInfo.AttackDistance = 600.f;
	m_CharAbilityInfo.insert(std::make_pair(EChar_Type::Jimmy, JimmyCharInfo));

	CharacterInfo PinkyCharInfo;
	PinkyCharInfo.Attack = 190, PinkyCharInfo.Armor = 70, PinkyCharInfo.HP = 450,
		PinkyCharInfo.HPMax = 450, PinkyCharInfo.MP = 300, PinkyCharInfo.MPMax = 300,
		PinkyCharInfo.Stemina = 3.f, PinkyCharInfo.SteminaMax = 3.f, PinkyCharInfo.MoveSpeed = 350.f,
		PinkyCharInfo.AttackSpeed = 1.f, PinkyCharInfo.AttackDistance = 800.f;
	m_CharAbilityInfo.insert(std::make_pair(EChar_Type::Pinky, PinkyCharInfo));

	CharacterInfo PunnyCharInfo;
	PunnyCharInfo.Attack = 110, PunnyCharInfo.Armor = 30, PunnyCharInfo.HP = 550,
		PunnyCharInfo.HPMax = 550, PunnyCharInfo.MP = 100, PunnyCharInfo.MPMax = 100,
		PunnyCharInfo.Stemina = 5.f, PunnyCharInfo.SteminaMax = 5.f, PunnyCharInfo.MoveSpeed = 450.f,
		PunnyCharInfo.AttackSpeed = 1.f, PunnyCharInfo.AttackDistance = 1000.f;
	m_CharAbilityInfo.insert(std::make_pair(EChar_Type::Punny, PunnyCharInfo));

	CharacterInfo RaffCharInfo;
	RaffCharInfo.Attack = 150, RaffCharInfo.Armor = 50, RaffCharInfo.HP = 450,
		RaffCharInfo.HPMax = 450, RaffCharInfo.MP = 400, RaffCharInfo.MPMax = 400,
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
