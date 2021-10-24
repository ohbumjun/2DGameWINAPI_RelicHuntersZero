#pragma once
#include "UIWindow.h"
class CUISelect :
    public CUIWindow
{
	friend class CScene;

private:
	CUISelect();
	virtual ~CUISelect();
public :
	void CharacterClick(EChar_Type &CharType);
private:
	void SetCharacterInfoBox();
	void SetCharacterInfo(EChar_Type &CharType);
	void SetCharacterAbility(EChar_Type& CharType);
	void SetCharacterImg(EChar_Type &CharType);
private :
	void SetHPAbility(CharacterInfo& CharInfo);
	void SetMPAbility(CharacterInfo& CharInfo);
	void SetAttackAbility(CharacterInfo& CharInfo);
	void SetArmorAbility(CharacterInfo& CharInfo);
	void SetSpeedAbility(CharacterInfo& CharInfo);
private:
	CSharedPtr<class CUIImage> m_CharImg;
	std::list<CSharedPtr<class CUIImage>> m_CharInfo;
	std::list<CSharedPtr<class CUIImage>> m_CommonElemInfo;
	std::vector<CSharedPtr<class CUIText>> m_CharAbilityTexts;
private:
	void CharacterBarInit();
	void AssInit();
	void BiuInit();
	void JimmyInit();
	void PingkyInit();
	void PunnyInit();
	void RaffInit();
private:
	std::unordered_map<std::string, CSharedPtr<class CUIBtnAnimation>> m_BtnAnimations;
public :
	void ResetClick(const CUIBtnAnimation* BtnAnim);
public:
	virtual bool Init();
private:
	void StartClick();
	void ExitClick();
};

