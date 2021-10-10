#pragma once
#include "GameObject.h"
#include "../Resource/Font.h"

class CEffectText :
    public CGameObject
{
public:
	CEffectText();
	CEffectText(const CEffectText& widget);
	virtual ~CEffectText();

protected:
	TCHAR* m_Text;
	int	m_TextCount;
	int	m_TextCapacity;
	COLORREF	m_TextColor;
	CSharedPtr<CFont>	m_Font;
private:
	// ���� ����� �ӵ�
	float m_DirX;
public:
	void SetDirX(float Dir)
	{
		m_DirX = Dir;
	}

public:
	void SetTextColor(unsigned char r, unsigned char g, unsigned char b)
	{
		m_TextColor = RGB(r, g, b);
	}

	void SetText(const TCHAR* Text)
	{
		// �����ڵ� ���ڿ��� ���̸� ���´�.
		int	Length = lstrlen(Text);

		if (Length + 1 > m_TextCapacity)
		{
			m_TextCapacity = Length + 1;

			SAFE_DELETE_ARRAY(m_Text);

			m_Text = new TCHAR[m_TextCapacity];

			memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);
		}

		lstrcpy(m_Text, Text);

		m_TextCount = lstrlen(Text);
	}

	void AddText(const TCHAR* Text)
	{
		int	Length = m_TextCount + lstrlen(Text);

		if (Length + 1 > m_TextCapacity)
		{
			m_TextCapacity = Length + 1;

			TCHAR* Array = new TCHAR[m_TextCapacity];

			memset(Array, 0, sizeof(TCHAR) * m_TextCapacity);

			lstrcpy(Array, m_Text);

			SAFE_DELETE_ARRAY(m_Text);

			m_Text = Array;
		}

		lstrcat(m_Text, Text);

		m_TextCount += lstrlen(Text);
	}

	void pop_back()
	{
		--m_TextCount;

		m_Text[m_TextCount] = 0;
	}

	void Clear()
	{
		m_TextCount = 0;

		memset(m_Text, 0, sizeof(TCHAR) * m_TextCapacity);
	}

	void SetFont(const std::string& Name);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual void Render(const Vector2& Pos, HDC hDC);
	virtual CEffectText* Clone();
};

