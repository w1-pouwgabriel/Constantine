#include "TriangleMesh.h"
#include "HitResult.h"
#include "../Ray.h"

#include "glm/geometric.hpp"
#include <limits>

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

void TriangleMesh::processPrimitive(const tinygltf::Model& model, const tinygltf::Primitive& primitive) {
    const auto& indicesAccessor = model.accessors[primitive.indices];
    const auto& indicesView = model.bufferViews[indicesAccessor.bufferView];
    const auto& indicesBuffer = model.buffers[indicesView.buffer];

    const auto& positionAccessor = model.accessors.at(primitive.attributes.at("POSITION"));
    const auto& positionView = model.bufferViews[positionAccessor.bufferView];
    const auto& positionBuffer = model.buffers[positionView.buffer];

    const uint16_t* indexData = reinterpret_cast<const uint16_t*>(
        indicesBuffer.data.data() + indicesView.byteOffset + indicesAccessor.byteOffset);
    const float* positionData = reinterpret_cast<const float*>(
        positionBuffer.data.data() + positionView.byteOffset + positionAccessor.byteOffset);

    for (size_t i = 0; i < indicesAccessor.count; i += 3) {
        glm::vec3 v0 = glm::vec3(positionData[3 * indexData[i]], positionData[3 * indexData[i] + 1], positionData[3 * indexData[i] + 2]);
        glm::vec3 v1 = glm::vec3(positionData[3 * indexData[i + 1]], positionData[3 * indexData[i + 1] + 1], positionData[3 * indexData[i + 1] + 2]);
        glm::vec3 v2 = glm::vec3(positionData[3 * indexData[i + 2]], positionData[3 * indexData[i + 2] + 1], positionData[3 * indexData[i + 2] + 2]);
        glm::vec3 normal = computeNormal(v0, v1, v2);
        triangles.push_back({v0, v1, v2, normal});
    }
}

glm::vec3 TriangleMesh::computeNormal(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) const {
    return glm::normalize(glm::cross(v1 - v0, v2 - v0));
}

std::optional<HitResult> TriangleMesh::intersect(Ray& ray) {
    HitResult closestHit;
    closestHit.t = std::numeric_limits<float>::max();
    bool hitFound = false;

    for (const auto& triangle : triangles) {
        // Möller–Trumbore algorithm
        glm::vec3 e1 = triangle.v1 - triangle.v0;
        glm::vec3 e2 = triangle.v2 - triangle.v0;
        glm::vec3 h = glm::cross(ray.direction, e2);
        float a = glm::dot(e1, h);

        if (std::abs(a) < 1e-8f) continue;

        float f = 1.0f / a;
        glm::vec3 s = ray.origin - triangle.v0;
        float u = f * glm::dot(s, h);
        if (u < 0.0f || u > 1.0f) continue;

        glm::vec3 q = glm::cross(s, e1);
        float v = f * glm::dot(ray.direction, q);
        if (v < 0.0f || u + v > 1.0f) continue;

        float t = f * glm::dot(e2, q);
        if (t > 1e-8f && t < closestHit.t) {
            closestHit.t = t;
            closestHit.point = ray.origin + t * ray.direction;
            closestHit.normal = triangle.normal;
            hitFound = true;
        }
    }

    return hitFound ? std::optional<HitResult>{closestHit} : std::nullopt;
}