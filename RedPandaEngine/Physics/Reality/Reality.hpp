#pragma once
#include <vector>
#include <thread>
#include <Physics/PhysicsObject/PhysicsObject.hpp>

class Reality
{
	std::vector<PhysicsObject *> objects;
	bool pause = false;
	bool run = false;
	std::thread worldThread;

public:
	Reality();
	~Reality();
	void Update();
	void Add(PhysicsObject *);
};