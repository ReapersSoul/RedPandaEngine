#include "Reality.hpp"

Reality::Reality()
{
	run = true;
	pause = false;
	worldThread = std::thread([this]()
							  {
			while (run) {
				while (pause) {
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}
				Update();
			} });
}
Reality::~Reality()
{
	run = false;
	worldThread.join();
}
void Reality::Update()
{
	for (int i = 0; i < objects.size(); i++)
	{
		for (int j = 0; j < objects.size(); j++)
		{
			objects[i]->CheckCollision(objects[j]);
		}
	}
}
void Reality::Add(PhysicsObject *obj)
{
	objects.push_back(obj);
}