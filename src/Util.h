#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <iostream>

extern "C"{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

static bool lua_Check(lua_State *L, int r){
	if(r != LUA_OK){
		std::string errmsg = lua_tostring(L, -1);
		std::cout << errmsg << std::endl;
		return false;
	}
	return true;
}

static std::string lua_GetTableStr(lua_State *L, const char* property){
	lua_pushstring(L, property);
	lua_gettable(L, -2);
	std::string ret = lua_tostring(L, -1);
	lua_pop(L, 1);

	return ret;
}

static lua_Number lua_GetTableNum(lua_State *L, const char* property){
	lua_pushstring(L, property);
	lua_gettable(L, -2);
	float ret = lua_tonumber(L, -1);
	lua_pop(L, 1);

	return ret;
}

#endif
