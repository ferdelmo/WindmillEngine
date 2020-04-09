#include "GameObject.h"

#include "World.h"
#include "Component.h"
#include "StaticMeshComponent.h"

GameObject::~GameObject() {
	for (auto& entry : _components) {
		delete entry;
	}

	for (auto& entry : _lateUpdateComponents) {
		delete entry;
	}
}

void GameObject::AddComponent(StaticMeshComponent* component) {
	if (component != nullptr) {
		component->SetOwner(this);
		_lateUpdateComponents.push_back(component);
		component->Initialize();
		component->Start();
	}
}

void GameObject::AddComponent(Component* component) {
	if (component != nullptr) {
		component->SetOwner(this);
		_components.push_back(component);
		component->Initialize();
		component->Start();
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
	for (auto& entry : _lateUpdateComponents) {
		entry->Update(deltaTime);
	}
}

void GameObject::End() {
	for (auto& entry : _components) {
		entry->End();
	}
}