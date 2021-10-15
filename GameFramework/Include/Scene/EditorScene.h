#pragma once
#include "Scene.h"
class CEditorScene :
    public CScene
{
	friend class CSceneManager;

public:
	CEditorScene();
	virtual ~CEditorScene();

private:
	bool m_Start;
	class CEditorDlg* m_Dlg;
	class CTileMap* m_TileMap;
	// ��ũ�� ���ǵ� : ī�޶� �̵��ӵ�
	float m_ScrollSpeed;
public :
	CEditorDlg* GetEditorDlg()
	{
		return m_Dlg;
	}

public :
	virtual bool Init();
	virtual bool Update(float DeltaTime);
	virtual bool PostUpdate(float DeltaTime);

private :
	void LoadAnimationSequence();
	void LoadSound();
public :
	void OnEditor(float DeltaTime);
	void CreateTileMap();
	void SetTileInfo(
		int CountX, int CountY,
		int SizeX, int SizeY
	);
	void SetTileTexture(class CTexture* Texture);
	void SetSelectedTileTexture(class CTexture* Texture);

public :
	// ī�޶� ������ ������ ȣ���ϴ� �Լ�
	void CameraMoveUp(float DeltaTime);
	void CameraMoveDown(float DeltaTime);
	void CameraMoveRight(float DeltaTime);
	void CameraMoveLeft(float DeltaTime);

	void MouseLButton(float DeltaTime);
	void MouseRButton(float DeltaTime);

public :
	void Save(const char* FullPath);
	void Load(const char* FullPath);
	void TileImageAllClear();
};

