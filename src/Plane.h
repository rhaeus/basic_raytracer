#pragma once

#include "TriangleMesh.h"

class Plane : public TriangleMesh
{
public:
    Plane(glm::vec3 lowerLeft, glm::vec3 uVec, glm::vec3 vVec, std::shared_ptr<Material> material);
};

