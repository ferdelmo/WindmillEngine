#include "Mesh.h"
#include "tiny_obj_loader.h"
#include <exception>
#include <stdexcept>


Mesh::Mesh() {
    vertices = {};
    indices = {};
}

Mesh* Mesh::LoadMesh(std::string path, float scale) {
	Mesh* mesh = new Mesh();
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
        throw std::runtime_error(warn + err);
    }

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            VertexNormal vertex = {};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };
            vertex.pos *= scale;

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            /*vertex.normal = {
                attrib.normals[3 * index.vertex_index + 0],
                attrib.normals[3 * index.vertex_index + 1],
                attrib.normals[3 * index.vertex_index + 2]
            };*/
            vertex.normal = { 0,0,1 };

            vertex.color = { 1.0f, 1.0f, 1.0f };

            mesh->vertices.push_back(vertex);
            mesh->indices.push_back(mesh->indices.size());
        }
    }
    /*
    // calculate normals
    for (int i = 0; i < mesh->indices.size(); i+=3) {
        glm::vec3 first = mesh->vertices[mesh->indices[i]+1].pos-mesh->vertices[mesh->indices[i]+0].pos;

        glm::vec3 sec = mesh->vertices[mesh->indices[i] + 2].pos - mesh->vertices[mesh->indices[i] + 0].pos;
        
        mesh->vertices[mesh->indices[i] + 0].normal = glm::cross(first, sec);
        mesh->vertices[mesh->indices[i] + 1].normal = glm::cross(first, sec);
        mesh->vertices[mesh->indices[i] + 2].normal = glm::cross(first, sec);
    }
    */
	return mesh;
}