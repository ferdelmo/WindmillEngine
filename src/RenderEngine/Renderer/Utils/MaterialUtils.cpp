#include "MaterialUtils.h"
#include "RenderEngine/Renderer/MaterialManager.h"
#include "RenderEngine/Renderer/MaterialInstance.h"

MaterialInstance* GetBasicLightMaterial(const Camera& cam, Texture* tex, const Lights& lights, const AmbientLight ambient,
    RenderPass* renderPass) {
    std::string materialName = "BasicLightMaterial";
    Material* resul = MaterialManager::GetInstance().GetMaterial(materialName);
    if (resul == nullptr) {
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

        Material* resul = new Material(materialName);
        resul->Initialize("../resources/shaders/BasicLight.vert.spv", vertexDescriptor,
            "../resources/shaders/BasicLight.frag.spv", fragmentDescriptor,
            renderPass,
            VertexNormal::getBindingDescription(),
            VertexNormal::getAttributeDescriptions());

        MaterialManager::GetInstance().AddMaterial(materialName, resul);
    }
    
    MaterialInstance* aux = new MaterialInstance(materialName);
    aux->SetValue<AmbientLight>("AmbientLight", ambient);
    aux->SetValue<Lights>("Lights", lights);
    aux->SetValue<Texture>("Texture", *tex);

    return aux;
}


MaterialInstance* GetBasicColorMaterial(const Camera& cam, glm::vec4 color, const Lights& lights, const AmbientLight ambient,
    RenderPass* renderPass) {
    std::string materialName = "BasicColorMaterial";
    Material* resul = MaterialManager::GetInstance().GetMaterial(materialName);

    UniformColor auxColor(color);
    if (resul == nullptr) {
        std::vector<UniformInfo*> vertexDescriptor(0);
        MVP aux;
        aux.proj = cam.GetProjection();
        aux.view = cam.GetView();
        UniformInfo* vertexInfo = UniformInfo::GenerateInfo(aux, "MVP", 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
        vertexDescriptor.push_back(vertexInfo);


        std::vector<UniformInfo*> fragmentDescriptor(0);

        UniformInfo* fragment1 = UniformInfo::GenerateInfo(lights, "Lights", 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);
        fragmentDescriptor.push_back(fragment1);


        UniformInfo* ambientUniform = UniformInfo::GenerateInfo(ambient, "AmbientLight", 2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);
        fragmentDescriptor.push_back(ambientUniform);

        UniformInfo* basicColor = UniformInfo::GenerateInfo(auxColor, "Color", 3, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);
        fragmentDescriptor.push_back(basicColor);

        Material* resul = new Material(materialName);
        resul->Initialize("../resources/shaders/SolidColor.vert.spv", vertexDescriptor,
            "../resources/shaders/SolidColor.frag.spv", fragmentDescriptor,
            renderPass,
            VertexNormal::getBindingDescription(),
            VertexNormal::getAttributeDescriptions());

        MaterialManager::GetInstance().AddMaterial(materialName, resul);
    }

    MaterialInstance* aux = new MaterialInstance(materialName);
    aux->SetValue<UniformColor>("Color", auxColor);

    return aux;
}