#pragma once
#include "Language.h"
#include <lua.hpp>
#include <tuple>

class Lua :public Language {
	lua_State* L;
public:
	Lua() {
		Name = "Lua";
		color = glm::vec3(221, 219, 59);
	}

	
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
	
	bool RegisterFunction(std::string Name, void* f) override{
		lua_pushlightuserdata(L, f);
		lua_pushcclosure(L, [](lua_State* L) {
			FT_StartCall();
			void* v = (void*)lua_topointer(L, lua_upvalueindex(1));
			int top = lua_gettop(L);
			std::string x;
			for (int i = top-1; i >= 0; i--)
			{
				if (lua_isnumber(L, i + 1)) {
					int x = lua_tonumber(L, i + 1);
					FT_PushIntPointer((void*)x);
				}
				else if (lua_isstring(L, i + 1)) {
					FT_PushIntPointer((void*)lua_tostring(L, i + 1));
				}
			}
			//pull function pointer off stack and call
			FT_CallFunction(v);

			return 1;
			}, 1);
		lua_setglobal(L, Name.c_str());
		lua_pop(L, lua_gettop(L));
		return false;
	};

	bool SetVar(std::string Name, bool value) {
		lua_pushboolean(L, value);
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
		//push the table to lua
		/*
			0 : bool
			1 : double
			2 : int
			3 : :std::string
			4 : void *
			5 : Table
			EX:
			std::get<2>(v) = (int)28;
		*/
		lua_newtable(L);
		for (std::map<std::string, std::pair<int, Language::Var>>::iterator it = value.data.begin(); it != value.data.end(); ++it)
		{
			lua_pushstring(L, it->first.c_str());
			switch (it->second.first)
			{
			case 0:
				lua_pushboolean(L, std::get<Language::e_bool>(it->second.second));
				break;
			case 1:
				lua_pushnumber(L, std::get<Language::e_double>(it->second.second));
				break;
			case 2:
				lua_pushinteger(L, std::get<Language::e_int>(it->second.second));
				break;
			case 3:
				lua_pushstring(L, std::get<Language::e_string>(it->second.second).c_str());
				break;
			case 4:
				lua_pushlightuserdata(L, std::get<Language::e_voidP>(it->second.second));
				break;
			case 5:
				
			default:
				break;
			}
			lua_settable(L, -3);
		}
		lua_setglobal(L, Name.c_str());
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
	Table GetVarAsTable(std::string Name) { 
		Table t;
		lua_getglobal(L, Name.c_str());
		lua_pushnil(L);
		while (lua_next(L, -2) != 0) {
			// key is at index -2 and value at index -1 
			std::string key = lua_tostring(L, -2);
			if (lua_isnumber(L, -1)) {
				std::get<int>(t.data[key].second) = lua_tointeger(L, -1);
				t.data[key].first = 0;
			}
			else if (lua_isstring(L, -1)) {
				std::get<std::string>(t.data[key].second) = lua_tostring(L, -1);
				t.data[key].first = 2;
			}
			else if (lua_isboolean(L, -1)) {
				std::get<bool>(t.data[key].second) = lua_toboolean(L, -1);
				t.data[key].first = 3;
			}
			// removes 'value'; keeps 'key' for next iteration
			lua_pop(L, 1);
		}
		// removes 'key'; after this, the stack is empty
		lua_pop(L, 1);
		return t;
	};
	
	double GetVarAsDouble(std::string Name) { return false; };
	char* GetVarAsCharP(std::string Name) { return (char*)""; };
	const char* GetVarAsConstCharP(std::string Name) { return ""; };
	


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