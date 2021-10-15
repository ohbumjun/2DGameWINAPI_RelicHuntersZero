#include "TileMap.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"

CTileMap::CTileMap() :
    m_TileCountX(0),
    m_TileCountY(0),
    m_StartX(-1),
    m_StartY(-1),
    m_EndX(-1),
    m_EndY(-1)
{
}

CTileMap::~CTileMap()
{
    size_t Size = m_vecTile.size();
    for (size_t i = 0; i < Size; i++)
    {
        SAFE_DELETE(m_vecTile[i]);
    }
    m_vecTile.clear();
}

bool CTileMap::CreateTile(int CountX, int CountY, const Vector2& TileSize)
{
    // 가로, 세로 크기 
    m_TileCountX = CountX;
    m_TileCountY = CountY;
    m_TileSize = TileSize;

    // 전체 타일을 다 날려버린다 
    // 새롭게 Map을 만드는 과정이기 때문에
    // 기존에 세팅된 타일들을 정리해주는 것이다 
    {
        auto iter = m_vecTile.begin();
        auto iterEnd = m_vecTile.end();
        for (; iter != iterEnd; ++iter)
        {
            SAFE_DELETE((*iter));
        }
        m_vecTile.clear();
    }

    // 가로, 세로 개수만큼 반복돌릴 것이다
    // Y : 행
    // X : 열 
    // 타일 전체 크기 
    m_Size = m_TileSize * Vector2((float)m_TileCountX, (float)m_TileCountY);

    // TileMap 크기로, Camera 의 크기를 세팅한다
    // 이렇게 하면, TileMap이 만들어진
    // 영역 안에서 움직이게 될 것이다 
    CCamera* Camera = m_Scene->GetCamera();
    Camera->SetWorldResolution(m_Size);

    for (int i = 0; i < m_TileCountY; ++i)
    {
        for (int j = 0; j < m_TileCountX; ++j)
        {
            CTile* Tile = new CTile;
            Tile->m_Scene = m_Scene;
            Tile->Init();

            // 현재 Tile Map 위치
            Vector2 Pos = Vector2((float)j,
                (float)i) * m_TileSize;
            
            // j : 가로 idx ( 열 )
            // i : 세로 idx ( 행 )
            // i*m_TileCountX+j : 총 idx 
            Tile->SetTileInfo(Pos,m_TileSize,
                j, i, i * m_TileCountX + j , m_TileTexture);
            m_vecTile.push_back(Tile);
        }
    }

    for (int i = 0; i < m_TileCountY; ++i)
    {
        for (int j = 0; j < m_TileCountX; ++j)
        {
            // Texture만 전체 세팅 해준다 
            m_vecTile[i * m_TileCountX + j]->SetTileTexture(m_TileTexture);
        }
    }

    return true;
}

bool CTileMap::SetSelectedTileTexture(CTexture* Texture)
{
    m_TileTexture = Texture;
    return true;
}

bool CTileMap::SetTileTexture(CTexture* Texture)
{
    m_TileTexture = Texture;
    for (int i = 0; i < m_TileCountY; i++)
    {
        for (int j = 0; j < m_TileCountX; j++)
        {
            // Texture만 전체 세팅 해준다 
            m_vecTile[i * m_TileCountX + j]->SetTileTexture(m_TileTexture);
        }
    }
    return true;
}

bool CTileMap::SetTileTexture(CTexture* Texture, int IndexX, int IndexY)
{
    m_TileTexture = Texture;
    m_vecTile[IndexY * m_TileCountX + IndexX]->SetTileTexture(m_TileTexture);
    return true;
}

void CTileMap::SetTileTexture(const Vector2& Pos, CTexture* Texture)
{
    CTile* Tile = GetTile(Pos);
    if (!Tile) return;
    Tile->SetTileTexture(Texture);
}

void CTileMap::ChangeTileOption(const Vector2& Pos, ETileOption Option)
{
    CTile* Tile = GetTile(Pos);
    if (!Tile) return;
    Tile->SetTileOption(Option);
}

void CTileMap::ChangeMonsterOption(const Vector2& Pos, EMonsterOption Option)
{
    CTile* Tile = GetTile(Pos);
    if (!Tile) return;
    Tile->SetMonsterOption(Option);
}

void CTileMap::SetTileFrame(const Vector2& Pos,const Vector2& Start, const Vector2& End)
{
    CTile* Tile = GetTile(Pos);
    if (!Tile) return;
    Tile->SetTileTexture(m_TileTexture);
    Tile->SetStartFrame(Start);
    Tile->SetEndFrame(End);
}

void CTileMap::SetTileWorldResolution()
{
    CCamera* Camera = m_Scene->GetCamera();
    Camera->SetWorldResolution(m_Size);
}

