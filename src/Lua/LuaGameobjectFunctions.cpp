#include "LuaGameobjectFunctions.h"

#include "Engine/GameObject.h"
#include "Input/InputManager.h"

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <ctype.h>

/*
	Incude Lua api
*/
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>  // Para inicializar la librería base de Lua
}

int LuaGameobjectFunctions::GetPosition(lua_State* lua) {
	GameObject* obj = nullptr;

	obj = (GameObject*)lua_tointeger(lua, -1);

	// create an array
	lua_newtable(lua);

	glm::vec3 pos = obj->transform.position;

	lua_pushnumber(lua, pos.x);
	lua_rawseti(lua, -2, 0);

	lua_pushnumber(lua, pos.y);
	lua_rawseti(lua, -2, 1);

	lua_pushnumber(lua, pos.z);
	lua_rawseti(lua, -2, 2);

	return 1;
}


int LuaGameobjectFunctions::SetPosition(lua_State* lua) {

	GameObject* obj = nullptr;

	obj = (GameObject*)lua_tointeger(lua, -4);
	double x, y, z;

	x = lua_tonumber(lua, -3);
	y = lua_tonumber(lua, -2);
	z = lua_tonumber(lua, -1);

	obj->transform.position = glm::vec3(x, y, z);

	return 0;
}


int LuaGameobjectFunctions::IsKeyPressed(lua_State* lua) {
	char key = '\0';

	size_t len = 0;
	const char* str = lua_tolstring(lua, -1, &len);

	if (len > 0) {
		key = str[0];
	}

	lua_pushboolean(lua, Input::InputManager::GetInstance().IsKeyPressed(toupper(key)));
	return 1;
}