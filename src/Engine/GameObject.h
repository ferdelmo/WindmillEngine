#pragma once

#include "Core/Transform.h"
#include <vector>

class Component;
class World;
class StaticMeshComponent;


/*
	Class to represent a game object. 
	For now it has a Transform and a list of componentes

	@author Fernando del Molino
*/
class GameObject
{
private:
	std::vector<Component*> _components;

	/* 
		Late update components, this vector is for components like render or colliders that 
		should be updated after the other componentes
	*/
	std::vector<Component*> _lateUpdateComponents;

	World* _world;


public:
	Transform transform;

	~GameObject();



	void AddComponent(StaticMeshComponent* component);

	void AddComponent(Component* component);
	
	template <typename T>
	T* GetComponent() {
		for (auto& entry : _components) {
			if ((T*)entry != nullptr) {
				return (T*)entry;
			}
		}

		for (auto& entry : _lateUpdateComponents) {
			if ((T*)entry != nullptr) {
				return (T*)entry;
			}
		}
	}

	void SetWorld(World* w);

	World* GetWorld() const;

	virtual void Initialize();

	virtual void Start();

	virtual void Update(float deltaTime);

	virtual void End();
};

