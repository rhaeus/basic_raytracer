#pragma once

#include "Light.h"

class RectangleLight : public Light
{
public:
    RectangleLight(glm::vec3 position, glm::vec3 uVec, glm::vec3 vVec, int uSamples, int vSamples, glm::vec3 color, float flux);
};