#include "CollisionBox.hpp"
#include <Physics/CollisionPrimatives/CollisionSphere/CollisionSphere.hpp>

CollisionBox::CollisionBox(glm::vec3 pos, glm::vec3 size)
{
	position = pos;
	this->size = size;
	name = "BOX";
}

void CollisionBox::CheckCollision(PhysicsObject *other)
{
	// check for collision
	if (other->GetName() == "BOX")
	{
		CollisionBox *box = (CollisionBox *)other;
		if (position.x < box->GetPos().x + box->GetSize().x &&
			position.x + size.x > box->GetPos().x &&
			position.y < box->GetPos().y + box->GetSize().y &&
			position.y + size.y > box->GetPos().y &&
			position.z < box->GetPos().z + box->GetSize().z &&
			position.z + size.z > box->GetPos().z && onCollision)
		{
			onCollision(this, other);
		}
	}
	else if (other->GetName() == "SPHERE")
	{
		CollisionSphere *sphere = (CollisionSphere *)other;
		glm::vec3 closestPoint = position;
		if (sphere->GetPos().x < position.x)
		{
			closestPoint.x = position.x;
		}
		else if (sphere->GetPos().x > position.x + size.x)
		{
			closestPoint.x = position.x + size.x;
		}
		if (sphere->GetPos().y < position.y)
		{
			closestPoint.y = position.y;
		}
		else if (sphere->GetPos().y > position.y + size.y)
		{
			closestPoint.y = position.y + size.y;
		}
		if (sphere->GetPos().z < position.z)
		{
			closestPoint.z = position.z;
		}
		else if (sphere->GetPos().z > position.z + size.z)
		{
			closestPoint.z = position.z + size.z;
		}
		glm::vec3 distance = sphere->GetPos() - closestPoint;
		if (glm::length(distance) < sphere->GetRad() && onCollision)
		{
			onCollision(this, other);
		}
	}
}

bool CollisionBox::Inside(glm::vec3 point)
{
	if (point.x >= position.x && point.x <= position.x + size.x &&
		point.y >= position.y && point.y <= position.y + size.y &&
		point.z >= position.z && point.z <= position.z + size.z)
	{
		return true;
	}
	return false;
}

glm::vec3 CollisionBox::GetSize()
{
	return size;
}