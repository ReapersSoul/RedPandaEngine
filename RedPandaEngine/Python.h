#pragma once
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "Scripting_Language.h"


class Python :public Scripting_Language {

public:

	bool Init() {
		Py_Initialize();
		return true;
	}

	bool Deconstruct() {
		Py_Finalize();
		return true;
	}

	bool LoadScript(std::string path) {

		return true;
	};
	bool LoadString(std::string str) {

		return true;
	};

	bool RunScript(std::string path) {
		PyObject* obj = Py_BuildValue("s", path.c_str());
		FILE* file = _Py_fopen_obj(obj, "r+");
		if (file != NULL) {
			PyRun_SimpleFile(file, path.c_str());
		}
		return true;
	};

	bool RunString(std::string str) {
		PyRun_SimpleString(str.c_str());
		return true;
	};

	//TODO::
	template<typename T>
	bool RegisterFunction(std::string Name, std::function<T(std::vector<Var>*)>* f);

	template<>
	bool RegisterFunction(std::string Name, std::function<double(std::vector<Var>*)>* f)
	{

		return false;
	}

	template<>
	bool RegisterFunction(std::string Name, std::function<int(std::vector<Var>*)>* f)
	{

		return false;
	}

	template<>
	bool RegisterFunction(std::string Name, std::function<bool(std::vector<Var>*)>* f)
	{

		return false;
	}

	template<>
	bool RegisterFunction(std::string Name, std::function<std::string(std::vector<Var>*)>* f)
	{

		return false;
	}

	template<typename T>
	bool RegisterVar(std::string Name, T value);

	template<>
	bool RegisterVar(std::string Name, bool value) {

		return true;
	};

	template<>
	bool RegisterVar(std::string Name, double value) {

		return true;
	};

	template<>
	bool RegisterVar(std::string Name, int value) {

		return true;
	};

	template<>
	bool RegisterVar(std::string Name, float value) {

		return true;
	};

	template<>
	bool RegisterVar(std::string Name, char* value) {

		return true;
	};

	template<>
	bool RegisterVar(std::string Name, const char* value) {

		return true;
	};

	template<>
	bool RegisterVar(std::string Name, std::string value) {

		return true;
	};

	//TODO::
	template<>
	bool RegisterVar(std::string Name, Table value) {

		return true;
	};

	//TODO::
	template<typename T>
	bool RegisterLinkedVar(std::string Name, T* value);


	template<typename T>
	bool SetVar(std::string Name, T val) {
		return RegisterVar<T>(Name, val);
	};

	template<typename T>
	T GetVar(std::string Name) {}

	template<>
	std::string GetVar(std::string Name)
	{

	}

	template<>
	int GetVar(std::string Name)
	{

	}

	template<>
	double GetVar(std::string Name)
	{

	}

	template<>
	bool GetVar(std::string Name)
	{

	}


	template<typename T, int numArgs>
	T CallFunction(std::string Name, ...) {};

	template<int numArgs>
	int CallFunction(std::string Name, ...)
	{
		
	}
};