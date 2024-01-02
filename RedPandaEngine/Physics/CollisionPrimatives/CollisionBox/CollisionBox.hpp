#pragma once
#include <glm/glm.hpp>
#include <Physics/PhysicsObject/PhysicsObject.hpp>

class CollisionBox : public PhysicsObject
{
	glm::vec3 size;

public:
	CollisionBox(glm::vec3, glm::vec3);
	void CheckCollision(PhysicsObject *);
	bool Inside(glm::vec3);
	// getters
	glm::vec3 GetSize();
};