#pragma once
#include "RenderEngine/Renderer/Material.h"
#include "RenderEngine/Renderer/Shader.h"
#include "RenderEngine/Renderer/Uniform.h"
#include "RenderEngine/Renderer/Camera.h"
#include "RenderEngine/Renderer/Vertex.h"

Material* GetBasicLightMaterial(const Camera& cam, Texture* tex, const Lights& lights, const AmbientLight ambient,
    RenderPass* renderPass);


Material* GetBasicColorMaterial(const Camera& cam, glm::vec4 color, const Lights& lights, const AmbientLight ambient,
    RenderPass* renderPass);