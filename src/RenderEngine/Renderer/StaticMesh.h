#pragma once
#include "Vertex.h"
#include <vector>
#include "Buffer.h"
#include "Camera.h"
#include "Texture.h"
#include "Renderizable.h"
#include "Uniform.h"
#include <map>

class VulkanInstance;
class GraphicsPipeline;
class Material;
/**
* Represent a static mesh with a list of vertex. Initially once initialized, the list of vertex
* should not change (TODO: fix that)
* TODO: More than one descriptor set, only change with the image
* @author Fernando del Molino
*/
class StaticMesh : public Renderizable
{
private:
	std::vector<Vertex> _vertices;
	Buffer _vertexBuffer;

	std::vector<Index> _indices;
	Buffer _indexBuffer;

	MVP _ubo;

	std::map<std::string, Buffer*> _uniforms;

	Material* _material;

	VkDescriptorPool _descriptorPool;
	VkDescriptorSet _descriptorSet;

	/*
		Create the descriptorPool
	*/
	void CreateDescriptorPool();

	/*
		Create the descriptor set
	*/
	void CreateDescriptoSet();

public:
	/*
		Constructor with a list of vertices and indices
	*/
	StaticMesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, Material* mat);

	~StaticMesh();

	/*
		Initialize the mesh -> Initialize the needed bufers and rest of vulkan things needed
	*/
	void Initialize();

	/*
		Update to call every frame
	*/
	void Update(float deltaTime);

	/*
		Set the active camera to change the MVP matrix
	*/
	void SetCamera(const Camera& cam);


	/*
		Get the vertex buffer of the mesh
	*/
	VkBuffer& GetVertexBuffer();

	/*
		Get the vertex buffer of the mesh
	*/
	VkBuffer& GetIndexBuffer();

	/*
		Get the uniform buffer
	*/
	VkBuffer& GetUniformBuffer();

	/*
		Bind to a command the draw orders
	*/
	void BindCommandsToBuffer(VkCommandBuffer& cmd) override;

	std::vector<Index> GetIndices();

	std::vector<Vertex> GetVertices();

	MVP& GetUniformObject();
};

