#include <iostream>
#include <random>

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


#include "Game/Colliders/SphereCollider.h"
#include "Game/Colliders/CapsuleCollider.h"
#include "Game/Managers/PhysicsManager.h"

#include "Game/GameObjects/FirstPersonPlayer.h"

#include "Lua/LuaComponent.h"

#include "Lua/LuaGameobjectFunctions.h"

#define M_PI           3.14159265358979323846

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


    Mesh* chalet = Mesh::LoadMesh("../resources/objs/Building_Cinema.obj", 1);
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
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(endFrame - currentTime).count();
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

    Lights lights;

    lights.num_lights = 1;

    AmbientLight ambient = { {1,.8,.8}, .3f };

    SceneLight l = { lights, ambient };
    World world;

    world.SetLights(l);

    Camera cam = Camera(glm::vec3(2, -35, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f), 90, 16 / 9.0f, 0.1f, 800.0f);

    world.SetCamera(cam);


    world.Initialize();

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

    world.GetLights().lights.lights[0].color = glm::vec3(1, 1, 1);
    world.GetLights().lights.lights[0].power = 25;

    FirstPersonPlayer* fpp = new FirstPersonPlayer(world.GetLights().lights.lights[0]);

    LuaComponent* luaComp = new LuaComponent("../resources/lua/player.lua");

    CapsuleCollider* fppCollider = new CapsuleCollider(fpp->transform.position, 5.0f, 5.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    
    world.AddObject(fpp);

    fpp->AddComponent(luaComp);
    fpp->AddComponent(fppCollider);

    glfwSetInputMode(VulkanInstance::GetInstance().window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    fpp->transform.position = glm::vec3(0, 0, 0);

    Physics::PhysicsManager::GetInstance().AddPlayer(fppCollider);

   /* StaticMeshComponent* mesh = new StaticMeshComponent("../resources/objs/Cube.obj", glm::vec4(1, 0, 0, 1));

    GameObject* luaObject = new GameObject();

    world.AddObject(luaObject);

    luaObject->AddComponent(mesh);

    luaObject->transform.scale = glm::vec3(15);*/

    for (int i = 0; i < 0; i++) {
        StaticMeshComponent* skullMesh = new StaticMeshComponent("../resources/objs/Ball.obj", glm::vec4(0, 0, 1, 1));

        GameObject* skull = new GameObject();

        LuaComponent* luaEnemy = new LuaComponent("../resources/lua/enemy.lua");

        world.AddObject(skull);

        skull->AddComponent(luaEnemy);

        skull->AddComponent(skullMesh);

        skull->transform.position = glm::vec3(0.0f, i * 5.0f, 10.0f);
    }

    std::random_device rd;
    std::mt19937 gen = std::mt19937(rd());
    std::uniform_real_distribution<float> random = std::uniform_real_distribution<float>(0, 1);

    for (int i = 0; i < 1; ++i) {
        

        StaticMeshComponent* daggerMesh = new StaticMeshComponent("../resources/objs/Cube.obj", glm::vec4(0, 1, 0, 1));
        GameObject* dagger = new GameObject();
        LuaComponent* luaDagger = new LuaComponent("../resources/lua/dagger.lua");
        world.AddObject(dagger);
        dagger->AddComponent(luaDagger);
        dagger->AddComponent(daggerMesh);
        float a = random(gen) * 2 * M_PI;
        dagger->transform.position = glm::vec3(80.0 * cos(a), 80.0 * sin(a), 0.0f);
        dagger->transform.scale = glm::vec3(4.0f, 4.0f, 25.0f);
    }
   

    int i = 0;

    /*
        Add the floor
    */
    GameObject* floor = new GameObject();
    world.AddObject(floor);
    StaticMeshComponent* floorMesh = new StaticMeshComponent("../resources/objs/Cube.obj", glm::vec4(0.3, 0.3, 0.3, 1));
    floor->AddComponent(floorMesh);

    floor->transform.scale = glm::vec3(250, 250, 0.1);
    floor->transform.position = glm::vec3(0, 0, -5);


    world.Start();
    
    while (!end) {
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(endFrame - currentTime).count();

        currentTime = std::chrono::high_resolution_clock::now();
        glfwPollEvents();

        world.Update(deltaTime);

        i = (i + 1) % 5;
        logicTicks++;

        renderer.DrawFrame();
        Physics::PhysicsManager::GetInstance().UpdateCollisions();

        world.DeletePendingToRemove();
        endFrame = std::chrono::high_resolution_clock::now();
    }

    world.End();

    for (auto& obj : world.GetObjects()) {
        delete obj;
    }

    realTimeExecuted = std::chrono::duration<float, std::chrono::seconds::period>(
        std::chrono::high_resolution_clock::now() - startTime).count();

    std::cout << "Time in execution: " << realTimeExecuted << std::endl;
    std::cout << "Frames: " << renderer.frames << std::endl;
    std::cout << "FPS: " << renderer.frames / realTimeExecuted << std::endl;
    std::cout << "Logic Ticks: " << logicTicks << std::endl;
    std::cout << "Logic Ticks per second: " << logicTicks / realTimeExecuted << std::endl;

    //rt.StopThread();


    renderer.CleanUp();

    delete renderPass;

    VulkanInstance::GetInstance().CleanUp();
}
