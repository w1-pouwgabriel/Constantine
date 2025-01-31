#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

#include <vector>
#include "./primitive/Triangle.h"
#include "./Texture.h"
#include "tiny_gltf.h"

class Ray;

class TriangleMesh 
{
public:
    TriangleMesh(const tinygltf::Model& model);
    TriangleMesh(std::vector<Triangle> triangles, std::vector<Texture> textures)
        : triangles(triangles), textures(textures) {};
    
    std::vector<Triangle>& getTriangles() { return triangles; }
    std::vector<Texture>& getTextures() { return textures; }

private:
    std::vector<Triangle> triangles;
    std::vector<Texture> textures;

    void processPrimitive(const tinygltf::Model& model, const tinygltf::Primitive& primitive);
    void loadGLTF(const tinygltf::Model& model);
    void loadTextures(const tinygltf::Model& model);
    glm::vec3 computeNormal(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) const;
};



#endif // TRIANGLEMESH_H
