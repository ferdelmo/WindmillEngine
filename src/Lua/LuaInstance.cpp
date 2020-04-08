#include "LuaInstance.h"

#include <iostream>

#include "LuaGameobjectFunctions.h"
/*
	Incude Lua api
*/
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>  // Para inicializar la librer�a base de Lua
}

LuaInstance* LuaInstance::_instance = nullptr;

LuaInstance::LuaInstance() : _lua(nullptr) {

}

LuaInstance& LuaInstance::GetInstance() {
	if (_instance == nullptr) {
		_instance = new LuaInstance();
		_instance->Open();
	}
	return *_instance;
}


bool LuaInstance::Initialize() {
	if (!Open()) {
		CleanUp();
		return false;
	}

	return true;
}

void LuaInstance::CleanUp() {
	Close();
}

bool LuaInstance::Open() {
	_lua = luaL_newstate();

	/*
		Open de libraries for lua
	*/
	luaL_openlibs(_lua);

	RegisterGameObjectsFunctions();

	return true;
}

void LuaInstance::Close() {
	if (_lua != nullptr) {
		return;
	}
	lua_close(_lua);
	_lua = nullptr;
}





bool LuaInstance::LoadScript(const char* script) {
	std::cout << "LOADING SCRIPT " << script << std::endl;

	int error = luaL_loadfile(_lua, script);

	if (error != 0) {
		std::cout << "ERROR: " << error << std::endl;
		return false;
	}

	lua_call(_lua, 0, 0);

	return true;
}


bool LuaInstance::ExecuteProcedure(const char* subroutineName, GameObject* obj, float dt) {
	lua_getglobal(_lua, subroutineName);

	if (!lua_isfunction(_lua, -1)) {
		lua_pop(_lua, 1);
		return false;
	}



	lua_pushinteger(_lua, (lua_Integer)obj);
	lua_pushnumber(_lua, dt);
	lua_call(_lua, 2, 0);

	return true;
}

bool LuaInstance::ExecuteProcedure(const char* subroutineName, GameObject* obj) {
	return true;
}

void LuaInstance::RegisterGameObjectsFunctions() {
	RegisterFunction(LuaGameobjectFunctions::GetPosition, "GetPosition");
	RegisterFunction(LuaGameobjectFunctions::SetPosition, "SetPosition");
	RegisterFunction(LuaGameobjectFunctions::IsKeyPressed, "IsKeyPressed");
}

/*
	Register a function for its use in lua
*/
void LuaInstance::RegisterFunction(lua_CFunction f, const char* luaName) {
	lua_register(_lua, luaName, f);
}