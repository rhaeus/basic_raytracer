#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
    : Camera(glm::vec3(0,0,0), glm::vec3(0,1,0), glm::vec3(0,0,1), 60, 1920, 1080)
{}

Camera::Camera(glm::vec3 position_, glm::vec3 up_, glm::vec3 lookAt_, float fov_, int width_, int height_)
    : position(position_), up(up_), lookAt(lookAt_), fov(fov_), width(width_), height(height_)
{
    // eye = camera position
    // center = where to look
    // glm::vec3 cameraTarget = lookAt_;
    // glm::vec3 cameraDirection = glm::normalize(position_ - cameraTarget);

    // viewMatrix = glm::lookAt(position_, lookAt_, glm::normalize(up_));
    direction = glm::normalize(lookAt_ - position_); // camera direction
    right = glm::cross(direction, up_);
    up = glm::cross(right, direction);
    // u = glm::cross(up_, w); //camera_right = Cross(camera_direction, camera_up)
    // v = glm::cross(w, u); 
    // camera_up = Cross(camera_right, camera_direction)
}

void Camera::getCameraRay(float sampleX, float sampleY, glm::vec3& origin, glm::vec3& dir) const
{
    // float normalized_i = 2*(i / (float)width) - 1;
    // float normalized_j = 1-2*(j / (float)height);
    glm::vec3 image_point = sampleX * -right +
                            sampleY * up +
                            position + direction;
    glm::vec3 ray_direction = glm::normalize(image_point - position);
    
    origin = position;
    dir = ray_direction;

}

// const glm::mat4& Camera::getViewMatrix() const
// {
//     return viewMatrix;
// }

float Camera::getFov() const
{
    return fov;
}