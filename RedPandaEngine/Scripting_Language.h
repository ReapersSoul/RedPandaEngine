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
	
	//add methods for all types
	//bool RegisterFunction(std::string Name, std::function<T(std::vector<Var>*)>* f) { return false; };
	//bool RegisterLinkedVar(std::string Name, T* value) { return false; };
	
	
	virtual bool SetVar(std::string Name, bool value) { return false; };
	virtual bool SetVar(std::string Name, double value) { return false; };
	virtual bool SetVar(std::string Name, int value) { return false; };
	virtual bool SetVar(std::string Name, float value) { return false; };
	virtual bool SetVar(std::string Name, char* value) { return false; };
	virtual bool SetVar(std::string Name, const char* value) { return false; };
	virtual bool SetVar(std::string Name, std::string value) { return false; };
	virtual bool SetVar(std::string Name, Table value) { return false; };
	virtual bool GetVarAsBool(std::string Name) { return false; };
	virtual int GetVarAsInt(std::string Name) { return false; };
	virtual float GetVarAsFloat(std::string Name) { return false; };
	virtual double GetVarAsDouble(std::string Name) { return false; };
	virtual char* GetVarAsCharP(std::string Name) { return (char*)""; };
	virtual const char* GetVarAsConstCharP(std::string Name) { return ""; };
	virtual std::string GetVarAsString(std::string Name) { return ""; };
	virtual Table GetVarAsTable(std::string Name) { return Table(); };
	template<typename T, int numArgs>
	T CallFunction(std::string Name, ...) { return false; };
};