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
    // inputs
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    // outputs
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            vertices.push_back({
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            });

            uvs.push_back({
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                });

            normals.push_back({
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]
                });
        }
    }

    ComputeTangentBasis(vertices, uvs, normals, tangents, bitangents);

    for(int i=0; i<vertices.size(); i++) {
        VertexNormalMapping vertex = {};

        vertex.pos = vertices[i];
        vertex.pos *= scale;

        vertex.texCoord = uvs[i];
            
        vertex.normal = normals[i];

        vertex.tangent = tangents[i];
        vertex.bitangent = bitangents[i];

        mesh->vertices.push_back(vertex);
        mesh->indices.push_back(mesh->indices.size());
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

void Mesh::ComputeTangentBasis(
    // inputs
    std::vector<glm::vec3>& vertices,
    std::vector<glm::vec2>& uvs,
    std::vector<glm::vec3>& normals,
    // outputs
    std::vector<glm::vec3>& tangents,
    std::vector<glm::vec3>& bitangents
) {
    for (int i = 0; i < vertices.size(); i += 3) {

        // Shortcuts for vertices
        glm::vec3& v0 = vertices[i + 0];
        glm::vec3& v1 = vertices[i + 1];
        glm::vec3& v2 = vertices[i + 2];

        // Shortcuts for UVs
        glm::vec2& uv0 = uvs[i + 0];
        glm::vec2& uv1 = uvs[i + 1];
        glm::vec2& uv2 = uvs[i + 2];

        // Edges of the triangle : position delta
        glm::vec3 deltaPos1 = v1 - v0;
        glm::vec3 deltaPos2 = v2 - v0;

        // UV delta
        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        // Set the same tangent for all three vertices of the triangle.
        // They will be merged later, in vboindexer.cpp
        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        // Same thing for bitangents
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    }
}