#ifndef CIRCLE_H
#define CIRCLE_H

struct Circle 
{
    float x;            // X coordinate of the center
    float y;            // Y coordinate of the center
    float radius;       // Radius of the circle
    float reflectivity; // Reflectivity of the circle

    Circle(float x, float y, float radius, float r, float g, float b, float reflectivity)
        : x(x), y(y), radius(radius), reflectivity(reflectivity) {}
};

#endif // CIRCLE_H