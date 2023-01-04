#pragma once
#include <string>
#include <functional>
#include <stdarg.h>

class Scripting_Languge {
	virtual bool Init();
	virtual bool Deconstruct();
	virtual bool LoadScript(std::string path);
	virtual bool LoadString(std::string str);
	virtual bool RunScript(std::string path);
	virtual bool RunString(std::string str);
	template<typename T>
	bool RegisterFunction(std::string Name, std::function<T> f);
	template<typename T>
	bool RegisterVar(std::string Name, T value);
	template<typename T>
	bool RegisterLinkedVar(std::string Name, T* value);
	template<typename T>
	bool SetVar(std::string Name, T val);
	template<typename T>
	T GetVar(std::string Name);
	template<typename T>
	T CallFunction(std::string Name, ...);
};