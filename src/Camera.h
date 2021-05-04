#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(glm::vec3 position, glm::vec3 up, glm::vec3 lookAt, float fov);

    const glm::mat4& getViewMatrix() const;
    float getFov() const;

private:
    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 lookAt;
    glm::mat4 viewMatrix;
    float fov;
};