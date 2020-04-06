#pragma once
#include "Component.h"

#include <string>


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
	StaticMeshComponent(std::string mesh, Material* mat = nullptr);

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

};

