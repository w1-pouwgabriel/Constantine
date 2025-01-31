#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Ray.h"

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
    void processMouseMovement(float deltaX, float deltaY, float sensitivity = 0.1f);

    // Ray generation
    Ray generateRay(float u, float v) const;

    // Matrices
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    float yaw;   // degrees
    float pitch; // degrees
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 right;
    glm::vec3 up;

    float fov;
    float aspectRatio;
    float aperture;
    float focusDist;

    glm::vec3 lowerLeftCorner;      // Lower left corner of the view frustum
    glm::vec3 horizontal;      // Horizontal span of the view frustum
    glm::vec3 vertical;        // Vertical span of the view frustum

    void updateCameraVectors(); // Update the right and up vectors based on the new direction
    void computeViewFrustum();  // Compute the view frustum (lower left, horizontal, vertical)
};

#endif // CAMERA_H