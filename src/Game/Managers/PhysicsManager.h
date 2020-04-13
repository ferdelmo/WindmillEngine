#pragma once

#include <vector>

class CapsuleCollider;
class SphereCollider;
class Component;

namespace Physics {


	class PhysicsManager {
	
	public:

		static PhysicsManager& GetInstance();

		void Update(float deltaTime);
		
		void AddSkull(SphereCollider* c);

		void AddDagger(SphereCollider* c);

		void AddBullet(CapsuleCollider* c);

		void AddPlayer(CapsuleCollider* c);

		void UpdateCollisions();

		void RemoveSphereCollider(SphereCollider* c);

		void RemoveCapsuleCollider(CapsuleCollider* c);

		~PhysicsManager();

	private:
		PhysicsManager();

		static PhysicsManager* _instance;

		std::vector<SphereCollider*> _skulls;

		std::vector<SphereCollider*> _daggers;

		std::vector<CapsuleCollider*> _bullets;

		CapsuleCollider* _player;

	};
}