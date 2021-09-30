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
};

