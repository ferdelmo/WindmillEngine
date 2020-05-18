#include "Mesh.h"
#include "tiny_obj_loader.h"
#include <exception>
#include <stdexcept>


bool FindSimilarVertex(
    // inputs
    std::vector<glm::vec3>& vertices,
    std::vector<glm::vec2>& uvs,
    std::vector<glm::vec3>& normals,

    glm::vec3 vertice,
    glm::vec2 uv,
    glm::vec3 normal,
    int& found) {

    bool foundIndex = false;
    return false;
    for (int i = 0; i < vertices.size() && !foundIndex; i++) {
        if (vertices[i] == vertice &&
            uvs[i] == uv &&
            normals[i] == normal) {
            foundIndex = true;
            found = i;
            break;
        }
    }

    return foundIndex;
}

Mesh::Mesh() {
    vertices = {};
    indices = {};
}

Mesh* Mesh::LoadMesh(std::string path) {
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
            if (index.texcoord_index != -1) {
                uvs.push_back({
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                    });
            }
            else {
                uvs.push_back({
                   0,
                  0
                    });
            }

            normals.push_back({
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]
                });
        }
    }

    ComputeTangentBasis(vertices, uvs, normals, tangents, bitangents);

    std::vector<glm::vec3> out_vertices;
    std::vector<glm::vec2> out_uvs;
    std::vector<glm::vec3> out_normals;

    std::vector<glm::vec3> out_tangents;
    std::vector<glm::vec3> out_bitangents;

    std::vector<Index> out_indices;

    for(int i=0; i<vertices.size(); i++) {
        int index = 0;
        if (FindSimilarVertex(out_vertices, out_uvs, out_normals,
            vertices[i], uvs[i], normals[i], index)) {

            out_indices.push_back(index);

            out_tangents[index] += tangents[i];
            out_bitangents[index] += bitangents[i];

        }
        else {

            out_vertices.push_back(vertices[i]);
            out_uvs.push_back(uvs[i]);
            out_normals.push_back(normals[i]);
            out_tangents.push_back(tangents[i]);
            out_bitangents.push_back(bitangents[i]);
            out_indices.push_back(out_vertices.size() - 1);
        }
    }

    for (int i = 0; i < out_vertices.size(); i++) {
        VertexNormalMapping vertex = {};
        vertex.pos = out_vertices[i];
        vertex.texCoord = out_uvs[i];
        vertex.normal = glm::normalize(out_normals[i]);
        // the operation ensure orthogonalization
        vertex.bitangent = glm::normalize(out_bitangents[i] - out_normals[i] * glm::dot(out_normals[i], out_bitangents[i]));
        vertex.tangent = glm::normalize(out_tangents[i] - out_normals[i] * glm::dot(out_normals[i], out_tangents[i]));
        
        //ensure the system is right-handed
        if (glm::dot(glm::cross(vertex.normal, vertex.tangent), vertex.bitangent) < 0.0f) {
            vertex.tangent = vertex.tangent * -1.0f;
        }

        mesh->vertices.push_back(vertex);
    }

    for (int i = 0; i < out_indices.size(); i++) {

        mesh->indices.push_back(out_indices[i]);
    }

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