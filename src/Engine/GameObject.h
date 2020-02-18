#pragma once

#include "Core/Transform.h"
#include <vector>

class Component;

/*
	Class to represent a game object. 
	For now it has a Transform and a list of componentes

	@author Fernando del Molino
*/
class GameObject
{
private:
	std::vector<Component*> components;

public:
	Transform transform;

};

