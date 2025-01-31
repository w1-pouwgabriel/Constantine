#include "glm/fwd.hpp"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct HitResult 
{
    float t;          // Distance along the ray
    glm::vec3 point;  // Point of intersection
    glm::vec3 normal; // Normal at the intersection
    glm::vec2 uv;     // Texture coordinates
    glm::vec3 color;  // Color at the intersection
};
