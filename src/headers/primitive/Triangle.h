#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include <optional>

class Ray;
struct HitResult;

class Triangle 
{
    public:
    glm::vec3 v0, v1, v2;
    glm::vec3 normal;
    glm::vec3 edge1, edge2; // Precomputed edges (For intrersectFast)
    glm::vec2 uv0, uv1, uv2; // UV coordinates

    Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& normal, const glm::vec2& uv0, const glm::vec2& uv1, const glm::vec2& uv2)
        : v0(v0), v1(v1), v2(v2), edge1(v1 - v0), edge2(v2 - v0), normal(normal), uv0(uv0), uv1(uv1), uv2(uv2) {}

    std::optional<HitResult> intersect(Ray& ray);
    std::optional<HitResult> intersectFast(Ray& ray);
};

#endif // TRIANGLE_H