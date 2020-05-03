#include "GameObject.h"

#include "World.h"
#include "Component.h"

GameObject::~GameObject() {
	for (auto& entry : _components) {
		delete entry;
	}
}

void GameObject::AddComponent(Component* component) {
	if (component != nullptr) {
		component->SetOwner(this);
		component->Initialize();
		_components.push_back(component);
	}
}

void GameObject::SetWorld(World* w) {
	_world = w;
}

World* GameObject::GetWorld() const {
	return _world;
}

void GameObject::Initialize() {
	for (auto& entry : _components) {
		entry->Initialize();
	}
}

void GameObject::Start() {
	for (auto& entry : _components) {
		entry->Start();
	}
}

void GameObject::Update(float deltaTime) {
	for (auto& entry : _components) {
		entry->Update(deltaTime);
	}
}

void GameObject::End() {
	for (auto& entry : _components) {
		entry->End();
	}
}