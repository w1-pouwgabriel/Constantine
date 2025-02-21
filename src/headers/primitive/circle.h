#ifndef CIRCLE_H
#define CIRCLE_H

#include "glm/ext/vector_float3.hpp"
#include <optional>

struct HitResult;
class Ray;

class Circle
{
public:
    Circle() : position(glm::vec3(0)), radius(0.125f) {};
    Circle(const glm::vec3& position, float radius);
    std::optional<HitResult> intersect(Ray& ray);

private:
    glm::vec3 position;
    float radius;
};

#endif // CIRCLE_H