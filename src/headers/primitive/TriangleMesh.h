#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include "glm/vec3.hpp"
#include <optional>
#include <vector>
#include "Triangle.h"
#include "tiny_gltf.h"

class Ray;

struct AABB 
{
    glm::vec3 min, max;
};

class TriangleMesh 
{
public:
    TriangleMesh () = default;
    
    void loadGLTF(const tinygltf::Model& model);
    std::vector<Triangle>& getTriangles() { return triangles; }
    
    // New method to compute the AABB
    AABB computeAABB() const;

private:
    std::vector<Triangle> triangles;

    void processPrimitive(const tinygltf::Model& model, const tinygltf::Primitive& primitive);
    glm::vec3 computeNormal(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) const;
};



#endif // TRIANGLEMESH_H
