#ifndef PLANE_H
#define PLANE_H

#include <glm/glm.hpp>

class Plane 
{
public:
    Plane() : normal(0.0f), d(0.0f) {}
    Plane(const glm::vec3& point, const glm::vec3& normal)
        : normal(glm::normalize(normal)), d(glm::dot(normal, point)) {}

    // Check if a point is inside (on the positive side of) the plane
    bool isPointInside(const glm::vec3& point) const {
        return glm::dot(normal, point) - d >= -1e-5f; // Allow small margin of error
    }

    glm::vec3 normal;
    float d;
};

#endif // PLANE_H
