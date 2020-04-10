#pragma once
#include <vector>

class SphereCollider;
class CapsuleCollider;

namespace Physics {

	class PhysicsManager {
	
	public:

		void AddSkull(SphereCollider* s);

		void AddBullet(CapsuleCollider* c);

		void AddDagger(SphereCollider* s);

		void AddGem(SphereCollider* s);

		void AddPlayer(CapsuleCollider* c);

		void UpdatePhysics();

	private:

		static PhysicsManager* _instance;

		std::vector<SphereCollider*> _skulls;
		std::vector<CapsuleCollider*> _bullets;
		std::vector<SphereCollider*> _daggers;
		std::vector<SphereCollider*> _gems;
		CapsuleCollider* _player;
	};
}