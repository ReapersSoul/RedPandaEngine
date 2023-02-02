#pragma once
#include <glm/glm.hpp>
#include <functional>
#include <vector>
#include <thread>

namespace Physics {
	
	class CollisionObject
	{
	protected:
		glm::vec3 position;
		std::string name;
	public:
		std::function<void(CollisionObject*)> onCollision;
		virtual void CheckCollision(CollisionObject*);
		std::string GetName();
		glm::vec3 GetPos();
	};

	class CollisionBox : public CollisionObject
	{
		glm::vec3 size;
	public:
		CollisionBox(glm::vec3, glm::vec3);
		void CheckCollision(CollisionObject*);
		bool Inside(glm::vec3);	
		//getters
		glm::vec3 GetSize();
	};
	
	class CollisionSphere : public CollisionObject
	{
		float radius;
	public:
		CollisionSphere(glm::vec3, float);
		void CheckCollision(CollisionObject*);
		//getters
		float GetRad();
	};

	class LinkedCollisionPointVec3 :public CollisionObject {
		glm::vec3* point;
	public:
		LinkedCollisionPointVec3(glm::vec3*, std::function<void(CollisionObject*)> oncol);
		void CheckCollision(CollisionObject*);
	};

	class LinkedCollisionPointVec4 :public CollisionObject {
		glm::vec4* point;
	public:
		LinkedCollisionPointVec4(glm::vec4*, std::function<void(CollisionObject*)> oncol);
		void CheckCollision(CollisionObject*);
	};
	
	class World
	{
		std::vector<CollisionObject*> objects;
		bool pause = false;
		bool run = false;
		std::thread worldThread;
	public:
		World();
		~World();
		void Update();
		void Add(CollisionObject*);
	};

}