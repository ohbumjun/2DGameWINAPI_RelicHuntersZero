
#include "SceneResource.h"
#include "../Resource/ResourceManager.h"

CSceneResource::CSceneResource()
{
}

CSceneResource::~CSceneResource()
{
	{
		auto	iter = m_mapAnimationSequence.begin();
		auto	iterEnd = m_mapAnimationSequence.end();

		for (; iter != iterEnd;)
		{
			std::string	Name = iter->first;

			// erase를 하면 SharedPtr이므로 자동으로 객체가 제거되며 카운트가
			// 1 감소한다. erase는 지운 다음 iterator를 반환하므로 ++을 안해줘도
			// 된다.
			iter = m_mapAnimationSequence.erase(iter);

			CResourceManager::GetInst()->ReleaseAnimationSequence(Name);
		}
	}

	{
		auto	iter = m_mapTexture.begin();
		auto	iterEnd = m_mapTexture.end();

		for (; iter != iterEnd;)
		{
			std::string	Name = iter->first;

			// erase를 하면 SharedPtr이므로 자동으로 객체가 제거되며 카운트가
			// 1 감소한다. erase는 지운 다음 iterator를 반환하므로 ++을 안해줘도
			// 된다.
			iter = m_mapTexture.erase(iter);

			CResourceManager::GetInst()->ReleaseTexture(Name);
		}
	}
	\
	{
		auto	iter = m_mapSound.begin();
		auto	iterEnd = m_mapSound.end();

		for (; iter != iterEnd;)
		{
			std::string	Name = iter->first;

			// erase를 하면 SharedPtr이므로 자동으로 객체가 제거되며 카운트가
			// 1 감소한다. erase는 지운 다음 iterator를 반환하므로 ++을 안해줘도
			// 된다.
			iter = m_mapSound.erase(iter);

			CResourceManager::GetInst()->ReleaseSound(Name);
		}
	}
}

bool CSceneResource::LoadTexture(const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	if (FindTexture(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
		return false;

	CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

bool CSceneResource::LoadTextureFullPath(const std::string& Name, 
	const TCHAR* FullPath)
{
	if (FindTexture(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
		return false;

	CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

bool CSceneResource::LoadTexture(const std::string& Name,
	const std::vector<std::wstring>& vecFileName, 
	const std::string& PathName)
{
	if (FindTexture(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName))
		return false;

	CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

void CSceneResource::SetTextureColorKey(const std::string& Name, 
	const unsigned char r, const unsigned char g, 
	const unsigned char b, int Index)
{
	CResourceManager::GetInst()->SetTextureColorKey(Name, r, g, b, Index);
}

CTexture* CSceneResource::FindTexture(const std::string& Name)
{
	auto	iter = m_mapTexture.find(Name);

	if (iter == m_mapTexture.end())
		return nullptr;

	return iter->second;
}

bool CSceneResource::CreateAnimationSequence(const std::string& SequenceName,
	const std::string& TextureName)
{
	if (FindAnimationSequence(SequenceName))
		return true;

	if (!CResourceManager::GetInst()->CreateAnimationSequence(
		SequenceName, TextureName))
		return false;

	CAnimationSequence* Sequence = CResourceManager::GetInst()->FindAnimationSequence(SequenceName);

	m_mapAnimationSequence.insert(std::make_pair(SequenceName, Sequence));

	return true;
}

bool CSceneResource::CreateAnimationSequence(const std::string& SequenceName,
	const std::string& TextureName, const TCHAR* FileName, 
	const std::string& PathName)
{
	if (FindAnimationSequence(SequenceName))
		return true;

	LoadTexture(TextureName, FileName, PathName);

	if (!CResourceManager::GetInst()->CreateAnimationSequence(
		SequenceName, TextureName))
		return false;

	CAnimationSequence* Sequence = CResourceManager::GetInst()->FindAnimationSequence(SequenceName);

	m_mapAnimationSequence.insert(std::make_pair(SequenceName, Sequence));

	return true;
}

bool CSceneResource::CreateAnimationSequence(const std::string& SequenceName,
	const std::string& TextureName, 
	const std::vector<std::wstring>& vecFileName,
	const std::string& PathName)
{
	if (FindAnimationSequence(SequenceName))
		return true;

	LoadTexture(TextureName, vecFileName, PathName);

	if (!CResourceManager::GetInst()->CreateAnimationSequence(
		SequenceName, TextureName))
		return false;

	CAnimationSequence* Sequence = CResourceManager::GetInst()->FindAnimationSequence(SequenceName);

	m_mapAnimationSequence.insert(std::make_pair(SequenceName, Sequence));

	return true;
}

void CSceneResource::AddAnimationFrameData(const std::string& SequenceName, 
	const Vector2& StartPos, const Vector2& Size)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);

	if (!Sequence)
		return;

	Sequence->AddFrameData(StartPos, Size);
}

void CSceneResource::AddAnimationFrameData(const std::string& SequenceName,
	float PosX, float PosY, float SizeX, float SizeY)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);

	if (!Sequence)
		return;

	Sequence->AddFrameData(PosX, PosY, SizeX, SizeY);
}

CAnimationSequence* CSceneResource::FindAnimationSequence(
	const std::string& Name)
{
	auto	iter = m_mapAnimationSequence.find(Name);

	if (iter == m_mapAnimationSequence.end())
		return nullptr;

	return iter->second;
}

bool CSceneResource::LoadSound(const std::string& GroupName, bool Loop,
	const std::string& Name, const char* FileName, 
	const std::string& PathName)
{
	if (FindSound(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadSound(
		GroupName, Loop, Name, FileName, PathName))
		return false;

	CSound* Sound = CResourceManager::GetInst()->FindSound(Name);

	m_mapSound.insert(std::make_pair(Name, Sound));

	return true;
}

bool CSceneResource::SetVolume(int Volume)
{
	return CResourceManager::GetInst()->SetVolume(Volume);
}

bool CSceneResource::SetVolume(const std::string& GroupName, int Volume)
{
	return CResourceManager::GetInst()->SetVolume(GroupName, Volume);
}

bool CSceneResource::SoundPlay(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundPlay(Name);
}

bool CSceneResource::SoundStop(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundStop(Name);
}

bool CSceneResource::SoundPause(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundPause(Name);
}

bool CSceneResource::SoundResume(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundResume(Name);
}

CSound* CSceneResource::FindSound(const std::string& Name)
{
	auto	iter = m_mapSound.find(Name);

	if (iter == m_mapSound.end())
		return nullptr;

	return iter->second;
}
