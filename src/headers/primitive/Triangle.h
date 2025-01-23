#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "HitResult.h"
#include "../Ray.h"
#include <glm/glm.hpp>
#include <optional>

class Triangle 
{
    public:
    std::optional<HitResult> intersect(Ray& ray);

    glm::vec3 v0, v1, v2;
    glm::vec3 normal;
};

inline std::optional<HitResult> Triangle::intersect(Ray& ray) {
    HitResult closestHit;
    closestHit.t = std::numeric_limits<float>::max();
    bool hitFound = false;

    // Möller–Trumbore algorithm
    glm::vec3 e1 = v1 - v0; // Edge 1
    glm::vec3 e2 = v2 - v0; // Edge 2
    glm::vec3 h = glm::cross(ray.direction, e2);
    float a = glm::dot(e1, h);

    // Check if the ray is parallel to the triangle
    if (std::abs(a) < 1e-8f) return std::nullopt;

    float f = 1.0f / a;
    glm::vec3 s = ray.origin - v0;
    float u = f * glm::dot(s, h);

    // Check if u is within the valid range
    if (u < 0.0f || u > 1.0f) return std::nullopt;

    glm::vec3 q = glm::cross(s, e1);
    float v = f * glm::dot(ray.direction, q);

    // Check if v is within the valid range and u + v <= 1
    if (v < 0.0f || u + v > 1.0f) return std::nullopt;

    float t = f * glm::dot(e2, q);

    // Check if the intersection is in the positive direction of the ray
    if (t > 1e-8f && t < closestHit.t) {
        closestHit.t = t;
        closestHit.point = ray.origin + t * ray.direction;
        closestHit.normal = normal;
        hitFound = true;
    }

    return hitFound ? std::optional<HitResult>{closestHit} : std::nullopt;
}

#endif // TRIANGLE_H