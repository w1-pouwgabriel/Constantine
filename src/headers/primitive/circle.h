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
    float x;            // X coordinate of the center
    float y;            // Y coordinate of the center
    float radius;       // Radius of the circle
};

#endif // CIRCLE_H