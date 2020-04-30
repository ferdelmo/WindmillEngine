#pragma once

#include <vector>

namespace Physics {

	class Collider;

	using namespace std;
	/*
		Physics engine: the calss that store all the colliders, bodys etc. and operate with them
		Is a singleton

		@author Fernando del Molino
	*/
	class PhysicsEngine
	{
	public:
		static PhysicsEngine& GetInstance();

		static void CleanUp();

		void AddCollider(Collider* col);

		void RemoveCollider(Collider* col);

		/*
			Check for collisions in the colliders and update them
		*/
		void Update();

	private:
		static PhysicsEngine* _instance;
	
		PhysicsEngine();

		~PhysicsEngine();

		vector<Collider*> _colliders = {};
	};
}

