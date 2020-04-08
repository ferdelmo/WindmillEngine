#pragma once

#include "LuaInstance.h"

class GameObject;

namespace LuaGameobjectFunctions {

	/*
		Get the position of a game object.

		Params: GameObject
		Return: position of the go
	*/
	int GetPosition(lua_State* lua);

	/*
		Get the position of a game object.

		Params: GameObject, new position (x,y,z)
		Return:
	*/
	int SetPosition(lua_State* lua);

	/*
		Get rotation
	*/
	int GetRotation(lua_State* lua);

	/*
		Set rotation
	*/
	int SetRotation(lua_State* lua);

	/*
		Get Scale
	*/
	int GetScale(lua_State* lua);

	/*
		Set Scale
	*/
	int SetScale(lua_State* lua);

	/*
		Return true if a key is pressed

		Params: key
		Return: ture/false
	*/
	int IsKeyPressed(lua_State* lua);

	/*
		Return the mouse position
		
		return an array with the mouse position
	*/
	int GetMousePosition(lua_State* lua);
}



