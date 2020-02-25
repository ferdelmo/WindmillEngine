#pragma once
#include "RenderEngine/Renderer/Material.h"
#include "RenderEngine/Renderer/Shader.h"
#include "RenderEngine/Renderer/Uniform.h"
#include "RenderEngine/Renderer/Camera.h"
#include "RenderEngine/Renderer/Vertex.h"

Material* GetBasicLightMaterial(const Camera& cam, std::string texturePath,  RenderPass* renderPass) {
    std::vector<UniformInfo*> vertexDescriptor(0);
    MVP aux;
    aux.proj = cam.GetProjection();
    aux.view = cam.GetView();
    UniformInfo* vertexInfo = UniformInfo::GenerateInfo(aux, "MVP", 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
    vertexDescriptor.push_back(vertexInfo);


    std::vector<UniformInfo*> fragmentDescriptor(0);
    PointLight light;
    light.color = glm::vec3(1, 1, 1);
    light.position = glm::vec3(15, -5, 20);
    light.power = 500;

    PointLight light2;
    light2.color = glm::vec3(1, 1, 1);
    light2.position = glm::vec3(-20, 0, 10);
    light2.power = 500;

    Lights lights;
    lights.lights[0] = light;
    //lights.lights[1] = light2;
    lights.num_lights = 1;

    UniformInfo* fragment1 = UniformInfo::GenerateInfo(lights, "Lights", 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);
    fragmentDescriptor.push_back(fragment1);

    Texture* tex = new Texture();
    tex->Initialize(texturePath);
    UniformInfo* texture = UniformInfo::GenerateInfoTexture(tex, "Texture", 2, VK_SHADER_STAGE_FRAGMENT_BIT);
    fragmentDescriptor.push_back(texture);

    Material* mat = new Material();
    mat->Initialize("../resources/shaders/BasicLight.vert.spv", vertexDescriptor,
        "../resources/shaders/BasicLight.frag.spv", fragmentDescriptor,
        renderPass,
        VertexNormal::getBindingDescription(),
        VertexNormal::getAttributeDescriptions());
    return mat;
}