#include "MaterialUtils.h"
#include "RenderEngine/Renderer/Managers/MaterialManager.h"
#include "RenderEngine/Renderer/MaterialInstance.h"
#include "RenderEngine/Renderer/SingleThreadRenderer/SingleThreadRenderer.h"
#include "RenderEngine/Renderer/Managers/TextureManager.h"

#include "Engine/World.h"

MaterialInstance* GetBasicLightMaterial(const Camera& cam, const std::string tex) {
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

        UniformInfo* fragment1 = UniformInfo::GenerateInfo(World::GetActiveWorld()->GetLights().lights, "Lights", 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);
        fragmentDescriptor.push_back(fragment1);

        UniformInfo* texture = UniformInfo::GenerateInfoTexture(TextureManager::GetInstance().LoadTexture(tex),
            "Texture", 2, VK_SHADER_STAGE_FRAGMENT_BIT);

        fragmentDescriptor.push_back(texture);

        UniformInfo* ambientUniform = UniformInfo::GenerateInfo(World::GetActiveWorld()->GetLights().ambient, "AmbientLight", 3, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);
        fragmentDescriptor.push_back(ambientUniform);

        Material* resul = new Material(materialName);
        resul->Initialize("../resources/shaders/BasicLight.vert.spv", vertexDescriptor,
            "../resources/shaders/BasicLight.frag.spv", fragmentDescriptor,
            SingleThreadRenderer::GetInstance().GetRenderPass(),
            VertexNormal::getBindingDescription(),
            VertexNormal::getAttributeDescriptions());

        MaterialManager::GetInstance().AddMaterial(materialName, resul);
    }
    
    MaterialInstance* aux = new MaterialInstance(materialName);
    aux->SetValue<AmbientLight>("AmbientLight", World::GetActiveWorld()->GetLights().ambient);
    aux->SetValue<Lights>("Lights", World::GetActiveWorld()->GetLights().lights);
    aux->SetTexture("Texture", tex);

    return aux;
}


MaterialInstance* GetBasicColorMaterial(const Camera& cam, const PhongShading& phong) {
    std::string materialName = "BasicColorMaterial";
    Material* resul = MaterialManager::GetInstance().GetMaterial(materialName);

    if (resul == nullptr) {
        std::vector<UniformInfo*> vertexDescriptor(0);
        MVP aux;
        aux.proj = cam.GetProjection();
        aux.view = cam.GetView();
        UniformInfo* vertexInfo = UniformInfo::GenerateInfo(aux, "MVP", 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
        vertexDescriptor.push_back(vertexInfo);


        std::vector<UniformInfo*> fragmentDescriptor(0);

        UniformInfo* fragment1 = UniformInfo::GenerateInfo(World::GetActiveWorld()->GetLights().lights, 
            "Lights", 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);
        fragmentDescriptor.push_back(fragment1);


        UniformInfo* ambientUniform = UniformInfo::GenerateInfo(World::GetActiveWorld()->GetLights().ambient,
            "AmbientLight", 2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);
        fragmentDescriptor.push_back(ambientUniform);

        UniformInfo* phongUniform = UniformInfo::GenerateInfo(phong, "Phong",
            3, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);
        fragmentDescriptor.push_back(phongUniform);

        Material* resul = new Material(materialName);
        resul->Initialize("../resources/shaders/SolidColor.vert.spv", vertexDescriptor,
            "../resources/shaders/SolidColor.frag.spv", fragmentDescriptor,
            SingleThreadRenderer::GetInstance().GetRenderPass(),
            VertexNormalMapping::getBindingDescription(),
            VertexNormalMapping::getAttributeDescriptions());

        MaterialManager::GetInstance().AddMaterial(materialName, resul);
    }

    MaterialInstance* aux = new MaterialInstance(materialName);
    aux->SetValue("AmbientLight", World::GetActiveWorld()->GetLights().ambient);
    aux->SetValue("Lights", World::GetActiveWorld()->GetLights().lights);
    aux->SetValue("Phong", phong);

    return aux;
}


MaterialInstance* GetBasicLightMaterialNormalMapping(const Camera& cam, const std::string tex,
    const std::string normal, const PhongShading& phong) {
        std::string materialName = "BasicLightNormalMappingMaterial";
        Material* resul = MaterialManager::GetInstance().GetMaterial(materialName);
        if (resul == nullptr) {
            std::vector<UniformInfo*> vertexDescriptor(0);
            MVP aux;
            aux.proj = cam.GetProjection();
            aux.view = cam.GetView();
            UniformInfo* vertexInfo = UniformInfo::GenerateInfo(aux, "MVP", 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
            vertexDescriptor.push_back(vertexInfo);


            std::vector<UniformInfo*> fragmentDescriptor(0);

            UniformInfo* fragment1 = UniformInfo::GenerateInfo(World::GetActiveWorld()->GetLights().lights, "Lights", 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);
            fragmentDescriptor.push_back(fragment1);



            UniformInfo* ambientUniform = UniformInfo::GenerateInfo(World::GetActiveWorld()->GetLights().ambient, "AmbientLight", 
                2, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);
            fragmentDescriptor.push_back(ambientUniform);

            UniformInfo* texture = UniformInfo::GenerateInfoTexture(TextureManager::GetInstance().LoadTexture(tex),
                "Texture", 3, VK_SHADER_STAGE_FRAGMENT_BIT);
            fragmentDescriptor.push_back(texture);

            UniformInfo* normalTex = UniformInfo::GenerateInfoTexture(TextureManager::GetInstance().LoadTexture(normal),
                "NormalTexture", 4, VK_SHADER_STAGE_FRAGMENT_BIT);
            fragmentDescriptor.push_back(normalTex);

            UniformInfo* phongUniform = UniformInfo::GenerateInfo(phong, "Phong",
                5, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_FRAGMENT_BIT);
            fragmentDescriptor.push_back(phongUniform);

            Material* resul = new Material(materialName);
            resul->Initialize("../resources/shaders/BasicLightNormalMapping.vert.spv", vertexDescriptor,
                "../resources/shaders/BasicLightNormalMapping.frag.spv", fragmentDescriptor,
                SingleThreadRenderer::GetInstance().GetRenderPass(),
                VertexNormalMapping::getBindingDescription(),
                VertexNormalMapping::getAttributeDescriptions());

            MaterialManager::GetInstance().AddMaterial(materialName, resul);
        }

        MaterialInstance* aux = new MaterialInstance(materialName);
        aux->SetValue("AmbientLight", World::GetActiveWorld()->GetLights().ambient);
        aux->SetValue("Lights", World::GetActiveWorld()->GetLights().lights);
        aux->SetTexture("Texture", tex);
        aux->SetTexture("NormalTexture", normal);
        aux->SetValue("Phong", phong);

        return aux;
    }