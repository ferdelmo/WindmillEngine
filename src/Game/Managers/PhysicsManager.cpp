#include "PhysicsManager.h"
#include "Game/Colliders/CapsuleCollider.h"
#include "Game/Colliders/SphereCollider.h"
#include <iostream>
#include "Lua/LuaInstance.h"

using namespace Physics;

PhysicsManager* PhysicsManager::_instance = nullptr;

PhysicsManager& PhysicsManager::GetInstance() {
	if (_instance == nullptr) {
		_instance = new PhysicsManager();
	}
	return *_instance;
}

void PhysicsManager::Update(float deltaTime) {

	for (SphereCollider* it : _skulls) {
		it->Update(deltaTime);
	}

	for (SphereCollider* it : _daggers) {
		it->Update(deltaTime);
	}

	for (CapsuleCollider* it : _bullets) {
		it->Update(deltaTime);
	}

	_player->Update(deltaTime);
}

void PhysicsManager::AddSkull(SphereCollider* c) {
	//std::cout << "skull added" << std::endl;
	_skulls.push_back(c);
}

void PhysicsManager::AddDagger(SphereCollider* c) {
	_daggers.push_back(c);
}

void PhysicsManager::AddBullet(CapsuleCollider* c) {
	//std::cout << "bullet added" << std::endl;
	_bullets.push_back(c);
}

void PhysicsManager::AddPlayer(CapsuleCollider* c) {
	if (_player != nullptr) {
		//std::cout << "Physiscs.cpp: a player already exists in the physics world." << std::endl;
		return;
	}
	_player = c;
}

void PhysicsManager::UpdateCollisions() {
	// bullet-skull collision
	for (int i = 0; i < _bullets.size(); ++i) {
		for (int j = 0; j < _skulls.size(); ++j) {
			if (_skulls.at(j)->Collision(_bullets.at(i))) {
				//std::cout << "bullet - skull collision" << std::endl;
				LuaInstance::GetInstance().ExecuteProcedure("enemy_Die", _skulls.at(j)->GetOwner());
				LuaInstance::GetInstance().ExecuteProcedure("bullet_Die", _bullets.at(i)->GetOwner());
				_bullets.erase(_bullets.begin() + i);
				_skulls.erase(_skulls.begin() + j);

				--i;
				break;
			}
		}
	}

	// bullet-dagger collision
	for (int i = 0; i < _bullets.size(); ++i) {
		for (int j = 0; j < _daggers.size(); ++j) {
			if (_daggers.at(j)->Collision(_bullets.at(i))) {
				//std::cout << "bullet - dagger collision" << std::endl;
				LuaInstance::GetInstance().ExecuteProcedure("dagger_Die", _skulls.at(j)->GetOwner());
				LuaInstance::GetInstance().ExecuteProcedure("bullet_Die", _bullets.at(i)->GetOwner());
				_bullets.erase(_bullets.begin() + i);
				_skulls.erase(_skulls.begin() + j);
				//--i;
				break;
			}
		}
	}

	//player-skull collision
	for (SphereCollider* skull : _skulls) {
		if (skull->Collision(_player)) {
			std::cout << "player - skull collision" << std::endl;
			// kill pllayer 
			break;
		}
	}

	//player-dagger collision
	for (SphereCollider* dagger : _daggers) {
		if (dagger->Collision(_player)) {
			std::cout << "player - dagger collision" << std::endl;
			// kill player
			break;
		}
	}

}

PhysicsManager::~PhysicsManager() {}

PhysicsManager::PhysicsManager() {
	_player = nullptr;
}
