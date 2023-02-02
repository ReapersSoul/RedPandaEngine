
#include "Physics.h"

namespace Physics {
	CollisionBox::CollisionBox(glm::vec3 pos, glm::vec3 size)
	{
		position = pos;
		this->size = size;
		name = "BOX";
	}

	void CollisionBox::CheckCollision(CollisionObject* other) {
		//check for collision
		if (other->GetName() == "BOX")
		{
			CollisionBox* box = (CollisionBox*)other;
			if (position.x < box->GetPos().x + box->GetSize().x &&
				position.x + size.x > box->GetPos().x &&
				position.y < box->GetPos().y + box->GetSize().y &&
				position.y + size.y > box->GetPos().y &&
				position.z < box->GetPos().z + box->GetSize().z &&
				position.z + size.z > box->GetPos().z && onCollision)
			{
				onCollision(other);
			}
		}
		else if (other->GetName() == "SPHERE") {
			CollisionSphere* sphere = (CollisionSphere*)other;
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
				onCollision(other);
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

	CollisionSphere::CollisionSphere(glm::vec3 pos, float rad)
	{
		position = pos;
		radius = rad;
		name = "SPHERE";
	}

	void CollisionSphere::CheckCollision(CollisionObject* other) {
		if (other->GetName() == "BOX")
		{
			CollisionBox* box = (CollisionBox*)other;
			

		}
		else if (other->GetName() == "SPHERE") {
			CollisionSphere* sphere = (CollisionSphere*)other;
			float distance = glm::distance(position, sphere->GetPos());
			if (distance <= radius + sphere->GetRad() && onCollision)
			{
				onCollision(other);
			}
		}
	}

	float CollisionSphere::GetRad()
	{
		return radius;
	}

	void CollisionObject::CheckCollision(CollisionObject*)
	{
	}
	std::string CollisionObject::GetName()
	{
		return name;
	}
	glm::vec3 CollisionObject::GetPos()
	{
		return glm::vec3();
	}
	World::World()
	{
		run = true;
		pause = false;
		worldThread = std::thread([this]() {
			while (run) {
				while (pause) {
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}
				Update();
			}
			});
	}
	World::~World()
	{
		run = false;
		worldThread.join();
	}
	void World::Update()
	{
		for (int i = 0; i < objects.size(); i++)
		{
			for (int j = 0; j < objects.size(); j++)
			{
				objects[i]->CheckCollision(objects[j]);
			}
		}
	}
	void World::Add(CollisionObject* obj)
	{
		objects.push_back(obj);
	}
	LinkedCollisionPointVec3::LinkedCollisionPointVec3(glm::vec3* p, std::function<void(CollisionObject*)> oncol)
	{
		point = p;
		onCollision = oncol;
	}
	void LinkedCollisionPointVec3::CheckCollision(CollisionObject* other)
	{
		if (other->GetName() == "BOX")
		{
			CollisionBox* box = (CollisionBox*)other;
			if (box->Inside(*point) && onCollision)
			{
				onCollision(other);
			}
		}
		else if (other->GetName() == "SPHERE") {
			CollisionSphere* sphere = (CollisionSphere*)other;
			if (glm::distance(*point, sphere->GetPos()) < sphere->GetRad() && onCollision)
			{
				onCollision(other);
			}
		}
	}
	LinkedCollisionPointVec4::LinkedCollisionPointVec4(glm::vec4* p, std::function<void(CollisionObject*)> oncol)
	{
		point = p;
		onCollision = oncol;
	}
	void LinkedCollisionPointVec4::CheckCollision(CollisionObject* other)
	{
		if (other->GetName() == "BOX")
		{
			CollisionBox* box = (CollisionBox*)other;
			if (box->Inside(glm::vec3(point->x, point->y, point->z))&&onCollision)
			{
				onCollision(other);
			}
		}
		else if (other->GetName() == "SPHERE") {
			CollisionSphere* sphere = (CollisionSphere*)other;
			if (glm::distance(glm::vec3(point->x, point->y, point->z), sphere->GetPos()) < sphere->GetRad() && onCollision)
			{
				onCollision(other);
			}
		}
	}
}