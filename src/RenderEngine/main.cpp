#include <iostream>

#include "Renderer/SingleThreadRenderer/SingleThreadRenderer.h"
#include <GLFW/glfw3.h>
#include "Renderer/GraphicsPipeline.h"
#include "Renderer/Shader.h"
#include "Renderer/StaticMesh.h"
#include "Renderer/Image.h"
#include "Renderer/Uniform.h"
#include "Renderer/RenderPass.h"
#include "Renderer/Material.h"
#include "Renderer/Utils/MaterialUtils.h"
#include "Renderer/Mesh.h"

#include "Input/InputManager.h"

#include "Engine/World.h"
#include "Engine/GameObject.h"
#include "Engine/StaticMeshComponent.h"

const std::vector<VertexNormal> vertices = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f,0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f,0.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f,1.0f}},
    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f,1.0f}},
    {{-0.5f, -0.5f, 1.0f}, {1.0f, 1.0f, 0.0f}, {0.0f,0.0f}},
    {{0.5f, -0.5f, 1.0f}, {0.0f, 1.0f, 1.0f}, {1.0f,0.0f}},
    {{0.5f, 0.5f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f,1.0f}},
    {{-0.5f, 0.5f, 1.0f}, {0.5f, 0.5f, 0.5f}, {0.0f,1.0f}}
};

const std::vector<Index> indices = {
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

const std::vector<VertexNormal> verticesNormal1 = {
    {{-0.5f, -0.5f, 1.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {0.0f,0.0f,1.0f}},
    {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f,0.0f,1.0f}},
    {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f,0.0f,1.0f}},
    {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f,0.0f,1.0f}}
};

