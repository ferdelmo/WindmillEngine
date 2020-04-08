#include "LuaGameobjectFunctions.h"

#include "RenderEngine/VulkanInstance.h"

#include "Engine/GameObject.h"
#include "Input/InputManager.h"

#include "Game/GameObjects/FirstPersonPlayer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


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





glm::vec3 ReadVector(lua_State* lua, int index) {
	double x, y, z;

	// read first
	lua_rawgeti(lua, index, 0);
	x = lua_tonumber(lua, -1);
	lua_pop(lua, 1);

	lua_rawgeti(lua, index, 1);
	y = lua_tonumber(lua, -1);
	lua_pop(lua, 1);

	lua_rawgeti(lua, index, 2);
	z = lua_tonumber(lua, -1);
	lua_pop(lua, 1);

	return glm::vec3(x, y, z);
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

int LuaGameobjectFunctions::GetRotation(lua_State* lua) {
	GameObject* obj = nullptr;

	obj = (GameObject*)lua_tointeger(lua, -1);

	// create an array
	lua_newtable(lua);

	glm::vec3 rot = obj->transform.rotation;

	lua_pushnumber(lua, rot.x);
	lua_rawseti(lua, -2, 0);

	lua_pushnumber(lua, rot.y);
	lua_rawseti(lua, -2, 1);

	lua_pushnumber(lua, rot.z);
	lua_rawseti(lua, -2, 2);

	return 1;
}

int LuaGameobjectFunctions::SetRotation(lua_State* lua) {
	GameObject* obj = nullptr;

	obj = (GameObject*)lua_tointeger(lua, -4);
	double x, y, z;

	x = lua_tonumber(lua, -3);
	y = lua_tonumber(lua, -2);
	z = lua_tonumber(lua, -1);

	obj->transform.rotation = glm::vec3(x, y, z);

	return 0;
}


int LuaGameobjectFunctions::GetScale(lua_State* lua) {
	GameObject* obj = nullptr;

	obj = (GameObject*)lua_tointeger(lua, -1);

	// create an array
	lua_newtable(lua);

	glm::vec3 scale = obj->transform.scale;

	lua_pushnumber(lua, scale.x);
	lua_rawseti(lua, -2, 0);

	lua_pushnumber(lua, scale.y);
	lua_rawseti(lua, -2, 1);

	lua_pushnumber(lua, scale.z);
	lua_rawseti(lua, -2, 2);

	return 1;
}

int LuaGameobjectFunctions::SetScale(lua_State* lua) {
	GameObject* obj = nullptr;

	obj = (GameObject*)lua_tointeger(lua, -4);

	double x, y, z;

	x = lua_tonumber(lua, -3);
	y = lua_tonumber(lua, -2);
	z = lua_tonumber(lua, -1);

	obj->transform.scale = glm::vec3(x, y, z);

	return 0;
}

int LuaGameobjectFunctions::GetMousePosition(lua_State* lua) {
	// create an array
	lua_newtable(lua);

	double x, y;

	Input::InputManager::GetInstance().GetMousePosition(x, y);

	x = x / VulkanInstance::GetInstance().width;
	y = y / VulkanInstance::GetInstance().height;

	x -= 0.5; 
	y -= 0.5;

	x *= 2;
	y *= 2;

	lua_pushnumber(lua, x);
	lua_rawseti(lua, -2, 0);

	lua_pushnumber(lua, y);
	lua_rawseti(lua, -2, 1);

	return 1;
}

int LuaGameobjectFunctions::SetFirstPersonLookAt(lua_State* lua) {

	FirstPersonPlayer* fpp = (FirstPersonPlayer*)lua_tointeger(lua, -2);

	double x, y, z;

	fpp->SetLookAt(ReadVector(lua,-1));

	return 0;
}


int LuaGameobjectFunctions::GetFirstPersonLookAt(lua_State* lua) {

	FirstPersonPlayer* fpp = (FirstPersonPlayer*)lua_tointeger(lua, -1);

	glm::vec3 resul = fpp->GetLookAt();
	
	lua_newtable(lua);

	lua_pushnumber(lua, resul.x);
	lua_rawseti(lua, -2, 0);

	lua_pushnumber(lua, resul.y);
	lua_rawseti(lua, -2, 1);

	lua_pushnumber(lua, resul.z);
	lua_rawseti(lua, -2, 2);

	return 1;
}

int LuaGameobjectFunctions::NormalizeVector(lua_State* lua) {
	glm::vec3 vec1 = ReadVector(lua, -1);

	// create an array
	lua_newtable(lua);

	glm::vec3 resul = glm::normalize(vec1);

	lua_pushnumber(lua, resul.x);
	lua_rawseti(lua, -2, 0);

	lua_pushnumber(lua, resul.y);
	lua_rawseti(lua, -2, 1);

	lua_pushnumber(lua, resul.z);
	lua_rawseti(lua, -2, 2);

	return 1;
}

int LuaGameobjectFunctions::CrossProduct(lua_State* lua) {
	glm::vec3 vec1 = ReadVector(lua, -2);
	glm::vec3 vec2 = ReadVector(lua, -1);

	// create an array
	lua_newtable(lua);

	glm::vec3 resul = glm::cross(vec1, vec2);

	lua_pushnumber(lua, resul.x);
	lua_rawseti(lua, -2, 0);

	lua_pushnumber(lua, resul.y);
	lua_rawseti(lua, -2, 1);

	lua_pushnumber(lua, resul.z);
	lua_rawseti(lua, -2, 2);

	return 1;
}

int LuaGameobjectFunctions::DotProduct(lua_State* lua) {
	glm::vec3 vec1 = ReadVector(lua, -2);
	glm::vec3 vec2 = ReadVector(lua, -1);

	double value = glm::dot(vec1, vec2);

	lua_pushnumber(lua, value);

	return 1;
}

int LuaGameobjectFunctions::RotateVector(lua_State* lua) {
	glm::vec3 original = ReadVector(lua, -3);
	glm::vec3 up = ReadVector(lua, -2);

	float angle = lua_tonumber(lua, -1);

	up = glm::normalize(up);
	
	glm::mat4 iden = glm::mat4(1);

	glm::mat4 rot = glm::rotate(iden, angle, up);

	glm::vec resul = rot * glm::vec4(original, 0);

	// create an array
	lua_newtable(lua);

	lua_pushnumber(lua, resul.x);
	lua_rawseti(lua, -2, 0);

	lua_pushnumber(lua, resul.y);
	lua_rawseti(lua, -2, 1);

	lua_pushnumber(lua, resul.z);
	lua_rawseti(lua, -2, 2);

	return 1;
}
int LuaGameobjectFunctions::VecAddVec(lua_State* lua) {
	glm::vec3 vec1 = ReadVector(lua, -2);
	glm::vec3 vec2 = ReadVector(lua, -1);

	glm::vec3 resul = vec1 + vec2;

	// create an array
	lua_newtable(lua);

	lua_pushnumber(lua, resul.x);
	lua_rawseti(lua, -2, 0);

	lua_pushnumber(lua, resul.y);
	lua_rawseti(lua, -2, 1);

	lua_pushnumber(lua, resul.z);
	lua_rawseti(lua, -2, 2);

	return 1;
}

int LuaGameobjectFunctions::VecMinusVec(lua_State* lua) {
	glm::vec3 vec1 = ReadVector(lua, -2);
	glm::vec3 vec2 = ReadVector(lua, -1);

	glm::vec3 resul = vec1 - vec2;
	// create an array
	lua_newtable(lua);

	lua_pushnumber(lua, resul.x);
	lua_rawseti(lua, -2, 0);

	lua_pushnumber(lua, resul.y);
	lua_rawseti(lua, -2, 1);

	lua_pushnumber(lua, resul.z);
	lua_rawseti(lua, -2, 2);

	return 1;
}

int LuaGameobjectFunctions::VecMulEsc(lua_State* lua) {
	glm::vec3 vec1 = ReadVector(lua, -2);
	float esc = lua_tonumber(lua, -1);

	glm::vec3 resul = vec1 * esc;

	// create an array
	lua_newtable(lua);

	lua_pushnumber(lua, resul.x);
	lua_rawseti(lua, -2, 0);

	lua_pushnumber(lua, resul.y);
	lua_rawseti(lua, -2, 1);

	lua_pushnumber(lua, resul.z);
	lua_rawseti(lua, -2, 2);

	return 1;
}