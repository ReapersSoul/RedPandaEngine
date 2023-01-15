#pragma once
#include <string>
#include <functional>
#include <stdarg.h>
#include <tuple>
#include <map>
#include "FunctionTools.h"

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

	struct Table;
	typedef std::tuple<bool,double,int,std::string,void *,Table> Var;

	struct Table {
	public:
		std::map<std::string, Var> data;
	};
	virtual bool Init() { return false; };
	virtual bool Deconstruct() { return false; };
	virtual bool LoadScript(std::string path) { return false; };
	virtual bool LoadString(std::string str) { return false; };
	virtual bool RunScript(std::string path) { return false; };
	virtual bool RunString(std::string str) { return false; };
	
	//add methods for all types
	bool RegisterFunction(std::string Name, std::function<void(std::vector<Var>)> f) { return false; };
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

	template<typename T>
	static T VarAs(Var) { return Var(); }

	template<>
	static bool VarAs(Var v) {
		return std::get<0>(v);
	}

	template<>
	static double VarAs(Var v) {
		return std::get<1>(v);
	}

	template<>
	static int VarAs(Var v) {
		return std::get<2>(v);
	}

	template<>
	static std::string VarAs(Var v) {
		return std::get<3>(v);
	}

	template<>
	static void* VarAs(Var v) {
		return std::get<4>(v);
	}
	template<>
	static Table VarAs(Var v) {
		return std::get<5>(v);
	}
};