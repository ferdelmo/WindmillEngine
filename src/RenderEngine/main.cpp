#include <iostream>

#include "RenderThread/RenderThread.h"
#include <GLFW/glfw3.h>
#include "Renderer/GraphicsPipeline.h"
#include "Renderer/Shader.h"
#include "Renderer/StaticMesh.h"
#include "Renderer/Image.h"
#include "Renderer/Uniform.h"
#include "Renderer/RenderPass.h"
#include "Renderer/Material.h"


const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f,0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f,0.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f,1.0f}},
    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f,1.0f}},
    {{-0.5f, -0.5f, 1.0f}, {1.0f, 1.0f, 0.0f}, {0.0f,0.0f}},
    {{0.5f, -0.5f, 1.0f}, {0.0f, 1.0f, 1.0f}, {1.0f,0.0f}},
    {{0.5f, 0.5f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f,1.0f}},
    {{-0.5f, 0.5f, 1.0f}, {0.5f, 0.5f, 0.5f}, {0.0f,1.0f}}
};

const std::vector<uint16_t> indices = {
    0, 2, 1, 3, 2, 0, 4, 5, 6, 6, 7, 4,
    0, 4, 3, 4, 7, 3, 3, 7, 2, 7, 6, 2,
    6, 5, 2, 5, 1, 2, 5, 4, 1, 4, 0, 1
};

const std::vector<Vertex> vertices1 = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}
};

const std::vector<uint16_t> indices1 = {
    0, 1, 2, 2, 3, 0
};
/*
const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
    {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
    {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
    {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}
};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
};*/

int main() {
	/*
	VulkanApplication app;

	try {
		app.run();
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
	*/

	RenderThread& rt = RenderThread::GetInstance();

    rt.InitializeSwapChain();

    Camera cam = Camera(glm::vec3(-2.0f, -2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 60, 16 / 9.0f, 0.1f, 100.0f);

    VkFormat depthFormat = Image::FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

    Image* depthImage = Image::CreateImage(rt.GetExtent().width, rt.GetExtent().height, depthFormat, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT);

    RenderPass* renderPass = new RenderPass();
    renderPass->Initialize(rt.GetFormat(), depthImage);

    rt.Initialize(renderPass);

    std::vector<UniformInfo*> vertexDescriptor(0);
    MVP aux;
    aux.proj = cam.GetProjection();
    aux.view = cam.GetView();
    UniformInfo* vertexInfo = UniformInfo::GenerateInfo(aux, "MVP", 1, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
    vertexDescriptor.push_back(vertexInfo);

    UniformColor auxColor;
    UniformInfo* vertexInfo2 = UniformInfo::GenerateInfo(auxColor, "Color", 0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
    vertexDescriptor.push_back(vertexInfo2);

    std::vector<UniformInfo*> fragmentDescriptor(0);

    //Texture tex;
    //tex.Initialize("../resources/textures/texture.jpg");
    //UniformInfo* fragmentInfo = UniformInfo::GenerateInfoTexture(&tex, "Texture", 1, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
    //fragmentDescriptor.push_back(fragmentInfo);

    Material* mat = new Material();
    mat->Initialize("../resources/shaders/SolidColor.vert.spv", vertexDescriptor,
        "../resources/shaders/SolidColor.frag.spv", fragmentDescriptor,
        renderPass,
        Vertex::getBindingDescription(),
        Vertex::getAttributeDescriptions());

    //StaticMesh* mesh = new StaticMesh(vertices, indices, mat);
    StaticMesh* mesh1 = new StaticMesh(vertices1, indices1, mat);

    //mesh->SetCamera(cam);
    mesh1->SetCamera(cam);

    //mesh->Initialize();
    mesh1->Initialize();

    //rt.AddObject(mesh);
    rt.AddObject(mesh1);
    
    rt.UpdateCommandBuffers();

    rt.StartThread();

    auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();


    auto endFrame = std::chrono::high_resolution_clock::now();
    float time = 0;
    int logicTicks = 0;
    while (time < 5.0f) {
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(endFrame-currentTime).count();
        currentTime = std::chrono::high_resolution_clock::now();
        mesh1->Update(deltaTime);
        logicTicks++;
        endFrame = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    }
    std::cout << "Frames: " << rt.frames << std::endl;
    std::cout << "FPS: " << rt.frames / 5.0f << std::endl;
    std::cout << "Logic Ticks: " << logicTicks << std::endl;
    std::cout << "Logic Ticks per second: " << logicTicks / 5.0f << std::endl;

    rt.StopThread();
}
