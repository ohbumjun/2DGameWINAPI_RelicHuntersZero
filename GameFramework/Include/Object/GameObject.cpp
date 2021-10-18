
#include "GameObject.h"
#include "../GameManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/SceneCollision.h"
#include "../Resource/AnimationSequence.h"
#include "../Scene/Camera.h"
#include "DamageFont.h"
#include "../Map/TileMap.h"

CGameObject::CGameObject()	:
	m_Scene(nullptr),
	m_TimeScale(1.f),
	m_Animation(nullptr),
	m_CameraCull(false),
	m_Start(false), // false�� ó���Ѵ� ! --> object�� �����ɶ�����, false�� ó�� 
	m_DamageEnable(true),
	m_ObjType(EObject_Type::GameObject),
	m_PhysicsSimulate(false),
	m_FallTime(0.f),
	m_FallStartY(0.f),
	m_Jump(false),
	m_JumpVelocity(0.f),
	m_IsGround(false),
	m_FloorCheck(false),
	m_GravityAccel(10.f),
	m_LifeTime(INT_MAX),
	m_MoveSpeed(NORMAL_SPEED),
	m_SideWallCheck(false),
	m_ProtoTypeName{}
{
	// ������ �� -1�� �ش� 
	m_PrevPos.y = -1.f;
}

CGameObject::CGameObject(const CGameObject& obj)	:
	CRef(obj)
{
	// this
	m_FloorCheck = obj.m_FloorCheck;
	m_LifeTime = obj.m_LifeTime;
	m_GravityAccel = obj.m_GravityAccel;
	m_IsGround = obj.m_IsGround;
	m_FallStartY = obj.m_FallStartY;
	m_FallTime = obj.m_FallTime;
	m_PhysicsSimulate = obj.m_PhysicsSimulate;
	m_DamageEnable = obj.m_DamageEnable;
	m_ObjType = obj.m_ObjType;
	m_SideWallCheck = obj.m_SideWallCheck;
	m_ProtoTypeName = obj.m_ProtoTypeName;

	// ��� ������ false�� �����ص־� �Ѵ�.
	m_Start = false;

	m_Scene = obj.m_Scene;

	if (obj.m_Animation)
		m_Animation = obj.m_Animation->Clone();
	else
		m_Animation = nullptr;

	if(m_Animation)
		m_Animation->m_Owner = this;
	
	m_ColliderList.clear();
	{
		auto	iter = obj.m_ColliderList.begin();
		auto	iterEnd = obj.m_ColliderList.end();

		for (; iter != iterEnd; ++iter)
		{
			CCollider* Collider = (*iter)->Clone();

			Collider->SetOwner(this);
			Collider->SetScene(m_Scene);

			m_ColliderList.push_back(Collider);
		}
	}
	
	m_WidgetComponentList.clear();
	{
		auto	iter = obj.m_WidgetComponentList.begin();
		auto	iterEnd = obj.m_WidgetComponentList.end();
		for (; iter != iterEnd; ++iter)
		{
			CWidgetComponent* Widget = (*iter)->Clone();
			// CWidgetComponent* Widget = (*iter);
			Widget->SetOwner(this);
			Widget->SetScene(m_Scene);
			m_WidgetComponentList.push_back(Widget);
		}
	}

	m_PrevPos      = obj.m_PrevPos;
	m_Pos          = obj.m_Pos;
	m_Size         = obj.m_Size;
	m_Pivot        = obj.m_Pivot;
	m_Velocity     = obj.m_Velocity;
	m_JumpVelocity = obj.m_JumpVelocity;
	m_Offset       = obj.m_Offset;
	m_MoveSpeed    = obj.m_MoveSpeed;
	m_TimeScale    = obj.m_TimeScale;
	m_Texture      = obj.m_Texture;
	m_ImageStart   = obj.m_ImageStart;
}

CGameObject::~CGameObject()
{
	SAFE_DELETE(m_Animation);

	auto	iter = m_ColliderList.begin();
	auto	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->ClearCollisionList();
	}
	m_ColliderList.clear();
	m_WidgetComponentList.clear();
}

void CGameObject::SetCollisionProfile(const std::string& Name)
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		(*iter)->SetCollisionProfile(Name);
	}
}

bool CGameObject::IsCollisionCheck()
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		// if collided with sth
		if (!((*iter)->IsCollisionListEmpty()))
		{
			return true;
		}
	}
	return false;
}

