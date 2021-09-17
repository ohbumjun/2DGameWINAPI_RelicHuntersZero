
#include "Font.h"

CFont::CFont()	:
	m_FontInfo{},
	m_Font(0),
	m_PrevFont(0)
{
}

CFont::~CFont()
{
	if (m_Font)
		DeleteObject(m_Font);
}

bool CFont::LoadFont(const std::string& Name, const TCHAR* FontName, 
	int Width, int Height)
{
	/*
	#define FW_DONTCARE         0
	#define FW_THIN             100
	#define FW_EXTRALIGHT       200
	#define FW_LIGHT            300
	#define FW_NORMAL           400
	#define FW_MEDIUM           500
	#define FW_SEMIBOLD         600
	#define FW_BOLD             700
	#define FW_EXTRABOLD        800
	#define FW_HEAVY            900
	*/

	m_FontInfo.lfHeight = (LONG)Height;
	m_FontInfo.lfWidth = (LONG)Width;
	m_FontInfo.lfCharSet = HANGEUL_CHARSET;
	m_FontInfo.lfWeight = FW_NORMAL;	// 굵기
	m_FontInfo.lfItalic = 0;	// 기울기
	//m_FontInfo.lfEscapement = 1;	// 방향
	m_FontInfo.lfUnderline = 0;	// 밑줄
	m_FontInfo.lfStrikeOut = 0;	// 취소선
	m_FontInfo.lfPitchAndFamily = 2;	// 자간

	lstrcpy(m_FontInfo.lfFaceName, FontName);

	m_Font = CreateFontIndirect(&m_FontInfo);

	return m_Font == nullptr ? false : true;
}

void CFont::SetFont(HDC hDC)
{
	m_PrevFont = (HFONT)SelectObject(hDC, m_Font);
}

void CFont::ResetFont(HDC hDC)
{
	SelectObject(hDC, m_PrevFont);
}
