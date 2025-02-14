#include "../headers/Camera.h"
#include "glm/ext/matrix_clip_space.hpp"

Camera::Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, 
               float fov, float aspectRatio, float aperture, float focusDist)
    : position(position), fov(fov), aspectRatio(aspectRatio), aperture(aperture), focusDist(focusDist) {
    // Compute direction vector
    direction = glm::normalize(target - position);

    // Calculate yaw and pitch from the direction vector
    yaw = glm::degrees(atan2(direction.z, direction.x));  // Horizontal angle
    pitch = glm::degrees(asin(direction.y));             // Vertical angle

    // Compute right and up vectors
    right = glm::normalize(glm::cross(direction, up));
    this->up = glm::cross(right, direction);

    computeViewFrustum();
}

// Move the camera by a given offset
void Camera::move(const glm::vec3& delta) {
    position += delta;
    computeViewFrustum();
}

// Rotate the camera by pitch (up/down) and yaw (left/right)
void Camera::rotate(float pitchOffset, float yawOffset) {
    pitch += pitchOffset;
    yaw += yawOffset;

    // Clamp pitch to avoid flipping
    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    // Recompute the direction vector using spherical coordinates
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    direction = glm::normalize(direction);

    // Update camera vectors
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

void Camera::processMouseMovement(float deltaX, float deltaY, float sensitivity) {
    deltaX *= sensitivity;
    deltaY *= sensitivity;

    yaw += deltaX;
    pitch += deltaY;

    // Clamp the pitch to prevent flipping
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Update direction vector
    glm::vec3 newDirection;
    newDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newDirection.y = sin(glm::radians(pitch));
    newDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction = glm::normalize(newDirection);

    updateCameraVectors();
    computeViewFrustum(); 
}

void Camera::updateCameraVectors() {
    right = glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f))); // Cross with world up
    up = glm::cross(right, direction); // Maintain orthogonality
}


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

    // Compute frustum planes (assuming near and far planes are 0.1 and 1000.0)
    glm::vec3 nearCenter = position + direction * 0.1f;
    glm::vec3 farCenter = position + direction * 1000.0f;
    
}