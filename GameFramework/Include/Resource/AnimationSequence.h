#pragma once

#include "Texture.h"

class CAnimationSequence :
    public CRef
{
	friend class CResourceManager;

private:
	CAnimationSequence();
	~CAnimationSequence();

private:
	ETexture_Type	m_TextureType;
	CSharedPtr<CTexture>	m_Texture;
	std::vector<AnimationFrameData>	m_vecFrameData;

public:
	CTexture* GetTexture()
	{
		return m_Texture;
	}

	ETexture_Type GetTextureType()	const
	{
		return m_TextureType;
	}

	const AnimationFrameData& GetFrameData(int Index)	const
	{
		return m_vecFrameData[Index];
	}

	int GetFrameCount()	const
	{
		return (int)m_vecFrameData.size();
	}

public:
	bool Init(const std::string& Name, CTexture* Texture);
	void AddFrameData(const Vector2& StartPos, const Vector2& Size);
	void AddFrameData(float PosX, float PosY, float SizeX, float SizeY);
};

