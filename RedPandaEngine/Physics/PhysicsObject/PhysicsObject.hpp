#pragma once
#include <glm/glm.hpp>
#include <functional>
#include <string>

class PhysicsObject
{
protected:
	glm::vec3 position;
	std::string name;
	bool EnableCollision = true;
	bool EnableGravity = true;
	bool IsKinematic = false;
	bool Colliding = false;

public:
	std::function<void(PhysicsObject *main, PhysicsObject *other)> onCollision;
	std::function<void(PhysicsObject *main, PhysicsObject *other)> onEnterCollision;
	std::function<void(PhysicsObject *main, PhysicsObject *other)> onExitCollision;
	virtual void CheckCollision(PhysicsObject *);
	std::string GetName();
	glm::vec3 GetPos();
};