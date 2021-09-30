
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
	m_Start(false), // false�� ó���Ѵ� ! --> object�� �����ɶ�����, false�� ó�� 
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

	// ��� ������ false�� �����ص־� �Ѵ�.
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

	// �߷��� �����Ѵ�
	// ���� ������� �ʰ�, ���� �ùķ��̼� �������̶��
	if (m_PhysicsSimulate && !m_IsGround)
	{
		// �������� ���� !
		// �������� �ð� ����
		m_FallTime += DeltaTime * m_GravityAccel;
		// m_FallTime += DeltaTime;

		// 0) �⺻ ����
		// https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=imchan123&logNo=10147703496
		// - �ӷ� : �̵��Ÿ� // �ɸ� �ð� ( ���� �ð��� �̵��� �Ÿ� ) --> ��Į�� , ���� x
		// - �ӵ� : ���� // �ɸ� �ð� --> ������� �������� �����Ÿ��� �� ���� ( �̵���ġ�� ��ȭ���� )
		// - ���ӵ� : ���� �ð� �� �ӵ��� ��ȭ�� 
		// - �ӵ� * �ð� == �Ÿ� ( �Ÿ��� �̺��ϸ� �ӵ� )
		// - ���ӵ� * �ð� == �ӵ� ( �ӵ��� �̺��ϸ� ���ӵ� )

		// 1) �������Ͽ ����
		// https://blog.naver.com/skh8464/220757273930
		// 1_1) ����
		// ��ӵ� � vs ��ӵ� �
		// ��ӵ� �  : ���� �ð�, ���� �Ÿ� �̵�
		// ex) 1m/s : 1�ʵ���, �����ϰ� 1m �̵�
		// ��ӵ� � : ���� �ð�, ���� �ӵ� ����
		//				  ���� �ð�, �̵� �Ÿ� ����
		// �ӷ� = �Ÿ� // �ð�
		// ��, ��ӵ� �������, �ӷ��� 1s �� 
		// ������ �����ϴµ�
		// �������Ͽ������ 1s�� 9.8m/s �� �����Ѵ�
		// ������ �ӷ��� �����Ѵٴ� ���̴�
		// ����
		// �Ÿ� = �ӷ� * �ð�
		// �Ÿ���, ������ �����ϰ� �Ǵ� ���̴�

		// 1_2) ��ӵ� � ����
		// A : ���ӵ�, S : �̵��Ÿ�, T : �ð� 
		// V : �ӷ�, V0 : �ʱ� �ӷ�
		// A = (V - V0) // T 
		// ���ӵ� : �ð��� �ӷ� ��ȭ�� 

		// A * T = V - V0
		// V = V0 + A*T

		// 1_3) ��ӵ� � �󿡼� S ����
		// ( ���� ��ũ ���� )
		// x�� �ð�, y�� �ӷ� �׷��������� �Ʒ� ����
		// S = T*(V + V0) // 2
		// S = T*(2*V0 + A*T) // 2
		// S = V0*T + A*T*T//2
		// �ӵ�,���ӵ�,�ð����� �� ������ ��ǥ(��ġ)�� ����ϴ� �����̴� 

		// t�� ���� y�� ( ���� ) �� ���ؾ� �Ѵ�
		// V: �ӵ�, A : ���ӵ� , G: �߷�
		// y = V*A - 0.5f * G * t * t
		// 0.5f * G * t * t : �̵��Ÿ� == ������ y ��
		// ���� ���� t�� ���� 2�����̴�
		// ���� 2���Ŀ� ���� ���� ���� ��
		// ���� ������ Ȱ���� �� �ִ�
		// (-b + ��Ʈ(b^2 - 4ac)) / 2a

		float Velocity = 0.f;

		// ���������̴� ?
		// ���� �ö󰡰� �ִٴ� �ǹ��̴�.
		if (m_Jump)
		{
			// �ð��� �帧�� ũ�� Ŭ����
			// m_FallTime�� Ŀ����
			// �׿� ���� Velocity�� Ŀ�� ���̴� 
			Velocity = m_JumpVelocity * m_FallTime;
		}

		float SaveY = m_Pos.y;
		// �ϴ� �߷°��ӵ� ������ �������� �ʴ´� 
		// jump�� �ߴٰ� �����غ���. 
		// �׷� ó������ Velocity�� (0.5 * GRAVITY * m_FallTime * m_FallTime) ���� �� Ŭ���̴�
		// ����, y.��ǥ�� -�� �ǰ�, �̴� �ٸ� ���� �ϸ�, ���� �ö󰣴ٴ� �ǹ�
		// ������, �ð��� ���� ���� m_FallTime�� ������ ���̸� , �ᱹ Velocity���� �� �۾�����
		// �̴� y��ǥ�� + �� �ȴٴ� ���̰� ( - (-) ) = +
		// �ᱹ �������� �ȴٴ� �ǹ��̴� 

		// ���� ���°� �ƴ϶��, velocity�� 0 �� �ǹǷ�, ���ۺ��� �� �������� �ȴ�.
		// ���� : ��ġ or �̵��Ÿ�(S) = ���� �ӵ� * �ð� + 0.5 * ���ӵ� * �ð� * �ð�  
		// Velocity : ���� �ӵ� * �ð� 
		// (0.5 * GRAVITY * m_FallTime * m_FallTime)  :  0.5 * ���ӵ� * �ð� * �ð�
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
	// Stun �ۿ�
	if (m_StunEnable)
	{
		// Stun Move ����
		StunMove();
		// Time ����
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

