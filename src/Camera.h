#pragma once

#include <glm/glm.hpp>
#include "Ray.h"

class Camera
{
public:
    Camera(glm::vec3 position, glm::vec3 up, glm::vec3 lookAt, float fov, int width, int height);

    // const glm::mat4& getViewMatrix() const;
    float getFov() const;
    void getCameraRay(float i, float j, glm::vec3& origin, glm::vec3& dir);

private:
    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 lookAt;
    // glm::mat4 viewMatrix;
    float fov;
    int width;
    int height;

    glm::vec3 direction;
    glm::vec3 right;
    glm::vec3 w;

};