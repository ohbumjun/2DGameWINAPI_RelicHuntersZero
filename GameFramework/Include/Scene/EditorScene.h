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
	// 스크롤 스피드 : 카메라 이동속도
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
	// 카메라를 움직일 때마다 호출하는 함수
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

