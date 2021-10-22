#pragma once

#include "GameInfo.h"

class CGameManager
{
private:
	CGameManager();
	~CGameManager();

private:
	static bool	m_Loop;
	/*
	- HWND =  �ڵ� ������ ���� , �������� �ڵ��ȣ�� �����ؼ� ��� , �ϳ��� ���α׷����� �������� �����츦 ���� �ִ�.
			   �� ������â�� ��ȣ��(HWND) �� �����ϴ°��̴�.
			   �� �������� ��ȣ�� �� ���̴� ��� �� �˰� �־�� �ü������ ������ �� �� �ִ�.
				������ ���� id�� �ĺ��ϱ� ���Ͽ� HWND�� �ʿ��ϴ�.
	- HINSTANCE = �ڵ� �ν��Ͻ�  , ���α׷��� �ν��Ͻ� �ĺ��� , ���� ���� ���α׷� ��ü�� ��üȭ�� �ּ�.
	- ��� : HINSTANCE�� ���α׷� ��ü�� �ڵ��̸� , HWND�� ���α׷����� ������â�� ��ȣ !!

	*/
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	HDC			m_hDC;
	HDC			m_hBackDC;
	HBITMAP		m_hBackBmp;
	HBITMAP		m_hPrevBackBmp;
	Resolution	m_RS;
	class CTimer* m_Timer;
	float		m_TimeScale;
	// Brush
	HBRUSH		m_hGreenBrush;
	HBRUSH		m_hRedBrush;
	HBRUSH		m_hBlueBrush;
	HBRUSH		m_hLightBlueBrush;
	HBRUSH		m_hDarkBlueBrush;

	// Pen
	HPEN		m_hGreenPen;
	HPEN		m_hRedPen;
	HPEN		m_hBluePen;

	// ���� ����Ʈ ������� �ƴ��� --> Editor Scene�� ���� true��
	bool        m_EditorMode;
private :
	int m_MasterVolume;
	int m_BGVolume;
	int m_EffectVolume;
public :
	void IncMasterVolume(){if(m_MasterVolume < 100 )m_MasterVolume += 1;}
	void DecMasterVolume() { if (m_MasterVolume > 0) m_MasterVolume -= 1;}
	void IncBGVolume(){ if (m_BGVolume < 100)m_BGVolume += 1; }
	void DecBGVolume(){ if (m_BGVolume > 0) m_BGVolume -= 1; }
	void IncEffectVolume(){ if (m_EffectVolume < 100)m_EffectVolume += 1; }
	void DecEffectVolume(){ if (m_EffectVolume > 0) m_EffectVolume -= 1; }
public :
	int GetMasterVolume() const{return m_MasterVolume;}
	int GetBGVolume() const{return m_BGVolume;}
	int GetEffectVolume() const{return m_EffectVolume;}
public:
	Resolution GetResolution()	const
	{
		return m_RS;
	}
	HPEN GetGreenPen()	const
	{
		return m_hGreenPen;
	}
	HPEN GetRedPen()	const
	{
		return m_hRedPen;
	}
	HPEN GetBluePen()	const
	{
		return m_hBluePen;
	}
	HBRUSH GetGreenBrush()	const
	{
		return m_hGreenBrush;
	}
	HBRUSH GetRedBrush()	const
	{
		return m_hRedBrush;
	}

	HBRUSH GetLightBlueBrush()	const
	{
		return m_hLightBlueBrush;
	}
	HBRUSH GetBlueBrush()	const
	{
		return m_hBlueBrush;
	}
	HBRUSH GetDarkBlueBrush()	const
	{
		return m_hDarkBlueBrush;
	}
	/*
	HBRUSH GetYellowBrush()	const
	{
		return m_hYellowBrush;
	}

	*/
	HDC GetWindowDC()	const
	{
		return m_hDC;
	}

	HWND GetWindowHandle()	const
	{
		return m_hWnd;
	}

	HINSTANCE GetWindowInstance()	const
	{
		return m_hInst;
	}
	bool IsEditorMode() const
	{
		return m_EditorMode;
	}
	CTimer* GetTimer() const { return m_Timer; }

public:
	void SetEditorMode(bool Mode)
	{
		m_EditorMode = Mode;
	}
	void SetTimeScale(float TimeScale)
	{
		m_TimeScale = TimeScale;
	}
	float GetTimeScale();

public:
	float GetDeltaTime()	const;
	void Exit();

public:
	bool Init(HINSTANCE hInst);
	int Run();


private:
	void Logic();
	bool Update(float DeltaTime);
	bool PostUpdate(float DeltaTime);
	bool Collision(float DeltaTime);
	void Render(float DeltaTime);


private:
	ATOM Register();
	BOOL Create();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


private:
	static CGameManager* m_Inst;

public:
	static CGameManager* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CGameManager;

		return m_Inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}
};

