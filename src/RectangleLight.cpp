#include "RectangleLight.h"

RectangleLight::RectangleLight(glm::vec3 position, glm::vec3 uVec_, glm::vec3 vVec_, int uSamples_, int vSamples_, glm::vec3 color, float flux)
    : Light(position, color, flux), uVec(uVec_), vVec(vVec_), uSamples(uSamples_), vSamples(vSamples_)
{
    uStep = glm::length(uVec) / (float)uSamples;
    vStep = glm::length(vVec) / (float)vSamples;

    uDir = glm::normalize(uVec);
    vDir = glm::normalize(vVec);
}

const std::vector<glm::vec3>& RectangleLight::getSamplePoints(){
    samplePoints.clear();

    for (int v = 0; v < vSamples; ++v) {
        for (int u = 0; u < uSamples; ++u) {
            // random number between 0 and 1 for offset in each cell
            float rndU = rand() / double(RAND_MAX);
            float rndV = rand() / double(RAND_MAX);

            glm::vec3 point = position + (u + rndU) * uStep * uDir + (v + rndV) * vStep * vDir;
            samplePoints.push_back(point);
        }
    }

    return samplePoints;
}
