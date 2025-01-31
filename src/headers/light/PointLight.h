#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <glm/glm.hpp>

class PointLight 
{
public:
    glm::vec3 position;
    glm::vec3 color;
    float intensity;

    PointLight(const glm::vec3& pos, const glm::vec3& col, float intens)
        : position(pos), color(col), intensity(intens) {}

    glm::vec3 computeLighting(const glm::vec3& hitPoint, const glm::vec3& normal) const {
        glm::vec3 lightDir = glm::normalize(position - hitPoint);
        float diff = glm::max(glm::dot(normal, lightDir), 0.0f);
        return color * intensity * diff;
    }
};

#endif // POINTLIGHT_H
