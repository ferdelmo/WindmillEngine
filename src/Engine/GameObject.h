#pragma once

#include "Core/Transform.h"
#include <vector>

class Component;
class World;

/*
	Class to represent a game object. 
	For now it has a Transform and a list of componentes

	@author Fernando del Molino
*/
class GameObject
{
private:
	std::vector<Component*> _components;

	World* _world;


public:
	Transform transform;

	~GameObject();


	void AddComponent(Component* component);
	
	template <typename T>
	T GetComponent() {
		for (auto& entry : _components) {
			if ((T*)entry != nullptr) {
				return (T*)entry;
			}
		}
	}

	void SetWorld(World* w);

	World* GetWorld() const;

	void Initialize();

	void Start();

	void Update(float deltaTime);

	void End();
};

