
#include "Texture.h"
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
	// 최종 경로를 만들어준다.
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

	TextureInfo* Info = new TextureInfo;

	// 메모리DC를 만든다.
	Info->hDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());
	Info->hBmp = (HBITMAP)LoadImage(CGameManager::GetInst()->GetWindowInstance(),
		FullPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// 위에서 생성한 메모리 DC에 불러온 비트맵을 지정해준다.
	// SelectObject는 아래처럼 HGDIOBJ을 지정하면 원래 가지고 있던
	// HGDIOBJ를 반환해준다.
	// 그래서 이 DC를 지워줄때에는 반드시 원본 HGDIOBJ로 지정을 해두고
	// 지워줘야 한다.
	Info->hPrev = (HBITMAP)SelectObject(Info->hDC, Info->hBmp);

	// 비트맵 정보를 얻어와서 저장해둔다.
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
	// 최종 경로를 만들어준다.
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
		// 1번인자 : 출력할 DC이다.
		// 2번인자 : 출력할 윈도우 상에서의 x좌표
		// 3번인자 : 출력할 윈도우 상에서의 y좌표
		// 4번인자 : 출력할 윈도우 상에서의 x크기
		// 5번인자 : 출력할 윈도우 상에서의 y크기
		// 6번인자 : 출력할 이미지의 DC이다.
		// 7번인자 : 출력할 이미지 상에서의 시작 좌표 x
		// 8번인자 : 출력할 이미지 상에서의 시작 좌표 y
		// 9번인자 : 출력할 이미지 상에서의 가로크기
		// 10번인자 : 출력할 이미지 상에서의 세로크기
		// 11번인자 : ColorKey 지정
		TransparentBlt(hDC, (int)WindowPos.x, (int)WindowPos.y, (int)Size.x, (int)Size.y,
			m_vecTextureInfo[Index]->hDC, (int)ImgPos.x, (int)ImgPos.y,
			(int)Size.x, (int)Size.y, m_vecTextureInfo[Index]->ColorKey);
	}

	else
	{
		// 1번인자 : 출력할 DC이다.
		// 2번인자 : 출력할 윈도우 상에서의 x좌표
		// 3번인자 : 출력할 윈도우 상에서의 y좌표
		// 4번인자 : 출력할 윈도우 상에서의 x크기
		// 5번인자 : 출력할 윈도우 상에서의 y크기
		// 6번인자 : 출력할 이미지의 DC이다.
		// 7번인자 : 출력할 이미지 상에서의 시작 좌표 x
		// 8번인자 : 출력할 이미지 상에서의 시작 좌표 y
		BitBlt(hDC, (int)WindowPos.x, (int)WindowPos.y, (int)Size.x, (int)Size.y,
			m_vecTextureInfo[Index]->hDC, (int)ImgPos.x, (int)ImgPos.y, SRCCOPY);
	}
}