CTile* CTileMap::GetTile(const Vector2& Pos)
{
    // 해당 위치에 있는 Tile 정보를 가져온다 
    int IndexX = (int)(Pos.x / m_TileSize.x);
    int IndexY = (int)(Pos.y / m_TileSize.y);
    if (IndexX < 0 || IndexX >= m_TileCountX) return nullptr;
    if (IndexY < 0 || IndexY >= m_TileCountY) return nullptr;
    return m_vecTile[IndexY * m_TileCountX + IndexX];
}

CTile* CTileMap::GetTile(int Index)
{
    return m_vecTile[Index];
}

CTile* CTileMap::GetTile(int IndexX, int IndexY)
{
    return m_vecTile[IndexY * m_TileCountX + IndexX];
}

int CTileMap::GetTileIndexX(float PosX)
{
    int IndexX = (int)(PosX / m_TileSize.x);
    if (IndexX < 0 ||
        IndexX >= m_TileCountX) return -1;

    return IndexX;
}

int CTileMap::GetTileIndexY(float PosY)
{
    int IndexY = (int)(PosY / m_TileCountY);
    if (IndexY < 0 ||
        IndexY >= m_TileCountY) return -1;

    return IndexY;
}

int CTileMap::GetTileIndex(const Vector2& Pos)
{
    int IndexX = GetTileIndexX(Pos.x);
    int IndexY = GetTileIndexY(Pos.y);
   
    if (IndexX == -1 || IndexY == -1) return -1;
    
    return IndexY * m_TileCountX + IndexX;
}

int CTileMap::GetOriginTileIndexX(float PosX)
{
    return (int)(PosX / m_TileSize.x);
}

int CTileMap::GetOriginTileIndexY(float PosY)
{
    return (int)(PosY/m_TileSize.y);
}

void CTileMap::ChangeTileSideCollision(const Vector2& Pos, bool SideCollision)
{

    CTile* Tile = GetTile(Pos);
    if (!Tile) return;
    Tile->SetSideCollision(SideCollision);
}

void CTileMap::Start()
{
    CMapBase::Start();
}

bool CTileMap::Init()
{
    if (!CMapBase::Init()) return false;
    return true;
}

void CTileMap::Update(float DeltaTime)
{
    CMapBase::Update(DeltaTime);

    if (!m_vecTile.empty())
    {
        // 전체 TileMap 들 중에서
        // 일부는 보이고, 일부는 안보일 것이다
        // 화면상에서 camera를 통해서, 보이는 것과 안보이는 것을 구분했듯이
        // TileMap 상에서도, 보이는 Tile과 안보이는 Tile을 구분할 것이다
        // 그런데 만약 타일 개수가 100* 100 = 10000 개라면
        // 이중 반복문 다 돌면서, 각 타일을 확인하는 것은
        // 시간이 매우 오래 걸린다
    
        // 간단한 방법은, 해당 x,y 위치를 tile size로 나누면
        // 해당 Tile의 idxX, idxY가 나오게 될 것이다
        CCamera* Camera    = m_Scene->GetCamera();
        Vector2 CameraPos  = Camera->GetPos();
        Vector2 Resolution = Camera->GetResolution();

        // m_StartX,m_StartY : 현재 카메라 화면 내에 보이는 첫번째 타일의 x,y Idx
        // m_EndX,m_EndY : 현재 카메라 화면 내에 보이는 마지막 타일의 x,y Idx
        m_StartX = (int)(CameraPos.x / m_TileSize.x);
        m_StartY = (int)(CameraPos.y / m_TileSize.y);
        m_EndX   = (int)((CameraPos.x + Resolution.x )/ m_TileSize.x);
        m_EndY   = (int)((CameraPos.y + Resolution.y )/ m_TileSize.y);

        // idx 범위 조정 
        m_StartX = m_StartX < 0 ? 0 : m_StartX;
        m_StartY = m_StartY < 0 ? 0 : m_StartY;
        m_EndX   = m_EndX >= m_TileCountX ? m_TileCountX - 1 : m_EndX;
        m_EndY   = m_EndY >= m_TileCountY ? m_TileCountY - 1 : m_EndY;

        // 매 Frame 마다 m_StartX,Y m_EndX,Y 는 Update 에서 한번씩만 구한다 
        for (int i = m_StartY; i <= m_EndY; ++i)
        {
            for (int j = m_StartX; j <= m_EndX; ++j)
            {
                m_vecTile[i * m_TileCountX + j]->Update(DeltaTime);
            }
        }
    }
}

