#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include <optional>

class Ray;
class Texture;
struct HitResult;

class Triangle 
{
    public:
    glm::vec3 v0, v1, v2;
    glm::vec3 normal;
    glm::vec2 uv0, uv1, uv2; // UV coordinates
    int textureIndex;

    glm::vec3 edge1, edge2; // Precomputed edges (For intrersectFast)

    Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& normal, const glm::vec2& uv0, const glm::vec2& uv1, const glm::vec2& uv2, int textureIndex)
        : v0(v0), v1(v1), v2(v2), edge1(v1 - v0), edge2(v2 - v0), normal(normal), uv0(uv0), uv1(uv1), uv2(uv2), textureIndex(textureIndex) {}

    std::optional<HitResult> intersect(const Ray& ray, const std::vector<Texture>& textures);
    std::optional<HitResult> intersectFast(const Ray& ray, const std::vector<Texture>& textures);
};

#endif // TRIANGLE_H