CCollider* CGameObject::FindCollider(const std::string& Name)
{
	auto	iter = m_ColliderList.begin();
	auto	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetName() == Name)
			return *iter;
	}

	return nullptr;
}

void CGameObject::CreateAnimation()
{
	if (!m_Animation)
	{
		m_Animation = new CAnimation;

		m_Animation->m_Owner = this;
		m_Animation->m_Scene = m_Scene;
		m_Animation->m_CurrentAnimation = nullptr;
	}
}

void CGameObject::AddAnimation(const std::string& SequenceName,
	bool Loop, float PlayTime, float PlayScale, bool Reverse)
{
	if (!m_Animation)
	{
		m_Animation = new CAnimation;

		m_Animation->m_Owner = this;
		m_Animation->m_Scene = m_Scene;
	}

	m_Animation->AddAnimation(SequenceName, Loop, PlayTime, PlayScale, Reverse);
}

void CGameObject::SetAnimationPlayTime(const std::string& Name, 
	float PlayTime)
{
	if (!m_Animation)
		return;

	m_Animation->SetPlayTime(Name, PlayTime);
}

void CGameObject::SetAnimationPlayScale(const std::string& Name, 
	float PlayScale)
{
	if (!m_Animation)
		return;

	m_Animation->SetPlayScale(Name, PlayScale);
}

void CGameObject::SetCurrentAnimation(const std::string& Name)
{
	if (!m_Animation)
		return;

	m_Animation->SetCurrentAnimation(Name);
}

void CGameObject::ChangeAnimation(const std::string& Name)
{
	if (!m_Animation)
		return;

	m_Animation->ChangeAnimation(Name);
}

bool CGameObject::CheckCurrentAnimation(const std::string& Name)
{
	return m_Animation->CheckCurrentAnimation(Name);
}

void CGameObject::SetAnimationReverse(const std::string& Name, bool Reverse)
{
	if (!m_Animation)
		return;

	m_Animation->SetReverse(Name, Reverse);
}

void CGameObject::SetAnimationLoop(const std::string& Name, bool Loop)
{
	if (!m_Animation)
		return;

	m_Animation->SetLoop(Name, Loop);
}

float CGameObject::GetBottom() const
{
	Vector2	Size = m_Size;

	if (m_Animation && m_Animation->m_CurrentAnimation)
	{
		AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;

		const AnimationFrameData& FrameData = AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		Size = FrameData.Size;
	}

	return m_Pos.y + (1.f - m_Pivot.y) * Size.y + m_Offset.y;
}

int CGameObject::GetArmor() const
{
	return 0;
}

int CGameObject::GetAttack() const
{
	return 0;
}

void CGameObject::SetScene(CScene* Scene)
{
	m_Scene = Scene;
	if (m_Animation)
		m_Animation->m_Scene = Scene;

	// Colliders
	{
		auto iter = m_ColliderList.begin();
		auto iterEnd = m_ColliderList.end();
		for (; iter != iterEnd; ++iter)
		{
			(*iter)->SetScene(Scene);
		}
	}

	// Widgets
	{
		auto iter = m_WidgetComponentList.begin();
		auto iterEnd = m_WidgetComponentList.end();
		for (; iter != iterEnd; ++iter)
		{
			(*iter)->SetScene(Scene);
		}
	}
}


void CGameObject::Move(const Vector2& Dir)
{
	SetDir(Dir);
	float DeltaTime = CGameManager::GetInst()->GetDeltaTime();
	// Vector2	CurrentMove = Dir * m_MoveSpeed * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale;
	Vector2	CurrentMove = Dir * m_MoveSpeed * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale;
	m_Velocity	+= CurrentMove;
	m_Pos += CurrentMove;
}

void CGameObject::Move(const Vector2& Dir, float Speed)
{
	SetDir(Dir);
	Vector2	CurrentMove = Dir * Speed * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale;
	m_Velocity += CurrentMove;
	m_Pos += CurrentMove;
}

