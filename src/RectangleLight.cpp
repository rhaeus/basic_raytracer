#include "RectangleLight.h"

RectangleLight::RectangleLight(glm::vec3 position, glm::vec3 uVec, glm::vec3 vVec, int uSamples, int vSamples, glm::vec3 color, float flux)
    : Light(position, color, flux)
{
    samplePoints.clear();

    float uStep = glm::length(uVec) / (float)uSamples;
    float vStep = glm::length(vVec) / (float)vSamples;

    glm::vec3 uDir = glm::normalize(uVec);
    glm::vec3 vDir = glm::normalize(vVec);

    for (int v = 0; v < vSamples; ++v) {
        for (int u = 0; u < uSamples; ++u) {
            // random number between 0 and 1 for offset in each cell
            float rndU = rand() / double(RAND_MAX);
            float rndV = rand() / double(RAND_MAX);

            // glm::vec3 point = position + (u + 0.5f) * uStep * uDir + (v + 0.5f) * vStep * vDir;
            glm::vec3 point = position + (u + rndU) * uStep * uDir + (v + rndV) * vStep * vDir;
            samplePoints.push_back(point);
        }
    }


}
