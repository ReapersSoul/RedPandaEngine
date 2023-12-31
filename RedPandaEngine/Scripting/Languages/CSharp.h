#pragma once
#include "Language.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <glm/glm.hpp>

class CSharp :public Language {
	//variables for the mono language
    MonoDomain* domain;
	CSharp() {
		Name = "CSharp";
		color = glm::vec3(130, 236, 236);
	}


	bool Init() {
		//initialize mono
		domain = mono_jit_init("RedPandaEngine");
		return true;
	}

	bool Deconstruct() {
		mono_jit_cleanup(domain);
		return true;
	}
	
	
	bool LoadScript(std::string path) {
		//load a script into the mono language
		MonoAssembly* assembly = mono_domain_assembly_open(domain, path.c_str());
		mono_jit_exec(domain, assembly, 0, NULL);
		return true;
	};
	bool LoadString(std::string str) {
		//load a string into the mono language
		return true;
	};

	bool RunScript(std::string path) {
		LoadScript(path);
		return true;
	};

	bool RunString(std::string str) {
		LoadString(str);
		return true;
	};

	bool RegisterFunction(std::string Name, void* f) override {
		//register a funciton into the mono language
		mono_add_internal_call(Name.c_str(), f);
		return true;
	};

	bool SetVar(std::string Name, bool value) {
		//set a variable in the mono language
		
		return true;
	};

	bool SetVar(std::string Name, double value) {

		return true;
	};

	bool SetVar(std::string Name, int value) {

		return true;
	};

	bool SetVar(std::string Name, float value) {

		return true;
	};
	bool SetVar(std::string Name, char* value) {

		return true;
	};

	bool SetVar(std::string Name, const char* value) {

		return true;
	};

	bool SetVar(std::string Name, std::string value) {

		return true;
	};
	//TODO::
	bool SetVar(std::string Name, Table value) {
		return true;
	};

	//TODO::
	//bool RegisterLinkedVar(std::string Name, * value);

	bool GetVarAsBool(std::string Name)
	{
		return 0;
	}
	int GetVarAsInt(std::string Name)
	{
		return 0;
	}
	float GetVarAsFloat(std::string Name)
	{
		return 0;
	}
	std::string GetVarAsString(std::string Name)
	{
		return std::string();
	}
	Table GetVarAsTable(std::string Name) {
		return Table();
	};

	double GetVarAsDouble(std::string Name) { return false; };
	char* GetVarAsCharP(std::string Name) { return (char*)""; };
	const char* GetVarAsConstCharP(std::string Name) { return ""; };



	template<typename T, int numArgs>
	T CallFunction(std::string Name, ...) {};

	template<int numArgs>
	void CallFunction(std::string Name, ...)
	{
	}
};