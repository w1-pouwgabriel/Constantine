#ifndef CIRCLE_H
#define CIRCLE_H

#include <optional>

class Ray;
struct HitResult;

class Circle
{
public:
    Circle(float x, float y, float radius);
    std::optional<HitResult> intersect(Ray& ray);

private:
    float x;
    float y;
    float radius;
};

#endif // CIRCLE_H