#pragma once
#include <string>
#include <glm/glm.hpp>

//write a class to represent a embedded language that is the parent of CompiledLanguage and VirtualMachineLanguage
class Language {
public:
	//public methods
	Language() {
		//constructor
	}
	~Language() {
		//deconstructor
	}
	//getters
	std::string GetName() {
		return Name;
	}
	glm::vec3 GetPrimaryColor() {
		return PrimaryColor;
	}
	glm::vec3 GetSecondaryColor() {
		return SecondaryColor;
	}
	//setters
	void SetName(std::string name) {
		Name = name;
	}
	void SetPrimaryColor(glm::vec3 color) {
		PrimaryColor = color;
	}
	void SetSecondaryColor(glm::vec3 color) {
		SecondaryColor = color;
	}
	//methods
	virtual bool RegisterFunction(std::string Name, void* f) = 0;
	virtual bool CallFunction(std::string Name, void* ReturnValue, ...) = 0;
	
	
	
protected:
	//private methods
	
	//private variables
	glm::vec3 PrimaryColor;
	glm::vec3 SecondaryColor;
	std::string Name;
};