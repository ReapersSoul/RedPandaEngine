#include "CollisionSphere.hpp"
#include <Physics/CollisionPrimatives/CollisionBox/CollisionBox.hpp>

CollisionSphere::CollisionSphere(glm::vec3 pos, float rad)
{
	position = pos;
	radius = rad;
	name = "SPHERE";
}

void CollisionSphere::CheckCollision(PhysicsObject *other)
{
	if (other->GetName() == "BOX")
	{
		CollisionBox *box = (CollisionBox *)other;
	}
	else if (other->GetName() == "SPHERE")
	{
		CollisionSphere *sphere = (CollisionSphere *)other;
		float distance = glm::distance(position, sphere->GetPos());
		if (distance <= radius + sphere->GetRad() && onCollision)
		{
			onCollision(this, other);
		}
	}
}

float CollisionSphere::GetRad()
{
	return radius;
}