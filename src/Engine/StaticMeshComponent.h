#pragma once
#include "Component.h"

#include <string>

#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS


class Mesh;
class StaticMesh;
class MaterialInstance;

/*
	Componente to renderize an static mesh

	@author Fernando del Molino
*/
class StaticMeshComponent : public Component
{

public:
	StaticMeshComponent(std::string mesh, MaterialInstance* mat = nullptr);

	StaticMeshComponent(std::string mesh, glm::vec4 color);

	virtual ~StaticMeshComponent() override;

	virtual void Initialize() override;

	virtual void Start() override;

	virtual void Update(float deltaTime) override;

	virtual void End() override;

	void SetVisibility(bool activate);

private:

	std::string _pathMesh;
	Mesh* _mesh;

	StaticMesh* _staticMesh;
	MaterialInstance* _material;

	glm::vec4 color;

};

