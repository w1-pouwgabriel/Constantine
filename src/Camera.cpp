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
    static float pitchAngle = 0.0f;
    static float yawAngle = 0.0f;

    pitchAngle += pitch;
    yawAngle += yaw;

    // Constrain pitch to avoid flipping
    pitchAngle = glm::clamp(pitchAngle, glm::radians(-89.0f), glm::radians(89.0f));

    // Update direction vector
    glm::vec3 front;
    front.x = cos(yawAngle) * cos(pitchAngle);
    front.y = sin(pitchAngle);
    front.z = sin(yawAngle) * cos(pitchAngle);
    direction = glm::normalize(front);

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
}

// Update the right and up vectors based on the new direction
void Camera::updateCameraVectors() {
    right = glm::normalize(glm::cross(direction, up));  // Right vector is orthogonal to direction and up
    this->up = glm::cross(right, direction);  // Recalculate up to maintain orthogonality
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
}
