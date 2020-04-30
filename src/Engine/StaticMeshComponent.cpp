#include "StaticMeshComponent.h"

#include "RenderEngine/Renderer/Mesh.h"
#include "RenderEngine/Renderer/StaticMesh.h"
#include "RenderEngine/Renderer/Material.h"
#include "RenderEngine/Renderer/Utils/MaterialUtils.h"
#include "RenderEngine/Renderer/SingleThreadRenderer/SingleThreadRenderer.h"

#include "World.h"

#include <glm/gtc/matrix_transform.hpp>

#include "RenderEngine/Renderer/MaterialInstance.h"

StaticMeshComponent::StaticMeshComponent(std::string mesh, MaterialInstance* mat) : _pathMesh(mesh),
	_mesh(nullptr), _material(mat), color(glm::vec4(1)), _staticMesh(nullptr) {

}

StaticMeshComponent::StaticMeshComponent(std::string mesh, glm::vec4 color) : _pathMesh(mesh),
	_mesh(nullptr), _material(nullptr), color(color), _staticMesh(nullptr) {

}

StaticMeshComponent::~StaticMeshComponent() {

	StaticMesh* staticMesh = _staticMesh;

	std::function<void()> cleanup = [staticMesh]() {
		delete staticMesh;
	};

	SingleThreadRenderer::GetInstance().RemoveObject(_staticMesh, cleanup);

	delete _material;
	delete _mesh;
}


void StaticMeshComponent::Initialize() {

	if (_material == nullptr) {
		World* world = _owner->GetWorld();
		_material = GetBasicColorMaterial(world->GetCamera(), 
			{ color, 0.4f, {1,1,1}, 0.1f, 50 });
	}

	_staticMesh = new StaticMesh(_pathMesh, _material);

	SingleThreadRenderer::GetInstance().AddObject(_staticMesh);
}


void StaticMeshComponent::Start() {

}

void StaticMeshComponent::Update(float deltaTime) {
	MVP& obj = _staticMesh->GetMVP();
	obj.proj = _owner->GetWorld()->GetCamera().GetProjection();
	obj.view = _owner->GetWorld()->GetCamera().GetView();

	obj.model = glm::mat4(1);

	Transform trans = _owner->transform;
	glm::normalize(trans.rotation);

	glm::mat4 rot = glm::toMat4(trans.rotation);
		/*
		glm::rotate(glm::mat4(1), glm::radians(trans.rotation.x), glm::vec3(1,0,0)) *
		glm::rotate(glm::mat4(1), glm::radians(trans.rotation.y), glm::vec3(0, 1, 0)) *
		glm::rotate(glm::mat4(1), glm::radians(trans.rotation.z), glm::vec3(0, 0, 1));
		*/

	obj.model = glm::translate(glm::mat4(1), trans.position) * rot * glm::scale(glm::mat4(1), trans.scale);

	_staticMesh->Update(deltaTime);
}

void StaticMeshComponent::End() {

}

void StaticMeshComponent::SetVisibility(bool activate) {
	if (activate) {
		SingleThreadRenderer::GetInstance().AddObject(_staticMesh);
	}
	else {
		SingleThreadRenderer::GetInstance().RemoveObject(_staticMesh);
	}
}