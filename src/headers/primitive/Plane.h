#ifndef PLANE_H
#define PLANE_H

#include "Triangle.h"

struct HitResult;

class Plane
{
public:
    glm::vec3 point;  // A point on the plane
    glm::vec3 normal; // Normal vector of the plane
    Triangle triangle; // Degenerate triangle for intersection

    // Constructor that directly sets the plane's point and normal
    Plane(const glm::vec3& point, const glm::vec3& normal)
        : point(point), normal(glm::normalize(normal)), 
          triangle(point, point + normal, point - normal, normal) {}

    // Ray-plane intersection
    std::optional<HitResult> intersect(Ray& ray) {
        // Calculate the denominator of the intersection equation
        float denom = glm::dot(ray.direction, normal);
        
        // If the denominator is near zero, the ray is parallel to the plane
        if (std::abs(denom) < 1e-6f) {
            return std::nullopt; // No intersection, ray is parallel
        }
        
        // Calculate the numerator (distance from ray origin to a point on the plane)
        float t = glm::dot(point - ray.origin, normal) / denom;
        
        // If t is negative, the intersection point is behind the ray's origin
        if (t < 0) {
            return std::nullopt; // No intersection in front of the ray
        }

        // Calculate the intersection point
        glm::vec3 intersectionPoint = ray.origin + t * ray.direction;
        
        // Return the hit result
        HitResult hitResult;
        hitResult.t = t;
        hitResult.point = intersectionPoint;
        hitResult.normal = normal;
        
        return hitResult;
    }
};

#endif // PLANE_H