void CGameObject::WallCollisionUpCheck(CTileMap* TileMap)
{
	float PrevTop = m_PrevPos.y - m_Pivot.y * m_Size.y;
	float CurTop = m_Pos.y - m_Pivot.y * m_Size.y;
	float PrevLeft = m_PrevPos.x - m_Pivot.x * m_Size.x;
	float CurLeft = m_Pos.x - m_Pivot.x * m_Size.x;
	float PrevRight = PrevLeft + m_Size.x;
	float CurRight = CurLeft + m_Size.x;

	// ���� ���� �� 
	float resultLeft = PrevLeft < CurLeft ? PrevLeft : CurLeft;
	// ���� ū ��   
	float resultRight = PrevRight > CurRight ? PrevRight : CurRight;
	// ���� ���� �� 
	float resultTop = PrevTop < CurTop ? PrevTop : CurTop;
	// ���� ū ��   
	float resultBottom = PrevTop > CurTop ? PrevTop : CurTop;

	// �Ʒ��� �������� �ִٴ� �ǹ� 
	// ���� ��ġ�� ���� ��ġ�� Ÿ�� �ε����� ���ؿ´�
	int LeftIndexX, TopIndexY, RightIndexX, BottomIndexY;

	// LeftIndexX = TileMap->GetOriginTileIndexX(resultLeft);
	LeftIndexX = TileMap->GetOriginTileIndexX(resultLeft);
	TopIndexY = TileMap->GetOriginTileIndexY(resultTop);
	RightIndexX = TileMap->GetOriginTileIndexX(resultRight);
	BottomIndexY = TileMap->GetOriginTileIndexY(resultBottom);

	if (LeftIndexX < 0)
		LeftIndexX = 0;
	else if (RightIndexX >= TileMap->GetTileCountX())
		RightIndexX = TileMap->GetTileCountX() - 1;
	if (TopIndexY < 0)
		TopIndexY = 0;
	else if (BottomIndexY >= TileMap->GetTileCountY())
		BottomIndexY = TileMap->GetTileCountY() - 1;

	// ������ �Ʒ��� �ݺ��Ѵ�
	// ������ Ÿ���� Y �ε����� �� �����Ƿ�
	// ���� �ε������� ū �ε��� ������ 
	// üũ�� �� �� �ֵ��� �Ѵ� (������ �Ʒ��� �������� )

	// ���ʿ��� ���������� , Ȥ�� ���������� ��������
	// �̵��ϴ� ���⿡ ���� , PrevIndexX, IndexX 
	// ũ�� ������ �����ȴ�.

	bool Check = false;
	// From Down to Up
	for (int i = BottomIndexY; i >= TopIndexY; i--)
	{
		for (int j = LeftIndexX; j <= RightIndexX; j++)
		{
			if (TileMap->GetTile(j, i)->GetPos().y > PrevTop)
				continue;
			ETileOption TileOption = TileMap->GetTile(j, i)->GetTileOption();
			if (TileOption == ETileOption::Wall)
			{
				float TilePosY = TileMap->GetTile(j, i)->GetPos().y;
				float TileSizeY = TileMap->GetTile(j, i)->GetSize().y;
				m_Pos.y = (TilePosY + TileSizeY) + m_Pivot.y * m_Size.y + 0.1f;
				break;
			}
		}
	}
}

