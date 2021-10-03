
#include "Texture.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../PathManager.h"
#include "../GameManager.h"

CTexture::CTexture()
{
}

CTexture::~CTexture()
{
	size_t	Size = m_vecTextureInfo.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecTextureInfo[i]);
	}
}

bool CTexture::LoadTexture(const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	// ���� ��θ� ������ش�.
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	TCHAR	FullPath[MAX_PATH] = {};

	if (Info)
		lstrcpy(FullPath, Info->Path);
	lstrcat(FullPath, FileName);

	return LoadTextureFullPath(Name, FullPath);
}

bool CTexture::LoadTextureFullPath(const std::string& Name, 
	const TCHAR* FullPath)
{
	SetName(Name);

	// FullPath����, Texture File Name�� Folder ��α��� �����ϴ� �ڵ�
	// ��,FileName �κи��� ���´�
	TCHAR FileName[MAX_PATH] = {};

	char FullPathMultibyte[MAX_PATH] = {};

#ifdef UNICODE
	// �����ڵ���, ��Ƽ����Ʈ�� ��ȯ���ش�
	// �����ڵ� ���ڿ��� ��Ƽ����Ʈ ���ڿ��� ��ȯ�Ѵ�.
	int ConvertLength = WideCharToMultiByte(CP_ACP, 0,
		FullPath, -1, nullptr, 0, 0, 0);

	// FullPath�� FullPathMultibyte�� ��ȯ�ؼ� �־�� 
	WideCharToMultiByte(CP_ACP, 0, FullPath, -1,
		FullPathMultibyte, ConvertLength, 0, 0);
#else
	strcpy_s(FullPathMultibyte, FullPath);
#endif 

	// ��ҹ��� ���о���, ���� �빮�ڷ� ���� ���
	_strupr_s(FullPathMultibyte);

	int PathLength = (int)strlen(FullPathMultibyte);

	// ���������� ó������ ���鼭 Ž�� 
	for (int i = PathLength - 1; i >= 0; i--)
	{
		// ex)
		// D:/Project/Bin/Texture/Test.bmp
		// D:/Project/Bin/Texture/Player/Test.bmp
		if (FullPathMultibyte[i] == '\\') // Texture ������ ������ ��
		{
			// ���� �̸� : /Texture
			char FolderName[9] = {}; // �ڿ� null���ڱ��� �ؼ� �� 9��
			strcpy_s(FolderName,"ERUTXET\\");
			bool Find = true;
			for (int j = 1; j <= 8; j++)
			{
				if (FullPathMultibyte[i - j] != FolderName[j-1])
				{
					Find = false;
					break;
				}
			}
			if (Find)
			{
				// Texture // ���� ���� �����ؼ�
				// FileName�� ���� ���̴� 
				lstrcpy(FileName,&FullPath[i+1]);
			}
		}
	}

	TextureInfo* Info = new TextureInfo;
	lstrcpy(Info->FileName, FileName);

	// �޸�DC�� �����.
	Info->hDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());
	Info->hBmp = (HBITMAP)LoadImage(CGameManager::GetInst()->GetWindowInstance(),
		FullPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// ������ ������ �޸� DC�� �ҷ��� ��Ʈ���� �������ش�.
	// SelectObject�� �Ʒ�ó�� HGDIOBJ�� �����ϸ� ���� ������ �ִ�
	// HGDIOBJ�� ��ȯ���ش�.
	// �׷��� �� DC�� �����ٶ����� �ݵ�� ���� HGDIOBJ�� ������ �صΰ�
	// ������� �Ѵ�.
	Info->hPrev = (HBITMAP)SelectObject(Info->hDC, Info->hBmp);

	// ��Ʈ�� ������ ���ͼ� �����صд�.
	GetObject(Info->hBmp, sizeof(BITMAP), &Info->BmpInfo);

	m_vecTextureInfo.push_back(Info);

	if (m_vecTextureInfo.size() == 1)
		m_TextureType = ETexture_Type::Atlas;

	else
		m_TextureType = ETexture_Type::Frame;

	return true;
}

