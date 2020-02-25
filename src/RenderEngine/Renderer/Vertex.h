#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

typedef uint32_t Index;

/*
	Data passed to the Vertex shader
*/
struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	/*
		bind the data to the data vertex shader receive
	*/
	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		//VK_VERTEX_INPUT_RATE_INSTANCE -> move the data entry after each instance for instanced rendering
		return bindingDescription;
	}
	/*
		describes how to extrat a vertex attribute from a chunk of vertex data
	*/
	static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions = { {}, {}, {} };

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		/*
			FORMAT:
			float: VK_FORMAT_R32_SFLOAT
			vec2: VK_FORMAT_R32G32_SFLOAT
			vec3: VK_FORMAT_R32G32B32_SFLOAT
			vec4: VK_FORMAT_R32G32B32A32_SFLOAT
		*/

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);


		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		return attributeDescriptions;
	}
};


/*
	Data passed to the Vertex shader
*/
struct VertexNormal {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;
	glm::vec3 normal;

	/*
		bind the data to the data vertex shader receive
	*/
	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(VertexNormal);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		//VK_VERTEX_INPUT_RATE_INSTANCE -> move the data entry after each instance for instanced rendering
		return bindingDescription;
	}
	/*
		describes how to extrat a vertex attribute from a chunk of vertex data
	*/
	static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() {
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions = { {}, {}, {}, {} };

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(VertexNormal, pos);

		/*
			FORMAT:
			float: VK_FORMAT_R32_SFLOAT
			vec2: VK_FORMAT_R32G32_SFLOAT
			vec3: VK_FORMAT_R32G32B32_SFLOAT
			vec4: VK_FORMAT_R32G32B32A32_SFLOAT
		*/

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(VertexNormal, color);


		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(VertexNormal, texCoord);

		attributeDescriptions[3].binding = 0;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[3].offset = offsetof(VertexNormal, normal);

		return attributeDescriptions;
	}
};