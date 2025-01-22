#ifndef CIRCLE_H
#define CIRCLE_H

#include "glm/ext/vector_float3.hpp"
#include <optional>

class Ray;
struct HitResult;

class Circle
{
public:
    Circle(glm::vec3 position, float radius);
    std::optional<HitResult> intersect(Ray& ray);

private:
    glm::vec3 position;
    float radius;
};

#endif // CIRCLE_H