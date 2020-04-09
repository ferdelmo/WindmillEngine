#include "LuaGameobjectFunctions.h"

#include "RenderEngine/VulkanInstance.h"

#include "Engine/World.h"
#include "Engine/GameObject.h"
#include "Engine/StaticMeshComponent.h"


#include "LuaComponent.h"


#include "Input/InputManager.h"

#include "Game/GameObjects/FirstPersonPlayer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <stdio.h>
#include <ctype.h>
#include <iostream>

/*
	Incude Lua api
*/
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>  // Para inicializar la librer�a base de Lua
}

glm::vec3 ReadVector(lua_State* lua, int index) {
	double x, y, z;

	// read first
	lua_rawgeti(lua, index, 1);
	x = lua_tonumber(lua, -1);
	lua_pop(lua, 1);

	lua_rawgeti(lua, index, 2);
	y = lua_tonumber(lua, -1);
	lua_pop(lua, 1);

	lua_rawgeti(lua, index, 3);
	z = lua_tonumber(lua, -1);
	lua_pop(lua, 1);

	return glm::vec3(x, y, z);
}

void PushVector(lua_State* lua, glm::vec3 vec) {
	lua_newtable(lua);

	lua_pushnumber(lua, vec.x);
	lua_rawseti(lua, -2, 1);

	lua_pushnumber(lua, vec.y);
	lua_rawseti(lua, -2, 2);

	lua_pushnumber(lua, vec.z);
	lua_rawseti(lua, -2, 3);
}

int LuaGameobjectFunctions::GetPosition(lua_State* lua) {
	GameObject* obj = nullptr;

	obj = (GameObject*)lua_tointeger(lua, -1);

	PushVector(lua, obj->transform.position);

	return 1;
}


int LuaGameobjectFunctions::SetPosition(lua_State* lua) {

	GameObject* obj = nullptr;

	obj = (GameObject*)lua_tointeger(lua, -2);

	obj->transform.position = ReadVector(lua, -1);

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


	PushVector(lua, glm::eulerAngles(obj->transform.rotation));

	return 1;
}

int LuaGameobjectFunctions::SetRotation(lua_State* lua) {
	GameObject* obj = nullptr;

	obj = (GameObject*)lua_tointeger(lua, -2);

	obj->transform.rotation = ReadVector(lua, -1);

	return 0;
}


int LuaGameobjectFunctions::SetRotationWithVector(lua_State* lua) {
	GameObject* obj = nullptr;

	obj = (GameObject*)lua_tointeger(lua, -2);

	glm::vec3 orient = ReadVector(lua, -1);


	obj->transform.rotation = glm::quatLookAt(orient, { 0,0,1 });

	return 0;

}


int LuaGameobjectFunctions::GetScale(lua_State* lua) {
	GameObject* obj = nullptr;

	obj = (GameObject*)lua_tointeger(lua, -1);

	// create an array
	lua_newtable(lua);

	PushVector(lua, obj->transform.scale);

	return 1;
}

int LuaGameobjectFunctions::SetScale(lua_State* lua) {
	GameObject* obj = nullptr;

	obj = (GameObject*)lua_tointeger(lua, -2);

	obj->transform.scale = ReadVector(lua, -1);

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
	
	PushVector(lua, resul);

	return 1;
}

int LuaGameobjectFunctions::NormalizeVector(lua_State* lua) {
	glm::vec3 vec1 = ReadVector(lua, -1);

	glm::vec3 resul = glm::normalize(vec1);

	PushVector(lua, resul);

	return 1;
}

int LuaGameobjectFunctions::Magnitude(lua_State* lua) {
	glm::vec3 vec = ReadVector(lua, -1);

	double result = glm::length(vec);

	lua_pushnumber(lua, result);

	return 1;
}

int LuaGameobjectFunctions::CrossProduct(lua_State* lua) {
	glm::vec3 vec1 = ReadVector(lua, -2);
	glm::vec3 vec2 = ReadVector(lua, -1);

	glm::vec3 resul = glm::cross(vec1, vec2);


	PushVector(lua, resul);

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

	PushVector(lua, resul);

	return 1;
}
int LuaGameobjectFunctions::VecAddVec(lua_State* lua) {
	glm::vec3 vec1 = ReadVector(lua, -2);
	glm::vec3 vec2 = ReadVector(lua, -1);

	glm::vec3 resul = vec1 + vec2;

	PushVector(lua, resul);

	return 1;
}

int LuaGameobjectFunctions::VecMinusVec(lua_State* lua) {
	glm::vec3 vec1 = ReadVector(lua, -2);
	glm::vec3 vec2 = ReadVector(lua, -1);

	glm::vec3 resul = vec1 - vec2;


	PushVector(lua, resul);

	return 1;
}

int LuaGameobjectFunctions::VecMulEsc(lua_State* lua) {
	glm::vec3 vec1 = ReadVector(lua, -2);
	float esc = lua_tonumber(lua, -1);

	glm::vec3 resul = vec1 * esc;

	PushVector(lua, resul);

	return 1;
}

/*
	Create a bullet
*/
int LuaGameobjectFunctions::CreateBullet(lua_State* lua) {
	GameObject* obj = nullptr;

	obj = (GameObject*)lua_tointeger(lua, -1);

	// create a bullet, for now we use new, this should be a pool of bullets to not use 
	// new almost each tick the player is shooting

	GameObject* bullet = new GameObject();

	glm::vec3 lookAt = ((FirstPersonPlayer*)obj)->GetLookAt();

	glm::normalize(lookAt);

	const float dist = 5.0f;

	obj->GetWorld()->AddObject(bullet);
	bullet->transform.position = obj->transform.position + lookAt*dist;
	bullet->transform.scale = glm::vec3(2,2,2);

	StaticMeshComponent* mesh = new StaticMeshComponent("../resources/objs/Cube.obj", glm::vec4(1,0,1,1));
	LuaComponent* luaComp = new LuaComponent("../resources/lua/bullet.lua");

	bullet->AddComponent(mesh);
	bullet->AddComponent(luaComp);

	lua_pushinteger(lua, (lua_Integer)bullet);

	return 1;
}

int LuaGameobjectFunctions::DestroyGameObject(lua_State* lua) {
	GameObject* obj = nullptr;

	obj = (GameObject*)lua_tointeger(lua, -1);

	obj->GetWorld()->RemoveObject(obj);
	delete obj;

	return 0;
}


int LuaGameobjectFunctions::GetPlayer(lua_State* lua) {
	GameObject* obj = nullptr;

	obj = (GameObject*)lua_tointeger(lua, -1);

	std::vector<GameObject*> objs = obj->GetWorld()->GetObjects();

	FirstPersonPlayer* fpp = nullptr;
	for (auto& entry : objs) {

		if ((FirstPersonPlayer*)entry != nullptr) {
			fpp = (FirstPersonPlayer*)entry;
			break;
		}

	}

	lua_pushinteger(lua, (lua_Integer)fpp);

	return 1;
}