#include <glm/vec3.hpp>

struct HitResult 
{
    float t;          // Distance along the ray
    glm::vec3 point;  // Point of intersection
    glm::vec3 normal; // Normal at the intersection
};
