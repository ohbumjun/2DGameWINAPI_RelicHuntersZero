
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
	m_Start(false), // false로 처리한다 ! --> object가 생성될때마다, false로 처리 
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
	// 시작할 때 -1로 준다 
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

	// 얘는 무조건 false로 세팅해둬야 한다.
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

	// 중력을 적용한다
	// 땅에 닿아있지 않고, 물리 시뮬레이션 동작중이라면
	if (m_PhysicsSimulate && !m_IsGround)
	{
		// 떨어지기 시작 !
		// 떨어지는 시간 누적
		m_FallTime += DeltaTime * m_GravityAccel;
		// m_FallTime += DeltaTime;

		// 0) 기본 개념
		// https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=imchan123&logNo=10147703496
		// - 속력 : 이동거리 // 걸린 시간 ( 단위 시간당 이동한 거리 ) --> 스칼라 , 방향 x
		// - 속도 : 변위 // 걸린 시간 --> 출발점과 도착점의 직선거리와 그 방향 ( 이동위치의 변화정도 )
		// - 가속도 : 단위 시간 당 속도의 변화량 
		// - 속도 * 시간 == 거리 ( 거리를 미분하면 속도 )
		// - 가속도 * 시간 == 속도 ( 속도를 미분하면 가속도 )

		// 1) 자유낙하운동 개념
		// https://blog.naver.com/skh8464/220757273930
		// 1_1) 개념
		// 등속도 운동 vs 등가속도 운동
		// 등속도 운동  : 일정 시간, 일정 거리 이동
		// ex) 1m/s : 1초동안, 일정하게 1m 이동
		// 등가속도 운동 : 일정 시간, 일정 속도 증가
		//				  일정 시간, 이동 거리 증가
		// 속력 = 거리 // 시간
		// 즉, 등가속도 운동에서는, 속력이 1s 당 
		// 일정량 증가하는데
		// 자유낙하운동에서는 1s당 9.8m/s 씩 증가한다
		// 꾸준히 속력이 증가한다는 것이다
		// 따라서
		// 거리 = 속력 * 시간
		// 거리도, 일정량 증가하게 되는 것이다

		// 1_2) 등가속도 운동 공식
		// A : 가속도, S : 이동거리, T : 시간 
		// V : 속력, V0 : 초기 속력
		// A = (V - V0) // T 
		// 가속도 : 시간당 속력 변화량 

		// A * T = V - V0
		// V = V0 + A*T

		// 1_3) 등가속도 운동 상에서 S 공식
		// ( 위의 링크 참고 )
		// x축 시간, y축 속력 그래프에서의 아래 면적
		// S = T*(V + V0) // 2
		// S = T*(2*V0 + A*T) // 2
		// S = V0*T + A*T*T//2
		// 속도,가속도,시간으로 한 시점의 좌표(위치)를 계산하는 공식이다 

		// t초 후의 y값 ( 높이 ) 를 구해야 한다
		// V: 속도, A : 가속도 , G: 중력
		// y = V*A - 0.5f * G * t * t
		// 0.5f * G * t * t : 이동거리 == 떨어진 y 양
		// 위의 식은 t에 대한 2차식이다
		// 따라서 2차식에 관한 근을 구할 때
		// 근의 공식을 활용할 수 있다
		// (-b + 루트(b^2 - 4ac)) / 2a

		float Velocity = 0.f;

		// 점프상태이다 ?
		// 위로 올라가고 있다는 의미이다.
		if (m_Jump)
		{
			// 시간의 흐름이 크면 클수록
			// m_FallTime도 커지고
			// 그에 따라 Velocity도 커질 것이다 
			Velocity = m_JumpVelocity * m_FallTime;
		}

		float SaveY = m_Pos.y;
		// 일단 중력가속도 까지는 적용하지 않는다 
		// jump를 했다고 가정해보자. 
		// 그럼 처음에는 Velocity가 (0.5 * GRAVITY * m_FallTime * m_FallTime) 보다 더 클것이다
		// 따라서, y.좌표는 -가 되고, 이는 다른 말로 하면, 위로 올라간다는 의미
		// 하지만, 시간이 지날 수록 m_FallTime은 증가할 것이며 , 결국 Velocity값이 더 작아지고
		// 이는 y좌표가 + 가 된다는 것이고 ( - (-) ) = +
		// 결국 내려가게 된다는 의미이다 

		// 점프 상태가 아니라면, velocity가 0 이 되므로, 시작부터 쭉 떨어지게 된다.
		// 정리 : 위치 or 이동거리(S) = 최조 속도 * 시간 + 0.5 * 가속도 * 시간 * 시간  
		// Velocity : 최초 속도 * 시간 
		// (0.5 * GRAVITY * m_FallTime * m_FallTime)  :  0.5 * 가속도 * 시간 * 시간
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

	// 수업용 : Side Collision 적용하기
	/*
	float DirX = m_Pos.x - m_PrevPos.x;
	// 이동을 하고 있다라는 의미 ( 왼쪽이던, 오른쪽이던 )
	// SideWallCollision을 고려하는 Object일 경우 
	if (DirX != 0.f && m_SideWallCheck)
	{
		int LTIndexX, LTIndexY, RBIndexX, RBIndexY;
		CTileMap* TileMap = m_Scene->GetTileMap();

		// 좌상단, 우하단 위치 구하기 
		// 충돌해서 겹치는 부분의 
		Vector2 PrevLT = m_PrevPos - m_Pivot * m_Size;
		Vector2 PrevRB = PrevLT + m_Size;
		Vector2 LT = m_Pos - m_Pivot * m_Size;
		Vector2 RB = LT +  m_Size;


		// Side Collision 여부 
		bool	SideCollision = false;

		// 오른쪽으로 이동할때
		if (DirX > 0.f)
		{
			Vector2 resultLT, resultRB;
			resultLT = LT;
			resultRB = RB;

			resultLT.x = PrevRB.x;
			// 둘중 작은 거 ( top )
			resultLT.y = LT.y < PrevLT.y ? LT.y : PrevLT.y;
			// 둘중 큰거 ( bottom )
			resultRB.y = RB.y > PrevLT.y ? LT.y : PrevLT.y;
			
			// LT ,RB 영억이 바로 충돌 영역 
			// 이 부분에 있는 tile 들을 구해줄 것이다
			LTIndexX = TileMap->GetOriginTileIndexX(resultLT.x);
			LTIndexY = TileMap->GetOriginTileIndexY(resultLT.y);
			RBIndexX = TileMap->GetOriginTileIndexX(resultRB.x);
			RBIndexY = TileMap->GetOriginTileIndexY(resultRB.y);

			// 범위 제한 ( 혹시나 )
			LTIndexX = LTIndexX < 0 ? 0 : LTIndexX;
			LTIndexY = LTIndexY < 0 ? 0 : LTIndexY;

			RBIndexX = RBIndexX > TileMap->GetTileCountX() - 1 ? TileMap->GetTileCountX() - 1 : RBIndexX;
			RBIndexY = RBIndexY > TileMap->GetTileCountY() - 1 ? TileMap->GetTileCountY() - 1 : RBIndexY;


			for (int i = LTIndexY; i <= RBIndexY; ++i)
			{
				for (int j = LTIndexX; j <= RBIndexX; ++j)
				{
					// Tile의 일차원 배열상의 idx 
					int	Index = i * TileMap->GetTileCountX() + j;

					// 벽이 아니거나, Side Collision 가 false 일 경우, 
					// Side Collision을 할 필요 없으므로, continue
					if (TileMap->GetTile(Index)->GetTileOption() !=
						ETileOption::Wall ||
						!TileMap->GetTile(Index)->GetSideCollision())
						continue;

					// 해당 Tile의 위치, size 정보를 가져온다 
					// TilePos : Tile의 위쪽을 의미한다 
					Vector2	TilePos = TileMap->GetTile(Index)->GetPos();
					Vector2	TileSize = TileMap->GetTile(Index)->GetSize();

					// Side Collision을 고려 중
					// Player가 위에 거의 딱 붙어있는 경우 제외 시키기 
					// 즉, Tile의 윗면과 거의 차이가 없을 때 제외 
					if (TilePos.y - 0.001f <= RB.y && 
						RB.y <= TilePos.y + 0.001f)
						continue;

					// 가로 길이
					float Width = RB.x - TilePos.x;

					// 이 가로 길이가 너무 크다면 ? 
					// 충돌을 무시해준다 
					// DeltaTime을 이용하여 움직이는 중인데
					// 8이라는 것은 매우 큰 것이다
					if (Width > 8.f)
						continue;

					// 렉트충돌
					if (LT.x <= TilePos.x + TileSize.x &&
						LT.y <= TilePos.y + TileSize.y &&
						RB.x >= TilePos.x && 
						RB.y >= TilePos.y)
					{
						SideCollision = true;

						// TilePos는 Tile의 왼쪽 상단
						// RB : 오른쪽 아래 
						// 즉, 충돌한 정도만큼, 오른쪽으로 밀어내려는 것이다 
						// MoveX는 - 값이 나오게 될 것이다 
						float	MoveX = TilePos.x - RB.x - 0.001f;

						//Move(Vector2(MoveX, 0.f));
						m_Pos.x += MoveX;
						m_Velocity.x += MoveX;
					}
				}
				if (SideCollision)
					break;
			}
		}
		// 왼쪽으로 이동할때
		else
		{
			Vector2 resultLT, resultRB;
			resultLT = LT;
			resultRB = RB;

			// 둘중 작은 거 ( top )
			resultLT.y = LT.y < PrevLT.y ? LT.y : PrevLT.y;
			// 둘중 큰거 ( bottom )
			resultRB.y = RB.y > PrevLT.y ? LT.y : PrevLT.y;
			resultRB.x = PrevLT.x;

			// LT ,RB 영억이 바로 충돌 영역 
			// 이 부분에 있는 tile 들을 구해줄 것이다
			LTIndexX = TileMap->GetOriginTileIndexX(resultLT.x);
			LTIndexY = TileMap->GetOriginTileIndexY(resultLT.y);
			RBIndexX = TileMap->GetOriginTileIndexX(resultRB.x);
			RBIndexY = TileMap->GetOriginTileIndexY(resultRB.y);

			// 범위 제한 ( 혹시나 )
			LTIndexX = LTIndexX < 0 ? 0 : LTIndexX;
			LTIndexY = LTIndexY < 0 ? 0 : LTIndexY;

			RBIndexX = RBIndexX > TileMap->GetTileCountX() - 1 ? TileMap->GetTileCountX() - 1 : RBIndexX;
			RBIndexY = RBIndexY > TileMap->GetTileCountY() - 1 ? TileMap->GetTileCountY() - 1 : RBIndexY;

			// 가로 길이
			float Width = RB.x - LT.x;

			// 이 가로 길이가 너무 크다면 ? 
			// 충돌을 무시해준다 
			// DeltaTime을 이용하여 움직이는 중인데
			// 8이라는 것은 매우 큰 것이다
			
			for (int i = LTIndexY; i <= RBIndexY; ++i)
			{
				for (int j = RBIndexX; j >= LTIndexX; --j)
				{
					int	Index = i * TileMap->GetTileCountX() + j;

					if (TileMap->GetTile(Index)->GetTileOption() !=
						ETileOption::Wall ||
						!TileMap->GetTile(Index)->GetSideCollision())
						continue;

					// 해당 Tile의 위치, size 정보를 가져온다 
					// TilePos : Tile의 위쪽을 의미한다 
					Vector2	TilePos = TileMap->GetTile(Index)->GetPos();
					Vector2	TileSize = TileMap->GetTile(Index)->GetSize();

					// Side Collision을 고려 중
					// Player가 위에 거의 딱 붙어있는 경우 제외 시키기 
					// 즉, Tile의 윗면과 거의 차이가 없을 때 제외 
					if (TilePos.y - 0.001f <= RB.y &&
						RB.y <= TilePos.y + 0.001f)
						continue;

					// 가로 길이 ( 파고든 정도 ) 
					float Width = (TilePos.x+TileSize.x) - resultLT.x;

					// 이 가로 길이가 너무 크다면 ? 
					// 충돌을 무시해준다 
					// DeltaTime을 이용하여 움직이는 중인데
					// 8이라는 것은 매우 큰 것이다
					if (Width > 8.f)
						continue;

					// 렉트충돌
					if (LT.x <= TilePos.x + TileSize.x &&
						LT.y <= TilePos.y + TileSize.y &&
						RB.x >= TilePos.x && RB.y >= TilePos.y)
					{
						SideCollision = true;

						// 오른쪽으로 밀리게 만들어준다
						// MoveX는 + 값이 나오게 될 것이다 
						// LT : 왼쪽 위 
						float	MoveX = TilePos.x + TileSize.x - LT.x + 0.001f;

						//Move(Vector2(MoveX, 0.f));
						m_Pos.x += MoveX;
						m_Velocity.x += MoveX;
					}
				}
				if (SideCollision)
					break;
			}
		}
	}
	*/

	// 수업용 : 중력 적용하기 
	/*
	CTileMap* TileMap = m_Scene->GetTileMap();

	if (TileMap && 
		m_PhysicsSimulate && 
		m_Pos.y - m_PrevPos.y >= 0.f)
	{
		// y값이 같을 경우는, 땅위를 걷고 있는 것으로 판단한다
		// 그러므로, 이 경우에는 바닥을 체크하고
		// 위로 올라가는 경우에는, 
		// 체크를 안한다. 

		float PrevBottom = m_PrevPos.y + (1.f - m_Pivot.y) * m_Size.y;
		float CurBottom = m_Pos.y + (1.f - m_Pivot.y) * m_Size.y;
		float PrevLeft  = m_PrevPos.x - m_Pivot.x * m_Size.x;
		float CurLeft   = m_Pos.x - m_Pivot.x * m_Size.x;
		float PrevRight = PrevLeft + m_Size.x;
		float CurRight = CurLeft + m_Size.x;

		// 둘중 작은 것 
		float resultLeft = PrevLeft < CurLeft ? PrevLeft : CurLeft;
		// 둘중 큰 거 
		float resultRight = PrevRight > CurRight ? PrevRight : CurRight;
		// 둘중 작은 거
		float resultTop    = PrevBottom < CurBottom ? PrevBottom : CurBottom;
		// 둘중 큰 거 
		float resultBottom = PrevBottom > CurBottom ? PrevBottom : CurBottom;

		// 아래로 떨어지고 있다는 의미 
		// 이전 위치와 현재 위치의 타일 인덱스를 구해온다
		int LeftIndexX, TopIndexY, RightIndexX, BottomIndexY;

		LeftIndexX     = TileMap->GetOriginTileIndexX(resultLeft);
		TopIndexY      = TileMap->GetOriginTileIndexY(resultTop);
		RightIndexX    = TileMap->GetOriginTileIndexX(resultRight);
		BottomIndexY   = TileMap->GetOriginTileIndexY(resultBottom);

		if (LeftIndexX < 0)
			LeftIndexX = 0;
		else if (RightIndexX >= TileMap->GetTileCountX())
			RightIndexX = TileMap->GetTileCountX() - 1;
		if (TopIndexY < 0)
			TopIndexY = 0;
		else if (BottomIndexY >= TileMap->GetTileCountY())
			BottomIndexY = TileMap->GetTileCountY() - 1;

		// 위에서 아래로 반복한다
		// 위쪽의 타일이 Y 인덱스가 더 작으므로
		// 작은 인덱스에서 큰 인덱스 순서로 
		// 체크를 할 수 있도록 한다 (위에서 아래로 떨어진다 )

		// 왼쪽에서 오른쪽으로 , 혹은 오른쪽으로 왼쪽으로
		// 이동하는 방향에 따라서 , PrevIndexX, IndexX 
		// 크고 작음이 결정된다.

		bool Check = false;
		// 위에서부터 아래로 차례로 검사한다 
		for (int i = TopIndexY; i <= BottomIndexY; i++)
		{
			for (int j = LeftIndexX; j <= RightIndexX; j++)
			{
				// 이전 위치의 Bottom이 무조건 위일 것이다
				// 내려가는 것이기 때문이다
				// 만약 이전 위치의 Bottom이 Top보다 클 경우 무시
				if (TileMap->GetTile(j, i)->GetPos().y < PrevBottom)
					continue;

				ETileOption TileOption = TileMap->GetTile(j, i)->GetTileOption();
				if (TileOption == ETileOption::Wall)
				{
					Check = true;
					// m_Pos : 발의 위치 
					m_Pos.y    = TileMap->GetTile(j, i)->GetPos().y - (1.f - m_Pivot.y) * m_Size.y;
					m_IsGround = true;
					m_Jump     = false; // 바닥에 붙어있으면 jump는 false로 
					m_FallTime = 0.f;
					break;
				}
			}
			if (Check) { break; }
		}
		if (!Check)
		{
			if (m_IsGround)
			{
				// True 였다가, False로 바뀌는 이 순간에
				// m_FallTime, m_FallStartY를 초기화
				// 왜 ? 
				// 현재 이 괄호 영역안의 의미는
				// 떨어지고 있지 않고, 현재 바닥에 붙어있는 경우
				// 그러면, 떨어졌다가, 바닥에 닿고
				// 그 바닥에서 다시 떨어진다고 가정할 때 
				// 다시 처음부터 천천히, 중력을 처음으로 적용해야
				// 그런데, 만약 여기서 초기화 해주지 않으면
				// m_FallTime은 계속 누적되기 때문에
				// 바닥에 닿은 다음, 또 다시 떨어질 때
				// 이전에 누적된 m_FallTime 으로 인해
				// 커진 Velocity에 의해
				// 추락하는 속도가 엄청 커진다
				// 따라서 이 영역에서 
				// 초기화를 해주어야 한다 
				m_FallTime = 0.f;
				m_FallStartY = m_Pos.y;
			}
			m_IsGround = false;
		}
	}
	*/
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
			// 이미지를 이용해서 출력한다.
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

