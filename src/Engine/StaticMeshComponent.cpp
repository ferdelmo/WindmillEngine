#include "StaticMeshComponent.h"

#include "RenderEngine/Renderer/Mesh.h"
#include "RenderEngine/Renderer/StaticMesh.h"
#include "RenderEngine/Renderer/Material.h"
#include "RenderEngine/Renderer/Utils/MaterialUtils.h"
#include "RenderEngine/RenderThread/RenderThread.h"

#include "World.h"

#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

StaticMeshComponent::StaticMeshComponent(std::string mesh, Material* mat) : _pathMesh(mesh), 
	_mesh(nullptr), _material(mat) {

}

StaticMeshComponent::~StaticMeshComponent() {

	StaticMesh* staticMesh = _staticMesh;
	Material* material = _material;

	std::function<void()> cleanup = [staticMesh, material]() {
		delete staticMesh;
		delete material;
	};

	RenderThread::GetInstance().RemoveObject(_staticMesh, cleanup);

	delete _mesh;
}


void StaticMeshComponent::Initialize() {
	_mesh = Mesh::LoadMesh(_pathMesh, 1);

	if (_material == nullptr) {
		World* world = _owner->GetWorld();
		_material = GetBasicColorMaterial(world->GetCamera(), glm::vec4(1, 1, 1, 1),
			world->GetLights().lights, world->GetLights().ambient, RenderThread::GetInstance().GetRenderPass());
	}

	_staticMesh = new StaticMesh(_mesh->vertices, _mesh->indices, _material);
	_staticMesh->Initialize();

	RenderThread::GetInstance().AddObject(_staticMesh);
}


void StaticMeshComponent::Start() {

}

void StaticMeshComponent::Update(float deltaTime) {
	MVP& obj = _staticMesh->GetMVP();
	obj.proj = _owner->GetWorld()->GetCamera().GetProjection();
	obj.view = _owner->GetWorld()->GetCamera().GetView();

	obj.model = glm::mat4(1);

	Transform trans = _owner->transform;
	glm::mat4 rot = glm::rotate(glm::mat4(1), glm::radians(trans.rotation.x), glm::vec3(1,0,0)) *
		glm::rotate(glm::mat4(1), glm::radians(trans.rotation.y), glm::vec3(0, 1, 0)) *
		glm::rotate(glm::mat4(1), glm::radians(trans.rotation.z), glm::vec3(0, 0, 1));

	obj.model = glm::translate(glm::mat4(1), trans.position)*rot*glm::scale(glm::mat4(1),trans.scale);

	_staticMesh->Update(deltaTime);
}

void StaticMeshComponent::End() {

}

void StaticMeshComponent::SetVisibility(bool activate) {
	if (activate) {
		RenderThread::GetInstance().AddObject(_staticMesh);
	}
	else {
		RenderThread::GetInstance().RemoveObject(_staticMesh);
	}
}