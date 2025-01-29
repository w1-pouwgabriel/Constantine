#include "TriangleMesh.h"

#include "glm/geometric.hpp"

// Function to load a GLTF model
void TriangleMesh::loadGLTF(const tinygltf::Model& model)
{
    triangles.clear();

    for (const auto& mesh : model.meshes) {
        for (const auto& primitive : mesh.primitives) {
            if (primitive.mode == TINYGLTF_MODE_TRIANGLES) {
                processPrimitive(model, primitive);
            }
        }
    }
}

AABB TriangleMesh::computeAABB() const
{
    glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
    glm::vec3 max = glm::vec3(-std::numeric_limits<float>::max());

    for (const auto& triangle : triangles) {
        min = glm::min(min, glm::min(triangle.v0, glm::min(triangle.v1, triangle.v2)));
        max = glm::max(max, glm::max(triangle.v0, glm::max(triangle.v1, triangle.v2)));
    }

    return AABB{min, max};
}

void TriangleMesh::processPrimitive(const tinygltf::Model& model, const tinygltf::Primitive& primitive) 
{
    const auto& indicesAccessor = model.accessors[primitive.indices];
    const auto& indicesView = model.bufferViews[indicesAccessor.bufferView];
    const auto& indicesBuffer = model.buffers[indicesView.buffer];

    const auto& positionAccessor = model.accessors.at(primitive.attributes.at("POSITION"));
    const auto& positionView = model.bufferViews[positionAccessor.bufferView];
    const auto& positionBuffer = model.buffers[positionView.buffer];

    bool hasNormals = primitive.attributes.count("NORMAL") > 0;
    const tinygltf::Accessor* normalAccessor = nullptr; 
    const tinygltf::BufferView* normalView = nullptr;
    const tinygltf::Buffer* normalBuffer = nullptr;

    if (hasNormals) {
        normalAccessor = &model.accessors.at(primitive.attributes.at("NORMAL"));
        normalView = &model.bufferViews[normalAccessor->bufferView];
        normalBuffer = &model.buffers[normalView->buffer];

        // Check if the buffer has data
        if (normalAccessor->count == 0 || normalBuffer->data.empty()) {
            hasNormals = false; // If empty, fall back to computing normals
        }
    }

    const auto& texCoordAcessor = model.accessors.at(primitive.attributes.at("TEXCOORD_0"));
    const auto& texCoordView = model.bufferViews[texCoordAcessor.bufferView];
    const auto& texCoordBuffer = model.buffers[texCoordView.buffer];

    const uint16_t* indexData = reinterpret_cast<const uint16_t*>(
        indicesBuffer.data.data() + indicesView.byteOffset + indicesAccessor.byteOffset);
    const float* positionData = reinterpret_cast<const float*>(
        positionBuffer.data.data() + positionView.byteOffset + positionAccessor.byteOffset);

    for (size_t i = 0; i < indicesAccessor.count; i += 3) {

        glm::vec3 v0 = glm::vec3(positionData[3 * indexData[i]], positionData[3 * indexData[i] + 1], positionData[3 * indexData[i] + 2]);;
        glm::vec3 v1 = glm::vec3(positionData[3 * indexData[i + 1]], positionData[3 * indexData[i + 1] + 1], positionData[3 * indexData[i + 1] + 2]);;
        glm::vec3 v2 = glm::vec3(positionData[3 * indexData[i + 2]], positionData[3 * indexData[i + 2] + 1], positionData[3 * indexData[i + 2] + 2]);;
        glm::vec3 normal;

        if (hasNormals) {
            const float* normalData = reinterpret_cast<const float*>(
                normalBuffer->data.data() + normalView->byteOffset + normalAccessor->byteOffset);
            normal = glm::vec3(normalData[3 * indexData[i]], normalData[3 * indexData[i] + 1], normalData[3 * indexData[i] + 2]);
        } else {
            normal = computeNormal(v0, v1, v2);
        }

        triangles.push_back({v0, v1, v2, normal});
    }
}

glm::vec3 TriangleMesh::computeNormal(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) const 
{
    return glm::normalize(glm::cross(v1 - v0, v2 - v0));
}