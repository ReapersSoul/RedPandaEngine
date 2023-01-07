#pragma once
#include <string>
#include <functional>
#include <stdarg.h>
#include <tuple>
#include <map>

class Scripting_Language {
public:
	enum Type {
		e_bool, e_double, e_int, e_string, e_voidP, e_Table
	};
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
	virtual bool Init() { return false; };
	virtual bool Deconstruct() { return false; };
	virtual bool LoadScript(std::string path) { return false; };
	virtual bool LoadString(std::string str) { return false; };
	virtual bool RunScript(std::string path) { return false; };
	virtual bool RunString(std::string str) { return false; };
	template<typename T>
	bool RegisterFunction(std::string Name, std::function<T(std::vector<Var>*)>* f) { return false; };
	template<typename T>
	bool RegisterVar(std::string Name, T value) { return false; };
	template<typename T>
	bool RegisterLinkedVar(std::string Name, T* value) { return false; };
	template<typename T>
	bool SetVar(std::string Name, T val) { return false; };
	template<typename T>
	T GetVar(std::string Name) { return false; };
	template<typename T, int numArgs>
	T CallFunction(std::string Name, ...) { return false; };
};