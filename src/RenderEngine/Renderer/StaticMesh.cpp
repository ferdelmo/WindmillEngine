#include "StaticMesh.h"

#include "../VulkanInstance.h"
#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "GraphicsPipeline.h"
#include "DescriptorSetLayout.h"
#include "MaterialInstance.h"
#include "Material.h"

#include "Managers/MaterialManager.h"
#include "Managers/TextureManager.h"

#include "Engine/World.h"

#include <iostream>

/*
	Constructor with a list of vertices and indices
*/
StaticMesh::StaticMesh(std::string path, std::string materialName) 
	: _mesh(MeshManager::GetInstance().LoadMesh(path)) {

	_material = MaterialManager::GetInstance().GetMaterial(materialName);
	_materialInstance = nullptr;


	_uniforms = _material->GenerateDescriptorSet(_descriptorPool, _descriptorSet);
}

StaticMesh::StaticMesh(std::string path, MaterialInstance* mat)
	: _mesh(MeshManager::GetInstance().LoadMesh(path)), _materialInstance(mat), 
	_material(mat->GetMaterial()) {


	_uniforms = _material->GenerateDescriptorSet(_descriptorPool, _descriptorSet);
	
	for (auto& entry : mat->GetValues()) {
		if (entry.second->GetType() == UniformTypes::UNIFORM) {
			entry.second->Fill(_uniforms.at(entry.first)->GetBuffer());
		}
		else {
			//update the texture
			Texture* aux = TextureManager::GetInstance().LoadTexture(
				((TextureParameter*)entry.second)->pathTex);
			_uniforms.at(entry.first)->SetTexture(aux);
			//update the descriptor set
			_material->UpdateDescriptorSet(_descriptorSet, entry.first, aux);
		}
	}
}

StaticMesh::~StaticMesh() {
	MeshManager::GetInstance().UnloadMesh(_mesh->path);

	for (auto& entry : _uniforms) {
		delete entry.second;
	}

	vkDestroyDescriptorPool(VulkanInstance::GetInstance().device, _descriptorPool, nullptr);
}

void StaticMesh::Update(float deltaTime) {
	std::vector<MVP> uniform = { _ubo };
	_uniforms.at("MVP")->GetBuffer()->Fill(uniform);

	World* world = World::GetActiveWorld();

	if (world != nullptr) {
		std::vector<AmbientLight> ambient = { world->GetLights().ambient };
		_uniforms.at("AmbientLight")->GetBuffer()->Fill(ambient);

		Lights l = world->GetLights().lights;

		for (int i = 0; i < l.num_lights; i++) {
			l.lights[i].LightPosition_cameraspace = (_ubo.view * glm::vec4(l.lights[i].position, 1));
		}
		std::vector<Lights> lights = { l };
		_uniforms.at("Lights")->GetBuffer()->Fill(lights);
	}


}

void StaticMesh::Rotate(float angle, glm::vec3 up) {
	float actualRot = angle;
	_ubo.model = glm::rotate(_ubo.model, glm::radians(actualRot), up);

	std::vector<MVP> uniform = { _ubo };
	_uniforms.at("MVP")->GetBuffer()->Fill(uniform);
}

void StaticMesh::Translate(glm::vec3 trans) {
	_ubo.model = glm::translate(_ubo.model, trans);

	std::vector<MVP> uniform = { _ubo };
	_uniforms.at("MVP")->GetBuffer()->Fill(uniform);
}

void StaticMesh::SetCamera(const Camera& cam) {
	_ubo.view = cam.GetView();
	_ubo.proj = cam.GetProjection();
}

VkBuffer& StaticMesh::GetVertexBuffer() {
	return _mesh->vertex.GetBuffer();
}

VkBuffer& StaticMesh::GetIndexBuffer() {
	return _mesh->index.GetBuffer();
}
/*
VkBuffer& StaticMesh::GetUniformBuffer() {
	return _uniformBuffer.GetBuffer();
}
*/

void StaticMesh::BindCommandsToBuffer(VkCommandBuffer& cmd) {
	VkDeviceSize offsets[] = { 0 };


	vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _material->GetPipeline().GetPipeline());

	vkCmdBindVertexBuffers(cmd, 0, 1, &_mesh->vertex.GetBuffer(), offsets);

	vkCmdBindIndexBuffer(cmd, _mesh->index.GetBuffer(), 0, VK_INDEX_TYPE_UINT32);

	vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, 
		_material->GetPipeline().GetPipelineLayout(), 0, 1, &_descriptorSet, 0, nullptr);

	vkCmdDrawIndexed(cmd, static_cast<uint32_t>(_mesh->mesh->indices.size()), 1, 0, 0, 0);

	//std::cout << "USING VERTEX: " << _vertexBuffer.GetBuffer() << std::endl;
	//std::cout << "USING INDEX: " << _indexBuffer.GetBuffer() << std::endl;
}

std::vector<Index> StaticMesh::GetIndices() {
	return _mesh->mesh->indices;
}

std::vector<VertexNormalMapping> StaticMesh::GetVertices() {
	return _mesh->mesh->vertices;
}

MVP& StaticMesh::GetMVP() {
	return _ubo;
}

MaterialInstance* StaticMesh::GetMaterialInstance() {
	return _materialInstance;
}