void CGameObject::WallCollisionDownCheck(CTileMap* TileMap)
{
	float PrevBottom = m_PrevPos.y + (1.f - m_Pivot.y) * m_Size.y;
	float CurBottom = m_Pos.y + (1.f - m_Pivot.y) * m_Size.y;
	float PrevLeft = m_PrevPos.x - m_Pivot.x * m_Size.x;
	float CurLeft = m_Pos.x - m_Pivot.x * m_Size.x;
	float PrevRight = PrevLeft + m_Size.x;
	float CurRight = CurLeft + m_Size.x;

	// ���� ���� �� 
	float resultLeft = PrevLeft < CurLeft ? PrevLeft : CurLeft;
	// ���� ū �� 
	float resultRight = PrevRight > CurRight ? PrevRight : CurRight;
	// ���� ���� ��
	float resultTop = PrevBottom < CurBottom ? PrevBottom : CurBottom;
	// ���� ū �� 
	float resultBottom = PrevBottom > CurBottom ? PrevBottom : CurBottom;

	// �Ʒ��� �������� �ִٴ� �ǹ� 
	// ���� ��ġ�� ���� ��ġ�� Ÿ�� �ε����� ���ؿ´�
	int LeftIndexX, TopIndexY, RightIndexX, BottomIndexY;

	// LeftIndexX = TileMap->GetOriginTileIndexX(resultLeft);
	LeftIndexX = TileMap->GetOriginTileIndexX(CurLeft);
	TopIndexY = TileMap->GetOriginTileIndexY(resultTop);
	RightIndexX = TileMap->GetOriginTileIndexX(resultRight);
	BottomIndexY = TileMap->GetOriginTileIndexY(resultBottom);

	if (LeftIndexX < 0)
		LeftIndexX = 0;
	else if (RightIndexX >= TileMap->GetTileCountX())
		RightIndexX = TileMap->GetTileCountX() - 1;
	if (TopIndexY < 0)
		TopIndexY = 0;
	else if (BottomIndexY >= TileMap->GetTileCountY())
		BottomIndexY = TileMap->GetTileCountY() - 1;

	// ������ �Ʒ��� �ݺ��Ѵ�
	// ������ Ÿ���� Y �ε����� �� �����Ƿ�
	// ���� �ε������� ū �ε��� ������ 
	// üũ�� �� �� �ֵ��� �Ѵ� (������ �Ʒ��� �������� )

	// ���ʿ��� ���������� , Ȥ�� ���������� ��������
	// �̵��ϴ� ���⿡ ���� , PrevIndexX, IndexX 
	// ũ�� ������ �����ȴ�.

	// ���������� �Ʒ��� ���ʷ� �˻��Ѵ� 
	for (int i = TopIndexY; i <= BottomIndexY; i++)
	{
		for (int j = LeftIndexX; j <= RightIndexX; j++)
		{
			// ���� ��ġ�� Bottom�� ������ ���� ���̴�
			// �������� ���̱� �����̴�
			// ���� ���� ��ġ�� Bottom�� Top���� Ŭ ��� ���� ( �� ���� ���� �ؼ� �ö���� ��)
			if (TileMap->GetTile(j, i)->GetPos().y < PrevBottom)
				continue;
			ETileOption TileOption = TileMap->GetTile(j, i)->GetTileOption();
			if (TileOption == ETileOption::Wall)
			{
				m_Pos.y = TileMap->GetTile(j, i)->GetPos().y - (1.f - m_Pivot.y) * m_Size.y - 0.1f;
				break;
			}
		}
	}
}

void CGameObject::WallCollisionRightCheck(CTileMap* TileMap)
{
	float PrevTop = m_PrevPos.y - m_Pivot.y * m_Size.y;
	float CurTop = m_Pos.y - m_Pivot.y * m_Size.y;
	float PrevLeft = m_PrevPos.x - m_Pivot.x * m_Size.x;
	float CurLeft = m_Pos.x - m_Pivot.x * m_Size.x;
	float PrevRight = PrevLeft + m_Size.x;
	float CurRight = CurLeft + m_Size.x;
	float PrevBottom = m_PrevPos.y + (1.f - m_Pivot.y) * m_Size.y;
	float CurBottom = m_Pos.y + (1.f - m_Pivot.y) * m_Size.y;

	// ���� ���� �� 
	float resultLeft = PrevRight;
	// ���� ū ��   
	float resultRight = CurRight;
	// ���� ���� �� 
	float resultTop = PrevTop < CurTop ? PrevTop : CurTop;
	// ���� ū ��   
	float resultBottom = PrevBottom > CurBottom ? PrevBottom : CurBottom;

	// �Ʒ��� �������� �ִٴ� �ǹ� 
	// ���� ��ġ�� ���� ��ġ�� Ÿ�� �ε����� ���ؿ´�
	int LeftIndexX, TopIndexY, RightIndexX, BottomIndexY;

	// LeftIndexX = TileMap->GetOriginTileIndexX(resultLeft);
	LeftIndexX = TileMap->GetOriginTileIndexX(resultLeft);
	TopIndexY = TileMap->GetOriginTileIndexY(resultTop);
	RightIndexX = TileMap->GetOriginTileIndexX(resultRight);
	BottomIndexY = TileMap->GetOriginTileIndexY(resultBottom);

	if (LeftIndexX < 0)
		LeftIndexX = 0;
	else if (RightIndexX >= TileMap->GetTileCountX())
		RightIndexX = TileMap->GetTileCountX() - 1;
	if (TopIndexY < 0)
		TopIndexY = 0;
	else if (BottomIndexY >= TileMap->GetTileCountY())
		BottomIndexY = TileMap->GetTileCountY() - 1;

	bool Check = false;
	// From Down to Up
	for (int j = LeftIndexX; j <= RightIndexX; j++)
	{
		for (int i = TopIndexY; i <= BottomIndexY; i++)
		{
			if (TileMap->GetTile(j, i)->GetPos().x < PrevLeft)
				continue;
			ETileOption TileOption = TileMap->GetTile(j, i)->GetTileOption();
			if (TileOption == ETileOption::Wall)
			{
				float TilePosX  = TileMap->GetTile(j, i)->GetPos().x;
				float TileSizeX = TileMap->GetTile(j, i)->GetSize().x;
				m_Pos.x = TilePosX - (1 - m_Pivot.x) * m_Size.x - 0.1f;
				break;
			}
		}
	}
}

