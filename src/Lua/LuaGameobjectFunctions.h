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
		Set the rotation using a vector
		params Gameobject, vector
		return 0
	*/
	int SetRotationWithVector(lua_State* lua);

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
		
		return an array with the mouse position in coordinates between -1,-1 and 1,1
	*/
	int GetMousePosition(lua_State* lua);

	/*
		Register key pressed function
		params key, function_name
	*/
	int RegisterMouseButtonRightPressedFunction(lua_State* lua);


	/*
		If the gameobject is a FirstPersonPlayer, change the camera view
	*/
	int SetFirstPersonLookAt(lua_State* lua);

	/*
		If the gameobject is a FirstPersonPlayer, return the direction view
	*/
	int GetFirstPersonLookAt(lua_State* lua);

	/*
		Normalize Vector
	*/
	int NormalizeVector(lua_State* lua);

	/*
		Cross Product
	*/
	int CrossProduct(lua_State* lua);

	/*
		Dot Product
	*/
	int DotProduct(lua_State* lua);

	/*
		Rotate vector
	*/
	int RotateVector(lua_State* lua);

	/*
		Vec + Vec
	*/
	int VecAddVec(lua_State* lua);

	/*
		Vec - vec
	*/
	int VecMinusVec(lua_State* lua);

	/*
		Vec*esc
	*/
	int VecMulEsc(lua_State* lua);

	/*
		Magnitude of a vector
	*/

	int Magnitude(lua_State* lua);

	/*
		Create GameObjects
	*/

	/*
		Create a bullet
		params Recives a pointer to the gameobejct thats going to instantiate the bullet
		return a pointer to the Bullet gameObject
	*/
	int CreateBullet(lua_State* lua);

	/*
		Destroy GameObject
		param obj to destryo
	*/
	int DestroyGameObject(lua_State* lua);

	/*
		Get the GameObject of the first player

		params A gameObject to get the world
		return the player
	*/
	int GetPlayer(lua_State* lua);
	
	/*
		Creates a skull on the scene 
		params Recives a pointer to the gameobejct thats going to instantiate the bullet
		return a pointer to the Bullet gameObject
	*/
	int CreateSkull(lua_State* lua);

}



