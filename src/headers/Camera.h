#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Ray.h" // Include the Ray class header

struct Plane {          // Plane structure for the view frustum
    glm::vec3 normal;   // Normal vector of the plane
    float d;            // Distance from the origin
};

class Camera 
{
public:
    Camera() : position(0.0f), direction(0.0f), right(0.0f), up(0.0f), fov(0.0f), aspectRatio(0.0f), aperture(0.0f), focusDist(0.0f) {};
    Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, 
           float fov, float aspectRatio, float aperture, float focusDist);

    // Methods to manipulate the camera
    void move(const glm::vec3& delta);  // Move the camera by a given offset
    void rotate(float pitch, float yaw); // Rotate the camera by pitch (up/down) and yaw (left/right)
    void setPosition(const glm::vec3& newPosition); // Set the camera position directly
    void setDirection(const glm::vec3& newTarget); // Set a new target/direction for the camera

    // Ray generation
    Ray generateRay(float u, float v) const;

    // Matrices
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 right;
    glm::vec3 up;

    float fov;
    float aspectRatio;
    float aperture;
    float focusDist;

    Plane frustum[6];
    glm::vec3 lowerLeftCorner; // Lower left corner of the view frustum
    glm::vec3 horizontal;      // Horizontal span of the view frustum
    glm::vec3 vertical;        // Vertical span of the view frustum

    void updateCameraVectors(); // Update the right and up vectors based on the new direction
    void computeViewFrustum();  // Compute the view frustum (lower left, horizontal, vertical)

    bool isPointInsideFrustum(const glm::vec3& point) const {
    for (int i = 0; i < 6; i++) {
        float distance = glm::dot(frustum[i].normal, point) + frustum[i].d;
        if (distance < 0) {
            return false; // Point is outside this plane
        }
    }
    return true; // Point is inside all planes
}
};

#endif // CAMERA_H