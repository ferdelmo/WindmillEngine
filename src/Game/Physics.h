#pragma once

#include <vector>

class CapsuleCollider;
class SphereCollider;
class GameObject;

namespace PhysicsManager {

	class Physics {
	
	public:

		static Physics& GetInstance();

		void Update(float deltaTime);
		
		void AddSkull(SphereCollider* c);

		void AddDagger(SphereCollider* c);

		void AddBullet(CapsuleCollider* c);

		void AddGem(SphereCollider* c);

		void AddPlayer(CapsuleCollider* c);

		void CheckCollisions();

		~Physics();

	private:
		Physics();

		static Physics* _instance;

		std::vector<SphereCollider*> _skulls;

		std::vector<SphereCollider*> _daggers;

		std::vector<CapsuleCollider*> _bullets;

		std::vector<SphereCollider*> _gems;

		CapsuleCollider* _player;
	};
}