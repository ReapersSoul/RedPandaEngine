#pragma once
#include <glm/glm.hpp>
#include <Physics/PhysicsObject/PhysicsObject.hpp>

class CollisionSphere : public PhysicsObject
{
	float radius;

public:
	CollisionSphere(glm::vec3, float);
	void CheckCollision(PhysicsObject *);
	// getters
	float GetRad();
};