#pragma once

#include "../GameInfo.h"

class CCollision
{
public:
	static bool CollisionBoxToBox(class CColliderBox* Src, class CColliderBox* Dest);
	static bool CollisionBoxToBox(const RectInfo& Src, const RectInfo& Dest);

	static bool CollisionSphereToSphere(class CColliderSphere* Src, class CColliderSphere* Dest);
	static bool CollisionSphereToSphere(const SphereInfo& Src, const SphereInfo& Dest);

	static bool CollisionBoxToSphere(class CColliderBox* Src, class CColliderSphere* Dest);
	static bool CollisionBoxToSphere(const RectInfo& Src, const SphereInfo& Dest);
};

