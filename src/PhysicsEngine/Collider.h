#pragma once

#include <vector>
#include "Shapes/Shape.h"
#include "Core/Delegate.h"
#include "Engine/GameObject.h"


namespace Physics {

	class Collider;


	typedef std::function<void(GameObject*, Collider*)> BeginOverlapEvent;
	typedef std::function<void(GameObject*, Collider*)> EndOverlapEvent;

	/*
		Collider represent by a shape
	*/
	class Collider
	{
	public: 
		Collider(glm::vec3 position, float r);

		Collider(glm::vec3 position, glm::vec3 extent);

		virtual ~Collider();

		/*
			Call by the physics engine when an object start overlapping another
		*/
		void BeginOverlap(Collider* col);

		/*
			Call by the physics engine when an object is overlapping another
		*/
		void IsOverlapping(Collider* col);

		/*
			Call by the physics engine when an object is overlapping another
		*/
		void EndOverlap(Collider* col);

		Shape* GetShape();

		std::vector<Collider*>& GetOverlappingColliders();

		void SetGameObject(GameObject* go);
		GameObject* GetGameObject() const;

		MultipleDelegate<GameObject*, Collider*>& GetBeginOverlapEvents();
		MultipleDelegate<GameObject*, Collider*>& GetEndOverlapEvents();

	private:
		// shape of the collider
		Shape* _shape;

		std::vector<Collider*> _overlappingColliders;

		// Gameobject associated
		GameObject* _owner = nullptr;

		MultipleDelegate<GameObject*, Collider*> bo;
		MultipleDelegate<GameObject*, Collider*> eo;
	};
}

