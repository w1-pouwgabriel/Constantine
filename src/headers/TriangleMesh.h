#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include <vector>
#include "./primitive/Triangle.h"
#include "./Texture.h"
#include "glm/fwd.hpp"
#include "tiny_gltf.h"

class Ray;

class TriangleMesh 
{
public:
    TriangleMesh(const tinygltf::Model& model);
    
    std::vector<Triangle>& getTriangles() { return triangles; }
    std::vector<Texture>& getTextures() { return textures; }

private:
    std::vector<Triangle> triangles;
    std::vector<Texture> textures;
    glm::mat4 transformMatrix;

    void processPrimitive(const tinygltf::Model& model, const tinygltf::Primitive& primitive);
    void loadGLTF(const tinygltf::Model& model);
    void loadTextures(const tinygltf::Model& model);
    glm::vec3 computeNormal(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) const;
};

#endif // TRIANGLEMESH_H