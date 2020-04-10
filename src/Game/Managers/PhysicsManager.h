#pragma once

#include <vector>

class CapsuleCollider;
class SphereCollider;

namespace Physics {

	class PhysicsManager {
	
	public:

		static PhysicsManager& GetInstance();

		void Update(float deltaTime);
		
		void AddSkull(SphereCollider* c);

		void AddDagger(SphereCollider* c);

		void AddBullet(CapsuleCollider* c);

		void AddGem(SphereCollider* c);

		void AddPlayer(CapsuleCollider* c);

		void UpdateCollisions();

		~PhysicsManager();

	private:
		PhysicsManager();

		static PhysicsManager* _instance;

		std::vector<SphereCollider*> _skulls;

		std::vector<SphereCollider*> _daggers;

		std::vector<CapsuleCollider*> _bullets;

		std::vector<SphereCollider*> _gems;

		CapsuleCollider* _player;
	};
}