#pragma once

#include <map>
#include <string>

class Material;


/*
	Material manager. used to only create pipeline and shader modules once per material,
	then the materialInstance will manage the descriptor sets.

	@author Fernando del Molino
*/
class MaterialManager
{
public:
	// maybe store number of references to clean the material at runtime when not used
	struct MaterialInfo {
		Material* material = nullptr;
	};
private:
	static MaterialManager* _instance;

	// maybe sabe 
	std::map<std::string, MaterialInfo> _materials;

	MaterialManager();

public:
	~MaterialManager();

	static MaterialManager& GetInstance();

	static void CleanUp();

	// return a material instance
	Material* GetMaterial(std::string name);

	// return a material instance
	void AddMaterial(std::string name, Material* material);

};

