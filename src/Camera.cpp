#include "headers/Camera.h"
#include "glm/ext/matrix_clip_space.hpp"

Camera::Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, 
               float fov, float aspectRatio, float aperture, float focusDist)
    : position(position), fov(fov), aspectRatio(aspectRatio), aperture(aperture), focusDist(focusDist) {
    direction = glm::normalize(target - position);
    right = glm::normalize(glm::cross(direction, up));
    this->up = glm::cross(right, direction);

    computeViewFrustum();
}

// Move the camera by a given offset
void Camera::move(const glm::vec3& delta) {
    position += delta;
    computeViewFrustum();  // Recompute the frustum after moving
}

// Rotate the camera by pitch (up/down) and yaw (left/right)
void Camera::rotate(float pitch, float yaw) {
    // Create rotation matrices for pitch and yaw
    glm::mat4 pitchRotation = glm::rotate(glm::mat4(1.0f), pitch, right); // Rotate around the right axis
    glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0f), yaw, up); // Rotate around the up axis

    // Apply the rotations to the direction vector
    direction = glm::vec3(pitchRotation * glm::vec4(direction, 0.0f)); // Rotate with pitch
    direction = glm::vec3(yawRotation * glm::vec4(direction, 0.0f));   // Rotate with yaw

    updateCameraVectors();
    computeViewFrustum();
}

// Set the camera position directly
void Camera::setPosition(const glm::vec3& newPosition) {
    position = newPosition;
    computeViewFrustum();
}

// Set the camera direction (target) directly
void Camera::setDirection(const glm::vec3& newTarget) {
    direction = glm::normalize(newTarget - position);
    updateCameraVectors();
    computeViewFrustum(); 
}

// Update the right and up vectors based on the new direction
void Camera::updateCameraVectors() {
    right = glm::normalize(glm::cross(direction, up));  // Right vector is orthogonal to direction and up
    this->up = glm::cross(right, direction);  // Recalculate up to maintain orthogonality
}

// Generate a ray for a given pixel (u, v) in screen space
Ray Camera::generateRay(float u, float v) const {
    glm::vec3 rayDirection = glm::normalize(lowerLeftCorner + u * horizontal + v * vertical - position);
    return Ray(position, rayDirection);
}

// Get the view matrix (camera transformation)
glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + direction, up);
}

// Get the projection matrix (perspective projection)
glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 1000.0f);
}

void Camera::computeViewFrustum() {
    float viewportHeight = 2.0f * focusDist * tan(glm::radians(fov) / 2.0f);
    float viewportWidth = aspectRatio * viewportHeight;

    horizontal = viewportWidth * right;
    vertical = viewportHeight * up;
    lowerLeftCorner = position + direction * focusDist - horizontal * 0.5f - vertical * 0.5f;
}
