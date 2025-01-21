#include "Circle.h"
#include "HitResult.h"
#include "../Ray.h"

#include <glm/glm.hpp> 

Circle::Circle(float x, float y, float radius)
    : x(x), y(y), radius(radius) {}

std::optional<HitResult> Circle::intersectCircle(Ray& ray) {
    glm::vec3 oc = ray.origin - glm::vec3(x, y, 0.0f); // Circle is in the XY plane
    float a = glm::dot(ray.direction, ray.direction);
    float b = 2.0f * glm::dot(oc, ray.direction);
    float c = glm::dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        float t = (-b - sqrt(discriminant)) / (2.0f * a);
        if (t > 0) {
            glm::vec3 hitPoint = ray.at(t);
            glm::vec3 normal = glm::normalize(hitPoint - glm::vec3(x, y, 0.0f));
            return HitResult{t, hitPoint, normal};
        }
    }
    return std::nullopt;
}
