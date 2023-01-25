#pragma once
#include <random>
#include <iostream>
#include <string>

template<typename T>
T random(T range_from, T range_to) {
	std::random_device                  rand_dev;
	std::mt19937                        generator(rand_dev());
	std::uniform_real_distribution<T>    distr(range_from, range_to);
	return distr(generator);
}
template<>
int random(int range_from, int range_to);

class Exception {
	std::string message;
	std::string error;
	int id;
public:
	Exception(std::string message, std::string error, int id = 0) : message(message), error(error), id(id) {};
	std::string GetMessage() { return message; };
	std::string GetError() { return error; };
	int GetID() { return id; };
	void Print() { std::cout << "Error: " << error << " Message: " << message << " ID: " << id << std::endl; };
};