#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "primitive/TriangleMesh.h"

#include "Ray.h" // Include the Ray class header

class Frustum 
{
public:
    glm::vec4 planes[6];  // Left, Right, Top, Bottom, Near, Far

    void extractPlanesFromMatrix(const glm::mat4& viewProjMatrix);
    bool isAABBInFrustum(const AABB& box) const;
    private:
    void normalizePlane(glm::vec4& plane) const;
};

inline void Frustum::extractPlanesFromMatrix(const glm::mat4& viewProjMatrix) {
    // Left plane
    planes[0] = glm::vec4(viewProjMatrix[0][3] + viewProjMatrix[0][0],
                          viewProjMatrix[1][3] + viewProjMatrix[1][0],
                          viewProjMatrix[2][3] + viewProjMatrix[2][0],
                          viewProjMatrix[3][3] + viewProjMatrix[3][0]);

    // Right plane
    planes[1] = glm::vec4(viewProjMatrix[0][3] - viewProjMatrix[0][0],
                          viewProjMatrix[1][3] - viewProjMatrix[1][0],
                          viewProjMatrix[2][3] - viewProjMatrix[2][0],
                          viewProjMatrix[3][3] - viewProjMatrix[3][0]);

    // Top plane
    planes[2] = glm::vec4(viewProjMatrix[0][3] - viewProjMatrix[0][1],
                          viewProjMatrix[1][3] - viewProjMatrix[1][1],
                          viewProjMatrix[2][3] - viewProjMatrix[2][1],
                          viewProjMatrix[3][3] - viewProjMatrix[3][1]);

    // Bottom plane
    planes[3] = glm::vec4(viewProjMatrix[0][3] + viewProjMatrix[0][1],
                          viewProjMatrix[1][3] + viewProjMatrix[1][1],
                          viewProjMatrix[2][3] + viewProjMatrix[2][1],
                          viewProjMatrix[3][3] + viewProjMatrix[3][1]);

    // Near plane
    planes[4] = glm::vec4(viewProjMatrix[0][3] + viewProjMatrix[0][2],
                          viewProjMatrix[1][3] + viewProjMatrix[1][2],
                          viewProjMatrix[2][3] + viewProjMatrix[2][2],
                          viewProjMatrix[3][3] + viewProjMatrix[3][2]);

    // Far plane
    planes[5] = glm::vec4(viewProjMatrix[0][3] - viewProjMatrix[0][2],
                          viewProjMatrix[1][3] - viewProjMatrix[1][2],
                          viewProjMatrix[2][3] - viewProjMatrix[2][2],
                          viewProjMatrix[3][3] - viewProjMatrix[3][2]);

    // Normalize the planes
    for (int i = 0; i < 6; ++i) {
        normalizePlane(planes[i]);
    }
}

// Function to normalize a plane (make sure it's a unit normal)
inline void Frustum::normalizePlane(glm::vec4& plane) const {
    float length = glm::length(glm::vec3(plane));
    plane /= length;
}

inline bool Frustum::isAABBInFrustum(const AABB& box) const {
    // Iterate through all frustum planes
    for (int i = 0; i < 6; ++i) {
        glm::vec4 plane = planes[i];
        
        // Find the AABB corners
        glm::vec3 min = box.min;
        glm::vec3 max = box.max;

        // Check if the AABB is outside the frustum's plane
        // If any corner is outside the frustum, return false
        if (plane.x * min.x + plane.y * min.y + plane.z * min.z + plane.w > 0) return false;
        if (plane.x * max.x + plane.y * min.y + plane.z * min.z + plane.w > 0) return false;
        if (plane.x * min.x + plane.y * max.y + plane.z * min.z + plane.w > 0) return false;
        if (plane.x * max.x + plane.y * max.y + plane.z * min.z + plane.w > 0) return false;
        if (plane.x * min.x + plane.y * min.y + plane.z * max.z + plane.w > 0) return false;
        if (plane.x * max.x + plane.y * min.y + plane.z * max.z + plane.w > 0) return false;
        if (plane.x * min.x + plane.y * max.y + plane.z * max.z + plane.w > 0) return false;
        if (plane.x * max.x + plane.y * max.y + plane.z * max.z + plane.w > 0) return false;
    }

    return true;  // If all tests pass, the AABB is inside the frustum
}

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