#pragma once
#include "Scripting_Language.h"
#include <lua.hpp>
#include <tuple>

class Lua :public Scripting_Language {
	lua_State* L;
public:

	bool Init() {
		L = luaL_newstate();
		luaL_openlibs(L);
		return true;
	}

	bool Deconstruct() {
		lua_close(L);
		return true;
	}


	void IfErrorThrowException(int error) {
		while (error && lua_gettop(L))
		{
			int stack = lua_gettop(L);
			int err = error;
			std::string message = lua_tostring(L, -1);
			lua_pop(L, 1);
			error = lua_pcall(L, 0, 0, 0);
			throw Exception(message, "Stack: " + std::to_string(stack) + " Error: " + std::to_string(err));
		}
	}
	bool LoadScript(std::string path) {
		IfErrorThrowException(luaL_loadfile(L, path.c_str()));
		return true;
	};
	bool LoadString(std::string str) {
		IfErrorThrowException(luaL_loadstring(L, str.c_str()));
		return true;
	};

	bool RunScript(std::string path) {
		LoadScript(path);
		IfErrorThrowException(lua_pcall(L, 0, 0, 0));
		return true;
	};

	bool RunString(std::string str) {
		LoadString(str);
		IfErrorThrowException(lua_pcall(L, 0, 0, 0));
		return true;
	};

	//TODO::
	template<typename T, typename FN>
	bool RegisterFunction(std::string Name,int NumArgs, FN f) {
		lua_pushlightuserdata(L, f);
		lua_pushcclosure(L, [](lua_State* L) {
			//get all variables passed to lua
			std::vector<Var> vars;
			int top = lua_gettop(L);
			for (int i = 0; i < top; i++)
			{
				Var v;
				if (lua_isboolean(L, i + 1)) {
					bool b = lua_toboolean(L, i + 1);
					PushArg(&b);
				}
				else if (lua_isnumber(L, i + 1)) {
					double d = lua_tonumber(L, i + 1);
					PushArg(&d);
				}
				vars.push_back(v);
			}
			//pull function pointer off stack and call
			lua_pushnumber(L, ((std::function<double(std::vector<Var>*)>*)lua_topointer(L, lua_upvalueindex(1)))->operator()(&vars));

			return 1;
			}, 1);
		lua_setglobal(L, Name.c_str());
		lua_pop(L, lua_gettop(L));
		return false;
	};

	bool SetVar(std::string Name, bool value) {
		lua_pushinteger(L, value);
		lua_setglobal(L, Name.c_str());
		lua_pop(L, lua_gettop(L));
		return true;
	};

	bool SetVar(std::string Name, double value) {
		lua_pushnumber(L, value);
		lua_setglobal(L, Name.c_str());
		lua_pop(L, lua_gettop(L));
		return true;
	};

	bool SetVar(std::string Name, int value) {
		lua_pushinteger(L, value);
		lua_setglobal(L, Name.c_str());
		lua_pop(L, lua_gettop(L));
		return true;
	};

	bool SetVar(std::string Name, float value) {
		lua_pushnumber(L, value);
		lua_setglobal(L, Name.c_str());
		lua_pop(L, lua_gettop(L));
		return true;
	};
	bool SetVar(std::string Name, char* value) {
		lua_pushstring(L, value);
		lua_setglobal(L, Name.c_str());
		lua_pop(L, lua_gettop(L));
		return true;
	};

	bool SetVar(std::string Name, const char* value) {
		lua_pushstring(L, value);
		lua_setglobal(L, Name.c_str());
		lua_pop(L, lua_gettop(L));
		return true;
	};

	bool SetVar(std::string Name, std::string value) {
		lua_pushstring(L, value.c_str());
		lua_setglobal(L, Name.c_str());
		lua_pop(L, lua_gettop(L));
		return true;
	};
	//TODO::
	bool SetVar(std::string Name, Table value) {
		for (auto itt = value.data.begin(); itt != value.data.end(); itt++) {
			std::string varname = itt->first;
			Var vardata = itt->second;
			
		}

		lua_pop(L, lua_gettop(L));
		return true;
	};

	//TODO::
	//bool RegisterLinkedVar(std::string Name, * value);

	bool GetVarAsBool(std::string Name)
	{
		lua_getglobal(L, Name.c_str());
		bool message = lua_toboolean(L, -1);
		lua_pop(L, 1);
		return message;
	}
	int GetVarAsInt(std::string Name)
	{
		lua_getglobal(L, Name.c_str());
		int message = lua_tointeger(L, -1);
		lua_pop(L, 1);
		return message;
	}
	float GetVarAsFloat(std::string Name)
	{
		lua_getglobal(L, Name.c_str());
		double message = lua_tonumber(L, -1);
		lua_pop(L, 1);
		return message;
	}
	std::string GetVarAsString(std::string Name)
	{
		lua_getglobal(L, Name.c_str());
		std::string message = lua_tostring(L, -1);
		lua_pop(L, 1);
		return message;
	}
	double GetVarAsDouble(std::string Name) { return false; };
	char* GetVarAsCharP(std::string Name) { return (char*)""; };
	const char* GetVarAsConstCharP(std::string Name) { return ""; };
	Table GetVarAsTable(std::string Name) { return Table(); };


	template<typename T, int numArgs>
	T CallFunction(std::string Name, ...) {};

	template<int numArgs>
	void CallFunction(std::string Name, ...)
	{
		lua_getglobal(L, Name.c_str());
		va_list lst;
		va_start(lst, Name);
		for (int i = 0; i < numArgs; i++) {
			int arg = va_arg(lst, int);
			lua_pushnumber(L, arg);
		}
		va_end(lst);

		int error = lua_pcall(L, numArgs, 1, 0);
		if (error == LUA_OK) {
			int result = lua_tointeger(L, -1);
			lua_pop(L, 1);
			lua_pop(L, lua_gettop(L));
			return;
		}
		else {
			while (error && lua_gettop(L))
			{
				int stack = lua_gettop(L);
				int err = error;
				std::string message = lua_tostring(L, -1);
				lua_pop(L, 1);
				error = lua_pcall(L, 0, 0, 0);
				throw Exception(message, "Stack: " + std::to_string(stack) + " Error: " + std::to_string(err));
			}
		}
	}

	template<int numArgs>
	int CallFunction(std::string Name, ...)
	{
		lua_getglobal(L, Name.c_str());
		va_list lst;
		va_start(lst, Name);
		for (int i = 0; i < numArgs; i++) {
			int arg = va_arg(lst, int);
			lua_pushnumber(L, arg);
		}
		va_end(lst);

		int error = lua_pcall(L, numArgs, 1, 0);
		if (error == LUA_OK) {
			int result = lua_tointeger(L, -1);
			lua_pop(L, 1);
			lua_pop(L, lua_gettop(L));
			return result;
		}
		else {
			while (error && lua_gettop(L))
			{
				int stack = lua_gettop(L);
				int err = error;
				std::string message = lua_tostring(L, -1);
				lua_pop(L, 1);
				error = lua_pcall(L, 0, 0, 0);
				throw Exception(message, "Stack: " + std::to_string(stack) + " Error: " + std::to_string(err));
			}
		}
	}
};