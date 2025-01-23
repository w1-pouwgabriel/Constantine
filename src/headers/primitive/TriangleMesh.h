#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include "glm/vec3.hpp"
#include <optional>
#include <vector>
#include "tiny_gltf.h"

class Ray;
struct HitResult;

struct Triangle 
{
    glm::vec3 v0, v1, v2;
    glm::vec3 normal;
};

class TriangleMesh 
{
public:
    TriangleMesh () = default;
    
    void loadGLTF(const tinygltf::Model& model);
    std::optional<HitResult> intersect(Ray& ray);

private:
    std::vector<Triangle> triangles;

    void processPrimitive(const tinygltf::Model& model, const tinygltf::Primitive& primitive);
    glm::vec3 computeNormal(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) const;
};

#endif // TRIANGLEMESH_H
