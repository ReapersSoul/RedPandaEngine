#pragma once
#include "Scripting_Language.h"
#include <lua.hpp>
#include <tuple>

class Lua :public Scripting_Languge {
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

    bool LoadScript(std::string path) {
        if (luaL_loadfile(L, path.c_str()))
        {
            throw Exception("FUCK!!!","Unable to find lua file");
        }
        return true;
    };
    bool LoadString(std::string str) {
        if (luaL_loadstring(L, str.c_str()))
        {
            throw Exception("FUCK!!!", "Unable to find lua file");
        }
        return true;
    };

    bool RunScript(std::string path) {
        LoadScript(path);

        int error = lua_pcall(L, 0, 0, 0);
        while (error && lua_gettop(L))
        {
            int stack= lua_gettop(L);
            int err= error;
            std::string message = lua_tostring(L, -1);
            lua_pop(L, 1);
            error = lua_pcall(L, 0, 0, 0);
            throw Exception(message,"Stack: " + std::to_string(stack) + " Error: " + std::to_string(err));
        }
        return true;
    };

    bool RunString(std::string str) {
        LoadString(str);

        int error = lua_pcall(L, 0, 0, 0);
        while (error && lua_gettop(L))
        {
            int stack = lua_gettop(L);
            int err = error;
            std::string message = lua_tostring(L, -1);
            lua_pop(L, 1);
            error = lua_pcall(L, 0, 0, 0);
            throw Exception(message, "Stack: " + std::to_string(stack) + " Error: " + std::to_string(err));
        }
        return true;
    };


    //TODO::
    template<typename T,int NumArgs>
    bool RegisterFunction(std::string Name, std::function<T(...)> f) {
        lua_register(L, Name.c_str(), [](lua_State* L) {
            std::vector<Var> args;
            for (int i = 0; i < NumArgs; i++)
            {
                //get arguments from lua
            }
            f;

            return 1;
            });
        return true;
    }

    
    template<typename T>
    bool RegisterVar(std::string Name, T value);

    template<>
    bool RegisterVar(std::string Name, bool value) {
        lua_pushinteger(L, value);
        lua_setglobal(L, Name.c_str());
        lua_pop(L, lua_gettop(L));
        return true;
    };

    template<>
    bool RegisterVar(std::string Name, double value) {
        lua_pushnumber(L, value);
        lua_setglobal(L, Name.c_str());
        lua_pop(L, lua_gettop(L));
        return true;
    };

    template<>
    bool RegisterVar(std::string Name, int value) {
        lua_pushinteger(L, value);
        lua_setglobal(L, Name.c_str());
        lua_pop(L, lua_gettop(L));
        return true;
    };

    template<>
    bool RegisterVar(std::string Name, float value) {
        lua_pushnumber(L, value);
        lua_setglobal(L, Name.c_str());
        lua_pop(L, lua_gettop(L));
        return true;
    };

    template<>
    bool RegisterVar(std::string Name, char* value) {
        lua_pushstring(L, value);
        lua_setglobal(L, Name.c_str());
        lua_pop(L, lua_gettop(L));
        return true;
    };

    template<>
    bool RegisterVar(std::string Name, const char* value) {
        lua_pushstring(L, value);
        lua_setglobal(L, Name.c_str());
        lua_pop(L, lua_gettop(L));
        return true;
    };

    template<>
    bool RegisterVar(std::string Name, std::string value) {
        lua_pushstring(L, value.c_str());
        lua_setglobal(L, Name.c_str());
        lua_pop(L, lua_gettop(L));
        return true;
    };

    //TODO::
    template<>
    bool RegisterVar(std::string Name, Table value) {


        lua_pop(L, lua_gettop(L));
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
        lua_getglobal(L, Name.c_str());
        std::string message = lua_tostring(L, -1);
        lua_pop(L, 1);
        return message;
    }

    template<>
    int GetVar(std::string Name)
    {
        lua_getglobal(L, Name.c_str());
        int message = lua_tointeger(L, -1);
        lua_pop(L, 1);
        return message;
    }

    template<>
    double GetVar(std::string Name)
    {
        lua_getglobal(L, Name.c_str());
        double message = lua_tonumber(L, -1);
        lua_pop(L, 1);
        return message;
    }

    template<>
    bool GetVar(std::string Name)
    {
        lua_getglobal(L, Name.c_str());
        bool message = lua_toboolean(L, -1);
        lua_pop(L, 1);
        return message;
    }


    template<typename T, int numArgs>
    T CallFunction(std::string Name, ...) {};

    template<int numArgs>
    int CallFunction(std::string Name, ...)
    {
        lua_getglobal(L, Name.c_str());
        va_list lst;
        va_start(lst, Name);
        for (int i = 0; i < numArgs; i++) {
            int arg = va_arg(lst,int);
            lua_pushnumber(L, arg);
        }
        va_end(lst);

        if (lua_pcall(L, numArgs, 1, 0) == LUA_OK) {
            int result = lua_tointeger(L, -1);
            lua_pop(L, 1);
            lua_pop(L, lua_gettop(L));
            return result;
        }
        throw Exception("fuck", "fuck");
    }
};