#pragma once

#include "../Ref.h"

struct TextureInfo
{
	// �̹��������� �ҷ��ͼ� HBITMAP�� ����� �� HBITMAP�� ȭ�鿡 �����
	// �޸�DC�� ���� ����ϰ� �Ѵ�.
	HDC		hDC;
	HBITMAP	hBmp;
	HBITMAP	hPrev;
	BITMAP	BmpInfo;
	bool	ColorKeyEnable;
	unsigned int	ColorKey;
	// ������ TextureInfo ���� FileName�� ���ϰ� �Ѵ�
	TCHAR FileName[MAX_PATH];

	TextureInfo()	:
		hDC(0),
		hBmp(0),
		hPrev(0),
		BmpInfo{},
		ColorKeyEnable(false),
		ColorKey(0),
		FileName{}
	{
	}

	~TextureInfo()
	{
		// ������ HGDIOBJ�� �ǵ����ش�.
		SelectObject(hDC, hPrev);

		// HBITMAP�� HDC�� �����Ѵ�.
		DeleteObject(hBmp);

		DeleteDC(hDC);
	}
};

class CTexture :
	public CRef
{
public:
	CTexture();
	~CTexture();

private:
	ETexture_Type		m_TextureType;
	std::vector<TextureInfo*>	m_vecTextureInfo;

public:
	ETexture_Type GetTextureType()	const
	{
		return m_TextureType;
	}

	unsigned int GetWidth(int idx = 0)	const
	{
		return (unsigned int)m_vecTextureInfo[idx]->BmpInfo.bmWidth;
	}

	unsigned int GetHeight(int idx = 0)	const
	{
		return (unsigned int)m_vecTextureInfo[idx]->BmpInfo.bmHeight;
	}

	void SetColorKey(const unsigned char r, const unsigned char g,
		const unsigned char b, int Index = 0)
	{
		m_vecTextureInfo[Index]->ColorKeyEnable = true;
		m_vecTextureInfo[Index]->ColorKey = RGB(r, g, b);
	}
	void SetColorKey(unsigned int ColorKey, int Index = 0)
	{
		m_vecTextureInfo[Index]->ColorKeyEnable = true;
		m_vecTextureInfo[Index]->ColorKey = ColorKey;
	}

public:
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name,
		const TCHAR* FullPath);
	bool LoadTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	void Render(HDC hDC, const Vector2& WindowPos, const Vector2& ImgPos,
		const Vector2& Size, int Index = 0);

public :
	void Save(FILE* pFile);
	void Load(FILE* pFile);
	
	// �Ʒ� �Լ��� ����� ������
	// ��ü�� �������� �ʰ��� loading�� �� �� �ִ�
	// �ý����� ����� �����̴�.
	static CTexture* LoadStatic(FILE* pFile, class CScene* Scene);
};

