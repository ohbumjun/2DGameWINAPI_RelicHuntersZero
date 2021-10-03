#include "MapBase.h"
#include "../PathManager.h"

CMapBase::CMapBase() :
	m_Scene(nullptr),
	m_ZOrder(0),
	m_Enable(true),
	m_Active(true)
{
}

CMapBase::~CMapBase()
{
}

void CMapBase::Start()
{
}

bool CMapBase::Init()
{
	return true;
}

void CMapBase::Update(float DeltaTime)
{
}

void CMapBase::PostUpdate(float DeltaTime)
{
}

void CMapBase::PrevRender()
{
}

void CMapBase::Render(HDC hDC)
{
}

void CMapBase::Save(const char* FileName, const std::string& PathName)
{
	char FullPath[MAX_PATH] = {};
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);
	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);
	strcat_s(FullPath, FileName);
	SaveFullPath(FullPath);
}

void CMapBase::Save(FILE* pFile)
{
	// 이름의 길이
	int Length = (int)m_Name.length();
	// 이름 길이 먼저 저장, 그 다음에 이름 저장
	fwrite(&Length, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), sizeof(char), Length, pFile);

	fwrite(&m_Pos, sizeof(Vector2),1,pFile);
	fwrite(&m_Size, sizeof(Vector2),1,pFile);

	fwrite(&m_ZOrder, sizeof(int),1,pFile);
	fwrite(&m_Active, sizeof(bool),1,pFile);
	fwrite(&m_Enable, sizeof(bool),1,pFile);

}

void CMapBase::SaveFullPath(const char* FullPath)
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, FullPath, "wb");
	if (!pFile) return;
	Save(pFile); // TileMap의 재정의된 함수가 호출된다.
	fclose(pFile);
}

void CMapBase::Load(const char* FileName, const std::string& PathName)
{
	char FullPath[MAX_PATH] = {};
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);
	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);
	strcat_s(FullPath, FileName);
	LoadFullPath(FullPath);
}

void CMapBase::Load(FILE* pFile)
{
	// 이름의 길이
	int Length = 0;
	fread(&Length, sizeof(int), 1, pFile);

	char Name[256] = {};
	fread(Name, sizeof(char), Length, pFile);
	m_Name = Name;

	fread(&m_Pos, sizeof(Vector2), 1, pFile);
	fread(&m_Size, sizeof(Vector2), 1, pFile);

	fread(&m_ZOrder, sizeof(int), 1, pFile);
	fread(&m_Active, sizeof(bool), 1, pFile);
	fread(&m_Enable, sizeof(bool), 1, pFile);

}

void CMapBase::LoadFullPath(const char* FullPath)
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, FullPath, "rb");
	if (!pFile) return;
	Load(pFile); // TileMap의 재정의된 함수가 호출된다.
	fclose(pFile);
}
