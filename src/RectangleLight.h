#pragma once

#include "Light.h"

class RectangleLight : public Light
{
public:
    RectangleLight(glm::vec3 position, glm::vec3 uVec, glm::vec3 vVec, int uSamples, int vSamples, glm::vec3 color, float flux);
    virtual const std::vector<glm::vec3>& getSamplePoints() override;

private:
    glm::vec3 uVec; 
    glm::vec3 vVec;
    int uSamples;
    int vSamples;
    float uStep;
    float vStep;
    glm::vec3 uDir;
    glm::vec3 vDir;

};