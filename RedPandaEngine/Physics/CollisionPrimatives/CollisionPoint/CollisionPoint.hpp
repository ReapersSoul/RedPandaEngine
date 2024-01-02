#pragma once
#include <glm/glm.hpp>
#include <Physics/PhysicsObject/PhysicsObject.hpp>

class CollisionPointVec3 : public PhysicsObject
{
	glm::vec3 point;

public:
	CollisionPointVec3(glm::vec3);
	void CheckCollision(PhysicsObject *);
};