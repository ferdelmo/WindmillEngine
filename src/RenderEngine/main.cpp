#include <iostream>

#include "VulkanApplication.h"
#include "RenderThread/RenderThread.h"
#include "Renderer/GraphicsPipeline.h"
#include "Renderer/Shader.h"
#include "Renderer/StaticMesh.h"

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

    GraphicsPipeline* pipeline = new GraphicsPipeline();
    Shader* vertex = new Shader();
    std::vector<DescriptorSetLayoutBinding> vertexDescriptor(0);
    vertexDescriptor.push_back(DescriptorSetLayoutBinding::GetUniform(0, 1, VK_SHADER_STAGE_VERTEX_BIT));
    vertex->Initialize("../resources/shaders/vert.spv", VK_SHADER_STAGE_VERTEX_BIT, vertexDescriptor);

    Shader* fragment = new Shader();
    std::vector<DescriptorSetLayoutBinding> fragmentDescriptor(0);
    fragmentDescriptor.push_back(DescriptorSetLayoutBinding::GetImageSampler(1, 1, VK_SHADER_STAGE_FRAGMENT_BIT));
    fragment->Initialize("../resources/shaders/frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT, fragmentDescriptor);


    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)rt.GetExtent().width;
    viewport.height = (float)rt.GetExtent().height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = {};
    scissor.offset = { 0, 0 };
    scissor.extent = rt.GetExtent();


    VkFormat depthFormat = Image::FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

    Image* depthImage = Image::CreateImage(rt.GetExtent().width, rt.GetExtent().height, depthFormat, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT);

    RenderPass* renderPass = new RenderPass();
    renderPass->Initialize(rt.GetFormat(), depthImage);

    rt.Initialize(renderPass);

    pipeline->Initialize(renderPass, vertex, fragment, rt.GetFormat(), viewport, scissor);

    Camera cam = Camera(glm::vec3(-2.0f, -2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 60, 16 / 9.0f, 0.1f, 100.0f);

    StaticMesh* mesh = new StaticMesh(vertices, indices, "../resources/textures/texture.jpg", pipeline);
    StaticMesh* mesh1 = new StaticMesh(vertices1, indices1, "../resources/textures/texture.jpg", pipeline);

    mesh->SetCamera(cam);
    mesh1->SetCamera(cam);

    mesh->Initialize();
    mesh1->Initialize();

    rt.AddObject(mesh);
    rt.AddObject(mesh1);
    
    rt.UpdateCommandBuffers();

    rt.StartThread();

    auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = 0;
    while (time < 10.0f) {
        currentTime = std::chrono::high_resolution_clock::now();
        time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    }

    std::cout << "Frames: " << rt.frames << std::endl;
    std::cout << "FPS: " << rt.frames / 5.0f << std::endl;

    rt.StopThread();

}
