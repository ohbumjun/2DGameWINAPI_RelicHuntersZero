#pragma once
#include "MapBase.h"

class CScrollMap :
    public CMapBase
{
	friend class CScene;
protected :
	CScrollMap();
	virtual ~CScrollMap();
protected :
	CSharedPtr<class CTexture> m_ScrollTexture;
	Vector2                    m_ScrollRatio;
public :
	bool SetTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool SetTextureFullPath(const std::string& Name,
		const TCHAR* FullPath);
	bool SetTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool SetTexture(const std::string& Name);
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render(HDC hDC);
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

