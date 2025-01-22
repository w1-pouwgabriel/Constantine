#ifndef RAY_H
#define RAY_H

#include <glm/vec3.hpp>

class Ray 
{
public:
    Ray(
        const glm::vec3& origin = glm::vec3(0), 
        const glm::vec3& direction = glm::vec3(0))
        : origin(origin), direction(direction) {}

    glm::vec3 at(float t) const { return origin + t * direction; }

    glm::vec3 origin;
    glm::vec3 direction;
};

#endif // RAY_H
