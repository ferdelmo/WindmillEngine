#pragma once

#include "GameObject.h"
#include <vector>

class GameObject;

/*
	Class to represent teh world, as a list of
*/
class World
{
private:
	std::vector<GameObject*> _objects;

public:
	World();
	~World(); 

	void AddObject(GameObject* go);

	void RemoveObject(GameObject* go);

	std::vector<GameObject*>& GetObjects();
};

