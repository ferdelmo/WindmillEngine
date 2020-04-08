#pragma once

// Context structure for the luaVM
struct lua_State;

// define the header of the functions that can be bind to Lua
typedef int (*lua_CFunction) (lua_State* L);

class GameObject;

/*
	Singleton to manage the acces to Lua
*/
class LuaInstance
{
public:

	static LuaInstance& GetInstance();

	/*
		Initialize the Lua VM
	*/
	bool Initialize();

	/*
		Clean up the resources
	*/
	void CleanUp();

	/*
		Load script
	*/
	bool LoadScript(const char* script);

	/*
		Execute procedure
	*/
	bool ExecuteProcedure(const char* subroutineName, GameObject* obj, float dt);

	bool ExecuteProcedure(const char* subroutineName, GameObject* obj);

	/*
		Register functios from luaGameObjectsFunctions
	*/
	void RegisterGameObjectsFunctions();

	/*
		Register a function for its use in lua
	*/
	void RegisterFunction(lua_CFunction f, const char* luaName);

private:
	static LuaInstance* _instance;
	
	/*
		Reference to Lua VM
	*/
	lua_State* _lua;

	LuaInstance();

	bool Open();

	void Close();
};

