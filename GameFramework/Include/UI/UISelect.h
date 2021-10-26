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
	CharacterInfo m_SelectedCharInfo;
	EChar_Type m_SelectedCharType;
private:
	CSharedPtr<class CUIImage> m_CharImg;
	CSharedPtr<class CButton> m_StartBtn;
	CSharedPtr<class CUIText> m_StartTxt;
	void SetCharacterInfoBox();
	void SetCharacterAbility(EChar_Type& CharType);
	void SetCharacterImg(EChar_Type &CharType);
	void SetStartBtn();
private :
	void SetHPAbility(CharacterInfo& CharInfo, int StartIdx);
	void SetMPAbility(CharacterInfo& CharInfo, int StartIdx);
	void SetAttackAbility(CharacterInfo& CharInfo, int StartIdx);
	void SetArmorAbility(CharacterInfo& CharInfo, int StartIdx);
	void SetSpeedAbility(CharacterInfo& CharInfo, int StartIdx);
private:
	void CharacterBarInit();
	void AssInit();
	void BiuInit();
	void JimmyInit();
	void PingkyInit();
	void PunnyInit();
	void RaffInit();
private :
	void CharacterStatsInit();
	std::vector<CSharedPtr<class CUIImage>> m_StatsUIs;
	std::list<CSharedPtr<class CUIImage>>   m_CharImgBackGrounds;
	std::vector<CSharedPtr<class CUIText>>  m_CharAbilityTexts;
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

