#pragma once
#include "Component.h"

#include <string>
#include <map>

#include <glm/glm.hpp>
#define GLM_FORCE_RADIANS


class Mesh;
class StaticMesh;
class Material;

/*
	Componente to renderize an static mesh

	@author Fernando del Molino
*/
class StaticMeshComponent : public Component
{

public:

	static std::map<std::string, Mesh*> meshes;

	StaticMeshComponent(std::string mesh, Material* mat = nullptr);

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
	Material* _material;

	glm::vec4 color;

};

