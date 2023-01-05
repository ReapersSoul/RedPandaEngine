#pragma once
#include <string>
#include <functional>
#include <stdarg.h>
#include <tuple>

class Scripting_Languge {
public:
	struct Exception {
		Exception(std::string DESC, std::string ID="",int nid=0) {
			Id = ID;
			Desc = DESC;
			Num_Id = nid;
		}
		int Num_Id;
		std::string Id, Desc;
	};

	class Table;
	typedef std::tuple<bool,double,int,std::string,void *,Table> Var;
	class Table {
		std::map<std::string, Var> data;
	};
protected:
	virtual bool Init() { return false; };
	virtual bool Deconstruct() { return false; };
	virtual bool LoadScript(std::string path) { return false; };
	virtual bool LoadString(std::string str) { return false; };
	virtual bool RunScript(std::string path) { return false; };
	virtual bool RunString(std::string str) { return false; };
	template<typename T>
	bool RegisterFunction(std::string Name, std::function<T> f) { return false; };
	template<typename T>
	bool RegisterVar(std::string Name, T value) { return false; };
	template<typename T>
	bool RegisterLinkedVar(std::string Name, T* value) { return false; };
	template<typename T>
	bool SetVar(std::string Name, T val) { return false; };
	template<typename T>
	T GetVar(std::string Name) { return false; };
	template<typename T>
	T CallFunction(std::string Name, ...) { return false; };
};