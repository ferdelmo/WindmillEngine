#pragma once

/*
	Abstract class to represent a component

	@author Fernando del Molino
*/
class Component
{
public:
	/*
		Function to call at the start of the execution
	*/
	virtual void Start() = 0;

	/*
		Update function, call each frame with the time passed between them
	*/
	virtual void Update(float deltaTime) = 0;

	/*
		Call it at the end of the execution, usefull to free resources
	*/
	virtual void End() = 0;
};

