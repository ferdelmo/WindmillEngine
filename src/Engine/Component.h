#pragma once



class World;
class GameObject;

/*
	Abstract class to represent a component

	@author Fernando del Molino
*/
class Component
{
protected:
	GameObject* _owner;

public:
	
	virtual ~Component() {}

	/*
		Function to call for the initialization
	*/
	virtual void Initialize() {}

	/*
		Function to call at the start of the execution
	*/
	virtual void Start() {}

	/*
		Update function, call each frame with the time passed between them
	*/
	virtual void Update(float deltaTime) {}

	/*
		Call it at the end of the execution, usefull to free resources
	*/
	virtual void End() {}

	void SetOwner(GameObject* go) {
		_owner = go;
	}

	GameObject* GetOwner() {
		return _owner;
	}
};

