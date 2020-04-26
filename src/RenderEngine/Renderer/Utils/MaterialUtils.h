#pragma once
#include "RenderEngine/Renderer/Material.h"
#include "RenderEngine/Renderer/Shader.h"
#include "RenderEngine/Renderer/Uniform.h"
#include "RenderEngine/Renderer/Camera.h"
#include "RenderEngine/Renderer/Vertex.h"
#include "RenderEngine/Renderer/MaterialInstance.h"

MaterialInstance* GetBasicLightMaterial(const Camera& cam, const std::string tex);


MaterialInstance* GetBasicColorMaterial(const Camera& cam, glm::vec4 color);