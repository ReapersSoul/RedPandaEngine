#include "CollisionPoint.hpp"
#include <Physics/CollisionPrimatives/CollisionBox/CollisionBox.hpp>
#include <Physics/CollisionPrimatives/CollisionSphere/CollisionSphere.hpp>

CollisionPointVec3::CollisionPointVec3(glm::vec3 p)
{
	point = p;
}
void CollisionPointVec3::CheckCollision(PhysicsObject *other)
{
	if (other->GetName() == "BOX")
	{
		CollisionBox *box = (CollisionBox *)other;
		if (box->Inside(point) && onCollision)
		{
			onCollision(this, other);
		}
	}
	else if (other->GetName() == "SPHERE")
	{
		CollisionSphere *sphere = (CollisionSphere *)other;
		if (glm::distance(point, sphere->GetPos()) < sphere->GetRad() && onCollision)
		{
			onCollision(this, other);
		}
	}
}