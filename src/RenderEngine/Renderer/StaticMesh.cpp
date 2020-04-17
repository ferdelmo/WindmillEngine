#include "StaticMesh.h"

#include "../VulkanInstance.h"
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "GraphicsPipeline.h"
#include "DescriptorSetLayout.h"
#include "Material.h"

#include "Engine/World.h"

#include <iostream>

StaticMesh::StaticMesh(std::string path, Material* mat)
	: _mesh(MeshManager::GetInstance().LoadMesh(path)), _material(mat)  {
	_ubo.model = glm::mat4(1.0f);

	_uniforms = _material->GenerateDescriptorSet(_descriptorPool, _descriptorSet);
}

StaticMesh::~StaticMesh() {
	MeshManager::GetInstance().UnloadMesh(_mesh->path);
	for (auto entry : _uniforms) {
		delete entry.second;
	}

	vkDestroyDescriptorPool(VulkanInstance::GetInstance().device, _descriptorPool, nullptr);
}

void StaticMesh::Update(float deltaTime) {
	std::vector<MVP> uniform = { _ubo };
	_uniforms.at("MVP")->Fill(uniform);

	World* world = World::GetActiveWorld();

	if (world != nullptr) {
		std::vector<AmbientLight> ambient = { world->GetLights().ambient };
		_uniforms.at("AmbientLight")->Fill(ambient);

		Lights l = world->GetLights().lights;

		for (int i = 0; i < l.num_lights; i++) {
			l.lights[i].LightPosition_cameraspace = (_ubo.view * glm::vec4(l.lights[i].position, 1));
		}
		std::vector<Lights> lights = { l };
		_uniforms.at("Lights")->Fill(lights);
	}


}

void StaticMesh::Rotate(float angle, glm::vec3 up) {
	float actualRot = angle;
	_ubo.model = glm::rotate(_ubo.model, glm::radians(actualRot), up);

	std::vector<MVP> uniform = { _ubo };
	_uniforms.at("MVP")->Fill(uniform);
}

void StaticMesh::Translate(glm::vec3 trans) {
	_ubo.model = glm::translate(_ubo.model, trans);
	std::vector<MVP> uniform = { _ubo };
	_uniforms.at("MVP")->Fill(uniform);
}

void StaticMesh::SetCamera(const Camera& cam) {
	_ubo.view = cam.GetView();
	_ubo.proj = cam.GetProjection();
}

VkBuffer& StaticMesh::GetVertexBuffer() {
	return _mesh->vertex->GetBuffer();
}

VkBuffer& StaticMesh::GetIndexBuffer() {
	return _mesh->index->GetBuffer();
}
/*
VkBuffer& StaticMesh::GetUniformBuffer() {
	return _uniformBuffer.GetBuffer();
}
*/

void StaticMesh::BindCommandsToBuffer(VkCommandBuffer& cmd) {
	VkDeviceSize offsets[] = { 0 };


	vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _material->GetPipeline().GetPipeline());

	vkCmdBindVertexBuffers(cmd, 0, 1, &_mesh->vertex->GetBuffer(), offsets);

	vkCmdBindIndexBuffer(cmd, _mesh->index->GetBuffer(), 0, VK_INDEX_TYPE_UINT32);

	vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _material->GetPipeline().GetPipelineLayout(), 0, 1, &_descriptorSet, 0, nullptr);

	vkCmdDrawIndexed(cmd, static_cast<uint32_t>(_mesh->mesh->indices.size()), 1, 0, 0, 0);

	//std::cout << "USING VERTEX: " << _vertexBuffer.GetBuffer() << std::endl;
	//std::cout << "USING INDEX: " << _indexBuffer.GetBuffer() << std::endl;
}

std::vector<Index> StaticMesh::GetIndices() {
	return _mesh->mesh->indices;
}

std::vector<VertexNormal> StaticMesh::GetVertices() {
	return _mesh->mesh->vertices;
}

MVP& StaticMesh::GetMVP() {
	return _ubo;
}