void CTileMap::PostUpdate(float DeltaTime)
{
    CMapBase::PostUpdate(DeltaTime);
    if (!m_vecTile.empty())
    {
        // Update 에서 아직 세팅이 안됐더라면 
        if (m_StartY == -1 || m_StartX == -1 || m_EndX == -1 || m_EndY == -1)
            return;
        for (int i = m_StartY; i <= m_EndY; ++i)
        {
            for (int j = m_StartX; j <= m_EndX; ++j)
            {
                m_vecTile[i * m_TileCountX + j]->PostUpdate(DeltaTime);
            }
        }
    }
}

void CTileMap::PrevRender()
{
    CMapBase::PrevRender();
    if (!m_vecTile.empty())
    {
        // Update 에서 아직 세팅이 안됐더라면 
        if (m_StartY == -1 || m_StartX == -1 || m_EndX == -1 || m_EndY == -1)
            return;
        for (int i = m_StartY; i <= m_EndY; ++i)
        {
            for (int j = m_StartX; j <= m_EndX; ++j)
            {
                m_vecTile[i * m_TileCountX + j]->PrevRender();
            }
        }
    }
}

void CTileMap::Render(HDC hDC)
{
    CMapBase::Render(hDC);
    if (!m_vecTile.empty())
    {
        // 혹시나 아직 세팅이 안됐다면 
        if (m_StartY == -1 || m_StartX == -1 ||
            m_EndX == -1 || m_EndY == -1) return;
        for (int i = m_StartY; i <= m_EndY; ++i)
        {
            for (int j = m_StartX; j <= m_EndX; ++j)
            {
                m_vecTile[i * m_TileCountX + j]->Render(hDC);
            }
        }
    }
}

void CTileMap::Save(FILE* pFile)
{
    CMapBase::Save(pFile);

    fwrite(&m_TileCountX, sizeof(int), 1, pFile);
    fwrite(&m_TileCountY, sizeof(int), 1, pFile);
    fwrite(&m_TileSize, sizeof(Vector2), 1, pFile);
    fwrite(&m_StartX, sizeof(int), 1, pFile);
    fwrite(&m_StartY, sizeof(int), 1, pFile);
    fwrite(&m_EndX, sizeof(int), 1, pFile);
    fwrite(&m_EndY, sizeof(int), 1, pFile);

    if (m_TileTexture)
    {
        // Texture 존재 여부를 저장한다 
        bool Tex = true;
        fwrite(&Tex, sizeof(bool), 1, pFile);
        m_TileTexture->Save(pFile);
    }
    else
    {
        bool Tex = false;
        fwrite(&Tex, sizeof(bool), 1, pFile);
    }

    int TileCount = (int)m_vecTile.size();
    fwrite(&TileCount, sizeof(int), 1, pFile);

    for (int i = 0; i < TileCount; ++i)
    {
        m_vecTile[i]->Save(pFile);
    }
}

void CTileMap::Load(FILE* pFile)
{
    // Load시, 기존에 혹여나 Tile 관련 정보가 있었다면
    // 모두 해제해준다 

    CMapBase::Load(pFile);

    fread(&m_TileCountX, sizeof(int), 1, pFile);
    fread(&m_TileCountY, sizeof(int), 1, pFile);
    fread(&m_TileSize, sizeof(Vector2), 1, pFile);
    fread(&m_StartX, sizeof(int), 1, pFile);
    fread(&m_StartY, sizeof(int), 1, pFile);
    fread(&m_EndX, sizeof(int), 1, pFile);
    fread(&m_EndY, sizeof(int), 1, pFile);

    // Texture의 존재여부 파악하기 
    bool Tex = true;
    fread(&Tex, sizeof(bool), 1, pFile);

    if (Tex)
    {
        m_TileTexture = CTexture::LoadStatic(pFile,m_Scene);
    }

    int TileCount = 0;
    fread(&TileCount, sizeof(int), 1, pFile);

    // 기존에 저장되었던 Tile 정보를 다 지운다
    size_t  TileSize = m_vecTile.size();
    for (size_t i = 0; i < TileSize; i++)
    {
        SAFE_DELETE(m_vecTile[i]);
    }
    m_vecTile.clear();

    // 다 날려버린 이후, 새로 생성하기 
    for (int i = 0; i < TileCount; ++i)
    {
        // 새로운 Tile 만들어서 Load 해주기
        CTile* Tile = new CTile;
        Tile->m_Scene = m_Scene;
        m_vecTile.push_back(Tile); // v
        m_vecTile[i]->Load(pFile);
    }

    // Load 시 World Resolution 만들기
    m_Size = m_TileSize * Vector2((float)m_TileCountX, (float)m_TileCountY);
    CCamera* Camera = m_Scene->GetCamera();
    Camera->SetWorldResolution(m_Size);

}

void CTileMap::TileImageAllClear()
{
    size_t Size = m_vecTile.size();
    for (size_t i = 0; i < Size; i++)
    {
        m_vecTile[i]->SetTileTexture(nullptr);
    }
}
