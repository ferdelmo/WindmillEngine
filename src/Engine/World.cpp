#include "World.h"

World::World() : _objects(0){

}
World::~World() {

}

void World::AddObject(GameObject* go) {
	_objects.push_back(go);
}

void World::RemoveObject(GameObject* go) {
	for (auto it = _objects.begin(); it != _objects.end(); ++it) {
		if (*it == go) {
			_objects.erase(it);
			it = _objects.end();
			break;
		}
	}
}

std::vector<GameObject*>& World::GetObjects() {
	return _objects;
}