#pragma once
#include "RenderEngine/Renderer/Material.h"
#include "RenderEngine/Renderer/Shader.h"
#include "RenderEngine/Renderer/Uniform.h"
#include "RenderEngine/Renderer/Camera.h"
#include "RenderEngine/Renderer/Vertex.h"

Material* GetBasicLightMaterial(const Camera& cam, Texture* tex, const Lights& lights, const AmbientLight ambient,  RenderPass* renderPass) {
    std::vector<UniformInfo*> vertexDescriptor(0);
    MVP aux;
    aux.proj = cam.GetProjection();
    aux.view = cam.GetView();
    UniformInfo* vertexInfo = UniformInfo::GenerateInfo(aux, "MVP", 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
    vertexDescriptor.push_back(vertexInfo);


    std::vector<UniformInfo*> fragmentDescriptor(0);

    UniformInfo* fragment1 = UniformInfo::GenerateInfo(lights, "Lights", 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);
    fragmentDescriptor.push_back(fragment1);

    UniformInfo* texture = UniformInfo::GenerateInfoTexture(tex, "Texture", 2, VK_SHADER_STAGE_FRAGMENT_BIT);
    fragmentDescriptor.push_back(texture);

    UniformInfo* ambientUniform = UniformInfo::GenerateInfo(ambient, "AmbientLight", 3, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);
    fragmentDescriptor.push_back(ambientUniform);

    Material* mat = new Material();
    mat->Initialize("../resources/shaders/BasicLight.vert.spv", vertexDescriptor,
        "../resources/shaders/BasicLight.frag.spv", fragmentDescriptor,
        renderPass,
        VertexNormal::getBindingDescription(),
        VertexNormal::getAttributeDescriptions());
    return mat;
}