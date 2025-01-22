#include "Circle.h"
#include "HitResult.h"
#include "../Ray.h"

#include <glm/glm.hpp> 

Circle::Circle(glm::vec3 position, float radius)
    : position(position), radius(radius) {}

std::optional<HitResult> Circle::intersect(Ray& ray) {
    glm::vec3 oc = ray.origin - position;
    float a = glm::dot(ray.direction, ray.direction);
    float b = 2.0f * glm::dot(oc, ray.direction);
    float c = glm::dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        float t = (-b - sqrt(discriminant)) / (2.0f * a);
        if (t > 0) {
            glm::vec3 hitPoint = ray.at(t);
            glm::vec3 normal = glm::normalize(hitPoint - position);
            return HitResult{t, hitPoint, normal};
        }
    }
    return std::nullopt;
}