void CGameObject::WallCollisionLeftCheck(CTileMap* TileMap)
{
	float PrevTop = m_PrevPos.y - m_Pivot.y * m_Size.y;
	float CurTop = m_Pos.y - m_Pivot.y * m_Size.y;
	float PrevLeft = m_PrevPos.x - m_Pivot.x * m_Size.x;
	float CurLeft = m_Pos.x - m_Pivot.x * m_Size.x;
	float PrevRight = PrevLeft + m_Size.x;
	float CurRight = CurLeft + m_Size.x;
	float PrevBottom = m_PrevPos.y + (1.f - m_Pivot.y) * m_Size.y;
	float CurBottom = m_Pos.y + (1.f - m_Pivot.y) * m_Size.y;

	// ���� ���� �� 
	float resultLeft = CurLeft;
	// ���� ū ��   
	float resultRight = PrevLeft;
	// ���� ���� �� 
	float resultTop = PrevTop < CurTop ? PrevTop : CurTop;
	// ���� ū ��   
	float resultBottom = PrevBottom > CurBottom ? PrevBottom : CurBottom;

	// ���� ��ġ�� ���� ��ġ�� Ÿ�� �ε����� ���ؿ´�
	int LeftIndexX, TopIndexY, RightIndexX, BottomIndexY;

	// LeftIndexX = TileMap->GetOriginTileIndexX(resultLeft);
	LeftIndexX = TileMap->GetOriginTileIndexX(resultLeft);
	TopIndexY = TileMap->GetOriginTileIndexY(resultTop);
	RightIndexX = TileMap->GetOriginTileIndexX(resultRight);
	BottomIndexY = TileMap->GetOriginTileIndexY(resultBottom);

	if (LeftIndexX < 0)
		LeftIndexX = 0;
	else if (RightIndexX >= TileMap->GetTileCountX())
		RightIndexX = TileMap->GetTileCountX() - 1;
	if (TopIndexY < 0)
		TopIndexY = 0;
	else if (BottomIndexY >= TileMap->GetTileCountY())
		BottomIndexY = TileMap->GetTileCountY() - 1;

	bool Check = false;
	// From Down to Up
	for (int j = RightIndexX; j >= LeftIndexX; j--)
	{
		for (int i = TopIndexY; i <= BottomIndexY; i++)
		{
			if (TileMap->GetTile(j, i)->GetPos().x > PrevRight)
				continue;
			ETileOption TileOption = TileMap->GetTile(j, i)->GetTileOption();
			if (TileOption == ETileOption::Wall)
			{
				float TilePosX = TileMap->GetTile(j, i)->GetPos().x;
				float TileSizeX = TileMap->GetTile(j, i)->GetSize().x;
				m_Pos.x = (TilePosX + TileSizeX) + m_Pivot.x * m_Size.x + 0.1f;
				break;
			}
		}
	}
}

