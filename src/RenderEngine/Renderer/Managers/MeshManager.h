#pragma once

#include <map>

#include "../Buffer.h"
#include "../Mesh.h"


/*
	Clas to share vertex buffer and index buffer of same model
*/
class MeshManager
{
public:
	struct MeshInfo {
		std::string path = "";
		Mesh* mesh = nullptr;
		Buffer* vertex = nullptr;
		Buffer* index = nullptr;
		int references = 0;
	};

private:

	static MeshManager* _instance;

	std::map<std::string, MeshInfo> _buffers;

	MeshManager();

public:
	~MeshManager();

	static MeshManager& GetInstance();

	static void CleanUp();

	MeshInfo* LoadMesh(std::string path);

	void UnloadMesh(std::string path);
};

