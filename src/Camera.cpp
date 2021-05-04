#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position_, glm::vec3 up_, glm::vec3 lookAt_, float fov_)
    : position(position_), up(up_), lookAt(lookAt_), fov(fov_)
{
    // eye = camera position
    // center = where to look
    glm::vec3 cameraTarget = lookAt_;
    glm::vec3 cameraDirection = glm::normalize(position_ - cameraTarget);

    viewMatrix = glm::lookAt(position_, lookAt_, glm::normalize(up_));
}

const glm::mat4& Camera::getViewMatrix() const
{
    return viewMatrix;
}

float Camera::getFov() const
{
    return fov;
}