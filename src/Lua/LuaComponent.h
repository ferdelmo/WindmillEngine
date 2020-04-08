#pragma once
#include "Engine/Component.h"

#include <string>

class LuaComponent : public Component
{
private:
	std::string _pathScript;

	/*
		return the name of the script
	*/
	std::string ScriptName();

public:

	LuaComponent(std::string _pathScript);

	/*
		Function to call for the initialization
	*/
	virtual void Initialize();

	/*
		Function to call at the start of the execution
	*/
	virtual void Start();

	/*
		Update function, call each frame with the time passed between them
	*/
	virtual void Update(float deltaTime);

	/*
		Call it at the end of the execution, usefull to free resources
	*/
	virtual void End();
};