bool CTexture::LoadTexture(const std::string& Name, 
	const std::vector<std::wstring>& vecFileName, 
	const std::string& PathName)
{
	// ���� ��θ� ������ش�.
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	size_t	Size = vecFileName.size();

	for (size_t i = 0; i < Size; ++i)
	{
		TCHAR	FullPath[MAX_PATH] = {};

		if (Info)
			lstrcpy(FullPath, Info->Path);
		lstrcat(FullPath, vecFileName[i].c_str());

		if (!LoadTextureFullPath(Name, FullPath))
			return false;
	}


	if (m_vecTextureInfo.size() == 1)
		m_TextureType = ETexture_Type::Atlas;

	else
		m_TextureType = ETexture_Type::Frame;

	return true;
}

void CTexture::Render(HDC hDC, const Vector2& WindowPos, const Vector2& ImgPos, const Vector2& Size, int Index)
{
	if (m_vecTextureInfo[Index]->ColorKeyEnable)
	{
		// 1������ : ����� DC�̴�.
		// 2������ : ����� ������ �󿡼��� x��ǥ
		// 3������ : ����� ������ �󿡼��� y��ǥ
		// 4������ : ����� ������ �󿡼��� xũ��
		// 5������ : ����� ������ �󿡼��� yũ��
		// 6������ : ����� �̹����� DC�̴�.
		// 7������ : ����� �̹��� �󿡼��� ���� ��ǥ x
		// 8������ : ����� �̹��� �󿡼��� ���� ��ǥ y
		// 9������ : ����� �̹��� �󿡼��� ����ũ��
		// 10������ : ����� �̹��� �󿡼��� ����ũ��
		// 11������ : ColorKey ����
		TransparentBlt(hDC, (int)WindowPos.x, (int)WindowPos.y, (int)Size.x, (int)Size.y,
			m_vecTextureInfo[Index]->hDC, (int)ImgPos.x, (int)ImgPos.y,
			(int)Size.x, (int)Size.y, m_vecTextureInfo[Index]->ColorKey);
	}

	else
	{
		// 1������ : ����� DC�̴�.
		// 2������ : ����� ������ �󿡼��� x��ǥ
		// 3������ : ����� ������ �󿡼��� y��ǥ
		// 4������ : ����� ������ �󿡼��� xũ��
		// 5������ : ����� ������ �󿡼��� yũ��
		// 6������ : ����� �̹����� DC�̴�.
		// 7������ : ����� �̹��� �󿡼��� ���� ��ǥ x
		// 8������ : ����� �̹��� �󿡼��� ���� ��ǥ y
		BitBlt(hDC, (int)WindowPos.x, (int)WindowPos.y, (int)Size.x, (int)Size.y,
			m_vecTextureInfo[Index]->hDC, (int)ImgPos.x, (int)ImgPos.y, SRCCOPY);
	}
}

void CTexture::Save(FILE* pFile)
{
	int Length = (int)m_Name.length();

	fwrite(&Length, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), sizeof(char), Length, pFile);

	fwrite(&m_TextureType, sizeof(int), 1, pFile);

	// Texture Info�� � �ִ���
	int Count = (int)m_vecTextureInfo.size();
	fwrite(&Count, sizeof(int), 1, pFile);

	for (int i = 0; i < Count; i++)
	{
		fwrite(&m_vecTextureInfo[i]->ColorKeyEnable, sizeof(bool), 1, pFile);
		fwrite(&m_vecTextureInfo[i]->ColorKey, sizeof(unsigned int), 1, pFile);
	
		int NameCount = lstrlen(m_vecTextureInfo[i]->FileName);
		fwrite(&NameCount, sizeof(int), 1, pFile);
		fwrite(&m_vecTextureInfo[i]->FileName, sizeof(TCHAR), NameCount, pFile);
	}
}