void CGameObject::SetTexture(const std::string& Name)
{
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CGameObject::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CGameObject::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CGameObject::SetTexture(const std::string& Name,
	const std::vector<std::wstring>& vecFileName, 
	const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

void CGameObject::SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
	if (m_Texture)
		m_Texture->SetColorKey(r, g, b, Index);
}

void CGameObject::Start()
{
	m_Start = true;
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Update(float DeltaTime)
{
	// this
	if (!m_Start)
	{
		Start();
	}

	if (m_LifeTime >= 0.f)
	{
		m_LifeTime -= DeltaTime;
		if (m_LifeTime <= 0.f)
		{
			Destroy();
			return;
		}
	}

	// �߷��� �����Ѵ�
	// ���� ������� �ʰ�, ���� �ùķ��̼� �������̶��
	if (m_PhysicsSimulate && !m_IsGround)
	{
		m_FallTime += DeltaTime * m_GravityAccel;
		float Velocity = 0.f;

		if (m_Jump)
		{
			Velocity = m_JumpVelocity * m_FallTime;
		}

		float SaveY = m_Pos.y;
		m_Pos.y = (float)(m_FallStartY - (Velocity - (0.5 * GRAVITY * m_FallTime * m_FallTime)));
	}

	if (m_Animation)
		m_Animation->Update(DeltaTime);

	{
		auto	iter = m_ColliderList.begin();
		auto	iterEnd = m_ColliderList.end();
		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				iter = m_ColliderList.erase(iter);
				iterEnd = m_ColliderList.end();
				continue;
			}
			else if ((*iter)->GetEnable())
			{
				(*iter)->Update(DeltaTime);
			}
			++iter;
		}
	}
	{
		auto	iter = m_WidgetComponentList.begin();
		auto	iterEnd = m_WidgetComponentList.end();
		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Update(DeltaTime);
		}
	}
}

void CGameObject::PostUpdate(float DeltaTime)
{
	if (m_Animation)
	{
		if (m_Animation->m_CurrentAnimation)
		{
			AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;
			const AnimationFrameData& FrameData = AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);
			m_Size = FrameData.Size;
		}
	}
	{
		auto	iter = m_ColliderList.begin();
		auto	iterEnd = m_ColliderList.end();
		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				iter = m_ColliderList.erase(iter);
				iterEnd = m_ColliderList.end();
				continue;
			}
			else if ((*iter)->GetEnable())
			{
				(*iter)->PostUpdate(DeltaTime);
			}
			++iter;
		}
	}
	{
		auto	iter = m_WidgetComponentList.begin();
		auto	iterEnd = m_WidgetComponentList.end();
		for (; iter != iterEnd; ++iter)
		{
			(*iter)->PostUpdate(DeltaTime);
		}
	}
}

void CGameObject::Collision(float DeltaTime)
{
	auto	iter = m_ColliderList.begin();
	auto	iterEnd = m_ColliderList.end();
	for (; iter != iterEnd; ++iter)
	{
		m_Scene->GetSceneCollision()->AddCollider(*iter);
	}
	{
		auto	iter = m_WidgetComponentList.begin();
		auto	iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Collision(DeltaTime);
		}
	}

}

void CGameObject::PrevRender()
{
	CCamera* Camera = m_Scene->GetCamera();
	Vector2 CameraPos = Camera->GetPos();
	m_RenderPos = m_Pos - CameraPos;

	Vector2	Size = m_Size;
	if (m_Animation && m_Animation->m_CurrentAnimation)
	{
		AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;
		const AnimationFrameData& FrameData = AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);
		Size = FrameData.Size;
	}
	Vector2	LT = m_RenderPos - m_Pivot * Size + m_Offset;

	m_CameraCull = false;

	Vector2 CameraResolution = Camera->GetResolution();

	if (LT.x + Size.x <= 0.f)
		m_CameraCull = true;

	else if (LT.x >= CameraResolution.x)
		m_CameraCull = true;

	else if (LT.y + Size.y <= 0.f)
		m_CameraCull = true;

	else if (LT.y >= CameraResolution.y)
		m_CameraCull = true;

}

void CGameObject::Render(HDC hDC)
{
	if (m_Animation && m_Animation->m_CurrentAnimation)
	{
		AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;
		const AnimationFrameData& FrameData = AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);
		Vector2	LT = m_RenderPos - m_Pivot * FrameData.Size + m_Offset;

		if (AnimInfo->Sequence->GetTextureType() == ETexture_Type::Atlas)
		{
			AnimInfo->Sequence->GetTexture()->Render(hDC, LT,
				FrameData.StartPos, FrameData.Size);
		}
		else
		{
			AnimInfo->Sequence->GetTexture()->Render(hDC, LT,
				FrameData.StartPos, FrameData.Size, AnimInfo->Frame);
		}
	}
	else
	{
		Vector2	LT = m_RenderPos - m_Pivot * m_Size + m_Offset;
		if (m_Texture)
		{
			// �̹����� �̿��ؼ� ����Ѵ�.
			m_Texture->Render(hDC, LT, m_ImageStart, m_Size);
		}
		else
		{
			// Rectangle(hDC, (int)LT.x, (int)LT.y,
				// (int)(LT.x + m_Size.x), (int)(LT.y + m_Size.y));
		}
	}


	auto	iter = m_ColliderList.begin();
	auto	iterEnd = m_ColliderList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
			continue;
		}
		else if ((*iter)->GetEnable())
		{
			(*iter)->Render(hDC);
		}
		++iter;
	}

	{
		auto	iter = m_WidgetComponentList.begin();
		auto	iterEnd = m_WidgetComponentList.end();
		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Render(hDC);
		}
	}

	m_PrevPos = m_Pos;
	m_Velocity = Vector2(0.f, 0.f);
}

