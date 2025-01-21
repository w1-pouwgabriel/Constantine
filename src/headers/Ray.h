#ifndef RAY_H
#define RAY_H

#include <glm/vec3.hpp>

class Ray 
{
public:
    Ray(const glm::vec3& origin, const glm::vec3& direction)
        : origin(origin), direction(direction) {}

    glm::vec3 at(float t) const { return origin + t * direction; }

    glm::vec3 origin;
    glm::vec3 direction;
};

#endif // RAY_H