void CTexture::Load(FILE* pFile)
{
	int Length = 0;
	char Name[256] = {};

	fread(&Length, sizeof(int), 1, pFile);
	fread(Name, sizeof(char), Length, pFile);

	m_Name = Name;

	fread(&m_TextureType, sizeof(int), 1, pFile);

	// Texture Info�� � �ִ���
	int Count = 0;
	fread(&Count, sizeof(int), 1, pFile);

	if (Count == 1)
	{
		bool ColorKeyEnable = false;
		unsigned int ColorKey = 0;

		fread(&ColorKeyEnable, sizeof(bool), 1, pFile);
		fread(&ColorKey, sizeof(unsigned int), 1, pFile);

		int NameCount = 0;
		fread(&NameCount, sizeof(int), 1, pFile);

		TCHAR FileName[MAX_PATH] = {};
		fread(&FileName, sizeof(TCHAR), NameCount, pFile);

		// Texture Load
		LoadTexture(m_Name, FileName);
		if (ColorKeyEnable)
		{
			SetColorKey(ColorKey);
		}
	}
	else // vector<wstring> ���·� �����;� �Ѵ� 
	{
		std::vector<std::wstring> vecFileName;
		std::vector<bool> vecColorkeyEnable;
		std::vector<unsigned int> vecColorkey;

		for (int i = 0; i < Count; i++)
		{
			bool ColorKeyEnable = false;
			unsigned int ColorKey = 0;

			fread(&ColorKeyEnable, sizeof(bool), 1, pFile);
			fread(&ColorKey, sizeof(unsigned int), 1, pFile);

			vecColorkeyEnable.push_back(ColorKeyEnable);
			vecColorkey.push_back(ColorKey);

			int NameCount = 0;
			fread(&NameCount, sizeof(int), 1, pFile);

			TCHAR FileName[MAX_PATH] = {};
			fread(&FileName, sizeof(TCHAR), NameCount, pFile);
		
			vecFileName.push_back(FileName);
		}

		// Texture Load
		LoadTexture(m_Name,vecFileName);

		// ��ü �ݺ� ���鼭 SetColorKey
		for (int i = 0; i < Count; i++)
		{
			if (!vecColorkeyEnable[i]) continue;
			SetColorKey(vecColorkey[i],i);
		}
	}
}

CTexture* CTexture::LoadStatic(FILE* pFile, CScene* Scene)
{
	CTexture* Result = nullptr;

	int Length = 0;
	char Name[256] = {};

	fread(&Length, sizeof(int), 1, pFile);
	fread(Name, sizeof(char), Length, pFile);

	ETexture_Type TextureType;
	fread(&TextureType, sizeof(ETexture_Type), 1, pFile);

	// Texture Info�� � �ִ���
	int Count = 0;
	fread(&Count, sizeof(int), 1, pFile);

	if (Count == 1)
	{
		bool ColorKeyEnable = false;
		unsigned int ColorKey = 0;

		fread(&ColorKeyEnable, sizeof(bool), 1, pFile);
		fread(&ColorKey, sizeof(unsigned int), 1, pFile);

		int NameCount = 0;
		fread(&NameCount, sizeof(int), 1, pFile);

		TCHAR FileName[MAX_PATH] = {};
		fread(&FileName, sizeof(TCHAR), NameCount, pFile);

		// Texture Load
		Scene->GetSceneResource()->LoadTexture(Name, FileName);
		if (ColorKeyEnable)
		{
			Scene->GetSceneResource()->SetTextureColorKey(Name,ColorKey);
		}
		Result = Scene->GetSceneResource()->FindTexture(Name);
	}
	else // vector<wstring> ���·� �����;� �Ѵ� 
	{
		bool ColorKeyEnable = false;
		unsigned int ColorKey = 0;

		std::vector<std::wstring> vecFileName;
		std::vector<bool> vecColorkeyEnable;
		std::vector<unsigned int> vecColorkey;

		for (int i = 0; i < Count; i++)
		{
			bool ColorKeyEnable = false;
			unsigned int ColorKey = 0;

			fread(&ColorKeyEnable, sizeof(bool), 1, pFile);
			fread(&ColorKey, sizeof(unsigned int), 1, pFile);

			vecColorkeyEnable.push_back(ColorKeyEnable);
			vecColorkey.push_back(ColorKey);

			int NameCount = 0;
			fread(&NameCount, sizeof(int), 1, pFile);

			TCHAR FileName[MAX_PATH] = {};
			fread(&FileName, sizeof(TCHAR), NameCount, pFile);

			vecFileName.push_back(FileName);
		}

		// Texture Load
		Scene->GetSceneResource()->LoadTexture(Name, vecFileName);

		// ��ü �ݺ� ���鼭 SetColorKey
		for (int i = 0; i < Count; i++)
		{
			if (!vecColorkeyEnable[i]) continue;
			Scene->GetSceneResource()->SetTextureColorKey(Name, vecColorkey[i]);
		}
		Result = Scene->GetSceneResource()->FindTexture(Name);
	}
	return Result;
}
