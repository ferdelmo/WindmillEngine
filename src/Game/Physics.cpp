#include "Physics.h"
#include "Colliders/CapsuleCollider.h"
#include "Colliders/SphereCollider.h"
#include <iostream>

using namespace PhysicsManager;

Physics& Physics::GetInstance() {
	if (_instance == nullptr) {
		_instance = new Physics();
	}
	return *_instance;
}

void Physics::Update(float deltaTime) {

	for (SphereCollider* it : _skulls) {
		it->Update(deltaTime);
	}

	for (SphereCollider* it : _daggers) {
		it->Update(deltaTime);
	}

	for (CapsuleCollider* it : _bullets) {
		it->Update(deltaTime);
	}

	for (SphereCollider* it : _gems) {
		it->Update(deltaTime);
	}

	_player->Update(deltaTime);
}

void Physics::AddSkull(SphereCollider* c) {
	_skulls.push_back(c);
}

void Physics::AddDagger(SphereCollider* c) {
	_daggers.push_back(c);
}

void Physics::AddBullet(CapsuleCollider* c) {
	_bullets.push_back(c);
}

void Physics::AddGem(SphereCollider* c) {
	_gems.push_back(c);
}

void Physics::AddPlayer(CapsuleCollider* c) {
	if (_player != nullptr) {
		std::cout << "Physiscs.cpp: a player already exists in the physics world." << std::endl;
		return;
	}
	_player = c;
}

void Physics::CheckCollisions() {
	// bullet-skull collision
	for (int i = 0; i < _bullets.size(); ++i) {
		for (int j = 0; j < _skulls.size(); ++j) {
			if (_skulls.at(j)->Collision(_bullets.at(i))) {
				//lower skull's health and delete bullet
				_bullets.erase(_bullets.begin() + i);
				--i;
				break;
			}
		}
	}

	// bullet-dagger collision
	for (int i = 0; i < _bullets.size(); ++i) {
		for (int j = 0; j < _daggers.size(); ++j) {
			if (_daggers.at(j)->Collision(_bullets.at(i))) {
				//lower dagger's health and delete bullet
				_bullets.erase(_bullets.begin() + i);
				--i;
				break;
			}
		}
	}

	//player-skull collision
	for (SphereCollider* skull : _skulls) {
		if (skull->Collision(_player)) {
			// kill pllayer 
			break;
		}
	}

	//player-dagger collision
	for (SphereCollider* dagger : _daggers) {
		if (dagger->Collision(_player)) {
			// kill player
			break;
		}
	}

	//player-gem collision
	for (SphereCollider* gem : _gems) {
		if (gem->Collision(_player)) {
			// increase level
		}
	}
}

Physics::~Physics() {}

Physics::Physics() {}