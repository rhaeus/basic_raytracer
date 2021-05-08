#pragma once

#include "TriangleMesh.h"

class Cube : public TriangleMesh
{
public:
    Cube(glm::vec3 center, std::shared_ptr<Material> material, glm::vec3 scale = glm::vec3(1,1,1), float roll = 0, float pitch = 0, float yaw = 0);

};

