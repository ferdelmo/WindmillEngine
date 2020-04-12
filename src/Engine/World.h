#pragma once

#include "GameObject.h"

#include "RenderEngine/Renderer/Uniform.h"
#include "RenderEngine/Renderer/Camera.h"

#include <vector>

class GameObject;

struct SceneLight {
	Lights lights;
	AmbientLight ambient;
};

/*
	Class to represent teh world, as a list of
*/
class World
{
private:
	std::vector<GameObject*> _objects;

	std::vector<GameObject*> _pendingToRemove;

	SceneLight _lights;

	Camera _camera;

	static World* _activeWorld;
public:
	World();
	~World(); 

	/*
		Add an object to the scene
		// render and call ticks
	*/
	void AddObject(GameObject* go);


	/*
		Remove de object from the world
	*/
	void RemoveObject(GameObject* go);

	std::vector<GameObject*>& GetObjects();

	void SetLights(const SceneLight& l);

	SceneLight& GetLights();

	void SetCamera(const Camera& cam);

	Camera& GetCamera();

	void Initialize();

	void Start();

	void Update(float deltaTime);

	void End();

	/* Destroy all the gameobjects in the world*/
	void Clean();

	static World& GetActiveWorld();

	/* Adds a game object to the pending to remove
	   game object vector
	*/
	void AddPendingToRemove(GameObject* go);

	/*
		Deletes all the pending to remove game objects
		from both _objects and _pendingToRemove
	*/
	void DeletePendingToRemove();
};

