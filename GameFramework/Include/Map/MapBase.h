#pragma once

// CRef를 사용하지 않는 이유
// 어디서 또 다시 가져다쓰거나 하지 않기 때문이다
#include "../GameInfo.h"
class CMapBase
{
	friend class CScene;
protected: 
	CMapBase();
	virtual ~CMapBase();
protected :
	std::string m_Name;
	Vector2 m_Pos;
	Vector2 m_Size;
	class CScene* m_Scene;
	int m_ZOrder;
	bool m_Enable;
	bool m_Active;
public :
	bool IsActive() const
	{
		return m_Active;
	}
	bool IsEnable() const
	{
		return m_Enable;
	}
	int GetZOrder() const
	{
		return m_ZOrder;
	}
	void SetZOrder(int ZOrder)
	{
		m_ZOrder = ZOrder;
	}
	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}
	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}
	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}
	void SetPos(float x, float y)
	{
		m_Pos = Vector2(x, y);
	}
	void SetSize(const Vector2& Size)
	{
		m_Size = Size;
	}
	void SetSize(float x, float y)
	{
		m_Size = Vector2(x, y);
	}
public :
	const std::string& GetName()
	{
		return m_Name;
	}
	const Vector2& GetPos()
	{
		return m_Pos;
	}
	const Vector2& GetSize()
	{
		return m_Size;
	}
	
public :
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render(HDC hDC);
	virtual void Save(const char* FileName, 
		const std::string& PathName = MAP_PATH);
	virtual void Save(FILE* File);
	virtual void SaveFullPath(const char* FullPath);
	virtual void Load(const char* FileName,
		const std::string& PathName = MAP_PATH);
	virtual void Load(FILE* File);
	virtual void LoadFullPath(const char* FullPath);

};

