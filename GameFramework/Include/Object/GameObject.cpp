
#include "GameObject.h"
#include "../GameManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/SceneCollision.h"
#include "../Resource/AnimationSequence.h"
#include "../Scene/Camera.h"
#include "DamageFont.h"

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
	m_GravityAccel(10.f),
	m_LifeTime(INT_MAX),
	m_StunDir{},
	m_StunEnable(false),
	m_StunTime(0.f),
	m_MoveSpeed(NORMAL_SPEED)
{
}

CGameObject::CGameObject(const CGameObject& obj)	:
	CRef(obj)
{
	m_StunDir = obj.m_StunDir;
	m_StunEnable = obj.m_StunEnable;
	m_StunTime = obj.m_StunTime;
	m_LifeTime = obj.m_LifeTime;
	m_GravityAccel = obj.m_GravityAccel;
	m_IsGround = obj.m_IsGround;
	m_FallStartY = obj.m_FallStartY;
	m_FallTime = obj.m_FallTime;
	m_PhysicsSimulate = obj.m_PhysicsSimulate;
	m_DamageEnable = obj.m_DamageEnable;
	m_ObjType = obj.m_ObjType;

	// 얘는 무조건 false로 세팅해둬야 한다.
	m_Start = false;

	m_Scene = obj.m_Scene;

	if (obj.m_Animation)
		m_Animation = obj.m_Animation->Clone();

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
			Widget->SetOwner(this);
			Widget->SetScene(m_Scene);
			m_WidgetComponentList.push_back(Widget);
		}
	}

	m_PrevPos = obj.m_PrevPos;
	m_Pos = obj.m_Pos;
	m_Size = obj.m_Size;
	m_Pivot = obj.m_Pivot;
	m_Velocity = obj.m_Velocity;
	m_Offset = obj.m_Offset;
	m_MoveSpeed = obj.m_MoveSpeed;
	m_TimeScale = obj.m_TimeScale;
	m_Texture = obj.m_Texture;
	m_ImageStart = obj.m_ImageStart;
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
}

void CGameObject::Stun()
{
	m_StunTime = STUN_TIME;
	m_StunEnable = true;
}

void CGameObject::StunMove()
{
	if (!m_StunEnable) return;
	Vector2	CurrentMove = m_StunDir * STUN_SPEED * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale;
	m_Velocity += CurrentMove;
	m_Pos += CurrentMove;
}

void CGameObject::SetStunDir(Vector2 Dir)
{
	m_StunDir = Dir;
	m_StunDir.Normalize();
}

void CGameObject::StunEnd()
{
	m_StunEnable = false;
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

	if (m_Animation)
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
}

void CGameObject::Move(const Vector2& Dir)
{
	if (m_StunEnable) return;
	SetDir(Dir);
	Vector2	CurrentMove = Dir * m_MoveSpeed * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale;
	m_Velocity	+= CurrentMove;
	m_Pos += CurrentMove;
}

void CGameObject::Move(const Vector2& Dir, float Speed)
{
	if(m_StunEnable) return;
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
	if (m_StunEnable)
	{
		StunMove();
		m_StunTime -= DeltaTime;
		if (m_StunTime < 0.f)
			StunEnd();
		return;
	}
}

void CGameObject::PostUpdate(float DeltaTime)
{
	if (m_Animation)
	{
		AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;
		const AnimationFrameData& FrameData = AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);
		m_Size = FrameData.Size;
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
	// Stun 작용
	if (m_StunEnable)
	{
		// Stun Move 조절
		StunMove();
		// Time 조절
		if(m_StunTime >= 0.f) m_StunTime -= DeltaTime;
		if (m_StunTime < 0.f) StunEnd();
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
	m_RenderPos = m_Pos - Camera->GetPos();
	Vector2	Size = m_Size;

	if (m_Animation)
	{
		AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;
		const AnimationFrameData& FrameData = AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);
		Size = FrameData.Size;
	}

	Vector2	LT = m_RenderPos - m_Pivot * Size + m_Offset;

	m_CameraCull = false;

	if (LT.x + Size.x <= 0.f)
		m_CameraCull = true;

	else if (LT.x >= Camera->GetResolution().x)
		m_CameraCull = true;

	else if (LT.y + Size.y <= 0.f)
		m_CameraCull = true;

	else if (LT.y >= Camera->GetResolution().y)
		m_CameraCull = true;
}

void CGameObject::Render(HDC hDC)
{
	if (m_Animation)
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