const std::vector<Index> indices1 = {
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

const float executionTime = 30.0f;

using namespace Input;

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

    /* Try input */
#if 0
    bool end = false;
    KeyboardCallback quit = [&end](CallbackAction ca) {
        std::cout << "Q PULSADA" << std::endl;
        end = true;
    };

    KeyboardCallback mant = [](CallbackAction ca) {
        std::cout << "TECLA MANTENIDA" << std::endl;
    };

    MousePositionCallback mouse = [](double x, double y) {
        std::cout << x << " " << y << std::endl;
    };

    MouseButtonCallback mouseButton = [](CallbackAction ca) {
        std::cout << "LEFT BUTTON MOUSE PRESSED" << std::endl;
    };

    MouseButtonCallback mouseButtonR = [](CallbackAction ca) {
        std::cout << "RIGHT BUTTON MOUSE PRESSED" << std::endl;
    };

    InputManager::GetInstance().SetWindow(VulkanInstance::GetInstance().window);

    InputManager::GetInstance().RegisterKeyboardCallback(GLFW_KEY_Q, CallbackAction::KEY_PRESSED, quit);

    InputManager::GetInstance().RegisterKeyboardCallback(GLFW_KEY_P, CallbackAction::KEY_REPEATED, mant);

    InputManager::GetInstance().RegisterMousePositionCallback(mouse);

    InputManager::GetInstance().RegisterMouseButtonCallback(GLFW_MOUSE_BUTTON_LEFT, CallbackAction::KEY_PRESSED, mouseButton);


    InputManager::GetInstance().RegisterMouseButtonCallback(GLFW_MOUSE_BUTTON_RIGHT, CallbackAction::KEY_RELEASED, mouseButtonR);

	RenderThread& rt = RenderThread::GetInstance();

    rt.InitializeSwapChain();

    Camera cam = Camera(glm::vec3(2, -35, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 90, 16 / 9.0f, 0.1f, 100.0f);

    VkFormat depthFormat = Image::FindSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

    Image* depthImage = Image::CreateImage(rt.GetExtent().width, rt.GetExtent().height, depthFormat, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT);

    RenderPass* renderPass = new RenderPass();
    renderPass->Initialize(rt.GetFormat(), depthImage);

    rt.Initialize(renderPass);
    
    /*
    std::vector<UniformInfo*> vertexDescriptor(0);
    MVP aux;
    aux.proj = cam.GetProjection();
    aux.view = cam.GetView();
    UniformInfo* vertexInfo = UniformInfo::GenerateInfo(aux, "MVP", 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
    vertexDescriptor.push_back(vertexInfo);

    UniformColor auxColor;
    auxColor.color = glm::vec4(0, 1, 1, 1);
    UniformInfo* vertexInfo2 = UniformInfo::GenerateInfo(auxColor, "Color", 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
    vertexDescriptor.push_back(vertexInfo2);

    std::vector<UniformInfo*> fragmentDescriptor(0);
    Material* mat = new Material();
    mat->Initialize("../resources/shaders/SolidColor.vert.spv", vertexDescriptor,
        "../resources/shaders/SolidColor.frag.spv", fragmentDescriptor,
        renderPass,
        Vertex::getBindingDescription(),
        Vertex::getAttributeDescriptions());

    */

    PointLight light;
    light.color = glm::vec3(1, 1, 1);
    light.position = glm::vec3(15, -20, 15.0f);
    light.power = 500;

    PointLight light2;
    light2.color = glm::vec3(0, 0, 1);
    light2.position = glm::vec3(-20, -10, 10);
    light2.power = 500;

    Lights lights;
    lights.lights[0] = light;
    lights.lights[1] = light2;

    lights.num_lights = 2;

    AmbientLight ambient = { {1,1,1}, .1f };

    Texture* tex = new Texture();
    tex->Initialize("../resources/textures/Building_Cinema.png");
    Material* mat = GetBasicLightMaterial(cam, tex, lights, ambient, renderPass);
    //Material* mat = GetBasicColorMaterial(cam, glm::vec4(1, 0, 1, 1), lights, ambient, renderPass);


    Mesh* chalet = Mesh::LoadMesh("../resources/objs/Building_Cinema.obj",1);
    //StaticMesh* mesh = new StaticMesh(vertices, indices, mat);
    StaticMesh* mesh1 = new StaticMesh(chalet->vertices, chalet->indices, mat);

    // mesh->SetCamera(cam);
    mesh1->SetCamera(cam);

    //mesh->Initialize();
    mesh1->Initialize();

   // rt.AddObject(mesh);
    rt.AddObject(mesh1);
    
    rt.UpdateCommandBuffers();

    rt.StartThread();

    auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();


    auto endFrame = std::chrono::high_resolution_clock::now();
    float time = 0;
    int logicTicks = 0;

    //mesh1->Translate({ 0, 2, 0 });

    mesh1->Rotate(-90, { 1,0,0 });

    mesh1->Rotate(180, { 0,0,1 });

    float realTimeExecuted = 0;

    auto realStartTime = std::chrono::high_resolution_clock::now();
    while (!end) {
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(endFrame-currentTime).count();
        currentTime = std::chrono::high_resolution_clock::now();
        glfwPollEvents();

        //mesh->Update(deltaTime);
        mesh1->Update(deltaTime);

        if (InputManager::GetInstance().IsKeyPressed(GLFW_KEY_K)) {
            std::cout << "K IS BEING PRESSED" << std::endl;
        }

        logicTicks++;
        endFrame = std::chrono::high_resolution_clock::now();
    }


    realTimeExecuted = std::chrono::duration<float, std::chrono::seconds::period>(
        std::chrono::high_resolution_clock::now() - startTime).count();

    std::cout << "Time in execution: " << realTimeExecuted << std::endl;
    std::cout << "Frames: " << rt.frames << std::endl;
    std::cout << "FPS: " << rt.frames / realTimeExecuted << std::endl;
    std::cout << "Logic Ticks: " << logicTicks << std::endl;
    std::cout << "Logic Ticks per second: " << logicTicks / realTimeExecuted << std::endl;

    rt.StopThread();

    delete depthImage;
    delete chalet;
    delete mesh1;
    delete mat;
    delete tex;
    delete renderPass;

    rt.CleanUp();

    VulkanInstance::GetInstance().CleanUp();
#endif

    /*
        Initialize render thread
    */
    SingleThreadRenderer& renderer = SingleThreadRenderer::GetInstance();

    renderer.InitializeSwapChain();


    RenderPass* renderPass = new RenderPass();
    renderPass->Initialize();

    renderer.Initialize(renderPass);

    /*
        Keyboard Callbacks
    */
    bool end = false;
    KeyboardCallback quit = [&end](CallbackAction ca) {
        std::cout << "Q PULSADA" << std::endl;
        end = true;
    };

    InputManager::GetInstance().SetWindow(VulkanInstance::GetInstance().window);

    InputManager::GetInstance().RegisterKeyboardCallback(GLFW_KEY_Q, CallbackAction::KEY_PRESSED, quit);

    /*
        Initialize world
    */
    PointLight light;
    light.color = glm::vec3(1, 1, 1);
    light.position = glm::vec3(15, -20, 15.0f);
    light.power = 500;

    PointLight light2;
    light2.color = glm::vec3(0, 0, 1);
    light2.position = glm::vec3(-20, -10, 10);
    light2.power = 500;

    Lights lights;
    lights.lights[0] = light;
    lights.lights[1] = light2;

    lights.num_lights = 2;

    AmbientLight ambient = { {1,1,1}, .1f };

    SceneLight l = { lights, ambient };
    World world;

    world.SetLights(l);

    Camera cam = Camera(glm::vec3(2, -35, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), 
        glm::vec3(0.0f, 0.0f, 1.0f), 90, 16 / 9.0f, 0.1f, 1000.0f);

    world.SetCamera(cam);


    world.Initialize();

    world.Start();

    float realTimeExecuted = 0;

    auto realStartTime = std::chrono::high_resolution_clock::now();

    auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();


    auto endFrame = std::chrono::high_resolution_clock::now();
    int logicTicks = 0;

    glm::vec3 position(0);

    float vel = 40;

    bool add = false;

    std::vector<GameObject*> objs = std::vector<GameObject*>(5, nullptr);
    
    int keys[5] = { GLFW_KEY_W, GLFW_KEY_E, GLFW_KEY_R, GLFW_KEY_T, GLFW_KEY_Y };

    glm::vec4 colors[5] = { {1,1,1,1}, {1,0,0,1}, {0,1,0,1}, {0,0,1,1}, {1,0,1,1} };

    bool show[5] = { false, false, false, false, false };

    KeyboardCallback callbacks[5];
    /*
    for (int i = 0; i < 5; i++) {
        // create
        objs[i] = new GameObject();

        StaticMeshComponent* mesh = new StaticMeshComponent("../resources/objs/Cube.obj", colors[i]);

        objs[i]->AddComponent(mesh);

        objs[i]->transform.scale = glm::vec3(10, 10, 10);

        objs[i]->transform.position = glm::vec3(-40 + i * 20, 0, 0);
        world.AddObject(objs[i]);

        mesh->Initialize();
        callbacks[i] = [&world, &objs, i, &colors, mesh, &show](CallbackAction ca) {
            mesh->SetVisibility(show[i]);
            show[i] = !show[i];
        };
        InputManager::GetInstance().RegisterKeyboardCallback(keys[i], CallbackAction::KEY_PRESSED, callbacks[i]);
    }
    */

    for (int i = 0; i < 5; i++) {
        // create
        callbacks[i] = [&world, &objs, i, &colors, &show](CallbackAction ca) {
            if (objs[i] == nullptr) {
                objs[i] = new GameObject();

                StaticMeshComponent* mesh = new StaticMeshComponent("../resources/objs/Cube.obj", colors[i]);

                objs[i]->AddComponent(mesh);

                objs[i]->transform.scale = glm::vec3(10, 10, 10);

                objs[i]->transform.position = glm::vec3(-40 + i * 20, 0, 0);
                world.AddObject(objs[i]);

                mesh->Initialize();
            }
            else {
                world.RemoveObject(objs[i]);
                delete objs[i];
                objs[i] = nullptr;
            }
        };
        InputManager::GetInstance().RegisterKeyboardCallback(keys[i], CallbackAction::KEY_PRESSED, callbacks[i]);
    }

    int i = 0;
    while (!end) {
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(endFrame - currentTime).count();

        currentTime = std::chrono::high_resolution_clock::now();
        glfwPollEvents();

        world.Update(deltaTime);
        //callbacks[i](CallbackAction::KEY_PRESSED);

        i = (i + 1) % 5;
        logicTicks++;

        renderer.DrawFrame();
        endFrame = std::chrono::high_resolution_clock::now();
    }

    world.End();

    realTimeExecuted = std::chrono::duration<float, std::chrono::seconds::period>(
        std::chrono::high_resolution_clock::now() - startTime).count();

    std::cout << "Time in execution: " << realTimeExecuted << std::endl;
    std::cout << "Frames: " << renderer.frames << std::endl;
    std::cout << "FPS: " << renderer.frames / realTimeExecuted << std::endl;
    std::cout << "Logic Ticks: " << logicTicks << std::endl;
    std::cout << "Logic Ticks per second: " << logicTicks / realTimeExecuted << std::endl;

    //rt.StopThread();

   

    for (auto& entry : objs) {
        delete entry;
    }


    renderer.CleanUp();

    delete renderPass;

    VulkanInstance::GetInstance().CleanUp();
}