CGameObject* CGameObject::Clone()
{
	return new CGameObject(*this);
}

float CGameObject::SetDamage(float Damage)
{
	if (!m_DamageEnable)
		return 0.f;

	return Damage;
}
 
/*
L-value, R-value

// L, R
std::string a = "abc"
// L, L
std::String b = a;

L-value : callable , ���߿�
�츮�� �θ� �� �ִ� ��

R-value : �ѹ����� �Ⱦ��� �� 

R-value, ��, �ѹ��� ���� ���� ����
���纸�ٴ�, �׳� �ѹ� Move �ع����� ��

�ݸ�, L-value��, Copy�� ����� �� 

------------------------------------
< std::vector :: push_back >
std::string a = "abc";

// l-value reference : copy
void push_back(const T& value);

ex) vec.push_back(a)

// r-value reference : move 
void push_back(T&& value

ex) vec.push_back("abc")

-----------------------------------
<L-value�� R-value�� �ٲٱ� : std::move>
R-value = std::move(L-value) 

#include<string>
void storeByValue(std::string s)
{
	// 1�� ���� : ���� a �� s �� ����
	// 2�� ���� : ���� s �� b �� ���� 
	std::string b = s;
}

void storeByLRef(std::string &s)
{
	// 1�� ���� : reference�� ������ ���̹Ƿ�
	// a�� ����Ű�� �������� �״�� �����Ѽ�
	// �ش� ������ string�� s�� ��������
	// b�� ����Ǵ�, 1���� ���� 
	std::string b = s;
}

void storeByRRef(std::string &&s)
{
	// Ʋ�� �ڵ� : �� ? 1���� ���簡 �Ͼ�� ����
	// std::string b = s;
	// ���� ������ �Ҵ����� �ʾҴ� .
	// ��, ���ڷ� ���� R-value string
	// ex) "abc"
	// �� ����Ű�� ������ ���� �� ������ 
	// Stack�� �ֱ�� ������, �ӽ÷θ� ����.
	// ���� ���� �ش� string �� ���� �� 

	// ��, s �� ��� ���ڴ� stack�� �ӽ� ������ ����
	// ������ abc�� ��� ������ �ִ´� 
	// ����, local ���� b�� stack�� �����
	// ������ �ش� abc��� string�� ���簡 �ȴ� 
	
	// �׷��ٸ� �Ʒ��� �ڵ�� �� Ʋ�� ���ϱ� ?
	// s �� ������ ��Ī�� �� �ִ�
	// ex) cout << s << endl;
	// �� ���, s�� l-value�� �� ���̴�
	// ��, = s ��� �ϴ� ���� s�� l-value�� �� ���̰�
	// 1���� ���簡 �Ͼ�� �ȴ� 


	// ���� �ڵ� : �� �ѹ��� ���絵 �Ͼ�� �ʴ´� 
	// s�� �켱, �ӽ� ������ stack�� �Ҵ�Ǿ� �ִ�
	// �׷���, s�� l-value�� ��������� �ʰ�
	// s�� ��� �ִ� string���� b�� �־��ֱ� ����
	// move�� ����ϸ�, s�� ��� string�� �״��
	// �Ű��ֱ⸸ �� ��, copy�� �Ͼ�� �ʴ´� 

	std::string b = std::move(s);
}

int main()
{
	std::string a = "abc";
	storeByValue(a);
	storeByLRef(a);
	// storeByRRef(a); wrong ! Change to R-value Form
	storeByRRef(std::move(a));
}

*/

