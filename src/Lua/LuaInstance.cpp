#include "LuaInstance.h"

/*
	Incude Lua api
*/
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>  // Para inicializar la librería base de Lua
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
	_lua = lua_open();

	/*
		Open de libraries for lua
	*/
	luaL_openlibs(_lua);

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
	if (luaL_loadfile(_lua, script) != 0) {
		return false;
	}

	lua_call(_lua, 0, 0);

	return true;
}

/*
	Execute procedure
*/
bool LuaInstance::ExecuteProcedure(const char* subroutineName, float param1) {

	lua_getglobal(_lua, subroutineName);

	if (!lua_isfunction(_lua, -1)) {
		lua_pop(_lua, 1);
		return false;
	}

	lua_pushnumber(_lua, param1);
	lua_call(_lua, 1, 0);

	return true;
}