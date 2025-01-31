#include "Triangle.h"
#include "../Texture.h"
#include "../Ray.h"

std::optional<HitResult> Triangle::intersect(Ray& ray, const std::vector<Texture>& textures) {
    HitResult closestHit;
    closestHit.t = std::numeric_limits<float>::max();

    glm::vec3 e1 = v1 - v0; // Edge 1
    glm::vec3 e2 = v2 - v0; // Edge 2
    glm::vec3 h = glm::cross(ray.direction, e2);
    float a = glm::dot(e1, h);

    if (std::abs(a) < 1e-8f) return std::nullopt;

    float f = 1.0f / a;
    glm::vec3 s = ray.origin - v0;
    float u = f * glm::dot(s, h);

    if (u < 0.0f || u > 1.0f) return std::nullopt;

    glm::vec3 q = glm::cross(s, e1);
    float v = f * glm::dot(ray.direction, q);
    if (v < 0.0f || u + v > 1.0f) return std::nullopt;

    float t = f * glm::dot(e2, q);
    if (t > 1e-8f && t < closestHit.t) {
        closestHit.t = t;
        closestHit.point = ray.origin + t * ray.direction;
        closestHit.normal = normal;

        // Barycentric interpolation for UV coordinates
        float w = 1.0f - u - v;
        glm::vec2 uv = w * uv0 + u * uv1 + v * uv2;

        // Sample the texture if it exists
        if (textureIndex >= 0) {
            // Fetch the texture based on the index
            const Texture& texture = textures[textureIndex];
            glm::vec3 textureColor = texture.sample(uv.x, uv.y);
            closestHit.color = textureColor; // Store the texture color
        }

        return closestHit;
    }

    return std::nullopt;
}

std::optional<HitResult> Triangle::intersectFast(Ray& ray) {
    HitResult closestHit;
    closestHit.t = std::numeric_limits<float>::max();

    // Compute determinant (a)
    glm::vec3 h = glm::cross(ray.direction, edge2);
    float a = glm::dot(edge1, h);

    if (std::abs(a) < 1e-8f) return std::nullopt; // Parallel
    //if (a > 0.0f) return std::nullopt;

    float f = 1.0f / a;
    glm::vec3 s = ray.origin - v0;
    float u = f * glm::dot(s, h);
    if (u < 0.0f || u > 1.0f) return std::nullopt;

    glm::vec3 q = glm::cross(s, edge1);
    float v = f * glm::dot(ray.direction, q);
    if (v < 0.0f || u + v > 1.0f) return std::nullopt;

    float t = f * glm::dot(edge2, q);
    if (t > 1e-8f && t < closestHit.t) {
        closestHit.t = t;
        closestHit.point = ray.origin + t * ray.direction;
        closestHit.normal = normal;
        closestHit.color = normal;
        return closestHit;
    }

    return std::nullopt;
}